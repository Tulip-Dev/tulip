#ifndef PYTHONPLUGINSIDE_H
#define PYTHONPLUGINSIDE_H

#include <QtGui/QWidget>
#include <QtCore/QDateTime>
#include <QtCore/QMap>
#include <QtCore/QUrl>

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
