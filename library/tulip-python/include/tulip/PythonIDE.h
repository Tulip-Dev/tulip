/**
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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

#include <QFrame>
#include <QDateTime>
#include <QMap>
#include <QUrl>

#include <tulip/tulipconf.h>
#include <tulip/PythonIDEInterface.h>
#include <tulip/PythonCodeEditor.h>

class QTabWidget;
class QComboBox;
class QLineEdit;

namespace Ui {
class PythonIDE;
}

class PythonPanel;

namespace tlp {

class Graph;
class GraphHierarchiesModel;
class TulipProject;
class PythonCodeEditor;
class PythonInterpreter;
class PythonEditorsTabWidget;
class DataSet;

class TLP_PYTHON_SCOPE PythonIDE : public PythonIDEInterface {

  Q_OBJECT
  Ui::PythonIDE *_ui;
  tlp::PythonInterpreter *_pythonInterpreter;
  PythonPanel *_pythonPanel;
  bool _dontTreatFocusIn;
  tlp::GraphHierarchiesModel *_graphsModel;
  bool _scriptStopped;

  QMap<QString, QString> _editedPluginsClassName;
  QMap<QString, QString> _editedPluginsType;
  QMap<QString, QString> _editedPluginsName;

  QWidget *_scriptEditorsWidget, *_scriptControlWidget;
  QWidget *_pluginEditorsWidget, *_pluginControlWidget;
  QWidget *_moduleEditorsWidget, *_moduleControlWidget;
  QFrame *_pipFrame;
  QLineEdit *_pipPackage;
  QComboBox *_pipCombo;

  bool loadPythonPlugin(const QString &fileName, bool clear = true);
  bool loadPythonPluginFromSrcCode(const QString &moduleName, const QString &pluginSrcCode,
                                   bool clear = true);
  bool savePythonPlugin(int tabIdx, bool saveAs = false);
  bool indicateErrors() const;
  void clearErrorIndicators() const;
  bool loadModule(const QString &fileName);
  bool saveModule(int tabIdx, bool saveAs = false);

  bool reloadAllModules() const;
  QString readProjectFile(tlp::TulipProject *project, const QString &filePath);

public:
  explicit PythonIDE();
  ~PythonIDE() override;

  bool projectNeedsPythonIDE(tlp::TulipProject *project) override;
  bool hasUnsavedFiles() override;
  void readProject(tlp::TulipProject *project) override;
  void setGraphsModel(tlp::GraphHierarchiesModel *model);
  void clearPythonCodeEditors() override;

  void setScriptEditorsVisible(bool visible);
  void setPluginEditorsVisible(bool visible);
  void setModuleEditorsVisible(bool visible);
  bool isCurrentScriptExecuting() override;
  void deleteStaticResources() override {
    PythonCodeEditor::deleteStaticResources();
  }

  // the Builder class used to instantiate PythonIDE
  class Builder : public PythonIDEInterface::Builder {
  public:
    PythonIDEInterface *newIDE(GraphHierarchiesModel *model) override;
    void loadPlugins() override;
  };

public slots:
  void executeCurrentScript() override;
  void stopCurrentScript() override;
  void pauseCurrentScript() override;

protected:
  void dragEnterEvent(QDragEnterEvent *) override;
  void dropEvent(QDropEvent *) override;
  bool eventFilter(QObject *obj, QEvent *event) override;
  void executePipCommand(int command, const QString &packageName);

private:
  int addMainScriptEditor(const QString &fileName = "");
  int addModuleEditor(const QString &fileName = "");
  int addPluginEditor(const QString &fileName = "");

  bool loadScript(const QString &fileName, bool clear = true);
  bool saveScript(int tabIdx, bool clear = true, bool saveAs = false);

  tlp::PythonCodeEditor *getCurrentMainScriptEditor() const;
  tlp::PythonCodeEditor *getMainScriptEditor(int idx) const;
  tlp::PythonCodeEditor *getModuleEditor(int idx) const;
  tlp::PythonCodeEditor *getCurrentModuleEditor() const;
  tlp::PythonCodeEditor *getPluginEditor(int idx) const;
  tlp::PythonCodeEditor *getCurrentPluginEditor() const;

  bool closeEditorTabRequested(PythonEditorsTabWidget *tabWidget, int idx);
  bool loadModuleFromSrcCode(const QString &moduleName, const QString &moduleSrcCode);
  bool checkUnsavedFiles(PythonEditorsTabWidget *editorsTabWidget, bool updateTabText = false);

  void loadScriptsAndModulesFromPythonScriptViewDataSet(const DataSet &dataSet);

private slots:

  void newPythonPlugin();
  void currentTabChanged(int index);
  void loadPythonPlugin();
  void savePythonPlugin();
  void saveAsPythonPlugin();
  void registerPythonPlugin(bool clear = true);
  void removePythonPlugin();
  void newModule();
  void loadModule();
  void saveModule();
  void saveAsModule();
  void scrollToEditorLine(const QUrl &);
  void increaseFontSize();
  void decreaseFontSize();
  void scriptSaved(int);
  void pluginSaved(int);
  void moduleSaved(int);
  void graphComboBoxIndexChanged();
  void fileEdited();

  void newScript();
  void loadScript();
  void saveScript();
  void saveAsScript();
  void currentScriptPaused();

  void closeModuleTabRequested(int index);
  void closeScriptTabRequested(int index);
  void closePluginTabRequested(int index);
  void pipCommandChanged(int index);

  tlp::Graph *getSelectedGraph() const;
};
} // namespace tlp

#endif // PYTHONPLUGINSIDE_H
