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

#ifndef PYTHONPLUGINSIDE_H
#define PYTHONPLUGINSIDE_H

#include <QWidget>
#include <QDateTime>
#include <QMap>
#include <QUrl>

class QTabWidget;

namespace Ui {
class PythonPluginsIDE;
}

namespace tlp {
class TulipProject;
class PythonCodeEditor;
class PythonInterpreter;
}

class PythonPluginsIDE : public QWidget {

  Q_OBJECT
  Ui::PythonPluginsIDE *_ui;
  tlp::PythonInterpreter *_pythonInterpreter;
  bool _dontTreatFocusIn;
  tlp::TulipProject *_project;

  QMap<QString, QString> _editedPluginsClassName;
  QMap<QString, QString> _editedPluginsType;
  QMap<QString, QString> _editedPluginsName;

  bool loadPythonPlugin(const QString &fileName, bool clear=true);
  bool loadPythonPluginFromSrcCode(const QString &moduleName, const QString &pluginSrcCode, bool clear=true);
  void savePythonPlugin(int tabIdx);
  bool indicateErrors() const;
  void clearErrorIndicators() const;
  bool loadModule(const QString &fileName, bool clear=true);
  void saveModule(int tabIdx, const bool reload=false);
  bool reloadAllModules() const;
  void writePluginsFilesList(int deleted=-1);
  void writeModulesFilesList(int deleted=-1);
  QString readProjectFile(const QString &filePath);
  void writePluginFileToProject(const QString &fileName, const QString &fileContent);
  void writeModuleFileToProject(const QString &fileName, const QString &fileContent);

public :

  explicit PythonPluginsIDE(QWidget *parent = 0);
  ~PythonPluginsIDE();

  void setProject(tlp::TulipProject *project);

protected :

  int addModuleEditor(const QString &fileName="");
  int addPluginEditor(const QString &fileName="");

  tlp::PythonCodeEditor *getModuleEditor(int idx) const;
  tlp::PythonCodeEditor *getCurrentModuleEditor() const;
  tlp::PythonCodeEditor *getPluginEditor(int idx) const;
  tlp::PythonCodeEditor *getCurrentPluginEditor() const;

protected slots:

  void newPythonPlugin();
  void currentTabChanged(int index);
  void loadPythonPlugin();
  void savePythonPlugin();
  void registerPythonPlugin(bool clear=true);
  void removePythonPlugin();
  void newFileModule();
  void loadModule();
  void saveModule();
  void closeModuleTabRequested(int tab);
  void closePluginTabRequested(int tab);
  void scrollToEditorLine(const QUrl &);
  void increaseFontSize();
  void decreaseFontSize();
  void pluginSaved(int);
  void moduleSaved(int);

};

#endif // PYTHONPLUGINSIDE_H
