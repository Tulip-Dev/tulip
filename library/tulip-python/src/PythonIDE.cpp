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

#include <tulip/PythonInterpreter.h>

#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextBlock>
#include <QInputDialog>
#include <QLineEdit>
#include <QXmlStreamReader>
#include <QMainWindow>
#include <QCryptographicHash>
#include <QByteArray>
#include <QRegExp>
#include <QShortcut>

#include <tulip/Graph.h>
#include <tulip/DoubleProperty.h>
#include <tulip/LayoutProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/StringProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/BooleanProperty.h>
#include <tulip/IntegerProperty.h>
#include <tulip/GraphProperty.h>
#include <tulip/Algorithm.h>
#include <tulip/ImportModule.h>
#include <tulip/ExportModule.h>
#include <tulip/PluginLister.h>
#include <tulip/PythonCodeEditor.h>
#include <tulip/PluginLister.h>
#include <tulip/TulipProject.h>
#include <tulip/TlpQtTools.h>
#include <tulip/GraphHierarchiesModel.h>
#include <tulip/TulipMimes.h>
#include <tulip/TulipMetaTypes.h>
#include <tulip/Perspective.h>

#include <tulip/PythonIDE.h>
#include <tulip/PythonPanel.h>
#include <tulip/PythonPluginCreationDialog.h>
#include <tulip/PythonEditorsTabWidget.h>

#include "ui_PythonIDE.h"

using namespace std;
using namespace tlp;

static QCryptographicHash hasher(QCryptographicHash::Md5);

static const QString utilityFunctions = R"(
import tuliputils

def updateVisualization(centerViews = True):
    tuliputils.updateVisualization(centerViews)

def pauseScript():
    tuliputils.pauseRunningScript()

def runGraphScript(scriptFile, graph):
    tuliputils.runGraphScript(scriptFile, graph)
)";

static QString cleanPropertyName(const QString &propertyName) {
  QString ret(propertyName);
  ret.replace(' ', '_');

  // check if the name is only numbers and prefix it by prop_ if necessary
  QRegExp re("\\d*"); // a digit (\d), zero or more times (*)
  if (re.exactMatch(ret)) {
    ret.insert(0, "prop_");
  }
  int i = 0;

  while (PythonInterpreter::pythonReservedCharacters[i]) {
    ret.remove(PythonInterpreter::pythonReservedCharacters[i++]);
  }

  for (unsigned i = 0; i < PythonInterpreter::pythonAccentuatedCharacters.size(); ++i) {
    ret.replace(PythonInterpreter::pythonAccentuatedCharacters[i],
                PythonInterpreter::pythonAccentuatedCharactersReplace[i]);
  }

  i = 0;

  while (PythonInterpreter::pythonKeywords[i]) {
    if (ret == PythonInterpreter::pythonKeywords[i++]) {
      ret += "_";
      break;
    }
  }

  QString builtinModName = "__builtin__";

  if (PythonInterpreter::getInstance()->getPythonVersion() >= 3.0) {
    builtinModName = "builtins";
  }

  PythonInterpreter::getInstance()->importModule(builtinModName);
  static QVector<QString> builtinDictContent =
      PythonInterpreter::getInstance()->getObjectDictEntries(builtinModName);

  for (i = 0; i < builtinDictContent.size(); ++i) {
    if (ret == builtinDictContent[i]) {
      ret += "_";
      break;
    }
  }

  return ret;
}

static QString getDefaultScriptCode(const QString &pythonVersion, Graph *graph) {

  QString scriptCode = R"(# Powered by Python %1
# To cancel the modifications performed by the script
# on the current graph, click on the undo button.
# Some useful keyboard shortcuts:
#   * Ctrl + D: comment selected lines.
#   * Ctrl + Shift + D: uncomment selected lines.
#   * Ctrl + I: indent selected lines.
#   * Ctrl + Shift + I: unindent selected lines.
#   * Ctrl + Return: run script.
#   * Ctrl + F: find selected text.
#   * Ctrl + R: replace selected text.
#   * Ctrl + Space: show auto-completion dialog.
from tulip import tlp
# The updateVisualization(centerViews = True) function can be called
# during script execution to update the opened views
# The pauseScript() function can be called to pause the script execution.
# To resume the script execution, you will have to click on the
# "Run script " button.
# The runGraphScript(scriptFile, graph) function can be called to launch
# another edited script on a tlp.Graph object.
# The scriptFile parameter defines the script name to call
# (in the form [a-zA-Z0-9_]+.py)
# The main(graph) function must be defined
# to run the script on the current graph
def main(graph):
)";
  QTextStream oss(&scriptCode);

  if (graph) {

    for (PropertyInterface *prop : graph->getObjectProperties()) {
      QString cleanPropName(tlp::tlpStringToQString(prop->getName()));
      cleanPropName.replace("\"", "\\\"");

#ifdef NDEBUG
      if (cleanPropName != "viewMetaGraph")
#endif
        oss << "    " << cleanPropertyName(tlp::tlpStringToQString(prop->getName())) << " = graph['"
            << cleanPropName << "']" << QT_ENDL;
    }
  }

  scriptCode += R"(
    for n in graph.getNodes():
        print(n)
  )";

  return scriptCode.arg(pythonVersion);
}

static QString PYTHON_PATH("/python");
static QString PYTHON_SCRIPTS_PATH(PYTHON_PATH + "/scripts");
static QString PYTHON_MODULES_PATH(PYTHON_PATH + "/modules");
static QString PYTHON_PLUGINS_PATH(PYTHON_PATH + "/plugins");
static QString PYTHON_SCRIPTS_FILES(PYTHON_SCRIPTS_PATH + "/files");
static QString PYTHON_PLUGINS_FILES(PYTHON_PLUGINS_PATH + "/files");
static QString PYTHON_MODULES_FILES(PYTHON_MODULES_PATH + "/files");

static QString getTulipPythonPluginSkeleton(const QString &pluginClassName,
                                            const QString &pluginType, const QString &pluginName,
                                            const QString &pluginAuthor, const QString &pluginDate,
                                            const QString &pluginInfo, const QString &pluginRelease,
                                            const QString &pluginGroup) {

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

  QString pluginPaths = PythonInterpreter::pythonPluginsPathHome;
#if defined(__APPLE__)
  if (!PythonInterpreter::pythonPluginsPath.contains(".app/Contents/"))
#elif defined(_LINUX)
  if (!PythonInterpreter::pythonPluginsPath.startsWith("/tmp/.mount"))
#endif
    pluginPaths += "\n# or " + PythonInterpreter::pythonPluginsPath;

  pluginSkeleton = QString(R"(
# When the plugin development is finished, you can copy the associated
# Python file to %1
# and it will be automatically loaded at Tulip startup

from tulip import tlp
import tulipplugins


class %2(%3):
    def __init__(self, context):
        %3.__init__(self, context)
        # You can add parameters to the plugin here through the
        # following syntax:
        # self.add<Type>Parameter('<paramName>', '<paramDoc>',
        #                         '<paramDefaultValue>')
        # (see the documentation of class tlp.WithParameter to see what
        #  parameter types are supported).
    )")
                       .arg(pluginPaths, pluginClassName, pluginClass);

  if (pluginType != "Import" && pluginType != "Export") {

    pluginSkeleton += R"(
    def check(self):
        # This method is called before applying the algorithm on the
        # input graph. You can perform some precondition checks here.
        # See comments in the run method to know how to have access to
        # the input graph.
        #
        # Must return a tuple (Boolean, string). First member indicates if the
        # algorithm can be applied and the second one can be used to provide
        # an error message.
        return (True, '')

    def run(self):
        # This method is the entry point of the algorithm when it is called
        # and must contain its implementation.
        #
        # The graph on which the algorithm is applied can be accessed through
        # the 'graph' class attribute (see documentation of class tlp.Graph).
        #
        # The parameters provided by the user are stored in a dictionary
        # that can be accessed through the 'dataSet' class attribute.)";

    QString propertyType;
    if (pluginType == "Layout") {
      propertyType = "layout";
    } else if (pluginType == "Size") {
      propertyType = "size";
    } else if (pluginType == "Measure") {
      propertyType = "double";
    } else if (pluginType == "Color") {
      propertyType = "color";
    } else if (pluginType == "Selection") {
      propertyType = "boolean";
    }
    if (!propertyType.isEmpty()) {
      QString resultProperty = R"(
        #
        # The result of this %1 algorithm must be stored in the
        # %1 property accessible through the 'result' class attribute
        # (see documentation to know how to work with graph properties).)";
      pluginSkeleton += resultProperty.arg(propertyType);
    }
  } else if (pluginType == "Import") {
    pluginSkeleton += R"(
    def importGraph(self):
        # This method is called to import a new graph.
        # An empty graph to populate is accessible through the 'graph'
        # class attribute (see documentation of class tlp.Graph).
        #
        # The parameters provided by the user are stored in a dictionary
        # that can be accessed through the 'dataSet' class attribute.)";
  } else if (pluginType == "Export") {
    pluginSkeleton += R"(
    def exportGraph(self, os):
        # This method is called to export a graph.
        # The graph to export is accessible through the 'graph' class attribute
        # (see documentation of class tlp.Graph).
        #
        # The parameters provided by the user are stored in dictionary
        # that can be accessed through the 'dataSet' class attribute.
        #
        # The os parameter is an output file stream (initialized by the
        # Tulip GUI or by the tlp.exportGraph function).
        # To write data to the file, you have to use the following syntax:
        #
        # write the number of nodes and edges to the file
        # os << self.graph.numberOfNodes() << '\n'
        # os << self.graph.numberOfEdges() << '\n')";
  }

  pluginSkeleton += R"(
        #
        # The method must return a Boolean indicating if the algorithm
        # has been successfully applied on the input graph.
        return True

# The line below does the magic to register the plugin into the plugin database
# and updates the GUI to make it accessible through the menus.)";

  if (pluginGroup.isEmpty()) {
    pluginSkeleton +=
        QString(R"(
tulipplugins.registerPlugin('%1', '%2', '%3', '%4', '%5', '%6')
)")
            .arg(pluginClassName, pluginName, pluginAuthor, pluginDate, pluginInfo, pluginRelease);
  } else {
    pluginSkeleton += QString(R"(
tulipplugins.registerPluginOfGroup('%1', '%2', '%3', '%4', '%5', '%6', '%7')
)")
                          .arg(pluginClassName, pluginName, pluginAuthor, pluginDate, pluginInfo,
                               pluginRelease, pluginGroup);
  }

  return pluginSkeleton.mid(1);
}

