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

#include "PythonInterpreter.h"
#include "ConsoleOutputModule.h"
#include "TulipUtilsModule.h"

#include <iostream>
#include <sstream>

#include <tulip/TulipRelease.h>
#include <tulip/TlpTools.h>

#include <QtCore/QDir>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QApplication>
#include <QtGui/QPushButton>

#include <sip.h>

#include <cstdio>
#ifndef WIN32 
#include <dlfcn.h>
#endif
using namespace std;

static const string printObjectDictFunction =
		"def printObjectDict(obj):\n"
		"	if hasattr(obj, \"__dict__\"):\n"
		"		for k in obj.__dict__.keys():\n"
		"			print k\n"
		"	if hasattr(obj, \"__class__\"):\n"
		"		for k in obj.__class__.__dict__.keys():\n"
		"			print k\n"
		""
		;

const sipAPIDef *get_sip_api(){
#if defined(SIP_USE_PYCAPSULE)
	return (const sipAPIDef *)PyCapsule_Import("sip._C_API", 0);
#else
	PyObject *sip_module;
	PyObject *sip_module_dict;
	PyObject *c_api;

	/* Import the SIP module. */
	sip_module = PyImport_ImportModule("sip");

	if (sip_module == NULL)
		return NULL;

	/* Get the module's dictionary. */
	sip_module_dict = PyModule_GetDict(sip_module);

	/* Get the "_C_API" attribute. */
	c_api = PyDict_GetItemString(sip_module_dict, "_C_API");

	if (c_api == NULL)
		return NULL;

	/* Sanity check that it is the right type. */
	if (!PyCObject_Check(c_api))
		return NULL;

	/* Get the actual pointer from the object. */
	return (const sipAPIDef *)PyCObject_AsVoidPtr(c_api);
#endif
}

