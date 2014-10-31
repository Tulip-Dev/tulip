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

/**
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

#ifndef PYTHONINTERPRETER_H_
#define PYTHONINTERPRETER_H_

#ifndef PyObject_HEAD
struct _object;
typedef _object PyObject;
#endif

#include <tulip/PythonCppTypesConverter.h>
#include <tulip/Graph.h>

#include <QVector>
#include <QSet>
#include <QString>

class QAbstractScrollArea;
class QPlainTextEdit;
class QTextBrowser;

namespace tlp {

class TLP_PYTHON_SCOPE PythonInterpreter {

  PythonInterpreter();
  ~PythonInterpreter();

  void holdGIL();
  void releaseGIL();

  void setDefaultConsoleWidget(QAbstractScrollArea *consoleWidget);
  void setConsoleWidget(QAbstractScrollArea *consoleWidget);

  static PythonInterpreter _instance;

  bool _wasInit;
  bool _runningScript;
  QSet<QString> _currentImportPaths;
  QAbstractScrollArea *_defaultConsoleWidget;
  QString _pythonVersion;

public :

  static const QString pythonPluginsPath;
  static const QString pythonPluginsPathHome;
  static const char pythonReservedCharacters[];
  static const char *pythonKeywords[];

  static PythonInterpreter *getInstance();

  bool interpreterInit() ;

  bool loadTulipPythonPlugin(const QString &pluginPath);

  void loadTulipPythonPluginsFromDir(const QString &pluginsPath);

  void loadTulipPythonPluginsFromDefaultDirs();

  bool importModule(const QString &moduleName);

  bool registerNewModuleFromString(const QString &moduleName, const QString &moduleSrcCode);

  bool runString(const QString &pyhtonCode, const QString &scriptFilePath="");

  bool runGraphScript(const QString &module, const QString &function, tlp::Graph *graph, const QString &scriptFilePath="");

  template<typename T>
  bool evalSingleStatementAndGetValue(const QString &pythonStatement, T &value);

  template<typename PARAM_TYPE>
  bool callFunctionOneParam(const QString &module, const QString &function, const PARAM_TYPE &parameter);

  template<typename PARAM1_TYPE, typename PARAM2_TYPE>
  bool callFunctionTwoParams(const QString &module, const QString &function, const PARAM1_TYPE &parameter1, const PARAM2_TYPE &parameter2);

  template<typename PARAM1_TYPE, typename PARAM2_TYPE, typename PARAM3_TYPE>
  bool callFunctionThreeParams(const QString &module, const QString &function, const PARAM1_TYPE &parameter1, const PARAM2_TYPE &parameter2,
                               const PARAM3_TYPE &parameter3);

  template<typename PARAM1_TYPE, typename PARAM2_TYPE, typename PARAM3_TYPE, typename PARAM4_TYPE>
  bool callFunctionFourParams(const QString &module, const QString &function, const PARAM1_TYPE &parameter1, const PARAM2_TYPE &parameter2,
                              const PARAM3_TYPE &parameter3, const PARAM4_TYPE &parameter4);

  bool callFunction(const QString &module, const QString &function, const tlp::DataSet &parameters);

  template<typename PARAM_TYPE, typename RETURN_TYPE>
  bool callFunctionOneParamAndGetReturnValue(const QString &module, const QString &function, const PARAM_TYPE &parameter, RETURN_TYPE &returnValue);

  template<typename PARAM1_TYPE, typename PARAM2_TYPE, typename RETURN_TYPE>
  bool callFunctionTwoParamsAndGetReturnValue(const QString &module, const QString &function, const PARAM1_TYPE &parameter1, const PARAM2_TYPE &parameter2,
      RETURN_TYPE &returnValue);

  template<typename PARAM1_TYPE, typename PARAM2_TYPE, typename PARAM3_TYPE, typename RETURN_TYPE>
  bool callFunctionThreeParamsAndGetReturnValue(const QString &module, const QString &function, const PARAM1_TYPE &parameter1, const PARAM2_TYPE &parameter2,
      const PARAM3_TYPE &parameter3, RETURN_TYPE &returnValue);

  template<typename PARAM1_TYPE, typename PARAM2_TYPE, typename PARAM3_TYPE, typename PARAM4_TYPE, typename RETURN_TYPE>
  bool callFunctionFourParamsAndGetReturnValue(const QString &module, const QString &function, const PARAM1_TYPE &parameter1, const PARAM2_TYPE &parameter2,
      const PARAM3_TYPE &parameter3, const PARAM4_TYPE &parameter4, RETURN_TYPE &returnValue);

  template<typename RETURN_TYPE>
  bool callFunctionAndGetReturnValue(const QString &module, const QString &function, const tlp::DataSet &parameters, RETURN_TYPE &returnValue);

  bool functionExists(const QString &moduleName, const QString &functionName);

  void addModuleSearchPath(const QString &path, const bool beforeOtherPaths = false);

  void deleteModule(const QString &moduleName);

  bool reloadModule(const QString &moduleName);

  void stopCurrentScript();

  void pauseCurrentScript(const bool pause=true);

  bool isScriptPaused() const;

  void setProcessQtEventsDuringScriptExecution(bool processQtEvents);

  bool isRunningScript() const {
    return _runningScript;
  }

  QString getPythonVersionStr() const {
    return _pythonVersion;
  }

  double getPythonVersion() const;

  QString getPythonShellBanner();

  void setDefaultSIGINTHandler();

  QVector<QString> getImportedModulesList();

  QVector<QString> getBaseTypesForType(const QString &type);

  QVector<QString> getGlobalDictEntries(const QString &prefixFilter = "");

  QVector<QString> getObjectDictEntries(const QString &objectName, const QString &prefixFilter = "");

  QString getVariableType(const QString &varName);

  void setDefaultConsoleWidget(QPlainTextEdit *consoleWidget);
  void setDefaultConsoleWidget(QTextBrowser *consoleWidget);

  void setConsoleWidget(QPlainTextEdit *consoleWidget);
  void setConsoleWidget(QTextBrowser *consoleWidget);

  void resetConsoleWidget();

  void initConsoleOutput();

  void loadTulipPythonPluginsFromDir();

  QString getStandardOutput() const;

  QString getStandardErrorOutput() const;

  void clearOutputBuffers();

  void setOutputEnabled(const bool enableOutput);

  bool outputEnabled() const;

  void sendOutputToConsole(const QString &output, bool stdErr);

  QString readLineFromConsole();

  PyObject* callPythonFunction(const QString &module, const QString &function, const tlp::DataSet &parameters);

  PyObject* evalPythonStatement(const QString &pythonStatement);

  void clearTracebacks();

};

#include "PythonInterpreter.cxx"

}

#endif /* PYTHONINTERPRETER_H_ */
