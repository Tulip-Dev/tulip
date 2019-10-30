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

#ifndef PYTHONINTERPRETER_H_
#define PYTHONINTERPRETER_H_

#ifndef PyObject_HEAD
struct _object;
typedef _object PyObject;
#endif

#include <tulip/PythonCppTypesConverter.h>
#include <tulip/Graph.h>

#include <QObject>
#include <QVector>
#include <QSet>
#include <QString>

class QAbstractScrollArea;
class QPlainTextEdit;
class QTextBrowser;

namespace tlp {

class TLP_PYTHON_SCOPE PythonInterpreter : public QObject {

  Q_OBJECT

  PythonInterpreter();
  ~PythonInterpreter() override;

  void holdGIL();
  void releaseGIL();

  void setDefaultConsoleWidget(QAbstractScrollArea *consoleWidget);
  void setConsoleWidget(QAbstractScrollArea *consoleWidget);

  bool _wasInit;
  bool _runningScript;
  QSet<QString> _currentImportPaths;
  QAbstractScrollArea *_defaultConsoleWidget;
  QString _pythonVersion;
  bool _outputEnabled;
  bool _errorOutputEnabled;

  template <typename T, typename... Param>
  void buildParamDataSet(DataSet *ds, T a, Param... param);
  template <typename T>
  void buildParamDataSet(DataSet *ds, T a);
  template <typename T>
  void addParameter(DataSet *ds, T a);

public:
  static const QString pythonPluginsPath;
  static const QString pythonPluginsPathHome;
  static const char pythonReservedCharacters[];
  static const std::vector<QString> pythonAccentuatedCharacters;
  static const std::vector<QString> pythonAccentuatedCharactersReplace;
  static const char *pythonKeywords[];

  static PythonInterpreter *getInstance();

  bool interpreterInit();

  bool importModule(const QString &moduleName);

  bool registerNewModuleFromString(const QString &moduleName, const QString &moduleSrcCode);

  bool runString(const QString &pyhtonCode, const QString &scriptFilePath = "");

  bool runGraphScript(const QString &module, const QString &function, tlp::Graph *graph,
                      const QString &scriptFilePath = "");

  template <typename T>
  bool evalSingleStatementAndGetValue(const QString &pythonStatement, T &value);

  bool callFunction(const QString &module, const QString &function, const tlp::DataSet &parameters);

  // use c++11 variadic template for more convenience
  template <typename RETURN_TYPE, typename... Param>
  bool callFunctionWithParamsAndGetReturnValue(const QString &module, const QString &function,
                                               RETURN_TYPE &returnValue, Param... param);
  template <typename... Param>
  bool callFunctionWithParams(const QString &module, const QString &function, Param... param);

  template <typename RETURN_TYPE>
  bool callFunctionAndGetReturnValue(const QString &module, const QString &function,
                                     const tlp::DataSet &parameters, RETURN_TYPE &returnValue);

  bool functionExists(const QString &moduleName, const QString &functionName);

  void addModuleSearchPath(const QString &path, const bool beforeOtherPaths = false);

  void deleteModule(const QString &moduleName);

  bool reloadModule(const QString &moduleName);

  void stopCurrentScript();

  void pauseCurrentScript(const bool pause = true);

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

  QVector<QString> getObjectDictEntries(const QString &objectName,
                                        const QString &prefixFilter = "");

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

  void setErrorOutputEnabled(const bool enableOutput);

  bool errorOutputEnabled() const;

  void sendOutputToConsole(const QString &output, bool stdErr);

  QString readLineFromConsole();

  PyObject *callPythonFunction(const QString &module, const QString &function,
                               const tlp::DataSet &parameters);

  PyObject *evalPythonStatement(const QString &pythonStatement, bool singleInput = false);

  void clearTracebacks();

signals:

  void scriptExecutionPaused();
};

#include "PythonInterpreter.cxx"
} // namespace tlp

#endif /* PYTHONINTERPRETER_H_ */
