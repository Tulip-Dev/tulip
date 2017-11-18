/**
 *
 * This file is part of Tulip (http://tulip.labri.fr)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
#endif

#include "tulip/PythonIncludes.h"
#include "tulip/PythonInterpreter.h"
#include "ConsoleHandlers.h"

#include <sstream>

#include <QMessageBox>
#include <QApplication>
#include <QLibrary>
#include <QTime>
#include <QTextStream>
#include <QMutex>
#include <QWaitCondition>
#include <QDir>

#include <tulip/TulipRelease.h>
#include <tulip/TlpTools.h>
#include <tulip/PythonVersionChecker.h>
#include <tulip/TlpQtTools.h>

#include <cstdio>
#ifndef WIN32
#include <dlfcn.h>
#else
#include <windows.h>
#endif

using namespace tlp;

static ConsoleOutputHandler *consoleOuputHandler = NULL;
static ConsoleOutputEmitter *consoleOuputEmitter = NULL;

extern QString consoleOuputString;
extern QString consoleErrorOuputString;
extern QString mainScriptFileName;

extern void initconsoleutils();
extern void inittuliputils();

#ifndef WIN32
static PyThreadState *mainThreadState;
#endif

static PyGILState_STATE gilState;

static const QString printObjectDictFunction =
    "def printObjectDict(obj):\n"
    "  if hasattr(obj, \"__dict__\"):\n"
    "    for k in obj.__dict__.keys():\n"
#if PY_MAJOR_VERSION >= 3
    "      print(k)\n"
#else
    "      print k\n"
#endif
    "  if hasattr(obj, \"__bases__\"):\n"
    "    for k in obj.__bases__:\n"
    "      printObjectDict(k)\n"
    "  if hasattr(obj, \"__class__\") and obj.__class__ != type(type):\n"
    "    printObjectDict(obj.__class__)\n"
    "";

static const QString printObjectClassFunction = "def printObjectClass(obj):\n"
                                                "  type = \"\"\n"
                                                "  if obj and hasattr(obj, \"__class__\"):\n"
                                                "    if hasattr(obj.__class__, \"__module__\"):\n"
                                                "      mod = obj.__class__.__module__\n"
                                                "      if mod == \"_tulip\":"
                                                "        mod = \"tlp\"\n"
                                                "      type = mod + \".\"\n"
                                                "    if hasattr(obj.__class__, \"__name__\"):\n"
                                                "      type = type + obj.__class__.__name__\n"
#if PY_MAJOR_VERSION >= 3
                                                "    print(type)\n"
#else
                                                "    print type\n"
#endif
                                                "";

#if PY_MAJOR_VERSION >= 3
static QString convertPythonUnicodeObjectToQString(PyObject *pyUnicodeObj) {
  PyObject *utf8Str = PyUnicode_AsUTF8String(pyUnicodeObj);
  QString ret = QString::fromUtf8(PyBytes_AsString(utf8Str));
  decrefPyObject(utf8Str);
  return ret;
}
#endif

static bool scriptPaused = false;
static bool processQtEvents = false;

QTime timer;

class SleepSimulator {
  QMutex localMutex;
  QWaitCondition sleepSimulator;

public:
  SleepSimulator() {
    localMutex.lock();
  }
  ~SleepSimulator() {
    localMutex.unlock();
  }

  void sleep(unsigned long sleepMS) {
    sleepSimulator.wait(&localMutex, sleepMS);
  }
};

static SleepSimulator ss;

void tlp::decrefPyObject(PyObject *obj) {
  Py_XDECREF(obj);
}

int tracefunc(PyObject *, PyFrameObject *, int what, PyObject *) {

  if (what == PyTrace_LINE) {
    if (processQtEvents && !scriptPaused && timer.elapsed() >= 50) {
      if (QApplication::hasPendingEvents()) {
        QApplication::processEvents();
      }

      timer.start();
    }

    while (scriptPaused) {
      if (processQtEvents && QApplication::hasPendingEvents())
        QApplication::processEvents();

      ss.sleep(30);
    }
  }

  return 0;
}

const QString PythonInterpreter::pythonPluginsPath(tlpStringToQString(tlp::TulipLibDir) +
                                                   "tulip/python/");

const QString PythonInterpreter::pythonPluginsPathHome(QDir::homePath() + "/.Tulip-" +
                                                       TULIP_MM_VERSION + "/plugins/python");

const char PythonInterpreter::pythonReservedCharacters[] = {
    '#', '%',  '/',  '+', '-', '&', '*', '<', '>', '|', '~', '^', '=',
    '!', '\'', '\"', '{', '}', '(', ')', '[', ']', '.', ':', '@', 0};

const char *PythonInterpreter::pythonKeywords[] = {
    "def",     "class",    "from",  "in",     "and",  "or",    "not",    "is",
    "with",    "assert",   "for",   "while",  "if",   "elif",  "import", "True",
    "False",   "pass",     "exec",  "else",   "None", "print", "global", "return",
    "break",   "continue", "as",    "lambda", "del",  "try",   "except", "raise",
    "finally", "yield",    "async", "await",  0};

PythonInterpreter PythonInterpreter::_instance;

#ifdef _MSC_VER
extern "C" {
BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
  switch (ul_reason_for_call) {
  case DLL_PROCESS_ATTACH:

    if (QApplication::instance()) {
      PythonInterpreter::getInstance()->initConsoleOutput();
    }

    break;

  case DLL_THREAD_ATTACH:
  case DLL_THREAD_DETACH:
    break;

  case DLL_PROCESS_DETACH:
    break;
  }

  return TRUE;
}
}
#endif

PythonInterpreter::PythonInterpreter()
    : _wasInit(false), _runningScript(false), _defaultConsoleWidget(NULL), _outputEnabled(true),
      _errorOutputEnabled(true) {

  if (Py_IsInitialized()) {
    _wasInit = true;
  }

  if (!_wasInit) {

    int argc = 1;
#if PY_MAJOR_VERSION >= 3
    static const std::wstring argv0 = L"tulip";
    wchar_t *argv[1];
    argv[0] = const_cast<wchar_t *>(argv0.c_str());
#else
    static const std::string argv0 = "tulip";
    char *argv[1];
    argv[0] = const_cast<char *>(argv0.c_str());
#endif

    Py_OptimizeFlag = 1;
    Py_NoSiteFlag = 1;

// Fix for GDB debugging on windows when compiling with MinGW.
// GDB contains an embedded Python interpreter that messes up Python Home value.
// When Tulip is compiled with a version of Python different from the one embedded in GDB,
// it crashes at startup when running it through GDB.
// So reset correct one to be able to debug it.
#ifdef __MINGW32__
    QString pythonHome = PythonVersionChecker::getPythonHome();

    if (!pythonHome.isEmpty()) {
#if PY_MAJOR_VERSION >= 3
      static std::wstring pythonHomeWString = pythonHome.toStdWString();
      Py_SetPythonHome(const_cast<wchar_t *>(pythonHomeWString.c_str()));
#else
      static std::string pythonHomeString = QStringToTlpString(pythonHome);
      Py_SetPythonHome(const_cast<char *>(pythonHomeString.c_str()));
#endif
    }

#endif

    Py_InitializeEx(0);

    PySys_SetArgv(argc, argv);

    PyEval_InitThreads();
#ifndef WIN32
    mainThreadState = PyEval_SaveThread();
#else
    PyThreadState *tcur = PyThreadState_Get();
    PyThreadState_Swap(NULL);
#if PY_MAJOR_VERSION != 3 && PY_MINOR_VERSION != 3
    PyThreadState_Clear(tcur);
#endif
    PyThreadState_Delete(tcur);
#endif

#if defined(WIN32) || (PY_MAJOR_VERSION < 3 && PY_MINOR_VERSION < 6)
    PyEval_ReleaseLock();
#endif
  }

  holdGIL();

  importModule("sys");

#if PY_MAJOR_VERSION >= 3
  PyObject *pName = PyUnicode_FromString("__main__");
#else
  PyObject *pName = PyString_FromString("__main__");
#endif
  PyObject *pMainModule = PyImport_Import(pName);
  decrefPyObject(pName);
  PyObject *pMainDict = PyModule_GetDict(pMainModule);
  PyObject *pVersion = PyRun_String("str(sys.version_info[0])+\".\"+str(sys.version_info[1])",
                                    Py_eval_input, pMainDict, pMainDict);

#if PY_MAJOR_VERSION >= 3
  _pythonVersion = convertPythonUnicodeObjectToQString(pVersion);
#else
  _pythonVersion = QString(PyString_AsString(pVersion));
#endif

  // checking if a QApplication is instanced before instancing any QWidget
  // allow to avoid segfaults when trying to instantiate the plugin outside the Tulip GUI (for
  // instance, with tulip_check_pl)
  if (QApplication::instance() && !_wasInit) {

#if PY_MAJOR_VERSION < 3
    reloadModule("sys");
    runString("sys.setdefaultencoding('utf-8')");
#endif

// hack for linux in order to be able to load dynamic python modules installed on the system (like
// numpy, matplotlib and other cool stuffs)
#ifndef WIN32
    QString libPythonName = QString("libpython") + _pythonVersion;
#ifdef __APPLE__
    libPythonName += QString(".dylib");
#else
    libPythonName += QString(".so.1.0");
#endif

    if (!dlopen(QStringToTlpString(libPythonName).c_str(), RTLD_LAZY | RTLD_GLOBAL)) {

      // for Python 3.2
      libPythonName = QString("libpython") + _pythonVersion + QString("mu");
#ifdef __APPLE__
      libPythonName += QString(".dylib");
#else
      libPythonName += QString(".so.1.0");
#endif

      if (!dlopen(QStringToTlpString(libPythonName).c_str(), RTLD_LAZY | RTLD_GLOBAL)) {
        // for Python 3.3
        libPythonName = QString("libpython") + _pythonVersion + QString("m");
#ifdef __APPLE__
        libPythonName += QString(".dylib");
#else
        libPythonName += QString(".so.1.0");
#endif
        dlopen(QStringToTlpString(libPythonName).c_str(), RTLD_LAZY | RTLD_GLOBAL);
      }
    }

#endif

#if !defined(_MSC_VER)
    initConsoleOutput();
#endif

    if (interpreterInit()) {

      addModuleSearchPath(pythonPluginsPath, true);
      addModuleSearchPath(pythonPluginsPathHome);

#if defined(__APPLE__)
      addModuleSearchPath(tlpStringToQString(tlp::TulipLibDir) + "../lib/tulip/python", true);
#else
      addModuleSearchPath(tlpStringToQString(tlp::TulipLibDir) + "/tulip/python", true);
#endif

      initconsoleutils();
      inittuliputils();

      // Try to import site package manually otherwise Py_InitializeEx can crash if Py_NoSiteFlag is
      // not set
      // and if the site module is not present on the host system
      // Disable output while trying to import the module to not confuse the user
      runString("import site");
#if PY_MAJOR_VERSION == 3 && PY_MINOR_VERSION >= 3
      runString("site.main()");
#endif
      runString("from tulip import tlp");
      runString("from tulipgui import tlpgui");

      // When importing the tulip module, Tulip Python plugins and
      // startup scripts will be possibly loaded and other Python modules can be loaded as a side
      // effect.
      // Some external modules (like numpy) overrides the SIGINT handler at import,
      // so reinstall the default one, otherwise Tulip can not be interrupted by hitting Ctrl-C in a
      // console
      setDefaultSIGINTHandler();

      runString(printObjectDictFunction);
      runString(printObjectClassFunction);

      runString("import sys;"
                "import tuliputils;"
                "import consoleutils;"
                "sys.stdout = consoleutils.ConsoleOutput(False);"
                "sys.stderr = consoleutils.ConsoleOutput(True);"
                "sys.stdin = consoleutils.ConsoleInput()\n");
    }

    PyEval_SetTrace(tracefunc, NULL);

// disable exit and quit functions
#if PY_MAJOR_VERSION >= 3
    runString("import builtins;"
              "builtins.exit = lambda *args: None;"
              "builtins.quit= lambda *args: None;");
#else
    runString("import __builtin__;"
              "__builtin__.exit = lambda *args: None;"
              "__builtin__.quit = lambda *args: None;");
#endif

    runString("import sys;"
              "sys.exit = lambda *args: None");
  }

  releaseGIL();
}

PythonInterpreter::~PythonInterpreter() {
  processQtEvents = false;

  if (!_wasInit && interpreterInit()) {
    consoleOuputString = "";

#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))

    if (QApplication::instance()) {

      // This is a hack to prevent segfaults when the PyQt4 module has been imported
      // during the Python session. Seems there is some garbage collection issue
      // on Qt objects wrapped by SIP. After looking at the SIP source code, the
      // segfault is raised when the sipQtSupport->qt_find_sipslot function is called.
      // So reset the sipQtSupport pointer to NULL, this way the problematic function will no
      // more be called when the Python interpreter is finalized.
      setOutputEnabled(false);
      setErrorOutputEnabled(false);
#ifdef TULIP_SIP
      runString("import tulipsip; sys.stdout.write(tulipsip.__file__)");
#else
      runString("import sip; sys.stdout.write(sip.__file__)");
#endif
      QString sipModulePath = consoleOuputString;
      sipQtAPI **sipQtSupport =
          static_cast<sipQtAPI **>(QLibrary::resolve(sipModulePath, "sipQtSupport"));

      if (sipQtSupport)
        *sipQtSupport = NULL;
    }

#endif

    runString(
        "sys.stdout = sys.__stdout__; sys.stderr = sys.__stderr__; sys.stdin = sys.__stdin__\n");
#ifndef WIN32
    PyEval_ReleaseLock();
    PyEval_RestoreThread(mainThreadState);
#else
    holdGIL();
#endif
    Py_Finalize();
  }

  delete consoleOuputEmitter;
  consoleOuputEmitter = NULL;
  delete consoleOuputHandler;
  consoleOuputHandler = NULL;
}

PythonInterpreter *PythonInterpreter::getInstance() {
  return &_instance;
}

void PythonInterpreter::initConsoleOutput() {
  consoleOuputHandler = new ConsoleOutputHandler();
  consoleOuputEmitter = new ConsoleOutputEmitter();
  QObject::connect(
      consoleOuputEmitter, SIGNAL(consoleOutput(QAbstractScrollArea *, const QString &, bool)),
      consoleOuputHandler, SLOT(writeToConsole(QAbstractScrollArea *, const QString &, bool)));
}

bool PythonInterpreter::interpreterInit() {
  holdGIL();
  bool ret = Py_IsInitialized();
  releaseGIL();
  return ret;
}

bool PythonInterpreter::importModule(const QString &moduleName) {
  return runString(QString("import ") + moduleName);
}

bool PythonInterpreter::registerNewModuleFromString(const QString &moduleName,
                                                    const QString &moduleSrcCode) {
  bool ret = true;
  holdGIL();
  PyObject *pycomp =
      Py_CompileString(QStringToTlpString(moduleSrcCode).c_str(),
                       QStringToTlpString(moduleName + ".py").c_str(), Py_file_input);

  if (pycomp == NULL) {
    PyErr_Print();
    PyErr_Clear();
    ret = false;
  } else {

    PyObject *pmod =
        PyImport_ExecCodeModule(const_cast<char *>(QStringToTlpString(moduleName).c_str()), pycomp);

    if (pmod == NULL) {
      PyErr_Print();
      PyErr_Clear();
      ret = false;
    }
  }

  releaseGIL();
  return ret;
}

bool PythonInterpreter::functionExists(const QString &moduleName, const QString &functionName) {
  holdGIL();
#if PY_MAJOR_VERSION >= 3
  PyObject *pName = PyUnicode_FromString(QStringToTlpString(moduleName).c_str());
#else
  PyObject *pName = PyString_FromString(QStringToTlpString(moduleName).c_str());
#endif
  PyObject *pModule = PyImport_Import(pName);
  decrefPyObject(pName);
  PyObject *pDict = PyModule_GetDict(pModule);
  PyObject *pFunc = PyDict_GetItemString(pDict, QStringToTlpString(functionName).c_str());
  bool ret = (pFunc != NULL && PyCallable_Check(pFunc));
  releaseGIL();
  return ret;
}

bool PythonInterpreter::runString(const QString &pythonCode, const QString &scriptFilePath) {
  if (scriptFilePath != "")
    mainScriptFileName = scriptFilePath;

  timer.start();
  int ret = 0;
  holdGIL();
  ret = PyRun_SimpleString(QStringToTlpString(pythonCode).c_str());

  if (PyErr_Occurred()) {
    PyErr_Print();
    PyErr_Clear();
  }

  releaseGIL();

  if (scriptFilePath != "")
    mainScriptFileName = "";

  return ret != -1;
}

PyObject *PythonInterpreter::evalPythonStatement(const QString &pythonStatement, bool singleInput) {
  holdGIL();
#if PY_MAJOR_VERSION >= 3
  PyObject *pName = PyUnicode_FromString("__main__");
#else
  PyObject *pName = PyString_FromString("__main__");
#endif
  PyObject *pMainModule = PyImport_Import(pName);
  decrefPyObject(pName);
  PyObject *pMainDict = PyModule_GetDict(pMainModule);

  PyObject *ret = PyRun_String(QStringToTlpString(pythonStatement).c_str(),
                               singleInput ? Py_single_input : Py_eval_input, pMainDict, pMainDict);

  if (PyErr_Occurred()) {
    PyErr_Print();
    PyErr_Clear();
  }

  releaseGIL();

  return ret;
}

PyObject *PythonInterpreter::callPythonFunction(const QString &module, const QString &function,
                                                const tlp::DataSet &parameters) {
  holdGIL();
  PyObject *ret = NULL;
#if PY_MAJOR_VERSION >= 3
  PyObject *pName = PyUnicode_FromString(QStringToTlpString(module).c_str());
#else
  PyObject *pName = PyString_FromString(QStringToTlpString(module).c_str());
#endif

  PyObject *pModule = PyImport_Import(pName);
  decrefPyObject(pName);

  PyObject *pDict = PyModule_GetDict(pModule);
  decrefPyObject(pModule);

  PyObject *pFunc = PyDict_GetItemString(pDict, QStringToTlpString(function).c_str());

  if (PyCallable_Check(pFunc)) {
    PyObject *argTup = PyTuple_New(parameters.size());
    int idx = 0;
    bool paramError = false;
    std::pair<std::string, DataType *> param;
    forEach(param, parameters.getValues()) {
      PyObject *pyParam = getPyObjectFromDataType(param.second);

      if (!pyParam) {
        paramError = true;
        break;
      }

      PyTuple_SetItem(argTup, idx++, pyParam);
    }

    if (!paramError) {
      ret = PyObject_CallObject(pFunc, argTup);

      if (PyErr_Occurred()) {
        PyErr_Print();
        PyErr_Clear();
      }
    }

    decrefPyObject(argTup);
  }

  releaseGIL();
  return ret;
}

bool PythonInterpreter::callFunction(const QString &module, const QString &function,
                                     const tlp::DataSet &parameters) {
  holdGIL();
  bool ok = false;
  PyObject *ret = callPythonFunction(module, function, parameters);

  if (ret) {
    ok = true;
  }

  decrefPyObject(ret);
  releaseGIL();
  return ok;
}

void PythonInterpreter::addModuleSearchPath(const QString &path, const bool beforeOtherPaths) {
  if (_currentImportPaths.find(path) == _currentImportPaths.end()) {
    QString pythonCode;
    QTextStream oss(&pythonCode);
    oss << "import sys" << endl;

    if (beforeOtherPaths) {
      oss << "sys.path.insert(0, \"" << path << "\")" << endl;
    } else {
      oss << "sys.path.append(\"" << path << "\")" << endl;
    }

    runString(pythonCode);
    _currentImportPaths.insert(path);
  }
}

bool PythonInterpreter::runGraphScript(const QString &module, const QString &function,
                                       tlp::Graph *graph, const QString &scriptFilePath) {

  if (scriptFilePath != "")
    mainScriptFileName = scriptFilePath;

  timer.start();

  holdGIL();

  clearTracebacks();

  // ensure to reset the trace function in order to be able to pause a script (need that call for
  // that feature to work on windows platform)
  PyEval_SetTrace(tracefunc, NULL);

  bool ret = true;
  scriptPaused = false;

// Build the name object
#if PY_MAJOR_VERSION >= 3
  PyObject *pName = PyUnicode_FromString(QStringToTlpString(module).c_str());
#else
  PyObject *pName = PyString_FromString(QStringToTlpString(module).c_str());
#endif
  // Load the module object
  PyObject *pModule = PyImport_Import(pName);
  decrefPyObject(pName);

  // If the module is not __main__, reload it (needed to avoid errors when calling the
  // runGraphScript
  // function from Python)
  if (module != "__main__") {
    PyObject *pModuleReloaded = PyImport_ReloadModule(pModule);
    decrefPyObject(pModule);
    pModule = pModuleReloaded;
  }

  // pDict is a borrowed reference
  PyObject *pDict = PyModule_GetDict(pModule);

  // pFunc is also a borrowed reference
  PyObject *pFunc = PyDict_GetItemString(pDict, QStringToTlpString(function).c_str());

  if (PyCallable_Check(pFunc)) {

    if (sipAPI() == NULL) {
      QMessageBox::critical(NULL, QObject::trUtf8("failed to initalize Python"),
                            QObject::trUtf8("SIP could not be initialized (sipApi is null)"));
      return false;
    }

    // Getting proper sipWrapperType
    const sipTypeDef *kpTypeDef = sipFindType("tlp::Graph");

    if (!kpTypeDef) {
      return false;
    }

    // Wrapping up C++ instance
    PyObject *pArgs = sipConvertFromType(graph, kpTypeDef, NULL);

    // Finally calling 'process'
    PyObject *argTup = Py_BuildValue("(O)", pArgs);
    _runningScript = true;

    try {
      PyObject_CallObject(pFunc, argTup);
    } catch (std::exception &e) {
      std::ostringstream oss;
      oss << "A C++ exception (" << e.what() << ") has been thrown while executing the script";
      PyErr_SetString(PyExc_Exception, oss.str().c_str());
    } catch (...) {
      PyErr_SetString(PyExc_Exception,
                      "A C++ exception has been thrown while executing the script");
    }

    _runningScript = false;
    decrefPyObject(argTup);
    decrefPyObject(pArgs);
    decrefPyObject(pModule);

    if (PyErr_Occurred()) {
      PyErr_Print();
      PyErr_Clear();
      ret = false;
    }

  } else {
    PyErr_Print();
    PyErr_Clear();
    ret = false;
  }

  releaseGIL();

  if (scriptFilePath != "")
    mainScriptFileName = "";

  return ret;
}

int stopScript(void *) {
  PyErr_SetString(PyExc_Exception, "Script execution terminated by user");
  return -1;
}

void PythonInterpreter::stopCurrentScript() {
  if (_runningScript) {
    holdGIL();
    Py_AddPendingCall(&stopScript, NULL);
    releaseGIL();
    scriptPaused = false;
  }
}

void PythonInterpreter::deleteModule(const QString &moduleName) {
  QString pythonCode;
  QTextStream oss(&pythonCode);
  oss << "import sys" << endl;
  oss << "if \"" << moduleName << "\" in sys.modules:" << endl;
  oss << "  del sys.modules[\"" << moduleName << "\"]" << endl;
  runString(pythonCode);
}

bool PythonInterpreter::reloadModule(const QString &moduleName) {
  QString pythonCode;
  QTextStream oss(&pythonCode);
  oss << "import sys" << endl;
  oss << "if sys.version_info[0] == 3:" << endl;
  oss << "  from imp import reload" << endl;
  oss << "import " << moduleName << endl;
  oss << "reload(" << moduleName << ")" << endl;
  return runString(pythonCode);
}

void PythonInterpreter::setDefaultConsoleWidget(QPlainTextEdit *console) {
  setDefaultConsoleWidget(static_cast<QAbstractScrollArea *>(console));
}

void PythonInterpreter::setConsoleWidget(QPlainTextEdit *console) {
  setConsoleWidget(static_cast<QAbstractScrollArea *>(console));
}

void PythonInterpreter::setDefaultConsoleWidget(QTextBrowser *console) {
  setDefaultConsoleWidget(static_cast<QAbstractScrollArea *>(console));
}

void PythonInterpreter::setConsoleWidget(QTextBrowser *console) {
  setConsoleWidget(static_cast<QAbstractScrollArea *>(console));
}

void PythonInterpreter::setDefaultConsoleWidget(QAbstractScrollArea *console) {
  _defaultConsoleWidget = console;
}

void PythonInterpreter::setConsoleWidget(QAbstractScrollArea *console) {
  setOutputEnabled(true);
  setErrorOutputEnabled(true);

  if (consoleOuputEmitter) {
    consoleOuputEmitter->setConsoleWidget(console);
  }
}

void PythonInterpreter::resetConsoleWidget() {
  setOutputEnabled(true);
  setErrorOutputEnabled(true);

  if (consoleOuputEmitter) {
    consoleOuputEmitter->setConsoleWidget(NULL);
  }
}

void PythonInterpreter::setDefaultSIGINTHandler() {
  setOutputEnabled(false);
  setErrorOutputEnabled(false);

  if (runString("import signal")) {
    runString("signal.signal(signal.SIGINT, signal.SIG_DFL)");
  }

  setOutputEnabled(true);
  setErrorOutputEnabled(true);
}

QString PythonInterpreter::getPythonShellBanner() {
  holdGIL();
  QString ret =
      QString("Python ") + QString(Py_GetVersion()) + QString(" on ") + QString(Py_GetPlatform());
  releaseGIL();
  return ret;
}

QVector<QString> PythonInterpreter::getGlobalDictEntries(const QString &prefixFilter) {
  QVector<QString> ret;
  QSet<QString> publicMembersSorted;
  setOutputEnabled(false);
  setErrorOutputEnabled(false);
  consoleOuputString = "";
  runString("import __main__;printObjectDict(__main__)");
  QStringList objectDictList = consoleOuputString.split("\n");

  for (int i = 0; i < objectDictList.count(); ++i) {
    if (objectDictList[i] != "") {
      if (objectDictList[i].startsWith("_")) {
        continue;
      } else {
        if (prefixFilter != "") {
          if (objectDictList[i].startsWith(prefixFilter)) {
            publicMembersSorted.insert(objectDictList[i]);
          }
        } else {
          publicMembersSorted.insert(objectDictList[i]);
        }
      }
    }
  }

  QSet<QString>::iterator it;

  for (it = publicMembersSorted.begin(); it != publicMembersSorted.end(); ++it) {
    ret.push_back(*it);
  }

  setOutputEnabled(true);
  setErrorOutputEnabled(true);
  return ret;
}

QString PythonInterpreter::getVariableType(const QString &varName) {
  // avoid any function call
  if (varName.contains('('))
    return "";

  setOutputEnabled(false);
  setErrorOutputEnabled(false);
  consoleOuputString = "";
  bool ok = runString(QString("printObjectClass(") + varName + ")");
  setOutputEnabled(true);
  setErrorOutputEnabled(true);

  if (ok)
    return consoleOuputString.mid(0, consoleOuputString.size() - 1);
  else
    return "";
}

QVector<QString> PythonInterpreter::getObjectDictEntries(const QString &objectName,
                                                         const QString &prefixFilter) {
  QVector<QString> ret;
  QSet<QString> publicMembersSorted;
  setOutputEnabled(false);
  setErrorOutputEnabled(false);
  consoleOuputString = "";

  if (runString(objectName)) {
    runString(QString("printObjectDict(") + objectName + ")");
    QStringList objectDictList = consoleOuputString.split("\n");

    for (int i = 0; i < objectDictList.count(); ++i) {
      if (objectDictList[i] != "") {
        if (objectDictList[i].startsWith("_")) {
          continue;
        } else {
          if (prefixFilter != "") {
            if (objectDictList[i].startsWith(prefixFilter)) {
              publicMembersSorted.insert(objectDictList[i]);
            }
          } else {
            publicMembersSorted.insert(objectDictList[i]);
          }
        }
      }
    }

    QSet<QString>::iterator it;

    for (it = publicMembersSorted.begin(); it != publicMembersSorted.end(); ++it) {
      ret.push_back(*it);
    }
  }

  setOutputEnabled(true);
  setErrorOutputEnabled(true);
  return ret;
}

QVector<QString> PythonInterpreter::getImportedModulesList() {
  QVector<QString> ret;
  setOutputEnabled(false);
  setErrorOutputEnabled(false);
  consoleOuputString = "";
#if PY_MAJOR_VERSION >= 3

  if (runString("import sys\nfor mod in sorted(sys.modules.keys()): print(mod)")) {
#else

  if (runString("import sys\nfor mod in sorted(sys.modules.keys()): print mod")) {
#endif
    QStringList modulesList = consoleOuputString.split("\n");

    for (int i = 0; i < modulesList.count(); ++i) {
      if (modulesList[i] != "") {
        if (modulesList[i].startsWith("_")) {
          continue;
        } else {
          ret.push_back(modulesList[i]);
        }
      }
    }
  }

  setOutputEnabled(true);
  setErrorOutputEnabled(true);
  return ret;
}

QVector<QString> PythonInterpreter::getBaseTypesForType(const QString &typeName) {
  QVector<QString> ret;
  setOutputEnabled(false);
  setErrorOutputEnabled(false);
  QStringList modules = typeName.split(".");
  QString curModule = "";

  for (int i = 0; i < modules.size() - 1; ++i) {
    curModule += modules[i];
    runString(QString("import ") + curModule);
    curModule += ".";
  }

  consoleOuputString = "";

  QString pythonCode;

#if PY_MAJOR_VERSION >= 3
  pythonCode = QString("for base in ") + typeName + ".__bases__ : print(base)";
#else
  pythonCode = QString("for base in ") + typeName + ".__bases__ : print base";
#endif

  if (runString(pythonCode)) {
    QStringList basesList = consoleOuputString.split("\n");

    for (int i = 0; i < basesList.count(); ++i) {
      int pos = basesList[i].indexOf('\'');
      int pos2 = basesList[i].lastIndexOf('\'');
      ret.push_back(basesList[i].mid(pos + 1, pos2 - pos - 1));
    }
  }

  setOutputEnabled(true);
  setErrorOutputEnabled(true);
  return ret;
}

void PythonInterpreter::holdGIL() {
  if (!_wasInit)
    gilState = PyGILState_Ensure();
}

void PythonInterpreter::releaseGIL() {
  if (!_wasInit)
    PyGILState_Release(gilState);
}

QString PythonInterpreter::getStandardOutput() const {
  return consoleOuputString;
}

QString PythonInterpreter::getStandardErrorOutput() const {
  return consoleErrorOuputString;
}

void PythonInterpreter::clearOutputBuffers() {
  consoleOuputString = "";
  consoleErrorOuputString = "";
}

void PythonInterpreter::pauseCurrentScript(const bool pause) {
  bool wasPaused = scriptPaused;
  scriptPaused = pause;

  if (!wasPaused && scriptPaused) {
    emit scriptExecutionPaused();
  }
}

bool PythonInterpreter::isScriptPaused() const {
  return scriptPaused;
}

void PythonInterpreter::setProcessQtEventsDuringScriptExecution(bool processEvents) {
  processQtEvents = processEvents;
}

void PythonInterpreter::setOutputEnabled(const bool enableOutput) {
  _outputEnabled = enableOutput;
}

bool PythonInterpreter::outputEnabled() const {
  return _outputEnabled;
}

void PythonInterpreter::setErrorOutputEnabled(const bool enableOutput) {
  _errorOutputEnabled = enableOutput;
}

bool PythonInterpreter::errorOutputEnabled() const {
  return _errorOutputEnabled;
}

double PythonInterpreter::getPythonVersion() const {
  return atof(QStringToTlpString(_pythonVersion).c_str());
}

void PythonInterpreter::sendOutputToConsole(const QString &output, bool stdErr) {
  bool textOutput = false;

  if (consoleOuputEmitter) {
    if ((outputEnabled() && !stdErr) || (errorOutputEnabled() && stdErr)) {
      consoleOuputEmitter->sendOutputToConsole(output, stdErr);
    }
  } else {
    textOutput = true;
  }

  if (textOutput) {
    if (errorOutputEnabled() && stdErr) {
      std::cerr << QStringToTlpString(output);
    } else if (outputEnabled() && !stdErr) {
      std::cout << QStringToTlpString(output);
    }
  }
}

QString PythonInterpreter::readLineFromConsole() {
  if (consoleOuputEmitter && consoleOuputEmitter->consoleWidget()) {
    ConsoleInputHandler cih;
    cih.setConsoleWidget(consoleOuputEmitter->consoleWidget());
    cih.startReadLine();

    while (!cih.lineRead()) {
      QApplication::processEvents();
      ss.sleep(30);
    }

    return cih.line();
  }

  return "";
}

void PythonInterpreter::clearTracebacks() {
  QString pythonCode = "import sys\n";
#if PY_MAJOR_VERSION < 3
  pythonCode += "sys.exc_clear()\n";
  pythonCode += "sys.exc_traceback = None\n";
#endif
  pythonCode += "sys.last_traceback = None\n";
  runString(pythonCode);
}

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif
