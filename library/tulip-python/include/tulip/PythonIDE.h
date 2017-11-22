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

#ifndef PYTHONPLUGINSIDE_H
#define PYTHONPLUGINSIDE_H

#include <QWidget>
#include <QDateTime>
#include <QMap>
#include <QUrl>

#include <tulip/tulipconf.h>

class QTabWidget;

namespace Ui {
class PythonIDE;
}

namespace tlp {

class Graph;
class GraphHierarchiesModel;
class TulipProject;
class PythonCodeEditor;
class PythonInterpreter;
class PythonEditorsTabWidget;
class DataSet;

class TLP_PYTHON_SCOPE PythonIDE : public QWidget {

  Q_OBJECT
  Ui::PythonIDE *_ui;
  tlp::PythonInterpreter *_pythonInterpreter;
  bool _dontTreatFocusIn;
  tlp::TulipProject *_project;
  tlp::GraphHierarchiesModel *_graphsModel;
  bool _scriptStopped;
  bool _saveFilesToProject;
  bool _notifyProjectModified;

  QMap<QString, QString> _editedPluginsClassName;
  QMap<QString, QString> _editedPluginsType;
  QMap<QString, QString> _editedPluginsName;

  QWidget *_scriptEditorsWidget, *_scriptControlWidget;
  QWidget *_pluginEditorsWidget, *_pluginControlWidget;
  QWidget *_moduleEditorsWidget, *_moduleControlWidget;

  bool loadPythonPlugin(const QString &fileName, bool clear = true);
  bool loadPythonPluginFromSrcCode(const QString &moduleName, const QString &pluginSrcCode,
                                   bool clear = true);
  void savePythonPlugin(int tabIdx);
  bool indicateErrors() const;
  void clearErrorIndicators() const;
  bool loadModule(const QString &fileName);
  void saveModule(int tabIdx);

  bool reloadAllModules() const;
  void createTulipProjectPythonPaths();
  void writeScriptsFilesList(int deleted = -1);
  void writePluginsFilesList(int deleted = -1);
  void writeModulesFilesList(int deleted = -1);
  QString readProjectFile(const QString &filePath);
  void writeScriptFileToProject(int idx, const QString &scriptFileName,
                                const QString &scriptContent);
  void writeFileToProject(const QString &projectFile, const QString &fileContent);
  void deleteFilesFromProjectIfRemoved(const QString &projectDir,
                                       const QStringList &existingFilenames);

public:
  explicit PythonIDE(QWidget *parent = nullptr);
  ~PythonIDE();

  void setProject(tlp::TulipProject *project);
  void savePythonFilesAndWriteToProject(bool notifyProjectModified = false);
  void setGraphsModel(tlp::GraphHierarchiesModel *model);
  void clearPythonCodeEditors();

  void setScriptEditorsVisible(bool visible);
  void setPluginEditorsVisible(bool visible);
  void setModuleEditorsVisible(bool visible);

protected:
  void dragEnterEvent(QDragEnterEvent *);
  void dropEvent(QDropEvent *);
  bool eventFilter(QObject *obj, QEvent *event);

private:
  int addMainScriptEditor(const QString &fileName = "");
  int addModuleEditor(const QString &fileName = "");
  int addPluginEditor(const QString &fileName = "");

  bool loadScript(const QString &fileName, bool clear = true);
  void saveScript(int tabIdx, bool clear = true, bool showFileDialog = false);

  tlp::PythonCodeEditor *getCurrentMainScriptEditor() const;
  tlp::PythonCodeEditor *getMainScriptEditor(int idx) const;
  tlp::PythonCodeEditor *getModuleEditor(int idx) const;
  tlp::PythonCodeEditor *getCurrentModuleEditor() const;
  tlp::PythonCodeEditor *getPluginEditor(int idx) const;
  tlp::PythonCodeEditor *getCurrentPluginEditor() const;

  bool closeEditorTabRequested(PythonEditorsTabWidget *tabWidget, int idx, bool mayCancel = false);
  bool loadModuleFromSrcCode(const QString &moduleName, const QString &moduleSrcCode);

  void loadScriptsAndModulesFromPythonScriptViewDataSet(const DataSet &dataSet);

private slots:

  void newPythonPlugin();
  void currentTabChanged(int index);
  void loadPythonPlugin();
  void savePythonPlugin();
  void saveAllPlugins();
  void registerPythonPlugin(bool clear = true);
  void removePythonPlugin();
  void newFileModule();
  void newStringModule();
  void loadModule();
  void saveModule();
  void saveAllModules();
  void scrollToEditorLine(const QUrl &);
  void increaseFontSize();
  void decreaseFontSize();
  void scriptSaved(int);
  void pluginSaved(int);
  void moduleSaved(int);
  void graphComboBoxIndexChanged();

  void newScript();
  void loadScript();
  void saveScript();
  void saveAllScripts();
  void executeCurrentScript();
  void stopCurrentScript();
  void pauseCurrentScript();
  void currentScriptPaused();

  void closeModuleTabRequested(int index);
  void closeScriptTabRequested(int index);
  void closePluginTabRequested(int index);

  tlp::Graph *getSelectedGraph() const;
};
}

#endif // PYTHONPLUGINSIDE_H
