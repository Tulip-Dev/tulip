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

#include <tulip/PythonInterpreter.h>

#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextBlock>

#include <tulip/PythonCodeEditor.h>
#include <tulip/PluginLister.h>
#include <tulip/TulipProject.h>
#include <tulip/TlpQtTools.h>
#include <tulip/FontIconManager.h>

#include "PythonPluginsIDE.h"
#include "PythonPluginCreationDialog.h"
#include "tulip/PythonEditorsTabWidget.h"
#include "ui_PythonPluginsIDE.h"

using namespace tlp;

static QString PYTHON_PATH("/python");
static QString PYTHON_MODULES_PATH(PYTHON_PATH + "/modules");
static QString PYTHON_PLUGINS_PATH(PYTHON_PATH + "/plugins");
static QString PYTHON_PLUGINS_FILES(PYTHON_PLUGINS_PATH + "/files");
static QString PYTHON_MODULES_FILES(PYTHON_MODULES_PATH + "/files");

static QString getTulipPythonPluginSkeleton(const QString &pluginClassName, const QString &pluginType, const QString &pluginName,
                                            const QString &pluginAuthor, const QString &pluginDate, const QString &pluginInfo,
                                            const QString &pluginRelease, const QString &pluginGroup) {

  QString pluginClass;

  if (pluginType == "General") {
    pluginClass = "tlp.Algorithm";
  } else if (pluginType == "Layout") {
    pluginClass = "tlp.LayoutAlgorithm";
  } else if (pluginType == "Size") {
    pluginClass = "tlp.SizeAlgorithm";
  } else if (pluginType == "Measure") {
    pluginClass = "tlp.DoubleAlgorithm";
  } else if (pluginType == "Color") {
    pluginClass = "tlp.ColorAlgorithm";
  } else if (pluginType == "Selection") {
    pluginClass = "tlp.BooleanAlgorithm";
  } else if (pluginType == "Import") {
    pluginClass = "tlp.ImportModule";
  } else {
    pluginClass = "tlp.ExportModule";
  }

  QString pluginSkeleton;
  QTextStream textStream(&pluginSkeleton);

  textStream << "from tulip import *" << endl;
  textStream << "import tulipplugins" << endl << endl;

  textStream << "class " << pluginClassName << "(" << pluginClass << "):" << endl;
  textStream << "  def __init__(self, context):" << endl;
  textStream << "    " << pluginClass << ".__init__(self, context)" << endl;
  textStream << "    # You can add parameters to the plugin here through the following syntax:" << endl;
  textStream << "    # self.add<Type>Parameter(\"<paramName>\", \"<paramDoc>\", \"<paramDefaultValue>\")" << endl;
  textStream << "    # (see the documentation of class tlp.WithParameter to see what parameter types are supported)." << endl << endl;

  if (pluginType != "Import" && pluginType != "Export") {

    textStream << "  def check(self):" << endl;
    textStream << "    # This method is called before applying the algorithm on the input graph." << endl;
    textStream << "    # You can perform some precondition checks here." << endl;
    textStream << "    # See comments in the run method to know how to have access to the input graph." << endl << endl;
    textStream << "    # Must return a tuple (Boolean, string). First member indicates if the algorithm can be applied" << endl;
    textStream << "    # and the second one can be used to provide an error message." << endl;
    textStream << "    return (True, \"\")" << endl << endl;
    textStream << "  def run(self):" << endl;
    textStream << "    # This method is the entry point of the algorithm when it is called" << endl;
    textStream << "    # and must contain its implementation." << endl << endl;
    textStream << "    # The graph on which the algorithm is applied can be accessed through" << endl;
    textStream << "    # the \"graph\" class attribute (see documentation of class tlp.Graph)." << endl << endl;
    textStream << "    # The parameters provided by the user are stored in a dictionnary" << endl;
    textStream << "    # that can be accessed through the \"dataSet\" class attribute." << endl << endl;

    if (pluginType == "Layout") {
      textStream << "    # The result of this layout algorithm must be stored in the" << endl;
      textStream << "    # layout property accessible through the \"result\" class attribute" << endl;
      textStream << "    # (see documentation to know how to work with graph properties)." << endl << endl;
    } else if (pluginType == "Size") {
      textStream << "    # The result of this size algorithm must be stored in the" << endl;
      textStream << "    # size property accessible through the \"result\" class attribute" << endl;
      textStream << "    # (see documentation to know how to work with graph properties)." << endl << endl;
    } else if (pluginType == "Measure") {
      textStream << "    # The result of this measure algorithm must be stored in the" << endl;
      textStream << "    # double property accessible through the \"result\" class attribute" << endl;
      textStream << "    # (see documentation to know how to work with graph properties)." << endl << endl;
    } else if (pluginType == "Color") {
      textStream << "    # The result of this color algorithm must be stored in the" << endl;
      textStream << "    # color property accessible through the \"result\" class attribute" << endl;
      textStream << "    # (see documentation to know how to work with graph properties)." << endl << endl;
    } else if (pluginType == "Selection") {
      textStream << "    # The result of this selection algorithm must be stored in the" << endl;
      textStream << "    # boolean property accessible through the \"result\" class attribute" << endl;
      textStream << "    # (see documentation to know how to work with graph properties)." << endl << endl;
    }

    textStream << "    # The method must return a boolean indicating if the algorithm" << endl;
    textStream << "    # has been successfully applied on the input graph." << endl;
    textStream << "    return True" << endl << endl;

  } else if (pluginType == "Import") {
    textStream << "  def importGraph(self):" << endl;
    textStream << "    # This method is called to import a new graph." << endl;
    textStream << "    # An empty graph to populate is accessible through the \"graph\" class attribute" << endl;
    textStream << "    # (see documentation of class tlp.Graph)." << endl << endl;
    textStream << "    # The parameters provided by the user are stored in a dictionnary" << endl;
    textStream << "    # that can be accessed through the \"dataSet\" class attribute." << endl << endl;

    textStream << "    # The method must return a Boolean indicating if the" << endl;
    textStream << "    # graph has been successfully imported." << endl;
    textStream << "    return True" << endl << endl;
  } else if (pluginType == "Export") {
    textStream << "  def exportGraph(self, os):" << endl;
    textStream << "    # This method is called to export a graph." << endl;
    textStream << "    # The graph to export is accessible through the \"graph\" class attribute" << endl;
    textStream << "    # (see documentation of class tlp.Graph)." << endl << endl;
    textStream << "    # The parameters provided by the user are stored in dictionnary" << endl;
    textStream << "    # that can be accessed through the \"dataSet\" class attribute." << endl << endl;

    textStream << "    # The os parameter is an output file stream (initialized by the Tulip GUI" << endl;
    textStream << "    # or by the tlp.exportGraph function.)." << endl;
    textStream << "    # To write data to the file, you have to use the following syntax:" << endl << endl;
    textStream << "    # write the number of nodes and edges to the file" << endl;
    textStream << "    # os << self.graph.numberOfNodes() << \"\\n\"" << endl;
    textStream << "    # os << self.graph.numberOfEdges() << \"\\n\"" << endl << endl;
    textStream << "    # The method must return a Boolean indicating if the" << endl;
    textStream << "    # graph has been successfully exported." << endl;
    textStream << "    return True" << endl << endl;
  }

  textStream << "# The line below does the magic to register the plugin into the plugin database" << endl;
  textStream << "# and updates the GUI to make it accessible through the menus." << endl;

  if (pluginGroup == "") {
    textStream << "tulipplugins.registerPlugin(\"" << pluginClassName << "\", \"" << pluginName << "\", \"" << pluginAuthor << "\", \"" << pluginDate
               << "\", \"" << pluginInfo << "\", \"" << pluginRelease << "\")" << endl;
  } else {
    textStream << "tulipplugins.registerPluginOfGroup(\"" << pluginClassName << "\", \"" << pluginName << "\", \"" << pluginAuthor << "\", \""
               << pluginDate << "\", \"" << pluginInfo << "\", \"" << pluginRelease << "\", \"" << pluginGroup << "\")" << endl;
  }

  return pluginSkeleton;
}

static QString infoMsg = QString("When the plugin development is finished, you can copy the associated Python file ") + "to <br> <b>" +
                         PythonInterpreter::pythonPluginsPathHome + "</b>";

PythonPluginsIDE::PythonPluginsIDE(QWidget *parent)
    : QWidget(parent), _ui(new Ui::PythonPluginsIDE), _pythonInterpreter(PythonInterpreter::getInstance()), _dontTreatFocusIn(false),
      _project(nullptr) {
  _ui->setupUi(this);
  _ui->newPluginButton->setIcon(FontIconManager::instance()->getMaterialDesignIcon(md::plusbox, Qt::white));
  _ui->loadPluginButton->setIcon(FontIconManager::instance()->getMaterialDesignIcon(md::fileimport, Qt::white));
  _ui->savePluginButton->setIcon(FontIconManager::instance()->getMaterialDesignIcon(md::fileexport, Qt::white));
  _ui->newModuleButton->setIcon(FontIconManager::instance()->getMaterialDesignIcon(md::plusbox, Qt::white));
  _ui->loadModuleButton->setIcon(FontIconManager::instance()->getMaterialDesignIcon(md::fileimport, Qt::white));
  _ui->saveModuleButton->setIcon(FontIconManager::instance()->getMaterialDesignIcon(md::fileexport, Qt::white));
  _ui->decreaseFontSizeButton->setIcon(FontIconManager::instance()->getMaterialDesignIcon(md::minuscircle, Qt::black));
  _ui->increaseFontSizeButton->setIcon(FontIconManager::instance()->getMaterialDesignIcon(md::pluscircle, Qt::black));
  _ui->tabWidget->setDrawTabBarBgGradient(true);
  _ui->tabWidget->setTextColor(QColor(200, 200, 200));
  _ui->modulesTabWidget->clear();
  _ui->pluginsTabWidget->clear();
  QList<int> sizes;
  sizes.push_back(550);
  sizes.push_back(200);
  _ui->splitter->setSizes(sizes);
  _ui->splitter->setCollapsible(0, false);

#if defined(__APPLE__)
  if (!PythonInterpreter::pythonPluginsPath.contains(".app/Contents/"))
#elif defined(_LINUX)
  if (!PythonInterpreter::pythonPluginsPath.startsWith("/tmp/.mount"))
#endif
    infoMsg += QString(" or <b>") + PythonInterpreter::pythonPluginsPath + "</b>";

  infoMsg += QString("<br/> and it will be automatically loaded at Tulip startup");
  _ui->pluginsInfoWidget->setText(infoMsg);

  connect(_ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(currentTabChanged(int)));

  connect(_ui->newModuleButton, SIGNAL(clicked()), this, SLOT(newFileModule()));
  connect(_ui->loadModuleButton, SIGNAL(clicked()), this, SLOT(loadModule()));
  connect(_ui->saveModuleButton, SIGNAL(clicked()), this, SLOT(saveModule()));
  connect(_ui->newPluginButton, SIGNAL(clicked()), this, SLOT(newPythonPlugin()));
  connect(_ui->loadPluginButton, SIGNAL(clicked()), this, SLOT(loadPythonPlugin()));
  connect(_ui->savePluginButton, SIGNAL(clicked()), this, SLOT(savePythonPlugin()));
  connect(_ui->registerPluginButton, SIGNAL(clicked()), this, SLOT(registerPythonPlugin()));
  connect(_ui->removePluginButton, SIGNAL(clicked()), this, SLOT(removePythonPlugin()));
  connect(_ui->pluginsInfoWidget, SIGNAL(anchorClicked(const QUrl &)), this, SLOT(scrollToEditorLine(const QUrl &)));
  connect(_ui->decreaseFontSizeButton, SIGNAL(clicked()), this, SLOT(decreaseFontSize()));
  connect(_ui->increaseFontSizeButton, SIGNAL(clicked()), this, SLOT(increaseFontSize()));
  connect(_ui->modulesTabWidget, SIGNAL(fileSaved(int)), this, SLOT(moduleSaved(int)));
  connect(_ui->pluginsTabWidget, SIGNAL(fileSaved(int)), this, SLOT(pluginSaved(int)));
  connect(_ui->modulesTabWidget, SIGNAL(filesReloaded()), _ui->pluginsTabWidget, SLOT(reloadCodeInEditorsIfNeeded()));
  connect(_ui->modulesTabWidget, SIGNAL(tabAboutToBeDeleted(int)), this, SLOT(closeModuleTabRequested(int)));
  connect(_ui->pluginsTabWidget, SIGNAL(filesReloaded()), _ui->modulesTabWidget, SLOT(reloadCodeInEditorsIfNeeded()));
  connect(_ui->pluginsTabWidget, SIGNAL(tabAboutToBeDeleted(int)), this, SLOT(closePluginTabRequested(int)));
}

