/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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

#include "PythonIncludes.h"
#include "PythonInterpreter.h"
#include "ConsoleOutputModule.h"
#include "TulipUtilsModule.h"

#include <iostream>
#include <sstream>

#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QApplication>
#include <QtGui/QPushButton>
#include <QtCore/QTime>

#include <cstdio>
#ifndef WIN32
#include <dlfcn.h>
#endif

using namespace std;

#ifndef WIN32
static PyThreadState*  mainThreadState;
#endif

static PyGILState_STATE gilState;

static const string printObjectDictFunction =
  "def printObjectDict(obj):\n"
  "     if hasattr(obj, \"__dict__\"):\n"
  "         for k in obj.__dict__.keys():\n"
#if PY_MAJOR_VERSION >= 3
  "             print(k)\n"
#else
  "             print k\n"
#endif
  "     if hasattr(obj, \"__bases__\"):\n"
  "         for k in obj.__bases__:\n"
  "             printObjectDict(k)\n"
  "     if hasattr(obj, \"__class__\") and obj.__class__ != type(type):\n"
  "         printObjectDict(obj.__class__)\n"
  ""
  ;

static const string printObjectClassFunction =
  "def printObjectClass(obj):\n"
  "	type = \"\"\n"
  "	if obj and hasattr(obj, \"__class__\"):\n"
  "		if hasattr(obj.__class__, \"__module__\"):\n"
  "			mod = obj.__class__.__module__\n"
  "			if mod == \"tulip\":"
  "				mod = \"tlp\"\n"
  "			type = mod + \".\"\n"
  "		if hasattr(obj.__class__, \"__name__\"):\n"
  "			type = type + obj.__class__.__name__\n"
#if PY_MAJOR_VERSION >= 3
  "		print(type)\n"
#else
  "		print type\n"
#endif
  ""
  ;


#if PY_MAJOR_VERSION >= 3
static std::string convertPythonUnicodeObjectToStdString(PyObject *pyUnicodeObj) {
  std::string ret;
  Py_ssize_t dataSize = PyUnicode_GET_DATA_SIZE(pyUnicodeObj);
  const char* data = PyUnicode_AS_DATA(pyUnicodeObj);

  for (Py_ssize_t i = 0 ; i < dataSize ; ++i) {
    ret += std::string(data+i);
  }

  return ret;
}
#endif

ConsoleOutputDialog::ConsoleOutputDialog(QWidget *parent) : QDialog(parent, Qt::Dialog | Qt::WindowStaysOnTopHint) {
  setWindowTitle("Python Interpreter Output");
  consoleWidget = new QPlainTextEdit(this);
  QHBoxLayout *hLayout = new QHBoxLayout();
  QPushButton *clearButton = new QPushButton("Clear");
  connect(clearButton, SIGNAL(clicked()), consoleWidget, SLOT(clear()));
  QPushButton *closeButton = new QPushButton("Close");
  connect(closeButton, SIGNAL(clicked()), this, SLOT(hideConsoleOutputDialog()));
  hLayout->addItem(new QSpacerItem(100, 20, QSizePolicy::Expanding));
  hLayout->addWidget(clearButton);
  hLayout->addWidget(closeButton);
  QVBoxLayout *layout = new QVBoxLayout();
  layout->addWidget(consoleWidget);
  layout->addLayout(hLayout);
  setLayout(layout);
  connect(consoleWidget, SIGNAL(textChanged()), this, SLOT(showOnOutputWrite()));
  resize(400,300);
}


void ConsoleOutputDialog::showOnOutputWrite() {
  if (!isVisible()) {
    move(lastPos);
    show();
  }
}

void ConsoleOutputDialog::hideConsoleOutputDialog() {
  lastPos = pos();
  hide();
}

static bool scriptPaused = false;
static bool processQtEvents = false;

QTime timer;

int tracefunc(PyObject *, PyFrameObject *, int what, PyObject *) {

  if (what == PyTrace_LINE) {
    if (!scriptPaused && timer.elapsed() >= 50) {
      if (processQtEvents && QApplication::hasPendingEvents())
        QApplication::processEvents();

      timer.start();
    }

    while (scriptPaused) {
      if (timer.elapsed() >= 50) {
        if (processQtEvents && QApplication::hasPendingEvents())
          QApplication::processEvents();

        timer.start();
      }
    }

  }

  return 0;
}

