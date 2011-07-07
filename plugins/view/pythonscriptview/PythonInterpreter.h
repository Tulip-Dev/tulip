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

// thanks to the VTK project for this patch for Visual Studio in debug mode
#if defined(_MSC_VER) && defined(_DEBUG)
// Include these low level headers before undefing _DEBUG. Otherwise when doing
// a debug build against a release build of python the compiler will end up 
// including these low level headers without DEBUG enabled, causing it to try 
// and link release versions of this low level C api.
# include <basetsd.h>
# include <assert.h>
# include <ctype.h>
# include <errno.h>
# include <io.h>
# include <math.h>
# include <sal.h>
# include <stdarg.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <time.h>
# include <wchar.h>
#  undef _DEBUG
#  if _MSC_VER >= 1400
#    define _CRT_NOFORCE_MANIFEST 1
#  endif
#  include <Python.h>
#  include <frameobject.h>
#  include <structmember.h>
#  include <import.h>
#  define _DEBUG
# else
#  include <Python.h>
#  include <frameobject.h>
#  include <structmember.h>
#  include <import.h>
# endif

#include <QtGui/QDialog>
#include <QtGui/QPlainTextEdit>



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
	void hideConsoleOutputDialog();

private:

	QPoint lastPos;

};

const char pythonReservedCharacters[] = {'#', '%', '/', '+', '-', '&', '*', '<', '>',
		'|', '~', '^', '=', '!', '\'', '\"', '{', '}',
		'(', ')', '[', ']', 0};

class PythonShellWidget;

class PythonInterpreter {

public :

	static PythonInterpreter *getInstance();

	bool interpreterInit() ;

	void registerNewModule(const std::string &moduleName, PyMethodDef *moduleDef);
	void registerNewModuleFromString(const std::string &moduleName, const std::string &moduleSrcCode);
	void setTraceFunction(Py_tracefunc tracefunc);
	bool runString(const std::string &pyhtonCode);
	bool runGraphScript(const std::string &module, const std::string &function, tlp::Graph *graph);
	bool functionExists(const std::string &moduleName, const std::string &functionName);
	void addModuleSearchPath(const std::string &path, const bool beforeOtherPaths = false);
	void deleteModule(const std::string &moduleName);
	void reloadModule(const std::string &moduleName);
	void stopCurrentScript();
	bool isRunningScript() const {return runningScript;}
	std::string getPythonVersion() const {return pythonVersion;}
	std::string getPythonShellBanner();
	void setDefaultSIGINTHandler();
	

	std::vector<std::string> getGlobalDictEntries(const std::string &prefixFilter = "");
	std::vector<std::string> getObjectDictEntries(const std::string &objectName, const std::string &prefixFilter = "");

	void setDefaultConsoleWidget();
	void setConsoleWidget(QPlainTextEdit *consoleWidget);
	void setPythonShellWidget(PythonShellWidget *shellWidget);

private :

	PythonInterpreter();
	~PythonInterpreter();

	void holdGIL();
	void releaseGIL();

	static PythonInterpreter instance;
	
	bool runningScript;

	std::set<std::string> currentImportPaths;

	ConsoleOutputDialog *consoleDialog;
	
	std::string pythonVersion; 
	PyThreadState*  mainThreadState;
	PyGILState_STATE gilState;

};

#endif /* PYTHONINTERPRETER_H_ */