PythonIDE::PythonIDE(QWidget *parent)
    : QFrame(parent), _ui(new Ui::PythonIDE), _pythonInterpreter(PythonInterpreter::getInstance()),
      _pythonPanel(new PythonPanel()), _dontTreatFocusIn(false), _project(nullptr),
      _graphsModel(nullptr), _scriptStopped(false), _saveFilesToProject(true),
      _notifyProjectModified(false) {
  _ui->setupUi(this);

  _ui->mainScriptsTabWidget->clear();
  _ui->modulesTabWidget->clear();
  _ui->pluginsTabWidget->clear();
  QVBoxLayout *layout = new QVBoxLayout();
  layout->addWidget(_pythonPanel);
  layout->setContentsMargins(0, 0, 0, 0);
  _ui->interpreterTab->setLayout(layout);

  QList<int> sizes;
  sizes.push_back(550);
  sizes.push_back(150);
  _ui->splitter->setSizes(sizes);

  _scriptEditorsWidget = _ui->tabWidget->widget(0);
  _scriptControlWidget = _ui->stackedWidget->widget(0);

  _pluginEditorsWidget = _ui->tabWidget->widget(1);
  _pluginControlWidget = _ui->stackedWidget->widget(1);

  _moduleEditorsWidget = _ui->tabWidget->widget(2);
  _moduleControlWidget = _ui->stackedWidget->widget(2);

  connect(_pythonInterpreter, SIGNAL(scriptExecutionPaused()), this, SLOT(currentScriptPaused()));
  _pythonInterpreter->runString(utilityFunctions);

  connect(_ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(currentTabChanged(int)));

  connect(_ui->newModuleButton, SIGNAL(clicked()), this, SLOT(newFileModule()));
  connect(_ui->newStringModuleButton, SIGNAL(clicked()), this, SLOT(newStringModule()));
  connect(_ui->loadModuleButton, SIGNAL(clicked()), this, SLOT(loadModule()));
  connect(_ui->saveModuleButton, SIGNAL(clicked()), this, SLOT(saveModule()));
  connect(_ui->newPluginButton, SIGNAL(clicked()), this, SLOT(newPythonPlugin()));
  connect(_ui->loadPluginButton, SIGNAL(clicked()), this, SLOT(loadPythonPlugin()));
  connect(_ui->savePluginButton, SIGNAL(clicked()), this, SLOT(savePythonPlugin()));
  connect(_ui->registerPluginButton, SIGNAL(clicked()), this, SLOT(registerPythonPlugin()));
  connect(_ui->removePluginButton, SIGNAL(clicked()), this, SLOT(removePythonPlugin()));
  connect(_ui->consoleWidget, SIGNAL(anchorClicked(const QUrl &)), this,
          SLOT(scrollToEditorLine(const QUrl &)));
  connect(_ui->decreaseFontSizeButton, SIGNAL(clicked()), this, SLOT(decreaseFontSize()));
  connect(_ui->increaseFontSizeButton, SIGNAL(clicked()), this, SLOT(increaseFontSize()));
  connect(_ui->decreaseFontSizeButton_2, SIGNAL(clicked()), this, SLOT(decreaseFontSize()));
  connect(_ui->increaseFontSizeButton_2, SIGNAL(clicked()), this, SLOT(increaseFontSize()));
  connect(_ui->decreaseFontSizeButton_3, SIGNAL(clicked()), this, SLOT(decreaseFontSize()));
  connect(_ui->increaseFontSizeButton_3, SIGNAL(clicked()), this, SLOT(increaseFontSize()));
  auto shortCut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Minus), _ui->tabWidget);
  connect(shortCut, SIGNAL(activated()), this, SLOT(decreaseFontSize()));
  SET_TOOLTIP_WITH_CTRL_SHORTCUT(_ui->decreaseFontSizeButton, "decrease font size", "-");
  SET_TOOLTIP_WITH_CTRL_SHORTCUT(_ui->decreaseFontSizeButton_2, "decrease font size", "-");
  SET_TOOLTIP_WITH_CTRL_SHORTCUT(_ui->decreaseFontSizeButton_3, "decrease font size", "-");
  shortCut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Plus), _ui->tabWidget);
  connect(shortCut, SIGNAL(activated()), this, SLOT(increaseFontSize()));
  SET_TOOLTIP_WITH_CTRL_SHORTCUT(_ui->increaseFontSizeButton, "increase font size", "-");
  SET_TOOLTIP_WITH_CTRL_SHORTCUT(_ui->increaseFontSizeButton_2, "increase font size", "-");
  SET_TOOLTIP_WITH_CTRL_SHORTCUT(_ui->increaseFontSizeButton_3, "increase font size", "-");

  connect(_ui->mainScriptsTabWidget, SIGNAL(fileSaved(int)), this, SLOT(scriptSaved(int)));
  connect(_ui->modulesTabWidget, SIGNAL(fileSaved(int)), this, SLOT(moduleSaved(int)));
  connect(_ui->pluginsTabWidget, SIGNAL(fileSaved(int)), this, SLOT(pluginSaved(int)));

  connect(_ui->runScriptButton, SIGNAL(clicked()), this, SLOT(executeCurrentScript()));
  connect(_ui->pauseScriptButton, SIGNAL(clicked()), this, SLOT(pauseCurrentScript()));
  connect(_ui->stopScriptButton, SIGNAL(clicked()), this, SLOT(stopCurrentScript()));
  _ui->progressBar->hide();

  connect(_ui->newMainScriptButton, SIGNAL(clicked()), this, SLOT(newScript()));
  connect(_ui->loadMainScriptButton, SIGNAL(clicked()), this, SLOT(loadScript()));
  connect(_ui->saveMainScriptButton, SIGNAL(clicked()), this, SLOT(saveScript()));
  connect(_ui->graphComboBox, SIGNAL(currentItemChanged()), this,
          SLOT(graphComboBoxIndexChanged()));

  connect(_ui->modulesTabWidget, SIGNAL(filesReloaded()), _ui->mainScriptsTabWidget,
          SLOT(reloadCodeInEditorsIfNeeded()));
  connect(_ui->modulesTabWidget, SIGNAL(filesReloaded()), _ui->pluginsTabWidget,
          SLOT(reloadCodeInEditorsIfNeeded()));
  connect(_ui->mainScriptsTabWidget, SIGNAL(filesReloaded()), _ui->modulesTabWidget,
          SLOT(reloadCodeInEditorsIfNeeded()));
  connect(_ui->mainScriptsTabWidget, SIGNAL(filesReloaded()), _ui->pluginsTabWidget,
          SLOT(reloadCodeInEditorsIfNeeded()));
  connect(_ui->pluginsTabWidget, SIGNAL(filesReloaded()), _ui->modulesTabWidget,
          SLOT(reloadCodeInEditorsIfNeeded()));
  connect(_ui->pluginsTabWidget, SIGNAL(filesReloaded()), _ui->mainScriptsTabWidget,
          SLOT(reloadCodeInEditorsIfNeeded()));

  connect(_ui->modulesTabWidget, SIGNAL(tabAboutToBeDeleted(int)), this,
          SLOT(closeModuleTabRequested(int)));
  connect(_ui->mainScriptsTabWidget, SIGNAL(tabAboutToBeDeleted(int)), this,
          SLOT(closeScriptTabRequested(int)));
  connect(_ui->pluginsTabWidget, SIGNAL(tabAboutToBeDeleted(int)), this,
          SLOT(closePluginTabRequested(int)));

  connect(_ui->modulesTabWidget->tabBar(), SIGNAL(tabMoved(int, int)), this,
          SLOT(saveAllModules()));
  connect(_ui->mainScriptsTabWidget->tabBar(), SIGNAL(tabMoved(int, int)), this,
          SLOT(saveAllScripts()));
  connect(_ui->pluginsTabWidget->tabBar(), SIGNAL(tabMoved(int, int)), this,
          SLOT(saveAllPlugins()));

  APIDataBase::getInstance()->loadApiFile(tlpStringToQString(tlp::TulipShareDir) +
                                          "/apiFiles/tulip.api");
  APIDataBase::getInstance()->loadApiFile(
      tlpStringToQString(tlp::TulipShareDir) + "/apiFiles/Python-" +
      PythonInterpreter::getInstance()->getPythonVersionStr() + ".api");
  APIDataBase::getInstance()->loadApiFile(tlpStringToQString(tlp::TulipShareDir) +
                                          "/apiFiles/tulipgui.api");
}

PythonIDE::~PythonIDE() {
  delete _ui;
}

int PythonIDE::addModuleEditor(const QString &fileName) {
  int idx = _ui->modulesTabWidget->addEditor(fileName);
  getModuleEditor(idx)->getAutoCompletionDb()->setGraph(getSelectedGraph());
  return idx;
}

int PythonIDE::addPluginEditor(const QString &fileName) {
  int idx = _ui->pluginsTabWidget->addEditor(fileName);
  getPluginEditor(idx)->getAutoCompletionDb()->setGraph(getSelectedGraph());
  _ui->registerPluginButton->setEnabled(true);
  _ui->removePluginButton->setEnabled(true);
  return idx;
}

PythonCodeEditor *PythonIDE::getCurrentModuleEditor() const {
  return _ui->modulesTabWidget->getCurrentEditor();
}

PythonCodeEditor *PythonIDE::getModuleEditor(int idx) const {
  return _ui->modulesTabWidget->getEditor(idx);
}

PythonCodeEditor *PythonIDE::getPluginEditor(int idx) const {
  return _ui->pluginsTabWidget->getEditor(idx);
}

PythonCodeEditor *PythonIDE::getCurrentPluginEditor() const {
  return _ui->pluginsTabWidget->getCurrentEditor();
}

PythonCodeEditor *PythonIDE::getMainScriptEditor(int idx) const {
  return _ui->mainScriptsTabWidget->getEditor(idx);
}

PythonCodeEditor *PythonIDE::getCurrentMainScriptEditor() const {
  return _ui->mainScriptsTabWidget->getCurrentEditor();
}

void PythonIDE::loadModule() {
  QString fileName = QFileDialog::getOpenFileName(this, "Open module", "", "Python script (*.py)");
  loadModule(fileName);
}

bool PythonIDE::loadModule(const QString &fileName) {
  QFile file(fileName);

  if (!file.exists())
    return false;

  QFileInfo fileInfo(file);

  int editorId = addModuleEditor(fileInfo.absoluteFilePath());

  _pythonInterpreter->addModuleSearchPath(fileInfo.absolutePath());

  saveModule(editorId);

  return true;
}

void PythonIDE::saveModule(int tabIdx) {
  if (tabIdx >= 0 && tabIdx < _ui->modulesTabWidget->count()) {
    QString moduleNameExt = _ui->modulesTabWidget->tabText(tabIdx);
    QString moduleName;

    if (moduleNameExt[moduleNameExt.size() - 1] == '*')
      moduleName = moduleNameExt.mid(0, moduleNameExt.size() - 4);
    else
      moduleName = moduleNameExt.mid(0, moduleNameExt.size() - 3);

    // workaround a Qt5 bug on linux
    moduleName = moduleName.replace("&", "");

    _pythonInterpreter->deleteModule(moduleName);
    _ui->modulesTabWidget->setTabText(tabIdx, moduleName + ".py");
    QString fileName = getModuleEditor(tabIdx)->getFileName();

    // string module special case
    if (fileName.isEmpty()) {
      QString tabText = _ui->modulesTabWidget->tabText(tabIdx);
      fileName = tabText.replace("&", "");
    }

    QFileInfo fileInfo(fileName);

    if (getModuleEditor(tabIdx)->saveCodeToFile()) {
      _ui->modulesTabWidget->setTabToolTip(tabIdx, fileInfo.absoluteFilePath());
    }

    writeModulesFilesList();
    writeFileToProject(PYTHON_MODULES_PATH + "/" + fileInfo.fileName(),
                       getModuleEditor(tabIdx)->getCleanCode());
  }
}

void PythonIDE::newFileModule() {
  QString fileName =
      QFileDialog::getSaveFileName(this, tr("Set module filename"), "", "Python script (*.py)");

  if (fileName.isEmpty())
    return;

  if (!fileName.endsWith(".py"))
    fileName += ".py";

  QFile file(fileName);

  if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    return;

  file.close();

  QFileInfo fileInfo(file);
  QString modulePath(fileInfo.absolutePath());

  int editorId = addModuleEditor(fileInfo.absoluteFilePath());
  saveModule(editorId);
  _pythonInterpreter->addModuleSearchPath(modulePath);
}

void PythonIDE::newStringModule() {
  bool ok;
  QString moduleName = QInputDialog::getText(this, "New string module  ",
                                             "module name :", QLineEdit::Normal, "", &ok);

  if (ok && !moduleName.isEmpty()) {
    if (!moduleName.endsWith(".py"))
      moduleName += ".py";

    int editorId = addModuleEditor(moduleName);
    saveModule(editorId);
  }
}

void PythonIDE::saveModule() {
  int curModule = _ui->modulesTabWidget->currentIndex();

  if (curModule == -1)
    return;

  saveModule(curModule);
}

void PythonIDE::saveAllModules() {
  for (int i = 0; i < _ui->modulesTabWidget->count(); ++i) {
    saveModule(i);
  }
}

bool PythonIDE::reloadAllModules() const {

  bool ret = true;

  for (int i = 0; i < _ui->modulesTabWidget->count(); ++i) {
    QString moduleNameExt = _ui->modulesTabWidget->tabText(i);
    QString moduleName;

    if (moduleNameExt[moduleNameExt.size() - 1] == '*')
      moduleName = moduleNameExt.mid(0, moduleNameExt.size() - 4);
    else
      moduleName = moduleNameExt.mid(0, moduleNameExt.size() - 3);

    // workaround a Qt5 bug on linux
    moduleName = moduleName.replace("&", "");

    _pythonInterpreter->deleteModule(moduleName);
    QFileInfo fileInfo(getModuleEditor(i)->getFileName());

    if (fileInfo.fileName() == getModuleEditor(i)->getFileName()) {
      ret = ret && _pythonInterpreter->registerNewModuleFromString(
                       moduleName, getModuleEditor(i)->getCleanCode());
    } else {
      _pythonInterpreter->addModuleSearchPath(fileInfo.absolutePath());
      ret = ret && _pythonInterpreter->reloadModule(moduleName);
    }
  }

  return ret;
}

void PythonIDE::newPythonPlugin() {
  PythonPluginCreationDialog pluginCreationDialog(this);

  if (pluginCreationDialog.exec() == QDialog::Accepted) {
    QFile file(pluginCreationDialog.getPluginFileName());

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
      return;

    QFileInfo fileInfo(file);
    QString modulePath(fileInfo.absolutePath());

    int editorId = addPluginEditor(fileInfo.absoluteFilePath());
    _ui->pluginsTabWidget->setTabToolTip(editorId, fileInfo.absoluteFilePath());
    PythonInterpreter::getInstance()->addModuleSearchPath(modulePath);
    _ui->pluginsTabWidget->setTabText(editorId, QString("[") +
                                                    pluginCreationDialog.getPluginType() +
                                                    QString("] ") + fileInfo.fileName());

    QString pluginFile = fileInfo.absoluteFilePath();
    _editedPluginsClassName[pluginFile] = pluginCreationDialog.getPluginClassName();
    _editedPluginsType[pluginFile] = pluginCreationDialog.getPluginType();
    _editedPluginsName[pluginFile] = pluginCreationDialog.getPluginName();
    QString pluginSkeleton = getTulipPythonPluginSkeleton(
        pluginCreationDialog.getPluginClassName(), pluginCreationDialog.getPluginType(),
        pluginCreationDialog.getPluginName(), pluginCreationDialog.getPluginAuthor(),
        pluginCreationDialog.getPluginDate(), pluginCreationDialog.getPluginInfo(),
        pluginCreationDialog.getPluginRelease(), pluginCreationDialog.getPluginGroup());

    getPluginEditor(editorId)->setPlainText(pluginSkeleton);
    savePythonPlugin();
  }
}

void PythonIDE::currentTabChanged(int index) {
  _ui->stackedWidget->setCurrentIndex(index);
}

static bool checkAndGetPluginInfoFromSrcCode(const QString &pluginCode, QString &pluginName,
                                             QString &pluginClassName, QString &pluginType,
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

    rx.setPattern("^.*registerPlugin.*\\(.*['\"]([^,]+)['\"],.*['\"]([^,]+)['\"],.*$");

    if (rx.indexIn(pluginCode) != -1) {
      pluginName = rx.cap(2);
      return true;
    }
  }

  return false;
}

void PythonIDE::loadPythonPlugin() {
  QString fileName =
      QFileDialog::getOpenFileName(this, "Open Tulip Python plugin", "", "Python script (*.py)");
  loadPythonPlugin(fileName);
}

