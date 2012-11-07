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

#include <QtGui/QDialog>
#include <QtGui/QPlainTextEdit>
#include <QtCore/QDir>
#include <QtCore/QVector>
#include <QtCore/QSet>

#include <tulip/Graph.h>

namespace tlp {

class TLP_PYTHON_SCOPE PythonInterpreter {

    PythonInterpreter();
    ~PythonInterpreter();

    void holdGIL();
    void releaseGIL();
    void loadTulipPythonPlugins(const QString &pluginsPath);

    static PythonInterpreter _instance;

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

    bool importModule(const QString &moduleName);

    bool registerNewModuleFromString(const QString &moduleName, const QString &moduleSrcCode);

    bool runString(const QString &pyhtonCode, const QString &scriptFilePath="");

    bool runGraphScript(const QString &module, const QString &function, tlp::Graph *graph, const QString &scriptFilePath="");

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

    void setDefaultConsoleWidget(QAbstractScrollArea *consoleWidget);

    void setConsoleWidget(QAbstractScrollArea *consoleWidget);

    void resetConsoleWidget();

    void initConsoleOutput();

    void loadTulipPythonPlugins();

    QString getStandardOutput() const;

    QString getStandardErrorOutput() const;

    void clearOutputBuffers();

    void setOutputEnabled(const bool enableOutput);

    bool outputEnabled() const;

    void sendOutputToConsole(const QString &output, bool stdErr);

};

}

#endif /* PYTHONINTERPRETER_H_ */