PythonPluginsIDE::~PythonPluginsIDE() {
  delete _ui;
}

int PythonPluginsIDE::addModuleEditor(const QString &fileName) {
  return _ui->modulesTabWidget->addEditor(fileName);
}

int PythonPluginsIDE::addPluginEditor(const QString &fileName) {
  return _ui->pluginsTabWidget->addEditor(fileName);
}

PythonCodeEditor *PythonPluginsIDE::getCurrentModuleEditor() const {
  return _ui->modulesTabWidget->getCurrentEditor();
}

PythonCodeEditor *PythonPluginsIDE::getModuleEditor(int idx) const {
  return _ui->modulesTabWidget->getEditor(idx);
}

PythonCodeEditor *PythonPluginsIDE::getPluginEditor(int idx) const {
  return _ui->pluginsTabWidget->getEditor(idx);
}

PythonCodeEditor *PythonPluginsIDE::getCurrentPluginEditor() const {
  return _ui->pluginsTabWidget->getCurrentEditor();
}

void PythonPluginsIDE::loadModule() {
  QString fileName = QFileDialog::getOpenFileName(this, "Open Module", "", "Python script (*.py)");
  loadModule(fileName);
}

bool PythonPluginsIDE::loadModule(const QString &fileName, bool clear) {
  QFile file(fileName);

  if (!file.exists())
    return false;

  QFileInfo fileInfo(file);

  int editorId = addModuleEditor(fileInfo.absoluteFilePath());

  _pythonInterpreter->setConsoleWidget(_ui->pluginsInfoWidget);

  if (clear) {
    _ui->pluginsInfoWidget->clear();
    _pythonInterpreter->clearOutputBuffers();
  }

  clearErrorIndicators();
  reloadAllModules();

  if (!indicateErrors()) {
    _ui->pluginsInfoWidget->setText(infoMsg);
  }

  _pythonInterpreter->resetConsoleWidget();

  saveModule(editorId, false);

  return true;
}

void PythonPluginsIDE::saveModule(int tabIdx, const bool reload) {
  if (tabIdx >= 0 && tabIdx < _ui->modulesTabWidget->count()) {
    QString moduleNameExt = _ui->modulesTabWidget->tabText(tabIdx);
    QString moduleName;

    if (moduleNameExt[moduleNameExt.size() - 1] == '*')
      moduleName = moduleNameExt.mid(0, moduleNameExt.size() - 4);
    else
      moduleName = moduleNameExt.mid(0, moduleNameExt.size() - 3);

    _pythonInterpreter->deleteModule(moduleName);
    _ui->modulesTabWidget->setTabText(tabIdx, moduleName + ".py");
    QFile file(getModuleEditor(tabIdx)->getFileName());
    QFileInfo fileInfo(file);

    getModuleEditor(tabIdx)->saveCodeToFile();
    _ui->modulesTabWidget->setTabToolTip(tabIdx, fileInfo.absoluteFilePath());

    writeModuleFileToProject(fileInfo.fileName(), getModuleEditor(tabIdx)->getCleanCode());
  }

  if (reload && !_pythonInterpreter->isRunningScript()) {
    _pythonInterpreter->setConsoleWidget(_ui->pluginsInfoWidget);
    _ui->pluginsInfoWidget->clear();
    _pythonInterpreter->clearOutputBuffers();
    clearErrorIndicators();
    reloadAllModules();

    if (!indicateErrors()) {
      _ui->pluginsInfoWidget->setText(infoMsg);
    }

    _pythonInterpreter->resetConsoleWidget();
  }
}

void PythonPluginsIDE::newFileModule() {
  QString fileName = QFileDialog::getSaveFileName(this, tr("Set Module filename"), "", "Python script (*.py)");

  if (fileName.isEmpty())
    return;

  if (!fileName.endsWith(".py"))
    fileName += ".py";

  QFile file(fileName);

  if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    return;

  file.close();

  QFileInfo fileInfo(file);
  QString moduleName(fileInfo.fileName());
  QString modulePath(fileInfo.absolutePath());

  int editorId = addModuleEditor(fileInfo.absoluteFilePath());
  getModuleEditor(editorId)->saveCodeToFile();
  _ui->modulesTabWidget->setTabToolTip(editorId, fileInfo.absoluteFilePath());
  _pythonInterpreter->addModuleSearchPath(modulePath);
  _ui->modulesTabWidget->setTabText(editorId, fileInfo.fileName());
}

void PythonPluginsIDE::saveModule() {
  int curModule = _ui->modulesTabWidget->currentIndex();

  if (curModule == -1)
    return;

  saveModule(curModule, true);
}

bool PythonPluginsIDE::reloadAllModules() const {

  bool ret = true;

  for (int i = 0; i < _ui->modulesTabWidget->count(); ++i) {
    QString moduleNameExt = _ui->modulesTabWidget->tabText(i);
    QString moduleName;

    if (moduleNameExt[moduleNameExt.size() - 1] == '*')
      moduleName = moduleNameExt.mid(0, moduleNameExt.size() - 4);
    else
      moduleName = moduleNameExt.mid(0, moduleNameExt.size() - 3);

    _pythonInterpreter->deleteModule(moduleName);
    QFileInfo fileInfo(getModuleEditor(i)->getFileName());

    if (fileInfo.fileName() == getModuleEditor(i)->getFileName()) {
      ret = ret && _pythonInterpreter->registerNewModuleFromString(moduleName, getModuleEditor(i)->getCleanCode());
    } else {
      _pythonInterpreter->addModuleSearchPath(fileInfo.absolutePath());
      ret = ret && _pythonInterpreter->reloadModule(moduleName);
    }
  }

  return ret;
}

void PythonPluginsIDE::newPythonPlugin() {
  PythonPluginCreationDialog pluginCreationDialog(this);

  if (pluginCreationDialog.exec() == QDialog::Accepted) {
    QFile file(pluginCreationDialog.getPluginFileName());

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
      return;

    QFileInfo fileInfo(file);
    QString moduleName(fileInfo.fileName());
    QString modulePath(fileInfo.absolutePath());

    int editorId = addPluginEditor(fileInfo.absoluteFilePath());
    _ui->pluginsTabWidget->setTabToolTip(editorId, fileInfo.absoluteFilePath());
    PythonInterpreter::getInstance()->addModuleSearchPath(modulePath);
    _ui->pluginsTabWidget->setTabText(editorId, QString("[") + pluginCreationDialog.getPluginType() + QString("] ") + fileInfo.fileName());

    QString pluginFile = fileInfo.absoluteFilePath();
    _editedPluginsClassName[pluginFile] = pluginCreationDialog.getPluginClassName();
    _editedPluginsType[pluginFile] = pluginCreationDialog.getPluginType();
    _editedPluginsName[pluginFile] = pluginCreationDialog.getPluginName();
    QString pluginSkeleton = getTulipPythonPluginSkeleton(pluginCreationDialog.getPluginClassName(), pluginCreationDialog.getPluginType(),
                                                          pluginCreationDialog.getPluginName(), pluginCreationDialog.getPluginAuthor(),
                                                          pluginCreationDialog.getPluginDate(), pluginCreationDialog.getPluginInfo(),
                                                          pluginCreationDialog.getPluginRelease(), pluginCreationDialog.getPluginGroup());

    getPluginEditor(editorId)->setPlainText(pluginSkeleton);
    savePythonPlugin();
  }
}

void PythonPluginsIDE::currentTabChanged(int index) {
  _ui->pluginControlFrame->setVisible(index <= 1);
}

static bool checkAndGetPluginInfoFromSrcCode(const QString &pluginCode, QString &pluginName, QString &pluginClassName, QString &pluginType,
                                             QString &pluginClass) {
  pluginClass = "";
  pluginClassName = "";
  pluginName = "";
  pluginType = "";
  QString s = "tulipplugins.register";
  int idx1 = pluginCode.indexOf(s);

  if (idx1 != -1) {
    QRegExp rx("class ([a-zA-Z_][a-zA-Z0-9_]*)\\(([^,\\(\\)]+)\\)");

    int pos = rx.indexIn(pluginCode);

    while (pos != -1) {
      pluginClassName = rx.cap(1);
      pluginClass = rx.cap(2);

      if (pluginClass == "tlp.Algorithm") {
        pluginType = "General";
        break;
      } else if (pluginClass == "tlp.ColorAlgorithm") {
        pluginType = "Color";
        break;
      } else if (pluginClass == "tlp.LayoutAlgorithm") {
        pluginType = "Layout";
        break;
      } else if (pluginClass == "tlp.DoubleAlgorithm") {
        pluginType = "Measure";
        break;
      } else if (pluginClass == "tlp.SizeAlgorithm") {
        pluginType = "Size";
        break;
      } else if (pluginClass == "tlp.BooleanAlgorithm") {
        pluginType = "Selection";
        break;
      } else if (pluginClass == "tlp.ImportModule") {
        pluginType = "Import";
        break;
      } else if (pluginClass == "tlp.ExportModule") {
        pluginType = "Export";
        break;
      }

      pos = rx.indexIn(pluginCode, pos + rx.matchedLength());
    }

    rx.setPattern("^.*register.*Plugin.*\\(.*,.*\"([^,]+)\",.*$");

    if (rx.indexIn(pluginCode) != -1) {
      pluginName = rx.cap(1);
      return true;
    }
  }

  return false;
}

void PythonPluginsIDE::loadPythonPlugin() {
  QString fileName = QFileDialog::getOpenFileName(this, "Open Tulip Python plugin", "", "Python script (*.py)");
  loadPythonPlugin(fileName);
}

bool PythonPluginsIDE::loadPythonPlugin(const QString &fileName, bool clear) {

  if (_editedPluginsClassName.find(fileName) != _editedPluginsClassName.end()) {
    return true;
  }

  QFile file(fileName);

  if (!file.exists())
    return false;

  QFileInfo fileInfo(file);
  QString moduleName(fileInfo.fileName());
  QString modulePath(fileInfo.absolutePath());

  QString pluginType = "";
  QString pluginClass = "";
  QString pluginClassName = "";
  QString pluginName = "";

  QString pluginCode;
  file.open(QIODevice::ReadOnly | QIODevice::Text);

  while (!file.atEnd()) {
    pluginCode += file.readLine();
  }

  file.close();

  if (checkAndGetPluginInfoFromSrcCode(pluginCode, pluginName, pluginClassName, pluginType, pluginClass)) {
    if (pluginClassName == "" || pluginName == "") {
      QMessageBox::critical(this, "Error", "Unable to retrieve the plugin class name and the plugin name from the source code\n.");
      return false;
    } else {
      int editorId = addPluginEditor(fileInfo.absoluteFilePath());
      _pythonInterpreter->addModuleSearchPath(modulePath);
      _ui->pluginsTabWidget->setTabToolTip(editorId, fileInfo.absoluteFilePath());
      _ui->pluginsTabWidget->setTabText(editorId, QString("[") + pluginType + QString("] ") + fileInfo.fileName());
      QString pluginFile = fileInfo.absoluteFilePath();
      _editedPluginsClassName[pluginFile] = pluginClassName;
      _editedPluginsType[pluginFile] = pluginType;
      _editedPluginsName[pluginFile] = pluginName;
      registerPythonPlugin(clear);
    }
  } else {
    QMessageBox::critical(this, "Error", "The file " + fileName + " does not seem to contain the source code of a Tulip Python plugin.");
    return false;
  }

  return true;
}

bool PythonPluginsIDE::loadPythonPluginFromSrcCode(const QString &moduleName, const QString &pluginSrcCode, bool clear) {
  QString pluginType = "";
  QString pluginClass = "";
  QString pluginClassName = "";
  QString pluginName = "";

  if (checkAndGetPluginInfoFromSrcCode(pluginSrcCode, pluginName, pluginClassName, pluginType, pluginClass)) {
    if (pluginName != "" && pluginClassName != "") {
      int editorId = addPluginEditor(moduleName);
      PythonCodeEditor *codeEditor = getPluginEditor(editorId);
      codeEditor->setPlainText(pluginSrcCode);
      codeEditor->setFileName(moduleName);
      _ui->pluginsTabWidget->setTabText(editorId, QString("[") + pluginType + QString("] ") + moduleName);
      QString pluginFile = moduleName;
      _editedPluginsClassName[pluginFile] = pluginClassName;
      _editedPluginsType[pluginFile] = pluginType;
      _editedPluginsName[pluginFile] = pluginName;
      registerPythonPlugin(clear);
      return true;
    }
  }

  return false;
}

void PythonPluginsIDE::writePluginFileToProject(const QString &fileName, const QString &fileContent) {
  writePluginsFilesList();
  QString projectFile = PYTHON_PLUGINS_PATH + "/" + fileName;

  if (!_project->exists(projectFile)) {
    _project->touch(projectFile);
  }

  QIODevice *fs = _project->fileStream(projectFile, QIODevice::WriteOnly | QIODevice::Text);
  fs->write(fileContent.toUtf8());
  fs->close();
  delete fs;

  if (!_project->projectFile().isEmpty()) {
    _project->write(_project->projectFile());
  }
}

void PythonPluginsIDE::writeModuleFileToProject(const QString &fileName, const QString &fileContent) {
  writeModulesFilesList();
  QString projectFile = PYTHON_MODULES_PATH + "/" + fileName;

  if (!_project->exists(projectFile)) {
    _project->touch(projectFile);
  }

  QIODevice *fs = _project->fileStream(projectFile, QIODevice::WriteOnly | QIODevice::Text);
  fs->write(fileContent.toUtf8());
  fs->close();
  delete fs;

  if (!_project->projectFile().isEmpty()) {
    _project->write(_project->projectFile());
  }
}

void PythonPluginsIDE::savePythonPlugin() {
  savePythonPlugin(_ui->pluginsTabWidget->currentIndex());
}

void PythonPluginsIDE::savePythonPlugin(int tabIdx) {

  if (tabIdx >= 0 && tabIdx < _ui->pluginsTabWidget->count()) {

    QString moduleNameExt = _ui->pluginsTabWidget->tabText(tabIdx);
    QString moduleName;

    if (moduleNameExt[moduleNameExt.size() - 1] == '*')
      moduleName = moduleNameExt.mid(0, moduleNameExt.size() - 4);
    else
      moduleName = moduleNameExt.mid(0, moduleNameExt.size() - 3);

    _ui->pluginsTabWidget->setTabText(tabIdx, moduleName + ".py");
    QFile file(getPluginEditor(tabIdx)->getFileName());
    QFileInfo fileInfo(file);

    getPluginEditor(tabIdx)->saveCodeToFile();
    _ui->pluginsTabWidget->setTabToolTip(tabIdx, getPluginEditor(tabIdx)->getFileName());

    writePluginFileToProject(fileInfo.fileName(), getPluginEditor(tabIdx)->getCleanCode());
  }
}

void PythonPluginsIDE::registerPythonPlugin(bool clear) {
  int tabIdx = _ui->pluginsTabWidget->currentIndex();

  if (tabIdx == -1)
    return;

  QString pluginFile = getPluginEditor(tabIdx)->getFileName();

  savePythonPlugin();

  QString moduleNameExt = _ui->pluginsTabWidget->tabText(tabIdx);
  moduleNameExt = moduleNameExt.mid(moduleNameExt.lastIndexOf("]") + 2);
  QString moduleName;

  if (moduleNameExt[moduleNameExt.size() - 1] == '*')
    moduleName = moduleNameExt.mid(0, moduleNameExt.size() - 4);
  else
    moduleName = moduleNameExt.mid(0, moduleNameExt.size() - 3);

  moduleName = moduleName.replace(".py", "");

  QString pluginCode = getPluginEditor(tabIdx)->getCleanCode();

  QString pluginType = "";
  QString pluginClass = "";
  QString pluginClassName = "";
  QString pluginName = "";

  checkAndGetPluginInfoFromSrcCode(pluginCode, pluginName, pluginClassName, pluginType, pluginClass);

  QString oldPluginName = _editedPluginsName[pluginFile];

  if (tlp::PluginLister::pluginExists(QStringToTlpString(oldPluginName))) {
    tlp::PluginLister::removePlugin(QStringToTlpString(oldPluginName));
  }

  _pythonInterpreter->setConsoleWidget(_ui->pluginsInfoWidget);

  if (clear) {
    _ui->pluginsInfoWidget->clear();
    _pythonInterpreter->clearOutputBuffers();
  }

  clearErrorIndicators();

  _pythonInterpreter->deleteModule(moduleName);

  _pythonInterpreter->importModule("tulipplugins");
  _pythonInterpreter->runString("tulipplugins.setTestMode(True)");

  bool codeOk = false;
  QFileInfo fileInfo(getPluginEditor(tabIdx)->getFileName());

  if (fileInfo.fileName() == getPluginEditor(tabIdx)->getFileName()) {
    codeOk = _pythonInterpreter->registerNewModuleFromString(moduleName, getPluginEditor(tabIdx)->getCleanCode());
  } else {
    codeOk = _pythonInterpreter->reloadModule(moduleName);
  }

  _pythonInterpreter->runString("tulipplugins.setTestMode(False)");

  QString pythonCode;
  QTextStream oss(&pythonCode);

  oss << "import " << moduleName << endl;
  oss << "plugin = " << moduleName << "." << pluginClassName << "(tlp.AlgorithmContext())";

  if (codeOk && _pythonInterpreter->runString(pythonCode)) {

    if (fileInfo.fileName() == getPluginEditor(tabIdx)->getFileName()) {
      _pythonInterpreter->registerNewModuleFromString(moduleName, getPluginEditor(tabIdx)->getCleanCode());
    } else {
      _pythonInterpreter->reloadModule(moduleName);
    }

    _ui->pluginStatusLabel->setText("Plugin has been successfully registered.");
    _ui->pluginsInfoWidget->setText(infoMsg);
    _editedPluginsClassName[pluginFile] = pluginClassName;
    _editedPluginsType[pluginFile] = pluginType;
    _editedPluginsName[pluginFile] = pluginName;
  } else {
    _ui->pluginStatusLabel->setText("Plugin registration has failed.");
    indicateErrors();
  }

  _pythonInterpreter->resetConsoleWidget();
}

void PythonPluginsIDE::removePythonPlugin() {

  int tabIdx = _ui->pluginsTabWidget->currentIndex();

  if (tabIdx == -1)
    return;

  QString pluginName = _editedPluginsName[getCurrentPluginEditor()->getFileName()];

  if (tlp::PluginLister::pluginExists(QStringToTlpString(pluginName))) {
    tlp::PluginLister::removePlugin(QStringToTlpString(pluginName));
    _ui->pluginStatusLabel->setText("Plugin has been successfully removed.");
  } else {
    _ui->pluginStatusLabel->setText("Plugin is not registered in the plugin database.");
  }
}

bool PythonPluginsIDE::indicateErrors() const {
  QRegExp rx("^.*File.*\"(.*)\".*line.*(\\d+).*$");
  QRegExp rx2("^.*File.*\"(.*)\".*line.*(\\d+).*in (.*)$");

  QMap<QString, QVector<int>> errorLines;
  QString consoleOutput = _pythonInterpreter->getStandardErrorOutput();
  QStringList outputLines = consoleOutput.split("\n");

  for (int i = 0; i < outputLines.count() - 1; ++i) {
    int pos = 0;

    while ((pos = rx.indexIn(outputLines[i], pos)) != -1) {
      rx2.indexIn(outputLines[i], pos);

      if (rx2.cap(3) != "tlpimporthook") {
        QString file = rx.cap(1);
        int line = rx.cap(2).toInt();
        errorLines[file].push_back(line);
      }

      pos += rx.matchedLength();
    }
  }

  _ui->pluginsTabWidget->indicateErrors(errorLines);
  _ui->modulesTabWidget->indicateErrors(errorLines);

  return !errorLines.isEmpty();
}

void PythonPluginsIDE::clearErrorIndicators() const {
  _ui->pluginsTabWidget->clearErrorIndicators();
  _ui->modulesTabWidget->clearErrorIndicators();
}

void PythonPluginsIDE::closeModuleTabRequested(int tab) {
  QString moduleFile = getModuleEditor(tab)->getFileName();
  QFileInfo fileInfo(moduleFile);
  QString projectFile = PYTHON_MODULES_PATH + "/" + fileInfo.fileName();

  if (_project->exists(projectFile)) {
    writeModulesFilesList(tab);
    _project->removeFile(projectFile);
  }

  if (!_project->projectFile().isEmpty()) {
    _project->write(_project->projectFile());
  }
}

void PythonPluginsIDE::closePluginTabRequested(int tab) {
  QString pluginFile = getPluginEditor(tab)->getFileName();
  QFileInfo fileInfo(pluginFile);
  _editedPluginsClassName.remove(pluginFile);
  _editedPluginsType.remove(pluginFile);
  _editedPluginsName.remove(pluginFile);
  QString projectFile = PYTHON_PATH + "plugins/" + fileInfo.fileName();

  if (_project->exists(projectFile)) {
    writePluginsFilesList(tab);
    _project->removeFile(projectFile);
  }

  if (!_project->projectFile().isEmpty()) {
    _project->write(_project->projectFile());
  }
}

void PythonPluginsIDE::scrollToEditorLine(const QUrl &link) {
  QString linkStr = QUrl::fromPercentEncoding(link.toEncoded());
#ifdef WIN32
  linkStr.replace("\\", "/");
#endif
  QStringList strList = linkStr.split(":");
  QString file = strList.at(0);
  int line = strList.at(1).toInt() - 1;

  for (int i = 0; i < _ui->modulesTabWidget->count(); ++i) {
    PythonCodeEditor *codeEditor = getModuleEditor(i);

    if (file == codeEditor->getFileName()) {
      _ui->tabWidget->setCurrentIndex(1);
      _ui->modulesTabWidget->setCurrentIndex(i);
      codeEditor->scrollToLine(line);
      return;
    }
  }

  for (int i = 0; i < _ui->pluginsTabWidget->count(); ++i) {
    PythonCodeEditor *codeEditor = getPluginEditor(i);

    if (file == codeEditor->getFileName()) {
      _ui->tabWidget->setCurrentIndex(3);
      _ui->pluginsTabWidget->setCurrentIndex(i);
      codeEditor->scrollToLine(line);
      return;
    }
  }
}

void PythonPluginsIDE::decreaseFontSize() {
  _ui->pluginsTabWidget->decreaseFontSize();
  _ui->modulesTabWidget->decreaseFontSize();
}