ConsoleOutputDialog::ConsoleOutputDialog(QWidget *parent) : QDialog(parent, Qt::Dialog | Qt::WindowStaysOnTopHint) {
	setWindowTitle("Python Interpreter Output");
	consoleWidget = new QPlainTextEdit(this);
	QHBoxLayout *hLayout = new QHBoxLayout();
	QPushButton *clearButton = new QPushButton("Clear");
	connect(clearButton, SIGNAL(clicked()), consoleWidget, SLOT(clear()));
	QPushButton *closeButton = new QPushButton("Close");
	connect(closeButton, SIGNAL(clicked()), this, SLOT(hideConsoleOutputDialog()));
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

PythonInterpreter PythonInterpreter::instance;




PythonInterpreter::PythonInterpreter() : runningScript(false) {
	int argc=1;
	char *argv[1];
	argv[0] = const_cast<char *>("");
	Py_OptimizeFlag = 1;
	Py_NoSiteFlag = 1;
	Py_InitializeEx(0);
	PySys_SetArgv(argc, argv);

	runString("import sys");
	PyObject *pName = PyString_FromString("__main__");
	PyObject *pMainModule = PyImport_Import(pName);
	Py_DECREF(pName);
	PyObject *pMainDict = PyModule_GetDict(pMainModule);
	PyObject *pVersion = PyRun_String("str(sys.version_info[0])+\".\"+str(sys.version_info[1])", Py_eval_input, pMainDict, pMainDict);
	pythonVersion = string(PyString_AsString(pVersion));

	// checking if a QApplication is instanced before instancing any QWidget
	// allow to avoid segfaults when trying to instantiate the plugin outside the Tulip GUI (for instance, with tulip_check_pl)
	if (QApplication::instance()) {

		// hack for linux in order to be able to load dynamic python modules installed on the system (like numpy, matplotlib and other cool stuffs)
#ifndef WIN32	
		string libPythonName = string("libpython") + pythonVersion;
#ifdef __APPLE__
		libPythonName += string(".dylib");
#else
		libPythonName += string(".so");
#endif
		dlopen(libPythonName.c_str(), RTLD_LAZY | RTLD_GLOBAL);
#endif

		consoleDialog = new ConsoleOutputDialog();

		if (interpreterInit()) {

			string pythonPluginsPath = tlp::TulipLibDir + "tulip/python/";
			addModuleSearchPath(pythonPluginsPath, true);

#ifdef __APPLE__
			addModuleSearchPath(tlp::TulipLibDir + "../Frameworks", true);
#else
			addModuleSearchPath(tlp::TulipLibDir, true);
#endif
			// Try to import site package manually otherwise Py_InitializeEx can crash if Py_NoSiteFlag is not set
			// and if the site module is not present on the host system
			// Disable output while trying to import the module to not confuse the user
			outputActivated = false;
			runString("import site");
			outputActivated = true;

			initscriptengine();
			_PyImport_FixupExtension(const_cast<char *>("scriptengine"), const_cast<char *>("scriptengine"));

			inittuliputils();
			_PyImport_FixupExtension(const_cast<char *>("tuliputils"), const_cast<char *>("tuliputils"));

			runString("import sys; import scriptengine ; sys.stdout = scriptengine.ConsoleOutput(False); sys.stderr = scriptengine.ConsoleOutput(True);\n");

			setDefaultConsoleWidget();

			runString("from tulip import *");

			QDir pythonPluginsDir(pythonPluginsPath.c_str());
			QStringList nameFilter;
			nameFilter << "*.py";
			QFileInfoList fileList = pythonPluginsDir.entryInfoList(nameFilter);
			for (int i = 0 ; i < fileList.size() ; ++i) {
				QFileInfo fileInfo = fileList.at(i);
				QString moduleName = fileInfo.fileName();
				moduleName.replace(".py", "");
				runString("import " + moduleName.toStdString());
			}
			// some external modules (like numpy) overrides the SIGINT handler at import
			// reinstall the default one, otherwise Tulip can not be interrupted by hitting Ctrl-C in a console
			setDefaultSIGINTHandler();

			runString(printObjectDictFunction);
		}
	}
}

PythonInterpreter::~PythonInterpreter() {
	if (interpreterInit()) {
		Py_Finalize();
	}
	delete consoleDialog;
}

PythonInterpreter *PythonInterpreter::getInstance() {
	return &instance;
}

bool PythonInterpreter::interpreterInit() const {
	return Py_IsInitialized();
}

void PythonInterpreter::registerNewModule(const string &moduleName, PyMethodDef *moduleDef) {
	Py_InitModule(moduleName.c_str(), moduleDef);
}

void PythonInterpreter::registerNewModuleFromString(const std::string &moduleName, const std::string &moduleSrcCode) {
	ostringstream oss;
	oss << moduleName << ".py";
	PyObject *pycomp = Py_CompileString(moduleSrcCode.c_str(),oss.str().c_str(), Py_file_input);
	if (pycomp == NULL) {
		PyErr_Print();
		PyErr_Clear();
		return;
	}
	PyObject *pmod = PyImport_ExecCodeModule(const_cast<char *>(moduleName.c_str()),pycomp);
	if (pmod == NULL){
		PyErr_Print();
		PyErr_Clear();
	}
}

bool PythonInterpreter::functionExists(const string &moduleName, const string &functionName) {
	PyObject *pName = PyString_FromString(moduleName.c_str());
	PyObject *pModule = PyImport_Import(pName);
	Py_DECREF(pName);
	PyObject *pDict = PyModule_GetDict(pModule);
	PyObject *pFunc = PyDict_GetItemString(pDict, functionName.c_str());
	return (pFunc != NULL && PyCallable_Check(pFunc));
}

bool PythonInterpreter::runString(const string &pyhtonCode) {
	int ret = PyRun_SimpleString(pyhtonCode.c_str());
	if (PyErr_Occurred()) {
		PyErr_Print();
		PyErr_Clear();
	}
	return  ret != -1;
}

void PythonInterpreter::addModuleSearchPath(const std::string &path, const bool beforeOtherPaths) {
	if (currentImportPaths.find(path) == currentImportPaths.end()) {
		ostringstream oss;
		oss << "import sys" << endl;
		if (beforeOtherPaths) {
			oss << "sys.path.insert(0, \"" << path << "\")" << endl;
		} else {
			oss << "sys.path.append(\"" << path << "\")" << endl;
		}
		runString(oss.str());
		currentImportPaths.insert(path);
	}
}

bool PythonInterpreter::runGraphScript(const string &module, const string &function, tlp::Graph *graph) {

	// Build the name object
	PyObject *pName = PyString_FromString(module.c_str());

	// Load the module object
	PyObject *pModule = PyImport_Import(pName);
	Py_DECREF(pName);

	// pDict is a borrowed reference
	PyObject *pDict = PyModule_GetDict(pModule);

	// pFunc is also a borrowed reference
	PyObject *pFunc = PyDict_GetItemString(pDict, function.c_str());

	if (PyCallable_Check(pFunc)) {
		const sipAPIDef *sisApi = get_sip_api();

		// Getting proper sipWrapperType
		const sipTypeDef* kpTypeDef     = sisApi->api_find_type("tlp::Graph");

		// Wrapping up C++ instance
		PyObject* pArgs = sisApi->api_convert_from_type(graph, kpTypeDef, NULL);

		// Finally calling 'process'
		PyObject* argTup = Py_BuildValue ("(O)", pArgs);
		runningScript = true;
		try {
			PyObject_CallObject(pFunc, argTup);
		} catch(...) {
			//
		}
		runningScript = false;
		Py_DECREF(argTup);
		Py_DECREF(pArgs);
		if (PyErr_Occurred()) {
			PyErr_Print();
			return false;
		}

	} else {
		PyErr_Print();
		return false;
	}

	return true;
}

int stopScript(void *) {
	PyErr_SetString(PyExc_Exception, "Script execution terminated by user");
	return -1;
}


void PythonInterpreter::stopCurrentScript() {
	Py_AddPendingCall(&stopScript, NULL);
}

void PythonInterpreter::setTraceFunction(Py_tracefunc tracefunc) {
	PyEval_SetTrace(tracefunc, NULL);
}

void PythonInterpreter::deleteModule(const std::string &moduleName) {
	ostringstream oss;
	oss << "import sys" << endl;
	oss << "if \"" << moduleName << "\" in sys.modules:" << endl;
	oss << "\tdel sys.modules[\"" << moduleName << "\"]" << endl;
	runString(oss.str());
}

void PythonInterpreter::reloadModule(const std::string &moduleName) {
	ostringstream oss;
	oss << "import " << moduleName << endl;
	oss << "reload(" << moduleName << ")" << endl;
	runString(oss.str());
}

void PythonInterpreter::setConsoleWidget(QPlainTextEdit *console) {
	consoleWidget = console;
	shellWidget = NULL;
}

void PythonInterpreter::setDefaultConsoleWidget() {
	consoleWidget = consoleDialog->consoleWidget;
	shellWidget = NULL;
}

void PythonInterpreter::setPythonShellWidget(PythonShellWidget *shell) {
	consoleWidget = NULL;
	shellWidget = shell;
}

void PythonInterpreter::setDefaultSIGINTHandler() {
	QPlainTextEdit *lastConsoleWidget = consoleWidget;
	consoleWidget = NULL;
	if (runString("import signal")) {
		runString("signal.signal(signal.SIGINT, signal.SIG_DFL)");
	}
	consoleWidget = lastConsoleWidget;
}

std::string PythonInterpreter::getPythonShellBanner() const {
	return string("Python ") + string(Py_GetVersion()) + string(" on ") + string(Py_GetPlatform());
}

std::vector<std::string> PythonInterpreter::getGlobalDictEntries(const std::string &prefixFilter) {
	std::vector<std::string> ret;
	std::set<std::string> publicMembersSorted;
	outputActivated = false;
	consoleOuput = "";
	runString("import __main__\nprintObjectDict(__main__)");
	QStringList objectDictList = QString(consoleOuput.c_str()).split("\n");
	for (int i = 0 ; i < objectDictList.count() ; ++i) {
		if (objectDictList[i] != "") {
			if (objectDictList[i].startsWith("_")) {
				continue;
			} else {
				if (prefixFilter != "") {
					if (objectDictList[i].startsWith(QString(prefixFilter.c_str()))) {
						publicMembersSorted.insert(objectDictList[i].toStdString());
					}
				} else {
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

std::vector<std::string> PythonInterpreter::getObjectDictEntries(const std::string &objectName, const std::string &prefixFilter) {
	std::vector<std::string> ret;
	std::set<std::string> publicMembersSorted;
	outputActivated = false;
	consoleOuput = "";
	if (runString(objectName)) {
		ostringstream oss;
		oss << "printObjectDict(" << objectName << ")";
		runString(oss.str());
		QStringList objectDictList = QString(consoleOuput.c_str()).split("\n");
		for (int i = 0 ; i < objectDictList.count() ; ++i) {
			if (objectDictList[i] != "") {
				if (objectDictList[i].startsWith("_")) {
					continue;
				} else {
					if (prefixFilter != "") {
						if (objectDictList[i].startsWith(QString(prefixFilter.c_str()))) {
							publicMembersSorted.insert(objectDictList[i].toStdString());
						}
					} else {
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