bool PythonIDE::loadPythonPlugin(const QString &fileName, bool clear) {

  if (_editedPluginsClassName.find(fileName) != _editedPluginsClassName.end()) {
    return true;
  }

  QFile file(fileName);

  if (!file.exists())
    return false;

  QFileInfo fileInfo(file);
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

  if (checkAndGetPluginInfoFromSrcCode(pluginCode, pluginName, pluginClassName, pluginType,
                                       pluginClass)) {
    if (pluginClassName.isEmpty() || pluginName.isEmpty()) {
      QMessageBox::critical(
          this, "Error",
          "Unable to retrieve the plugin class name and the plugin name from the source code\n.");
      return false;
    } else {
      int editorId = addPluginEditor(fileInfo.absoluteFilePath());
      _pythonInterpreter->addModuleSearchPath(modulePath);
      _ui->pluginsTabWidget->setTabToolTip(editorId, fileInfo.absoluteFilePath());
      _ui->pluginsTabWidget->setTabText(editorId, QString("[") + pluginType + QString("] ") +
                                                      fileInfo.fileName());
      QString pluginFile = fileInfo.absoluteFilePath();
      _editedPluginsClassName[pluginFile] = pluginClassName;
      _editedPluginsType[pluginFile] = pluginType;
      _editedPluginsName[pluginFile] = pluginName;
      registerPythonPlugin(clear);
      savePythonPlugin(editorId);
    }
  } else {
    QMessageBox::critical(
        this, "Error",
        "The file " + fileName +
            " does not seem to contain the source code of a Tulip Python plugin.");
    return false;
  }

  return true;
}