void PythonPluginsIDE::increaseFontSize() {
  _ui->pluginsTabWidget->increaseFontSize();
  _ui->modulesTabWidget->increaseFontSize();
}

QString PythonPluginsIDE::readProjectFile(const QString &filePath) {
  QIODevice *fs = _project->fileStream(filePath, QIODevice::ReadOnly | QIODevice::Text);
  QString content;
  QString currentLine = fs->readLine();

  while (!currentLine.isEmpty()) {
    content += currentLine;
    currentLine = fs->readLine();
  }

  fs->close();
  delete fs;
  return content;
}

void PythonPluginsIDE::setProject(tlp::TulipProject *project) {
  _project = project;

  if (!_project->exists(PYTHON_PATH)) {
    _project->mkpath(PYTHON_PATH);
  }

  if (!_project->exists(PYTHON_PLUGINS_PATH)) {
    _project->mkpath(PYTHON_PLUGINS_PATH);
  }

  if (!_project->exists(PYTHON_MODULES_PATH)) {
    _project->mkpath(PYTHON_MODULES_PATH);
  }

  if (_project->exists(PYTHON_MODULES_FILES)) {
    QIODevice *fs = _project->fileStream(PYTHON_MODULES_FILES, QIODevice::ReadOnly | QIODevice::Text);
    QString file = fs->readLine();

    while (!file.isEmpty()) {
      file = file.mid(0, file.size() - 1);

      if (!loadModule(file)) {
        QFileInfo fileInfo(file);
        QString projectFile = PYTHON_MODULES_PATH + "/" + fileInfo.fileName();

        if (_project->exists(projectFile)) {
          QString code = readProjectFile(projectFile);
          QString moduleNameExt = fileInfo.fileName();
          QString moduleName = moduleNameExt.mid(0, moduleNameExt.size() - 3);
          int editorId = addModuleEditor(moduleNameExt);
          getModuleEditor(editorId)->setPlainText(code);
          getModuleEditor(editorId)->setFileName(moduleNameExt);
          _ui->modulesTabWidget->setTabText(editorId, moduleNameExt);
          _ui->modulesTabWidget->setTabToolTip(editorId, moduleNameExt);
          _pythonInterpreter->registerNewModuleFromString(moduleName, code);
        }
      }

      file = fs->readLine();
    }

    fs->close();
    delete fs;
  }

  if (_project->exists(PYTHON_PLUGINS_FILES)) {
    QIODevice *fs = _project->fileStream(PYTHON_PLUGINS_FILES, QIODevice::ReadOnly | QIODevice::Text);
    QString file = fs->readLine();

    while (!file.isEmpty()) {
      file = file.mid(0, file.size() - 1);

      if (!loadPythonPlugin(file)) {
        QFileInfo fileInfo(file);
        QString projectFile = PYTHON_PLUGINS_PATH + "/" + fileInfo.fileName();

        if (_project->exists(projectFile)) {
          QString code = readProjectFile(projectFile);
          loadPythonPluginFromSrcCode(fileInfo.fileName(), code);
        }
      }

      file = fs->readLine();
    }

    fs->close();
    delete fs;
  }
}

void PythonPluginsIDE::writePluginsFilesList(int deleted) {
  if (_project->exists(PYTHON_PLUGINS_FILES)) {
    _project->removeFile(PYTHON_PLUGINS_FILES);
  }

  _project->touch(PYTHON_PLUGINS_FILES);
  QIODevice *fs = _project->fileStream(PYTHON_PLUGINS_FILES, QIODevice::WriteOnly | QIODevice::Text);

  for (int i = 0; i < _ui->pluginsTabWidget->count(); ++i) {
    if (deleted == -1 || i != deleted)
      fs->write((getPluginEditor(i)->getFileName() + "\n").toUtf8());
  }

  fs->close();
  delete fs;
}

void PythonPluginsIDE::writeModulesFilesList(int deleted) {
  if (_project->exists(PYTHON_MODULES_FILES)) {
    _project->removeFile(PYTHON_MODULES_FILES);
  }

  _project->touch(PYTHON_MODULES_FILES);
  QIODevice *fs = _project->fileStream(PYTHON_MODULES_FILES, QIODevice::WriteOnly | QIODevice::Text);

  for (int i = 0; i < _ui->modulesTabWidget->count(); ++i) {
    if (deleted == -1 || i != deleted)
      fs->write((getModuleEditor(i)->getFileName() + "\n").toUtf8());
  }

  fs->close();
  delete fs;
}

void PythonPluginsIDE::pluginSaved(int idx) {
  QString fileName = getPluginEditor(idx)->getFileName();
  QFileInfo fileInfo(fileName);
  writePluginFileToProject(fileInfo.fileName(), getPluginEditor(idx)->getCleanCode());
}

void PythonPluginsIDE::moduleSaved(int idx) {
  QString fileName = getModuleEditor(idx)->getFileName();
  QFileInfo fileInfo(fileName);
  writeModuleFileToProject(fileInfo.fileName(), getModuleEditor(idx)->getCleanCode());
  saveModule(idx, true);
}
