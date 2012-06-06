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

#if defined(__GNUC__) && __GNUC__ >= 4 && ((__GNUC_MINOR__ == 2 && __GNUC_PATCHLEVEL__ >= 1) || (__GNUC_MINOR__ >= 3))
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

#ifndef PYTHONINTERPRETER_H_
#define PYTHONINTERPRETER_H_

#include <QtGui/QDialog>
#include <QtGui/QPlainTextEdit>
#include <QtCore/QDir>

#include <tulip/TulipRelease.h>
#include <tulip/TlpTools.h>
#include <tulip/Graph.h>

#include <string>
#include <set>

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

static const char pythonReservedCharacters[] = {'#', '%', '/', '+', '-', '&', '*', '<', '>',
    '|', '~', '^', '=', '!', '\'', '\"', '{', '}',
    '(', ')', '[', ']', '.', 0
                                               };

static const char *pythonKeywords[] = {
  "def", "class","from", "in", "and", "or", "not", "is", "with", "assert",
  "for", "while", "if", "elif", "import", "True", "False", "pass", "exec",
  "else", "None", "print", "global", "return", "break", "continue", "as",
  "lambda", "del", "try", "except", "raise", "finally", "yield", 0
};

static std::string pythonPluginsPath = tlp::TulipLibDir + "tulip/python/";
static std::string pythonPluginsPathHome = (QDir::homePath()+"/.Tulip-"+TULIP_MM_RELEASE+"/plugins/python").toUtf8().data();

class PythonInterpreter {

public :

  static PythonInterpreter *getInstance();

  bool interpreterInit() ;

  bool registerNewModuleFromString(const std::string &moduleName, const std::string &moduleSrcCode);

  bool runString(const std::string &pyhtonCode, const std::string &scriptFilePath="");

  bool runGraphScript(const std::string &module, const std::string &function, tlp::Graph *graph, const std::string &scriptFilePath="");

  bool functionExists(const std::string &moduleName, const std::string &functionName);

  void addModuleSearchPath(const std::string &path, const bool beforeOtherPaths = false);

  void deleteModule(const std::string &moduleName);

  bool reloadModule(const std::string &moduleName);

  void stopCurrentScript();

  void pauseCurrentScript(const bool pause=true);

  bool isScriptPaused() const;

  void setProcessQtEventsDuringScriptExecution(bool processQtEvents);

  bool isRunningScript() const {
    return runningScript;
  }

  std::string getPythonVersion() const {
    return pythonVersion;
  }
  std::string getPythonShellBanner();

  void setDefaultSIGINTHandler();

  std::vector<std::string> getImportedModulesList();

  std::vector<std::string> getBaseTypesForType(const std::string &type);

  std::vector<std::string> getGlobalDictEntries(const std::string &prefixFilter = "");

  std::vector<std::string> getObjectDictEntries(const std::string &objectName, const std::string &prefixFilter = "");

  std::string getVariableType(const std::string &varName);

  void setDefaultConsoleWidget();

  void setConsoleWidget(QAbstractScrollArea *consoleWidget);

  void initConsoleOutput();

  void loadTulipPythonPlugins();

  std::string getStandardOutput() const;

  std::string getStandardErrorOutput() const;

  void clearOutputBuffers();

  void setOutputEnabled(const bool enableOutput);

  bool outputEnabled() const;

private :

  PythonInterpreter();

  ~PythonInterpreter();

  void holdGIL();

  void releaseGIL();

  void loadTulipPythonPlugins(const std::string &pluginsPath);

  static PythonInterpreter instance;

  bool runningScript;

  std::set<std::string> currentImportPaths;

  ConsoleOutputDialog *consoleDialog;

  std::string pythonVersion;

  bool wasInit;


};

#endif /* PYTHONINTERPRETER_H_ */
