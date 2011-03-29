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

#ifndef PYTHONINTERPRETER_H_
#define PYTHONINTERPRETER_H_

#include <Python.h>

#include <QtGui/QDialog>
#include <QtGui/QPlainTextEdit>

#include <frameobject.h>
#include <structmember.h>
#include <import.h>

#include <string>
#include <set>

namespace tlp {
	class Graph;
}

class ConsoleOutputDialog : public QDialog {

	Q_OBJECT

public :

	ConsoleOutputDialog(QWidget *parent = 0);

	QPlainTextEdit *consoleWidget;

public slots:

	void showOnOutputWrite();

};

class PythonInterpreter {

public :

	static PythonInterpreter *getInstance();

	bool interpreterInit() const;

	void registerNewModule(const std::string &moduleName, PyMethodDef *moduleDef);
	void registerNewModuleFromString(const std::string &moduleName, const std::string &moduleSrcCode);
	void setTraceFunction(Py_tracefunc tracefunc);
	bool runString(const std::string &pyhtonCode);
	bool runGraphScript(const std::string &module, const std::string &function, tlp::Graph *graph);
	bool functionExists(const std::string &moduleName, const std::string &functionName);
	void addModuleSearchPath(const std::string &path);
	void deleteModule(const std::string &moduleName);
	void reloadModule(const std::string &moduleName);
	void stopCurrentScript();
	bool isRunningScript() const {return runningScript;}
	std::string getPythonVersion() const {return pythonVersion;}
	
	void setDefaultConsoleWidget();
	void setConsoleWidget(QPlainTextEdit *consoleWidget);

private :

	PythonInterpreter();
	~PythonInterpreter();

	static PythonInterpreter instance;
	
	bool runningScript;

	std::set<std::string> currentImportPaths;

	ConsoleOutputDialog *consoleDialog;
	
	std::string pythonVersion; 

};

#endif /* PYTHONINTERPRETER_H_ */