PythonInterpreter PythonInterpreter::instance;

#ifdef _MSC_VER
BOOL APIENTRY DllMain(HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
  switch (ul_reason_for_call) {
  case DLL_PROCESS_ATTACH:

    if (QApplication::instance()) {
      PythonInterpreter::getInstance()->initConsoleOutput();
      PythonInterpreter::getInstance()->loadTulipPythonPlugins();
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
#endif

PythonInterpreter::PythonInterpreter() : runningScript(false), consoleDialog(NULL) {

  wasInit = Py_IsInitialized();

  if (wasInit)
    return;

  int argc=1;
#if PY_MAJOR_VERSION >= 3
  wchar_t *argv[1];
  argv[0] = const_cast<wchar_t *>(std::wstring(L"").c_str());
#else
  char *argv[1];
  argv[0] = const_cast<char *>("");
#endif

  Py_OptimizeFlag = 1;
  Py_NoSiteFlag = 1;
  Py_InitializeEx(0);

  PyEval_InitThreads();
#ifndef WIN32
  mainThreadState = PyEval_SaveThread();
#else
  PyThreadState* tcur = PyThreadState_Get() ;
  PyThreadState_Swap(NULL);
  PyThreadState_Clear(tcur);
  PyThreadState_Delete(tcur);
#endif

#if defined(WIN32) || PY_MAJOR_VERSION < 3
  PyEval_ReleaseLock();
#endif

  holdGIL();

  PySys_SetArgv(argc, argv);

  runString("import sys");
#if PY_MAJOR_VERSION >= 3
  PyObject *pName = PyUnicode_FromString("__main__");
#else
  PyObject *pName = PyString_FromString("__main__");
#endif
  PyObject *pMainModule = PyImport_Import(pName);
  Py_DECREF(pName);
  PyObject *pMainDict = PyModule_GetDict(pMainModule);
  PyObject *pVersion = PyRun_String("str(sys.version_info[0])+\".\"+str(sys.version_info[1])", Py_eval_input, pMainDict, pMainDict);

#if PY_MAJOR_VERSION >= 3
  pythonVersion = convertPythonUnicodeObjectToStdString(pVersion);
#else
  pythonVersion = string(PyString_AsString(pVersion));
#endif

  // checking if a QApplication is instanced before instancing any QWidget
  // allow to avoid segfaults when trying to instantiate the plugin outside the Tulip GUI (for instance, with tulip_check_pl)
  if (QApplication::instance()) {

    // hack for linux in order to be able to load dynamic python modules installed on the system (like numpy, matplotlib and other cool stuffs)
#ifndef WIN32
    string libPythonName = string("libpython") + pythonVersion;
#ifdef __APPLE__
    libPythonName += string(".dylib");
#else
    libPythonName += string(".so.1.0");
#endif

    if (!dlopen(libPythonName.c_str(), RTLD_LAZY | RTLD_GLOBAL)) {
      libPythonName = string("libpython") + pythonVersion + string("mu");
#ifdef __APPLE__
      libPythonName += string(".dylib");
#else
      libPythonName += string(".so.1.0");
#endif
      dlopen(libPythonName.c_str(), RTLD_LAZY | RTLD_GLOBAL);
    }

#endif

#if !defined(_MSC_VER)
    initConsoleOutput();
#endif

    if (interpreterInit()) {

      addModuleSearchPath(pythonPluginsPath, true);
      addModuleSearchPath(pythonPluginsPathHome);

#if defined(__APPLE__)
      addModuleSearchPath(tlp::TulipLibDir + "../Frameworks/python", true);
#elif defined(WIN32)
      addModuleSearchPath(tlp::TulipLibDir + "../bin/python", true);
#else
      addModuleSearchPath(tlp::TulipLibDir + "/python", true);
#endif
      initscriptengine();
      inittuliputils();

      runString("import sys; import scriptengine ; import tuliputils ; sys.stdout = scriptengine.ConsoleOutput(False); sys.stderr = scriptengine.ConsoleOutput(True);\n");

      // Try to import site package manually otherwise Py_InitializeEx can crash if Py_NoSiteFlag is not set
      // and if the site module is not present on the host system
      // Disable output while trying to import the module to not confuse the user
      outputActivated = false;
      runString("import site");
      runString("import sip");
      runString("from tulip import *");
      runString("from tulipogl import *");
      runString("from tulipgui import *");
      runString("from tulipqt import *");
      outputActivated = true;

#ifndef _MSC_VER
      loadTulipPythonPlugins();
#endif

      runString(printObjectDictFunction);
      runString(printObjectClassFunction);
    }

    PyEval_SetTrace(tracefunc, NULL);
  }

  releaseGIL();

}

PythonInterpreter::~PythonInterpreter() {
  if (wasInit)
    return;

  processQtEvents = false;

  if (interpreterInit()) {
#ifndef WIN32
    PyEval_ReleaseLock();
    PyEval_RestoreThread(mainThreadState);
#else
    holdGIL();
#endif
    Py_Finalize();
  }

  delete consoleDialog;
  delete consoleOuputEmitter;
  consoleOuputEmitter = NULL;
  delete consoleOuputHandler;
  consoleOuputHandler = NULL;
}

PythonInterpreter *PythonInterpreter::getInstance() {
  return &instance;
}

void PythonInterpreter::initConsoleOutput() {
  consoleOuputHandler = new ConsoleOutputHandler();
  consoleOuputEmitter = new ConsoleOutputEmitter();
  consoleDialog = new ConsoleOutputDialog();
  setDefaultConsoleWidget();
}

void PythonInterpreter::loadTulipPythonPlugins() {
  loadTulipPythonPlugins(pythonPluginsPath);
  loadTulipPythonPlugins(pythonPluginsPathHome);
}

void PythonInterpreter::loadTulipPythonPlugins(const std::string &pluginsPath) {
  QDir pythonPluginsDir(pluginsPath.c_str());
  QStringList nameFilter;
  nameFilter << "*.py";
  QFileInfoList fileList = pythonPluginsDir.entryInfoList(nameFilter);

  for (int i = 0 ; i < fileList.size() ; ++i) {
    QFileInfo fileInfo = fileList.at(i);
    QString moduleName = fileInfo.fileName();
    moduleName.replace(".py", "");
    runString(std::string("import ") + moduleName.toUtf8().data());
  }

  // some external modules (like numpy) overrides the SIGINT handler at import
  // reinstall the default one, otherwise Tulip can not be interrupted by hitting Ctrl-C in a console
  setDefaultSIGINTHandler();
}

bool PythonInterpreter::interpreterInit() {
  holdGIL();
  bool ret = Py_IsInitialized();
  releaseGIL();
  return ret;
}

bool PythonInterpreter::registerNewModuleFromString(const std::string &moduleName, const std::string &moduleSrcCode) {
  bool ret = true;
  holdGIL();
  ostringstream oss;
  oss << moduleName << ".py";
  PyObject *pycomp = Py_CompileString(moduleSrcCode.c_str(),oss.str().c_str(), Py_file_input);

  if (pycomp == NULL) {
    PyErr_Print();
    PyErr_Clear();
    ret = false;
  }
  else {
    PyObject *pmod = PyImport_ExecCodeModule(const_cast<char *>(moduleName.c_str()),pycomp);

    if (pmod == NULL) {
      PyErr_Print();
      PyErr_Clear();
      ret = false;
    }
  }

  releaseGIL();
  return ret;
}

bool PythonInterpreter::functionExists(const string &moduleName, const string &functionName) {
  holdGIL();
#if PY_MAJOR_VERSION >= 3
  PyObject *pName = PyUnicode_FromString(moduleName.c_str());
#else
  PyObject *pName = PyString_FromString(moduleName.c_str());
#endif
  PyObject *pModule = PyImport_Import(pName);
  Py_DECREF(pName);
  PyObject *pDict = PyModule_GetDict(pModule);
  PyObject *pFunc = PyDict_GetItemString(pDict, functionName.c_str());
  bool ret = (pFunc != NULL && PyCallable_Check(pFunc));
  releaseGIL();
  return ret;
}

bool PythonInterpreter::runString(const string &pyhtonCode, const std::string &scriptFilePath) {
  if (scriptFilePath != "")
    mainScriptFileName = scriptFilePath.c_str();

  timer.start();
  int ret = 0;
  holdGIL();
  ret = PyRun_SimpleString(pyhtonCode.c_str());

  if (PyErr_Occurred()) {
    PyErr_Print();
    PyErr_Clear();
  }

  releaseGIL();

  if (scriptFilePath != "")
    mainScriptFileName = "";

  return  ret != -1;
}

void PythonInterpreter::addModuleSearchPath(const std::string &path, const bool beforeOtherPaths) {
  if (currentImportPaths.find(path) == currentImportPaths.end()) {
    ostringstream oss;
    oss << "import sys" << endl;

    if (beforeOtherPaths) {
      oss << "sys.path.insert(0, \"" << path << "\")" << endl;
    }
    else {
      oss << "sys.path.append(\"" << path << "\")" << endl;
    }

    runString(oss.str());
    currentImportPaths.insert(path);
  }
}

bool PythonInterpreter::runGraphScript(const string &module, const string &function, tlp::Graph *graph, const std::string &scriptFilePath) {

  if (scriptFilePath != "")
    mainScriptFileName = scriptFilePath.c_str();

  timer.start();

  holdGIL();

  bool ret = true;
  scriptPaused = false;

  // Build the name object
#if PY_MAJOR_VERSION >= 3
  PyObject *pName = PyUnicode_FromString(module.c_str());
#else
  PyObject *pName = PyString_FromString(module.c_str());
#endif
  // Load the module object
  PyObject *pModule = PyImport_Import(pName);
  Py_DECREF(pName);

  // If the module is not __main__, reload it (needed to avoid errors when calling the runGraphScript
  // function from Python)
  if (module != "__main__") {
    PyObject *pModuleReloaded = PyImport_ReloadModule(pModule);
    Py_DECREF(pModule);
    pModule = pModuleReloaded;
  }

  // pDict is a borrowed reference
  PyObject *pDict = PyModule_GetDict(pModule);

  // pFunc is also a borrowed reference
  PyObject *pFunc = PyDict_GetItemString(pDict, function.c_str());

  if (PyCallable_Check(pFunc)) {
    const sipAPIDef *sipApi = get_sip_api();

    // Getting proper sipWrapperType
    const sipTypeDef* kpTypeDef     = sipApi->api_find_type("tlp::Graph");

    // Wrapping up C++ instance
    PyObject* pArgs = sipApi->api_convert_from_type(graph, kpTypeDef, NULL);

    // Finally calling 'process'
    PyObject* argTup = Py_BuildValue ("(O)", pArgs);
    runningScript = true;

    try {
      PyObject_CallObject(pFunc, argTup);
    }
    catch (exception &e) {
      ostringstream oss;
      oss << "A C++ exception (" << e.what() << ") has been thrown while executing the script";
      PyErr_SetString(PyExc_Exception, oss.str().c_str());
    }
    catch(...) {
      PyErr_SetString(PyExc_Exception, "A C++ exception has been thrown while executing the script");
    }

    runningScript = false;
    Py_DECREF(argTup);
    Py_DECREF(pArgs);
    Py_DECREF(pModule);

    if (PyErr_Occurred()) {
      PyErr_Print();
      PyErr_Clear();
      ret = false;
    }
  }
  else {
    PyErr_Print();
    PyErr_Clear();
    ret =  false;
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
  holdGIL();
  Py_AddPendingCall(&stopScript, NULL);
  releaseGIL();
  scriptPaused = false;
}

void PythonInterpreter::deleteModule(const std::string &moduleName) {
  ostringstream oss;
  oss << "import sys" << endl;
  oss << "if \"" << moduleName << "\" in sys.modules:" << endl;
  oss << "\tdel sys.modules[\"" << moduleName << "\"]" << endl;
  runString(oss.str());
}

bool PythonInterpreter::reloadModule(const std::string &moduleName) {
  ostringstream oss;
  oss << "import sys" << endl;
  oss << "if sys.version_info[0] == 3:" << endl;
  oss << "  from imp import reload" << endl;
  oss << "import " << moduleName << endl;
  oss << "reload(" << moduleName << ")" << endl;
  return runString(oss.str());
}

void PythonInterpreter::setConsoleWidget(QAbstractScrollArea *console) {
  if (consoleOuputHandler) {
    consoleOuputEmitter->setOutputActivated(true);
    consoleOuputEmitter->setConsoleWidget(console);
    QObject::disconnect(consoleOuputEmitter, SIGNAL(consoleOutput(QAbstractScrollArea*, const QString &, bool)), consoleOuputHandler, SLOT(writeToConsole(QAbstractScrollArea*, const QString &, bool)));
    QObject::connect(consoleOuputEmitter, SIGNAL(consoleOutput(QAbstractScrollArea*, const QString &, bool)), consoleOuputHandler, SLOT(writeToConsole(QAbstractScrollArea*, const QString &, bool)));
  }
}

void PythonInterpreter::setDefaultConsoleWidget() {
  if (consoleDialog) {
    consoleOuputEmitter->setOutputActivated(true);
    consoleOuputEmitter->setConsoleWidget(consoleDialog->consoleWidget);
    QObject::disconnect(consoleOuputEmitter, SIGNAL(consoleOutput(QAbstractScrollArea*, const QString &, bool)), consoleOuputHandler, SLOT(writeToConsole(QAbstractScrollArea*, const QString &, bool)));
    QObject::connect(consoleOuputEmitter, SIGNAL(consoleOutput(QAbstractScrollArea*, const QString &, bool)), consoleOuputHandler, SLOT(writeToConsole(QAbstractScrollArea*, const QString &, bool)), Qt::QueuedConnection);
  }
}

void PythonInterpreter::setDefaultSIGINTHandler() {
  if (consoleOuputEmitter) {
    consoleOuputEmitter->setOutputActivated(false);
  }

  if (runString("import signal")) {
    runString("signal.signal(signal.SIGINT, signal.SIG_DFL)");
  }

  if (consoleOuputEmitter) {
    consoleOuputEmitter->setOutputActivated(true);
  }
}

std::string PythonInterpreter::getPythonShellBanner() {
  holdGIL();
  string ret = string("Python ") + string(Py_GetVersion()) + string(" on ") + string(Py_GetPlatform());
  releaseGIL();
  return ret;
}

std::vector<std::string> PythonInterpreter::getGlobalDictEntries(const std::string &prefixFilter) {
  std::vector<std::string> ret;
  std::set<std::string> publicMembersSorted;
  outputActivated = false;
  consoleOuputString = "";
  runString("import __main__;printObjectDict(__main__)");
  QStringList objectDictList = QString(consoleOuputString.c_str()).split("\n");

  for (int i = 0 ; i < objectDictList.count() ; ++i) {
    if (objectDictList[i] != "") {
      if (objectDictList[i].startsWith("_")) {
        continue;
      }
      else {
        if (prefixFilter != "") {
          if (objectDictList[i].startsWith(QString(prefixFilter.c_str()))) {
            publicMembersSorted.insert(objectDictList[i].toStdString());
          }
        }
        else {
          publicMembersSorted.insert(objectDictList[i].toStdString());
        }
      }

    }
  }

  set<string>::iterator it;

  for (it = publicMembersSorted.begin() ; it != publicMembersSorted.end() ; ++it) {
    ret.push_back(*it);
  }

  outputActivated = true;
  return ret;
}

std::string PythonInterpreter::getVariableType(const std::string &varName) {
  // avoid any function call
  if (varName.find('(') != std::string::npos)
    return "";

  outputActivated = false;
  consoleOuputString = "";
  bool ok = runString("printObjectClass("+varName+")");
  outputActivated = true;

  if (ok)
    return consoleOuputString.substr(0, consoleOuputString.size() - 1);
  else
    return "";
}

std::vector<std::string> PythonInterpreter::getObjectDictEntries(const std::string &objectName, const std::string &prefixFilter) {
  std::vector<std::string> ret;
  std::set<std::string> publicMembersSorted;
  outputActivated = false;
  consoleOuputString = "";

  if (runString(objectName)) {
    ostringstream oss;
    oss << "printObjectDict(" << objectName << ")";
    runString(oss.str());
    QStringList objectDictList = QString(consoleOuputString.c_str()).split("\n");

    for (int i = 0 ; i < objectDictList.count() ; ++i) {
      if (objectDictList[i] != "") {
        if (objectDictList[i].startsWith("_")) {
          continue;
        }
        else {
          if (prefixFilter != "") {
            if (objectDictList[i].startsWith(QString(prefixFilter.c_str()))) {
              publicMembersSorted.insert(objectDictList[i].toStdString());
            }
          }
          else {
            publicMembersSorted.insert(objectDictList[i].toStdString());
          }
        }

      }
    }

    set<string>::iterator it;

    for (it = publicMembersSorted.begin() ; it != publicMembersSorted.end() ; ++it) {
      ret.push_back(*it);
    }
  }

  outputActivated = true;
  return ret;
}

vector<string> PythonInterpreter::getImportedModulesList() {
  std::vector<std::string> ret;
  outputActivated = false;
  consoleOuputString = "";
#if PY_MAJOR_VERSION >= 3

  if (runString("import sys\nfor mod in sorted(sys.modules.keys()): print(mod)")) {
#else

  if (runString("import sys\nfor mod in sorted(sys.modules.keys()): print mod")) {
#endif
    QStringList modulesList = QString(consoleOuputString.c_str()).split("\n");

    for (int i = 0 ; i < modulesList.count() ; ++i) {
      if (modulesList[i] != "") {
        if (modulesList[i].startsWith("_")) {
          continue;
        }
        else {
          ret.push_back(modulesList[i].toStdString());
        }
      }
    }
  }

  outputActivated = true;
  return ret;
}

vector<string> PythonInterpreter::getBaseTypesForType(const string &typeName) {
  vector<string> ret;
  outputActivated = false;
  ostringstream oss;
  QStringList modules = QString(typeName.c_str()).split(".");
  string curModule = "";

  for (int i = 0 ; i < modules.size() -1 ; ++i) {
    curModule += modules[i].toStdString();
    oss.str("");
    oss << "import " << curModule;
    runString(oss.str());
    curModule += ".";
  }

  consoleOuputString = "";
  oss.str("");
#if PY_MAJOR_VERSION >= 3
  oss << "for base in " << typeName << ".__bases__ : print(base)";
#else
  oss << "for base in " << typeName << ".__bases__ : print base";
#endif

  if (runString(oss.str())) {
    QStringList basesList = QString(consoleOuputString.c_str()).split("\n");

    for (int i = 0 ; i < basesList.count() ; ++i) {
      int pos = basesList[i].indexOf('\'');
      int pos2 = basesList[i].lastIndexOf('\'');
      ret.push_back(basesList[i].mid(pos+1, pos2-pos-1).toStdString());
    }
  }

  outputActivated = true;
  return ret;
}

void PythonInterpreter::holdGIL() {
  gilState = PyGILState_Ensure();
}

void PythonInterpreter::releaseGIL() {
  PyGILState_Release(gilState);
}

std::string PythonInterpreter::getStandardOutput() const {
  return consoleOuputString;
}

std::string PythonInterpreter::getStandardErrorOutput() const {
  return consoleErrorOuputString;
}

void PythonInterpreter::clearOutputBuffers() {
  consoleOuputString = "";
  consoleErrorOuputString = "";
}

void PythonInterpreter::pauseCurrentScript(const bool pause) {
  scriptPaused = pause;
}

bool PythonInterpreter::isScriptPaused() const {
  return scriptPaused;
}

void PythonInterpreter::setProcessQtEventsDuringScriptExecution(bool processEvents) {
  processQtEvents = processEvents;
}

void PythonInterpreter::setOutputEnabled(const bool enableOutput) {
  outputActivated = enableOutput;
}

bool PythonInterpreter::outputEnabled() const {
  return outputActivated;
}
