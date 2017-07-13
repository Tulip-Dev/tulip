/*
 *
 * This file is part of Tulip (www.tulip-software.org)
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

// +-------------------------------------------------------------------------+
// | Tulip Python Bindings                                                   |
// | inspired from bindings by the Booggie project development team          |
// | (https://sourceforge.net/projects/booggie/)                             |
// +-------------------------------------------------------------------------+

%Module _tulipgui

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
#ifdef __APPLE__
  // Hack for MacOS to avoid importing the qt_menu.nib file as it is
  // not needed for our purpose and it forbids the tulipgui module to
  // be imported when it has been installed through pip
  QApplication::setAttribute(Qt::AA_MacPluginApplication);
#endif
  new QApplication(argc, argv);

  tlp::initTulipSoftware();
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

namespace tlp {
extern void initInteractorsDependencies();
}

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
        sipInstanceDestroyed(wrapper);
    }
}

namespace tlp {

void initInteractorsDependencies() {
  tlp::InteractorLister::initInteractorsDependencies();
}

}

#include <QCoreApplication>
#include <QThread>

#if defined(Q_OS_WIN32) || defined(Q_OS_WIN)
#include <conio.h>
#include <QTimer>
#else
#include <QSocketNotifier>
#endif

// This is the input hook that will process events while the interpreter is
// waiting for interactive input (from PyQt).
extern "C" {static int qtInputHook();}

static int qtInputHook() {
    QCoreApplication *app = QCoreApplication::instance();

    if (app && app->thread() == QThread::currentThread()) {
#if defined(Q_OS_WIN32) || defined(Q_OS_WIN)
        QTimer timer;
        QObject::connect(&timer, SIGNAL(timeout()), app, SLOT(quit()));

        while (!_kbhit()) {
            timer.start(35);
            QCoreApplication::exec();
            timer.stop();
        }

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

namespace tlp {
  void initInteractorsDependencies();
};

%Include Interactor.sip
%Include View.sip
%Include GlMainView.sip
%Include NodeLinkDiagramComponent.sip
%Include TulipGui.sip
%Include ColorScalesManager.sip

%Import ../tulip-ogl/Module.sip