bool PythonIDE::loadPythonPluginFromSrcCode(const QString &moduleName, const QString &pluginSrcCode,
                                            bool clear) {
  QString pluginType = "";
  QString pluginClass = "";
  QString pluginClassName = "";
  QString pluginName = "";

  if (checkAndGetPluginInfoFromSrcCode(pluginSrcCode, pluginName, pluginClassName, pluginType,
                                       pluginClass)) {
    if (!pluginName.isEmpty() && !pluginClassName.isEmpty()) {
      int editorId = addPluginEditor(moduleName);
      PythonCodeEditor *codeEditor = getPluginEditor(editorId);
      codeEditor->setPlainText(pluginSrcCode);
      codeEditor->setFileName(moduleName);
      savePythonPlugin(editorId);
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

void PythonIDE::writeFileToProject(const QString &projectFile, const QString &fileContent) {

  if (!_project || !_saveFilesToProject) {
    return;
  }

  hasher.reset();
  hasher.addData(fileContent.toUtf8());
  QByteArray fileContentHash = hasher.result();

  bool fileModified = false;

  if (!_project->exists(projectFile)) {
    _project->touch(projectFile);
    fileModified = true;
  } else {
    hasher.reset();
    QIODevice *fs = _project->fileStream(projectFile, QIODevice::ReadOnly | QIODevice::Text);
    hasher.addData(fs->readAll());
    delete fs;
    fileModified = fileContentHash != hasher.result();
  }

  if (fileModified) {
    QIODevice *fs = _project->fileStream(projectFile, QIODevice::WriteOnly | QIODevice::Text);
    fs->write(fileContent.toUtf8());
    fs->close();
    delete fs;
  }

  // notify the Tulip main window that the Tulip project has been modified
  if (Perspective::instance() && _notifyProjectModified && fileModified) {
    Perspective::instance()->mainWindow()->setWindowModified(true);
  }
}

void PythonIDE::savePythonPlugin() {
  savePythonPlugin(_ui->pluginsTabWidget->currentIndex());
}

void PythonIDE::savePythonPlugin(int tabIdx) {

  if (tabIdx >= 0 && tabIdx < _ui->pluginsTabWidget->count()) {

    QString moduleNameExt = _ui->pluginsTabWidget->tabText(tabIdx);
    QString moduleName;

    if (moduleNameExt[moduleNameExt.size() - 1] == '*')
      moduleName = moduleNameExt.mid(0, moduleNameExt.size() - 4);
    else
      moduleName = moduleNameExt.mid(0, moduleNameExt.size() - 3);

    // workaround a Qt5 bug on linux
    moduleName = moduleName.replace("&", "");

    _ui->pluginsTabWidget->setTabText(tabIdx, moduleName + ".py");
    QFile file(getPluginEditor(tabIdx)->getFileName());
    QFileInfo fileInfo(file);

    getPluginEditor(tabIdx)->saveCodeToFile();
    _ui->pluginsTabWidget->setTabToolTip(tabIdx, getPluginEditor(tabIdx)->getFileName());

    writePluginsFilesList();
    writeFileToProject(PYTHON_PLUGINS_PATH + "/" + fileInfo.fileName(),
                       getPluginEditor(tabIdx)->getCleanCode());
  }
}

void PythonIDE::saveAllPlugins() {
  for (int i = 0; i < _ui->pluginsTabWidget->count(); ++i) {
    savePythonPlugin(i);
  }
}

void PythonIDE::registerPythonPlugin(bool clear) {
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

  // workaround a Qt5 bug on linux
  moduleName = moduleName.replace("&", "");

  QString pluginCode = getPluginEditor(tabIdx)->getCleanCode();

  QString pluginType = "";
  QString pluginClass = "";
  QString pluginClassName = "";
  QString pluginName = "";

  checkAndGetPluginInfoFromSrcCode(pluginCode, pluginName, pluginClassName, pluginType,
                                   pluginClass);

  QString oldPluginName = _editedPluginsName[pluginFile];

  if (tlp::PluginLister::pluginExists(QStringToTlpString(oldPluginName))) {
    tlp::PluginLister::removePlugin(QStringToTlpString(oldPluginName));
  }

  _pythonInterpreter->setConsoleWidget(_ui->consoleWidget);

  if (clear) {
    _ui->consoleWidget->clear();
    _pythonInterpreter->clearOutputBuffers();
  }

  clearErrorIndicators();

  _pythonInterpreter->deleteModule(moduleName);

  _pythonInterpreter->importModule("tulipplugins");
  _pythonInterpreter->runString("tulipplugins.setTestMode(True)");

  bool codeOk = false;
  QFileInfo fileInfo(getPluginEditor(tabIdx)->getFileName());

  if (fileInfo.fileName() == getPluginEditor(tabIdx)->getFileName()) {
    codeOk = _pythonInterpreter->registerNewModuleFromString(
        moduleName, getPluginEditor(tabIdx)->getCleanCode());
  } else {
    codeOk = _pythonInterpreter->reloadModule(moduleName);
  }

  _pythonInterpreter->runString("tulipplugins.setTestMode(False)");

  QString pythonCode;
  QTextStream oss(&pythonCode);

  oss << "import " << moduleName << QT_ENDL;
  oss << "plugin = " << moduleName << "." << pluginClassName << "(tlp.AlgorithmContext())";

  if (codeOk && _pythonInterpreter->runString(pythonCode)) {

    if (fileInfo.fileName() == getPluginEditor(tabIdx)->getFileName()) {
      _pythonInterpreter->registerNewModuleFromString(moduleName,
                                                      getPluginEditor(tabIdx)->getCleanCode());
    } else {
      _pythonInterpreter->reloadModule(moduleName);
    }

    _ui->pluginStatusLabel->setText("Plugin has been successfully registered.");
    _editedPluginsClassName[pluginFile] = pluginClassName;
    _editedPluginsType[pluginFile] = pluginType;
    _editedPluginsName[pluginFile] = pluginName;
  } else {
    _ui->pluginStatusLabel->setText("Plugin registration has failed.");
    indicateErrors();
  }

  _pythonInterpreter->resetConsoleWidget();
}

void PythonIDE::removePythonPlugin() {

  int tabIdx = _ui->pluginsTabWidget->currentIndex();

  if (tabIdx == -1)
    return;

  QString pluginName = _editedPluginsName[getCurrentPluginEditor()->getFileName()];

  if (tlp::PluginLister::pluginExists(QStringToTlpString(pluginName))) {
    tlp::PluginLister::removePlugin(QStringToTlpString(pluginName));
    _ui->pluginStatusLabel->setText("Plugin has been successfully unregistered.");
  } else {
    _ui->pluginStatusLabel->setText("Plugin is not registered in the plugins list");
  }
}

bool PythonIDE::indicateErrors() const {
  QRegExp rx("^.*File.*\"(.*)\".*line.*(\\d+).*$");
  QRegExp rx2("^.*File.*\"(.*)\".*line.*(\\d+).*in (.*)$");

  QMap<QString, QVector<int>> errorLines;
  QString consoleOutput = _pythonInterpreter->getStandardErrorOutput();
  QStringList outputLines = consoleOutput.split("\n");

  for (int i = 0; i < outputLines.count() - 1; ++i) {
    int pos = 0;

    while ((pos = rx.indexIn(outputLines[i], pos)) != -1) {
      rx2.indexIn(outputLines[i], pos);

      QString file = rx.cap(1);
#ifdef WIN32
      file.replace("\\", "/");
#endif
      int line = rx.cap(2).toInt();
      errorLines[file].push_back(line);

      pos += rx.matchedLength();
    }
  }

  _ui->pluginsTabWidget->indicateErrors(errorLines);
  _ui->modulesTabWidget->indicateErrors(errorLines);
  _ui->mainScriptsTabWidget->indicateErrors(errorLines);

  if (errorLines.find("<unnamed script>") != errorLines.end()) {
    for (int i = 0; i < errorLines["<unnamed script>"].size(); ++i) {
      if (errorLines["<unnamed script>"][i] > 1) {
        getCurrentMainScriptEditor()->indicateScriptCurrentError(errorLines["<unnamed script>"][i] -
                                                                 1);
      }
    }
  }

  return !errorLines.isEmpty();
}

void PythonIDE::clearErrorIndicators() const {
  _ui->mainScriptsTabWidget->clearErrorIndicators();
  _ui->pluginsTabWidget->clearErrorIndicators();
  _ui->modulesTabWidget->clearErrorIndicators();
}

void PythonIDE::scrollToEditorLine(const QUrl &link) {
  QString linkStr = QUrl::fromPercentEncoding(link.toEncoded());
#ifdef WIN32
  linkStr.replace("\\", "/");
#endif
  QStringList strList = linkStr.split(":");
  QString file = strList.at(0);

  for (int i = 1; i < strList.size() - 1; ++i) {
    file += (":" + strList.at(i));
  }

  int line = strList.at(strList.size() - 1).toInt() - 1;

  if (file == "<unnamed script>") {
    _ui->tabWidget->setCurrentIndex(0);
    getCurrentMainScriptEditor()->scrollToLine(line);
    return;
  }

// Qt5 on windows sets the drive letter as lowercase when converting the url to a string
// Resets it to uppercase as it was originally
#if defined(WIN32)
  file[0] = file[0].toUpper();
#endif

  for (int i = 0; i < _ui->mainScriptsTabWidget->count(); ++i) {
    PythonCodeEditor *codeEditor = getMainScriptEditor(i);

    if (file == codeEditor->getFileName()) {
      _ui->tabWidget->setCurrentIndex(0);
      _ui->mainScriptsTabWidget->setCurrentIndex(i);
      codeEditor->scrollToLine(line);
      return;
    }
  }

  for (int i = 0; i < _ui->pluginsTabWidget->count(); ++i) {
    PythonCodeEditor *codeEditor = getPluginEditor(i);

    if (file == codeEditor->getFileName()) {
      _ui->tabWidget->setCurrentIndex(1);
      _ui->pluginsTabWidget->setCurrentIndex(i);
      codeEditor->scrollToLine(line);
      return;
    }
  }

  for (int i = 0; i < _ui->modulesTabWidget->count(); ++i) {
    PythonCodeEditor *codeEditor = getModuleEditor(i);

    if (file == codeEditor->getFileName()) {
      _ui->tabWidget->setCurrentIndex(2);
      _ui->modulesTabWidget->setCurrentIndex(i);
      codeEditor->scrollToLine(line);
      return;
    }
  }
}

void PythonIDE::decreaseFontSize() {
  _ui->mainScriptsTabWidget->decreaseFontSize();
  _ui->pluginsTabWidget->decreaseFontSize();
  _ui->modulesTabWidget->decreaseFontSize();
  _pythonPanel->decreaseFontSize();
  _ui->consoleWidget->zoomOut();
}

void PythonIDE::increaseFontSize() {
  _ui->mainScriptsTabWidget->increaseFontSize();
  _ui->pluginsTabWidget->increaseFontSize();
  _ui->modulesTabWidget->increaseFontSize();
  _pythonPanel->increaseFontSize();
  _ui->consoleWidget->zoomIn();
}

QString PythonIDE::readProjectFile(const QString &filePath) {
  QString content;

  if (_project) {
    QIODevice *fs = _project->fileStream(filePath, QIODevice::ReadOnly | QIODevice::Text);

    QString currentLine = fs->readLine();

    while (!currentLine.isEmpty()) {
      content += currentLine;
      currentLine = fs->readLine();
    }

    fs->close();
    delete fs;
  }

  return content;
}

void PythonIDE::createTulipProjectPythonPaths() {
  if (!_project) {
    return;
  }

  if (!_project->exists(PYTHON_SCRIPTS_PATH)) {
    _project->mkpath(PYTHON_SCRIPTS_PATH);
  }

  if (!_project->exists(PYTHON_PLUGINS_PATH)) {
    _project->mkpath(PYTHON_PLUGINS_PATH);
  }

  if (!_project->exists(PYTHON_MODULES_PATH)) {
    _project->mkpath(PYTHON_MODULES_PATH);
  }
}

bool PythonIDE::projectNeedsPythonIDE(tlp::TulipProject *project) {
  if (project->exists(PYTHON_MODULES_FILES) || project->exists(PYTHON_PLUGINS_FILES) ||
      project->exists(PYTHON_SCRIPTS_FILES))
    return true;
  // for backward compatibility with Tulip < 5.0, load scripts and modules saved in old Python
  // Script view configurations
  QStringList entries = project->entryList("views", QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name);

  for (const QString &entry : entries) {
    QIODevice *xmlFile = project->fileStream("views/" + entry + "/view.xml");
    QXmlStreamReader doc(xmlFile);

    if (doc.readNextStartElement()) {
      bool needPython = false;
      if (!doc.hasError()) {
        QString viewName = doc.attributes().value("name").toString();
        needPython = (viewName == "Python Script view");
      }
      xmlFile->close();
      delete xmlFile;
      if (needPython)
        return true;
    }
  }
  return false;
}

void PythonIDE::setProject(tlp::TulipProject *project) {

  _project = project;

  if (!_project) {
    return;
  }

  _notifyProjectModified = false;

  // we don't want to save files to project as a side effect here
  _saveFilesToProject = false;

  createTulipProjectPythonPaths();

  if (_project->exists(PYTHON_MODULES_FILES)) {
    QIODevice *fs =
        _project->fileStream(PYTHON_MODULES_FILES, QIODevice::ReadOnly | QIODevice::Text);
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
    QIODevice *fs =
        _project->fileStream(PYTHON_PLUGINS_FILES, QIODevice::ReadOnly | QIODevice::Text);
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

  if (_project->exists(PYTHON_SCRIPTS_FILES)) {
    QIODevice *fs =
        _project->fileStream(PYTHON_SCRIPTS_FILES, QIODevice::ReadOnly | QIODevice::Text);
    QString file = fs->readLine();

    while (!file.isEmpty()) {
      file = file.mid(0, file.size() - 1);

      if (!loadScript(file)) {

        QFileInfo fileInfo(file);
        QString projectFile = PYTHON_SCRIPTS_PATH + "/" + fileInfo.fileName();

        if (_project->exists(projectFile)) {
          QString code = readProjectFile(projectFile);
          QString fileName = fileInfo.fileName();
          int editorId = addMainScriptEditor();

          if (fileName.startsWith("[no file]")) {
            fileName = "[no file]";
            _ui->mainScriptsTabWidget->setTabToolTip(
                editorId, "string main script, do not forget to save the current Tulip project "
                          "or\n save the script to a file to not lose your source code "
                          "modifications.");
          } else {
            _ui->mainScriptsTabWidget->setTabToolTip(editorId, fileName);
          }

          getMainScriptEditor(editorId)->setPlainText(code);
          _ui->mainScriptsTabWidget->setTabText(editorId, fileName);
        }
      }

      file = fs->readLine();
    }

    fs->close();
    delete fs;

    // for backward compatibility with Tulip < 5.0, load scripts and modules saved in old Python
    // Script view configurations
  } else {
    QStringList entries =
        project->entryList("views", QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name);

    for (const QString &entry : entries) {
      QIODevice *xmlFile = project->fileStream("views/" + entry + "/view.xml");
      QXmlStreamReader doc(xmlFile);

      if (doc.readNextStartElement()) {
        if (!doc.hasError()) {
          QString viewName = doc.attributes().value("name").toString();
          doc.readNextStartElement();
          QString data(doc.readElementText());
          xmlFile->close();
          delete xmlFile;

          if (viewName == "Python Script view") {
            DataSet dataSet;
            std::istringstream iss(QStringToTlpString(data));
            DataSet::read(iss, dataSet);
            loadScriptsAndModulesFromPythonScriptViewDataSet(dataSet);
          }
        }
      }
    }
  }

  if (_ui->mainScriptsTabWidget->count() > 0) {
    _ui->mainScriptsTabWidget->setCurrentIndex(0);
  }

  if (_ui->pluginsTabWidget->count() > 0) {
    _ui->pluginsTabWidget->setCurrentIndex(0);
  }

  if (_ui->modulesTabWidget->count() > 0) {
    _ui->modulesTabWidget->setCurrentIndex(0);
  }

  _notifyProjectModified = true;
  _saveFilesToProject = true;
}

void PythonIDE::writeScriptsFilesList(int deleted) {
  if (!_project || !_saveFilesToProject) {
    return;
  }

  QStringList existingScriptFilenames;
  QString scriptFilesList;

  for (int i = 0; i < _ui->mainScriptsTabWidget->count(); ++i) {
    QString fileName = getMainScriptEditor(i)->getFileName();

    if (deleted == -1 || i != deleted) {

      if (fileName.isEmpty()) {
        QString tabText = _ui->mainScriptsTabWidget->tabText(i);
        tabText = tabText.replace("&", "");

        if (tabText.endsWith(".py")) {
          fileName = tabText;
        } else {
          fileName = "[no file]" + QString::number(i);
        }
      } else if (!_project->projectFile().isEmpty()) {
        // if the Python file is relatively located to the project file
        // only save a relative path for reloading the code from the file even on a different
        // computer
        // this is useful for instance when Tulip project and Python files are stored on a git
        // repository
        QFileInfo projectFileInfo(_project->projectFile());

        if (fileName.startsWith(projectFileInfo.absolutePath())) {
          fileName = fileName.mid(projectFileInfo.absolutePath().length() + 1);
        }
      }

      scriptFilesList += (fileName + "\n");
      existingScriptFilenames.append(QFileInfo(fileName).fileName());
    }
  }

  hasher.reset();
  hasher.addData(scriptFilesList.toUtf8());
  QByteArray scriptFilesListHash = hasher.result();

  bool fileModified = false;

  createTulipProjectPythonPaths();

  if (!_project->exists(PYTHON_SCRIPTS_FILES)) {
    _project->touch(PYTHON_SCRIPTS_FILES);
    fileModified = true;
  } else {
    QIODevice *fs =
        _project->fileStream(PYTHON_SCRIPTS_FILES, QIODevice::ReadOnly | QIODevice::Text);
    hasher.reset();
    hasher.addData(fs->readAll());
    delete fs;
    fileModified = scriptFilesListHash != hasher.result();
  }

  if (fileModified) {
    QIODevice *fs =
        _project->fileStream(PYTHON_SCRIPTS_FILES, QIODevice::WriteOnly | QIODevice::Text);
    fs->write(scriptFilesList.toUtf8());
    fs->close();
    delete fs;
  }

  deleteFilesFromProjectIfRemoved(PYTHON_SCRIPTS_PATH, existingScriptFilenames);

  // notify the Tulip main window that the Tulip project has been modified
  if (Perspective::instance() && _notifyProjectModified && fileModified) {
    Perspective::instance()->mainWindow()->setWindowModified(true);
  }
}

void PythonIDE::writePluginsFilesList(int deleted) {
  if (!_project || !_saveFilesToProject) {
    return;
  }

  QStringList existingPluginsFilenames;
  QString pluginFilesList;

  for (int i = 0; i < _ui->pluginsTabWidget->count(); ++i) {
    if (deleted == -1 || i != deleted) {
      pluginFilesList += (getPluginEditor(i)->getFileName() + "\n");
      existingPluginsFilenames.append(QFileInfo(getPluginEditor(i)->getFileName()).fileName());
    }
  }

  hasher.reset();
  hasher.addData(pluginFilesList.toUtf8());
  QByteArray pluginFilesListHash = hasher.result();

  bool fileModified = false;

  createTulipProjectPythonPaths();

  if (!_project->exists(PYTHON_PLUGINS_FILES)) {
    _project->touch(PYTHON_PLUGINS_FILES);
    fileModified = true;
  } else {
    hasher.reset();
    QIODevice *fs =
        _project->fileStream(PYTHON_PLUGINS_FILES, QIODevice::ReadOnly | QIODevice::Text);
    hasher.addData(fs->readAll());
    delete fs;
    fileModified = pluginFilesListHash != hasher.result();
  }

  if (fileModified) {
    QIODevice *fs =
        _project->fileStream(PYTHON_PLUGINS_FILES, QIODevice::WriteOnly | QIODevice::Text);
    fs->write(pluginFilesList.toUtf8());
    fs->close();
    delete fs;
  }

  deleteFilesFromProjectIfRemoved(PYTHON_PLUGINS_PATH, existingPluginsFilenames);

  // notify the Tulip main window that the Tulip project has been modified
  if (Perspective::instance() && _notifyProjectModified && fileModified) {
    Perspective::instance()->mainWindow()->setWindowModified(true);
  }
}

void PythonIDE::writeModulesFilesList(int deleted) {
  if (!_project || !_saveFilesToProject) {
    return;
  }

  QStringList existingModuleFilenames;
  QString moduleFilesList;

  for (int i = 0; i < _ui->modulesTabWidget->count(); ++i) {
    QString fileName = getModuleEditor(i)->getFileName();

    if (deleted == -1 || i != deleted) {
      // string module special case
      if (fileName.isEmpty()) {
        QString tabText = _ui->modulesTabWidget->tabText(i);
        fileName = tabText.replace("&", "");
      }

      moduleFilesList += (fileName + "\n");
      existingModuleFilenames.append(QFileInfo(fileName).fileName());
    }
  }

  hasher.reset();
  hasher.addData(moduleFilesList.toUtf8());

  QByteArray moduleFilesListHash = hasher.result();

  bool fileModified = false;

  createTulipProjectPythonPaths();

  if (!_project->exists(PYTHON_MODULES_FILES)) {
    _project->touch(PYTHON_MODULES_FILES);
    fileModified = true;
  } else {
    hasher.reset();
    QIODevice *fs =
        _project->fileStream(PYTHON_MODULES_FILES, QIODevice::ReadOnly | QIODevice::Text);
    hasher.addData(fs->readAll());
    delete fs;
    fileModified = moduleFilesListHash != hasher.result();
  }

  if (fileModified) {
    QIODevice *fs =
        _project->fileStream(PYTHON_MODULES_FILES, QIODevice::WriteOnly | QIODevice::Text);
    fs->write(moduleFilesList.toUtf8());
    fs->close();
    delete fs;
  }

  deleteFilesFromProjectIfRemoved(PYTHON_MODULES_PATH, existingModuleFilenames);

  // notify the Tulip main window that the Tulip project has been modified
  if (Perspective::instance() && _notifyProjectModified && fileModified) {
    Perspective::instance()->mainWindow()->setWindowModified(true);
  }
}

void PythonIDE::scriptSaved(int idx) {
  QString fileName = getMainScriptEditor(idx)->getFileName();
  QFileInfo fileInfo(fileName);
  writeScriptsFilesList();
  writeScriptFileToProject(idx, fileInfo.fileName(), getMainScriptEditor(idx)->getCleanCode());
}

void PythonIDE::writeScriptFileToProject(int idx, const QString &scriptFileName,
                                         const QString &scriptContent) {
  QString fileName = scriptFileName;

  if (fileName.isEmpty()) {
    fileName = "[no file]" + QString::number(idx);
  }

  writeFileToProject(PYTHON_SCRIPTS_PATH + "/" + fileName, scriptContent);
}

void PythonIDE::deleteFilesFromProjectIfRemoved(const QString &projectDir,
                                                const QStringList &existingFilenames) {
  QStringList filesInProject = _project->entryList(projectDir);

  for (const QString &filename : filesInProject) {
    if (filename == "files")
      continue;

    if (!existingFilenames.contains(filename)) {
      _project->removeFile(projectDir + "/" + filename);
    }
  }
}

void PythonIDE::pluginSaved(int idx) {
  QString fileName = getPluginEditor(idx)->getFileName();
  QFileInfo fileInfo(fileName);
  writePluginsFilesList();
  writeFileToProject(PYTHON_PLUGINS_PATH + "/" + fileInfo.fileName(),
                     getPluginEditor(idx)->getCleanCode());
}

void PythonIDE::moduleSaved(int idx) {
  saveModule(idx);
}

int PythonIDE::addMainScriptEditor(const QString &fileName) {
  int idx = _ui->mainScriptsTabWidget->addEditor(fileName);
  getMainScriptEditor(idx)->installEventFilter(this);
  getMainScriptEditor(idx)->getAutoCompletionDb()->setGraph(getSelectedGraph());

  if (_ui->mainScriptsTabWidget->count() == 1) {
    _ui->runScriptButton->setEnabled(true);
  }

  return idx;
}

void PythonIDE::pauseCurrentScript() {
  _pythonInterpreter->pauseCurrentScript();
}

void PythonIDE::currentScriptPaused() {
  Observable::unholdObservers();
  _ui->pauseScriptButton->setEnabled(false);
  _ui->runScriptButton->setEnabled(true);
  _ui->runScriptButton->setToolTip("Resume script (Ctrl + Return)");
  _ui->progressBar->setRange(0, 100);
  _ui->progressBar->reset();
}

void PythonIDE::newScript() {
  int editorId = addMainScriptEditor();
  QString defaultScriptCode =
      getDefaultScriptCode(_pythonInterpreter->getPythonVersionStr(), getSelectedGraph());
  getMainScriptEditor(editorId)->setPlainText(defaultScriptCode);
  _ui->mainScriptsTabWidget->setTabText(editorId, "[no file]");
  _ui->mainScriptsTabWidget->setTabToolTip(
      editorId, "string main script, do not forget to save the current Tulip project or\n save the "
                "script to a file to not lose your source code modifications.");
  saveScript(editorId, false);
}

void PythonIDE::loadScript() {
  QString fileName =
      QFileDialog::getOpenFileName(this, "Open main script", "", "Python script (*.py)");
  loadScript(fileName);
}

bool PythonIDE::loadScript(const QString &fileName, bool clear) {
  QFile file(fileName);

  if (!file.exists()) {
    return false;
  }

  QFileInfo fileInfo(file);

  int editorId = addMainScriptEditor(fileInfo.absoluteFilePath());

  QString modulePath = fileInfo.absolutePath();
  _pythonInterpreter->addModuleSearchPath(modulePath);
  _pythonInterpreter->setConsoleWidget(_ui->consoleWidget);

  if (clear) {
    _ui->consoleWidget->clear();
    _pythonInterpreter->clearOutputBuffers();
  }

  clearErrorIndicators();
  _pythonInterpreter->setOutputEnabled(false);
  _pythonInterpreter->reloadModule(fileInfo.fileName().replace(".py", ""));
  _pythonInterpreter->setOutputEnabled(true);
  indicateErrors();
  _pythonInterpreter->resetConsoleWidget();

  saveScript(editorId, false);

  return true;
}

void PythonIDE::saveScript() {
  saveScript(_ui->mainScriptsTabWidget->currentIndex(), true, true);
}

void PythonIDE::saveScript(int tabIdx, bool clear, bool showFileDialog) {
  if (tabIdx >= 0 && tabIdx < _ui->mainScriptsTabWidget->count()) {
    QString fileName;
    QString mainScriptFileName = getMainScriptEditor(tabIdx)->getFileName();

    QString tabText = _ui->mainScriptsTabWidget->tabText(tabIdx);

    // workaround a Qt5 bug on linux
    tabText = tabText.replace("&", "");

    if (mainScriptFileName.isEmpty() && showFileDialog) {
      QString dir = "";

      if (!tabText.startsWith("[")) {
        dir = tabText;
      }

      fileName =
          QFileDialog::getSaveFileName(this, tr("Save main script"), dir, "Python script (*.py)");
    } else
      fileName = mainScriptFileName;

    if (!fileName.isEmpty()) {
      if (!fileName.endsWith(".py"))
        fileName += ".py";

      QFile file(fileName);
      QFileInfo fileInfo(file);

      getMainScriptEditor(tabIdx)->setFileName(fileInfo.absoluteFilePath());
      getMainScriptEditor(tabIdx)->saveCodeToFile();
      _ui->mainScriptsTabWidget->setTabText(tabIdx, fileInfo.fileName());
      _ui->mainScriptsTabWidget->setTabToolTip(tabIdx, fileInfo.absoluteFilePath());

      QString modulePath = fileInfo.absolutePath();
      _pythonInterpreter->addModuleSearchPath(modulePath);
      _pythonInterpreter->setConsoleWidget(_ui->consoleWidget);

      if (clear) {
        _ui->consoleWidget->clear();
        _pythonInterpreter->clearOutputBuffers();
      }

      clearErrorIndicators();
      _pythonInterpreter->setOutputEnabled(false);
      _pythonInterpreter->reloadModule(fileInfo.fileName().replace(".py", ""));
      _pythonInterpreter->setOutputEnabled(true);
      indicateErrors();
      _pythonInterpreter->resetConsoleWidget();

      fileName = fileInfo.fileName();
    } else {

      if (tabText.contains(".py")) {
        fileName = tabText;
        tabText.replace(".py", "");
        _pythonInterpreter->setOutputEnabled(false);
        _pythonInterpreter->registerNewModuleFromString(
            tabText, getMainScriptEditor(tabIdx)->getCleanCode());
        _pythonInterpreter->importModule(tabText);
        _pythonInterpreter->setOutputEnabled(true);
      }
    }

    writeScriptsFilesList();
    writeScriptFileToProject(tabIdx, fileName, getMainScriptEditor(tabIdx)->getCleanCode());
  }
}

void PythonIDE::saveAllScripts() {
  for (int i = 0; i < _ui->mainScriptsTabWidget->count(); ++i) {
    saveScript(i, false);
  }
}

void PythonIDE::setGraphsModel(tlp::GraphHierarchiesModel *model) {
  _graphsModel = model;
  _ui->graphComboBox->setModel(model);
  _pythonPanel->setModel(model);
}

void PythonIDE::dragEnterEvent(QDragEnterEvent *dragEv) {
  const tlp::GraphMimeType *mimeType = dynamic_cast<const tlp::GraphMimeType *>(dragEv->mimeData());

  if (mimeType != nullptr) {
    dragEv->accept();
  }
}

void PythonIDE::dropEvent(QDropEvent *dropEv) {
  const tlp::GraphMimeType *mimeType = dynamic_cast<const tlp::GraphMimeType *>(dropEv->mimeData());

  if (mimeType != nullptr) {
    tlp::GraphHierarchiesModel *model =
        static_cast<tlp::GraphHierarchiesModel *>(_ui->graphComboBox->model());
    QModelIndex graphIndex = model->indexOf(mimeType->graph());

    if (graphIndex == _ui->graphComboBox->selectedIndex())
      return;

    _ui->graphComboBox->selectIndex(graphIndex);
    dropEv->accept();
  }
}

void PythonIDE::executeCurrentScript() {

  Graph *graph = getSelectedGraph();

  if (!graph) {
    QMessageBox::information(
        this, "Script execution not allowed",
        "A graph to process must first be selected before running the script.");
    return;
  }

  if (_pythonInterpreter->isScriptPaused()) {
    Observable::holdObservers();
    _pythonInterpreter->pauseCurrentScript(false);
    _ui->runScriptButton->setEnabled(false);
    _ui->runScriptButton->setToolTip("Run script (Ctrl + Return)");
    _ui->pauseScriptButton->setEnabled(true);
    _ui->progressBar->setRange(0, 0);
    return;
  }

  _ui->consoleWidget->clear();
  _pythonInterpreter->clearOutputBuffers();
  clearErrorIndicators();

  QString scriptFileName = getCurrentMainScriptEditor()->getFileName();

  if (scriptFileName.isEmpty()) {
    scriptFileName = "<unnamed script>";
  }

  saveAllScripts();

  saveAllModules();

  _pythonInterpreter->setConsoleWidget(_ui->consoleWidget);

  if (!reloadAllModules() || !_pythonInterpreter->runString(
                                 getCurrentMainScriptEditor()->getCleanCode(), scriptFileName)) {
    indicateErrors();
    return;
  }

  if (_ui->useUndoCB->isChecked()) {
    graph->push();
  }

  Observable::holdObservers();

  _pythonInterpreter->setProcessQtEventsDuringScriptExecution(true);

  _ui->progressBar->setRange(0, 0);

  _ui->runScriptButton->setEnabled(false);
  _ui->stopScriptButton->setEnabled(true);
  _ui->pauseScriptButton->setEnabled(true);
  _ui->progressBar->show();
  _ui->useUndoCB->setEnabled(false);

  QApplication::processEvents();

  bool scriptExecOk = _pythonInterpreter->runGraphScript("__main__", "main", graph, scriptFileName);

  _pythonInterpreter->setProcessQtEventsDuringScriptExecution(false);
  _ui->stopScriptButton->setEnabled(false);
  _ui->runScriptButton->setEnabled(true);
  _ui->pauseScriptButton->setEnabled(false);

  if (scriptExecOk) {
    _pythonInterpreter->runString("del main");

    // no need to keep new graph state in memory as the script
    // only performed read only operations on the graph
    if (_ui->useUndoCB->isChecked()) {
      graph->popIfNoUpdates();
    }

  } else {

    if (!_scriptStopped) {
      indicateErrors();
    }

    if (_ui->useUndoCB->isChecked()) {
      graph->pop(false);
    }
  }

  _ui->progressBar->setRange(0, 100);
  _ui->progressBar->reset();

  _pythonInterpreter->resetConsoleWidget();

  if (Observable::observersHoldCounter() > 0)
    Observable::unholdObservers();

  // some external modules (like numpy) overrides the SIGINT handler at import
  // reinstall the default one, otherwise Tulip can not be interrupted by hitting Ctrl-C in a
  // console
  _pythonInterpreter->setDefaultSIGINTHandler();

  _ui->useUndoCB->setEnabled(true);
  _ui->progressBar->hide();
  _scriptStopped = false;
}

void PythonIDE::stopCurrentScript() {
  _scriptStopped = true;
  _pythonInterpreter->stopCurrentScript();
  _ui->useUndoCB->setEnabled(true);
  _ui->progressBar->hide();
}

bool PythonIDE::closeEditorTabRequested(PythonEditorsTabWidget *tabWidget, int idx) {
  QString curTabText = tabWidget->tabText(idx);

  // workaround a Qt5 bug on linux
  curTabText = curTabText.replace("&", "");

  if (curTabText.isEmpty())
    return true;

  PythonCodeEditor *editor = tabWidget->getEditor(idx);
  QString fileName = editor->getFileName();

  if (curTabText[curTabText.size() - 1] == '*' || fileName.isEmpty() ||
      !QFileInfo(fileName).exists()) {

    QMessageBox::StandardButton button = QMessageBox::question(
        QApplication::activeWindow(), QString("Save edited Python code"),
        QString("The code of ") +
            // if the editor has not yet a file name
            // show the tab text instead
            (fileName.isEmpty() ? curTabText : fileName) +
            QString("\n has been edited but has not been saved to disk.\n"
                    "Do you want to save it to disk ?"),
        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel, QMessageBox::Save);

    if (button == QMessageBox::Save) {
      if (fileName.isEmpty()) {
        // unsaved main script code, user must choose a file
        saveScript(idx, false, true);
      } else {
        // module or plugin without an associated file
        QString saveFileName =
            QFileDialog::getSaveFileName(this, tr("Save Python file"), "", "Python file (*.py)");

        if (!saveFileName.isEmpty()) {
          editor->setFileName(saveFileName);
          editor->saveCodeToFile();
        }
      }
    }

    return button != QMessageBox::Cancel;
  }

  return true;
}

void PythonIDE::graphComboBoxIndexChanged() {
  tlp::Graph *graph = getSelectedGraph();

  for (int i = 0; i < _ui->mainScriptsTabWidget->count(); ++i) {
    getMainScriptEditor(i)->getAutoCompletionDb()->setGraph(graph);
  }

  for (int i = 0; i < _ui->modulesTabWidget->count(); ++i) {
    getModuleEditor(i)->getAutoCompletionDb()->setGraph(graph);
  }

  for (int i = 0; i < _ui->pluginsTabWidget->count(); ++i) {
    getPluginEditor(i)->getAutoCompletionDb()->setGraph(graph);
  }
}

bool PythonIDE::loadModuleFromSrcCode(const QString &moduleName, const QString &moduleSrcCode) {
  int editorId = addModuleEditor();
  PythonCodeEditor *codeEditor = getModuleEditor(editorId);
  codeEditor->setFileName(moduleName);
  codeEditor->setPlainText(moduleSrcCode);

  saveModule(editorId);

  bool ret = _pythonInterpreter->registerNewModuleFromString(QString(moduleName).replace(".py", ""),
                                                             moduleSrcCode);

  if (ret) {
    codeEditor->analyseScriptCode(true);
  }

  return ret;
}

bool PythonIDE::eventFilter(QObject *obj, QEvent *event) {
#ifdef __APPLE__
  Qt::KeyboardModifiers modifier = Qt::MetaModifier;
#else
  Qt::KeyboardModifiers modifier = Qt::ControlModifier;
#endif

  if (event->type() == QEvent::KeyPress) {
    QKeyEvent *keyEvt = static_cast<QKeyEvent *>(event);

    if (obj == getCurrentMainScriptEditor() && keyEvt->modifiers() == modifier &&
        keyEvt->key() == Qt::Key_Return) {
      executeCurrentScript();
      return true;
    }
  }

  return false;
}

void PythonIDE::closeModuleTabRequested(int idx) {
  QString moduleFile = getModuleEditor(idx)->getFileName();
  QFileInfo fileInfo(moduleFile);
  QString projectFile = PYTHON_MODULES_PATH + "/" + fileInfo.fileName();

  if (_project && _project->exists(projectFile) && !_saveFilesToProject) {
    return;
  }

  if (closeEditorTabRequested(_ui->modulesTabWidget, idx)) {
    if (_project) {
      writeModulesFilesList(idx);

      if (_project->exists(projectFile)) {
        _project->removeFile(projectFile);
      }
    }

    _ui->modulesTabWidget->closeTab(idx);
  }
}

void PythonIDE::closeScriptTabRequested(int idx) {
  QString scriptFile = getMainScriptEditor(idx)->getFileName();
  QFileInfo fileInfo(scriptFile);
  QString projectFile = PYTHON_SCRIPTS_PATH + "/" + fileInfo.fileName();

  if (_project && _project->exists(projectFile) && !_saveFilesToProject) {
    return;
  }

  if (closeEditorTabRequested(_ui->mainScriptsTabWidget, idx)) {
    if (_project) {

      writeScriptsFilesList(idx);

      if (_project->exists(projectFile)) {
        _project->removeFile(projectFile);
      }
    }
    _ui->mainScriptsTabWidget->closeTab(idx);

    if (_ui->mainScriptsTabWidget->count() == 1) {
      _ui->runScriptButton->setEnabled(false);
    }
  }
}

void PythonIDE::closePluginTabRequested(int idx) {
  QString pluginFile = getPluginEditor(idx)->getFileName();
  QFileInfo fileInfo(pluginFile);
  QString projectFile = PYTHON_PLUGINS_PATH + "/" + fileInfo.fileName();

  if (_project && _project->exists(projectFile) && !_saveFilesToProject) {
    return;
  }

  if (closeEditorTabRequested(_ui->pluginsTabWidget, idx)) {
    _editedPluginsClassName.remove(pluginFile);
    _editedPluginsType.remove(pluginFile);
    _editedPluginsName.remove(pluginFile);

    if (_project) {
      writePluginsFilesList(idx);

      if (_project->exists(projectFile)) {
        _project->removeFile(projectFile);
      }
    }

    _ui->pluginsTabWidget->closeTab(idx);

    if (_ui->pluginsTabWidget->count() == 1) {
      _ui->registerPluginButton->setEnabled(false);
      _ui->removePluginButton->setEnabled(false);
    }
  }
}

void PythonIDE::loadScriptsAndModulesFromPythonScriptViewDataSet(const tlp::DataSet &dataSet) {
  DataSet modulesDataSet;

  if (dataSet.get("modules", modulesDataSet)) {
    int i = 0;
    string module;
    ostringstream oss;
    oss << "module" << i;

    while (modulesDataSet.get(oss.str(), module)) {
      bool moduleLoaded = loadModule(tlpStringToQString(module));

      if (!moduleLoaded) {
        string moduleSrc;
        oss.str("");
        oss << "module_src" << i;

        if (modulesDataSet.get(oss.str(), moduleSrc)) {
          QFileInfo fileInfo(tlpStringToQString(module));
          loadModuleFromSrcCode(fileInfo.fileName(), tlpStringToQString(moduleSrc));
        }
      }

      oss.str("");
      oss << "module" << ++i;
    }
  }

  DataSet mainScriptsDataSet;

  if (dataSet.get("main_scripts", mainScriptsDataSet)) {
    int i = 0;
    string mainScript;
    string mainscriptname("main_script");
    mainscriptname.append(to_string(i));

    while (mainScriptsDataSet.get(mainscriptname, mainScript)) {
      QString qmainScript = tlpStringToQString(mainScript);
      bool mainScriptLoaded = loadScript(qmainScript, false);

      if (!mainScriptLoaded) {
        string mainScriptSrc;
        string mainScriptSrcname("main_script_src");
        mainScriptSrcname.append(to_string(i));
        QFileInfo fileInfo(qmainScript);

        if (mainScriptsDataSet.get(mainScriptSrcname, mainScriptSrc)) {
          int mainScriptId = addMainScriptEditor();
          PythonCodeEditor *codeEditor = getMainScriptEditor(mainScriptId);
          codeEditor->setPlainText(tlpStringToQString(mainScriptSrc));

          if (!mainScript.empty())
            _ui->mainScriptsTabWidget->setTabText(mainScriptId, fileInfo.fileName());
          else
            _ui->mainScriptsTabWidget->setTabText(mainScriptId, "[no file]");

          saveScript(mainScriptId, false);
        }
      }

      mainscriptname = "main_script" + to_string(++i);
    }
  }
}

void PythonIDE::savePythonFilesAndWriteToProject(bool notifyProjectModified) {
  _notifyProjectModified = notifyProjectModified;
  saveAllScripts();
  saveAllPlugins();
  saveAllModules();
  _notifyProjectModified = true;
}

void PythonIDE::clearPythonCodeEditors() {
  // we don't want to save files to project as a side effect here
  _saveFilesToProject = false;

  for (int i = _ui->mainScriptsTabWidget->count() - 1; i >= 0; --i) {
    closeScriptTabRequested(i);
    _ui->mainScriptsTabWidget->removeTab(i);
  }

  for (int i = _ui->pluginsTabWidget->count() - 1; i >= 0; --i) {
    closePluginTabRequested(i);
    _ui->pluginsTabWidget->removeTab(i);
  }

  for (int i = _ui->modulesTabWidget->count() - 1; i >= 0; --i) {
    closeModuleTabRequested(i);
    _ui->modulesTabWidget->removeTab(i);
  }

  _ui->consoleWidget->clear();
  _saveFilesToProject = true;
}

void PythonIDE::setScriptEditorsVisible(bool visible) {
  if (!visible && _ui->tabWidget->indexOf(_scriptEditorsWidget) != -1) {
    _ui->tabWidget->removeTab(0);
    _ui->stackedWidget->removeWidget(_scriptControlWidget);
  } else if (visible && _ui->tabWidget->indexOf(_scriptEditorsWidget) == -1) {
    _ui->tabWidget->insertTab(0, _scriptEditorsWidget, "Scripts editor");
    _ui->stackedWidget->insertWidget(0, _scriptControlWidget);
  }
}

void PythonIDE::setPluginEditorsVisible(bool visible) {
  if (!visible && _ui->tabWidget->indexOf(_pluginEditorsWidget) != -1) {
    if (_ui->tabWidget->indexOf(_scriptEditorsWidget) != -1) {
      _ui->tabWidget->removeTab(1);
    } else {
      _ui->tabWidget->removeTab(0);
    }

    _ui->stackedWidget->removeWidget(_pluginControlWidget);
  } else if (visible && _ui->tabWidget->indexOf(_pluginEditorsWidget) == -1) {
    if (_ui->tabWidget->indexOf(_scriptEditorsWidget) != -1) {
      _ui->tabWidget->insertTab(1, _pluginEditorsWidget, "Plugins editor");
      _ui->stackedWidget->insertWidget(1, _pluginControlWidget);
    } else {
      _ui->tabWidget->insertTab(0, _pluginEditorsWidget, "Plugins editor");
      _ui->stackedWidget->insertWidget(0, _pluginControlWidget);
    }
  }
}

void PythonIDE::setModuleEditorsVisible(bool visible) {
  if (!visible && _ui->tabWidget->indexOf(_moduleEditorsWidget) != -1) {
    _ui->tabWidget->removeTab(_ui->tabWidget->count() - 1);
    _ui->stackedWidget->removeWidget(_moduleControlWidget);
  } else if (visible && _ui->tabWidget->indexOf(_moduleEditorsWidget) == -1) {
    _ui->tabWidget->insertTab(_ui->tabWidget->count(), _moduleEditorsWidget, "Modules editor");
    _ui->stackedWidget->insertWidget(_ui->stackedWidget->count(), _moduleControlWidget);
  }
}

Graph *PythonIDE::getSelectedGraph() const {
  return _graphsModel->data(_ui->graphComboBox->selectedIndex(), TulipModel::GraphRole)
      .value<Graph *>();
}
