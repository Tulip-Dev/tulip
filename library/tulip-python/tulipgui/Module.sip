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

%Module tulipgui

// Hack to avoid overriding the tlp Python class (containing all the module classes as
// nested classes and declared in each module : tulip, tulipogl, tulipqt) in the current 
// global namespace when importing the module the following way : from tulipqt import *		 
%PreInitialisationCode
    std::string tlpImportHook =
    "try:\n"
    "   _origimportfunc\n"
    "except NameError:\n"
    "   import sys\n"
#if PY_MAJOR_VERSION >= 3
    "   import builtins\n"
    "   _origimportfunc = builtins.__import__\n"
#else
    "   import __builtin__\n"
    "   _origimportfunc = __builtin__.__import__\n"
#endif
    "   def tlpimporthook(name, globals={}, locals={}, fromlist=[], level=-1):\n"
    "       fullname = name\n"
    "       m = _origimportfunc(name, globals, locals, fromlist, level)\n"
    "       if fullname.startswith(\"tulip\"):\n"
    "           if fromlist and len(fromlist) > 0 and fromlist[0] == \"*\":\n"
    "               curTlpDict = None\n"
    "               if \"tlp\" in locals:\n"
    "                   curTlpDict = locals[\"tlp\"].__dict__\n"
    "                   if curTlpDict:\n"
    "                       m = type(fullname, (object,), dict(m.__dict__))\n"
    "                       m.tlp = type(\"tlp\", (object,), dict(m.tlp.__dict__))\n"
    "                       for k in curTlpDict.keys():\n"
    "                           if not k.startswith(\"_\"):\n"
    "                               setattr(m.tlp, k, curTlpDict[k])\n"
    "       return m\n"
#if PY_MAJOR_VERSION >= 3
    "   builtins.__import__ = tlpimporthook\n"
#else
    "   __builtin__.__import__ = tlpimporthook\n"
#endif
     ;

    PyRun_SimpleString(tlpImportHook.c_str());

    // If there is no QApplication instance, it means that we are working from the classical Python shell
    // or we are executing a script with the interpreter.
    // Instantiate a QApplication but don't run its event loop continually through the exec method.
    // Instead use the same hack as in PyQt4 in order to process qt events when the Python shell
    // is waiting for input.
    if (!QApplication::instance()) {
        installQtInputHook();
        new QApplication(argc, argv);
        if (tlp::TulipPluginsPath.empty()) {
            tlp::initTulipLib();
        }
        tlp::InteractorManager::getInst().loadPlugins();
        tlp::ViewPluginsManager::getInst().loadPlugins();
        tlp::GlyphManager::getInst().loadPlugins();

        std::string resetSIGINTHandler =
            "import signal\n"
            "signal.signal(signal.SIGINT, signal.SIG_DFL)\n";

        PyRun_SimpleString(resetSIGINTHandler.c_str());

        // Check if we are in script execution mode (sys.ps1 is not defined in that case)
        // If so, register an exit callback that will run the Qt event loop if some widgets are
        // still in a visible state
        PyObject *ps1 = PySys_GetObject("ps1");
        if (!ps1 && !Py_InteractiveFlag) {
            std::string installExitFunc =
                "def tulipguiExitFunc():\n"
                "   import tulipgui\n"
                "   tulipgui.tlp.runQtMainLoop()\n"
                "import atexit\n"
                "atexit.register(tulipguiExitFunc)\n";
            PyRun_SimpleString(installExitFunc.c_str());
        }
    }

%End

%ModuleHeaderCode
#include <string>
#include <QtGui/QApplication>

#include <tulip/InteractorManager.h>
#include <tulip/ViewPluginsManager.h>
#include <tulip/GlyphManager.h>

static int argc = 1;
static char *argv[1] = {const_cast<char*>("tulipgui")};
extern void printErrorMessage(const std::string &errMsg);
extern void releaseSIPWrapper(void *wrappedCppObject, const sipTypeDef *td);

extern void installQtInputHook();
extern void removeQtInputHook();
%End

%ModuleCode

void printErrorMessage(const std::string &errMsg) {
    std::string pythonCode = "import sys\n"
                             "sys.stderr.write(\"";
    pythonCode += errMsg;
    pythonCode += "\\n\")";
    PyRun_SimpleString(pythonCode.c_str());
}

void releaseSIPWrapper(void *wrappedCppObject, const sipTypeDef *td) {
    PyObject *pyObj = sipGetPyObject(wrappedCppObject, td);
    if (pyObj) {
        sipTransferTo(pyObj, pyObj);
        sipSimpleWrapper *wrapper = reinterpret_cast<sipSimpleWrapper *>(pyObj);
        sipCommonDtor(wrapper);
    }
}

#include <QCoreApplication>
#include <QThread>

#if defined(Q_OS_WIN32)
#include <conio.h>
#include <QTimer>
#else
#include <QSocketNotifier>
#endif

// This is the input hook that will process events while the interpreter is
// waiting for interactive input (from PyQt4).
extern "C" {static int qtInputHook();}

static int qtInputHook() {
    QCoreApplication *app = QCoreApplication::instance();

    if (app && app->thread() == QThread::currentThread()) {
#if defined(Q_OS_WIN32)
        QTimer timer;
        QObject::connect(&timer, SIGNAL(timeout()), app, SLOT(quit()));

        do {
            timer.start(100);
            QCoreApplication::exec();
            timer.stop();
        } while (!_kbhit());

        QObject::disconnect(&timer, SIGNAL(timeout()), app, SLOT(quit()));
#else
        QSocketNotifier notifier(0, QSocketNotifier::Read, 0);
        QObject::connect(&notifier, SIGNAL(activated(int)), app, SLOT(quit()));
        QCoreApplication::exec();
        QObject::disconnect(&notifier, SIGNAL(activated(int)), app, SLOT(quit()));
#endif
    }

    return 0;
}

void installQtInputHook() {
    PyOS_InputHook = qtInputHook;
}

void removeQtInputHook() {
    PyOS_InputHook = 0;
}

%End

namespace tlp;

%Include View.sip
%Include NodeLinkDiagramComponent.sip
%Include TulipGui.sip

%Import tulip-ogl/Module.sip
