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

%PreInitialisationCode

bool initQt = true;

// test if there is an X server running on linux
// special case for PPA build, otherwise Python documentation is not generated
#if defined(__linux)
char *display = getenv("DISPLAY");
initQt = (display != NULL);
#endif

// If there is no QApplication instance, it means that we are working from the classical Python shell
// or we are executing a script with the interpreter.
// Instantiate a QApplication but don't run its event loop continually through the exec method.
// Instead use the same hack as in PyQt4 in order to process qt events when the Python shell
// is waiting for input.
if (!QApplication::instance() && initQt) {
    installQtInputHook();
    new QApplication(argc, argv);

    // fix an ubuntu bug when using the nvidia driver,
    // when the python interpreter dynamically loads the GL library,
    // it ends up loading the mesa one instead of the nvidia one.
    // The use of the tulipgui module through the classical python intrepreter
    // was quite unusable because of that ...
    // Apply the workaround proposed here : https://bugs.launchpad.net/ubuntu/+source/python-qt4/+bug/941826
    std::string fixUbuntuGl =
            "import os\n"
            "if hasattr(os, 'uname'):\n"
            "  if os.uname()[3].lower().find('ubuntu') != -1:\n"
            "    import ctypes.util\n"
            "    ctypes.CDLL(ctypes.util.find_library('GL'), ctypes.RTLD_GLOBAL)\n"
            ;
    PyRun_SimpleString(fixUbuntuGl.c_str());

    tlp::initTulipSoftware();

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
                "   tulipgui.tlpgui.runQtMainLoop()\n"
                "import atexit\n"
                "atexit.register(tulipguiExitFunc)\n"
                ;
        PyRun_SimpleString(installExitFunc.c_str());
    }

}

%End

%PostInitialisationCode
// rename tlp namespace into tlpgui
PyObject *moduleDict = PyModule_GetDict(sipModule);
PyObject *tlpClass = PyDict_GetItemString(moduleDict, "tlp");
PyDict_DelItemString(moduleDict, "tlp");
PyDict_SetItemString(moduleDict, "tlpgui", tlpClass);
%End

%ModuleHeaderCode
#include <string>
#include <cstdlib>
#include <QApplication>

#include <tulip/TlpTools.h>
#include <tulip/TlpQtTools.h>
#include <tulip/PluginLister.h>
#include <tulip/GlyphManager.h>
#include <tulip/EdgeExtremityGlyphManager.h>
#include <tulip/Interactor.h>
#include <tulip/PluginLibraryLoader.h>
#include <tulip/Perspective.h>
#include <tulip/Workspace.h>

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


%Include Interactor.sip
%Include View.sip
%Include GlMainView.sip
%Include NodeLinkDiagramComponent.sip
%Include TulipGui.sip

%Import ../tulip-ogl/Module.sip




