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

#include <tulip/PythonInterpreter.h>

#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QFileDialog>
#include <QLineEdit>
#include <QRegularExpression>
#include <QShortcut>
#include <QTimer>

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
#include <tulip/TulipProject.h>
#include <tulip/TlpQtTools.h>
#include <tulip/GraphHierarchiesModel.h>
#include <tulip/TulipMimes.h>
#include <tulip/TulipMetaTypes.h>
#include <tulip/TulipSettings.h>
#include <tulip/Perspective.h>

#include <tulip/PythonIDE.h>
#include <tulip/PythonPanel.h>
#include <tulip/PythonPluginCreationDialog.h>
#include <tulip/PythonEditorsTabWidget.h>

#include "ui_PythonIDE.h"
#include "ui_PythonIDEWarningDialog.h"

using namespace std;
using namespace tlp;

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
  QRegularExpression re("\\d*"); // a digit (\d), zero or more times (*)
  if (re.match(ret).hasMatch()) {
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

  QString builtinModName = "builtins";

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
            << cleanPropName << "']" << Qt::endl;
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

// the possible pip commands to deal with python packages
struct _pipCommand {
  QString name;
  std::string argList;
  bool needPackage, needInternetAccess;
  QString progressMsg;
};

static std::vector<_pipCommand> pipCommands{
    // install a package in the home directory
    {"install", "install', '--user", true, true, "installing"},
    // list home directory installed packages
    {"list", "list', '--user", false, false, ""},
    // list all installed packages
    {"list all", "list", false, false, ""},
    // show package info
    {"show", "show", true, false, ""},
    // uninstall a package
    {"uninstall", "uninstall', '--yes", false, false, "uninstalling"},
    // upgrade a package
    {"upgrade", "install', '--upgrade", true, true, "upgrading"}};

// the name of the python exe
static std::string pyExe;

// beginning of a pip invokation through python
static std::string beginPipScript() {
  // construct the script to execute
  std::string pipScript("import subprocess;import sys");
  // set the execution environment
#ifdef APPIMAGE_BUILD
  // appimage needs a specific LD_LIBRARY_PATH configuration
  std::string pyEnv(R"(
import os
ld_library_path=sys.exec_prefix + '/lib'
exec_env=dict(os.environ, LD_LIBRARY_PATH=ld_library_path)
)");
#else
  std::string pyEnv("\nexec_env=None");
#endif
  pipScript += pyEnv;
  // set the subprocess command to run
  pipScript += R"(
result = subprocess.run([)";
  // with the right python exe and the pip command beginning
  return pipScript + pyExe + ", '-m', 'pip', '";
}

// display a warning message on top of PythonIDE
//  about python files no longer saved in project files
static void showWarningDialog(PythonIDE *ide) {
  QWidget *parent = ide->parentWidget();
  // wait until parent is visible
  if (parent && parent->isVisible()) {
    QDialog wd(parent);
    auto ui = Ui::PythonIDEWarningDialog();
    ui.setupUi(&wd);
    tlpFixCBRBs(&wd);
    wd.exec();
    if (!ui.displayDialogCB->isChecked()) {
      TulipSettings::instance().beginGroup("pythonIDE");
      TulipSettings::instance().setValue("displayPythonIDEWarningDialog", false);
      TulipSettings::instance().endGroup();
    }
  } else
    QTimer::singleShot(100, [ide]() { showWarningDialog(ide); });
}

PythonIDEInterface *PythonIDE::Builder::newIDE(GraphHierarchiesModel *model) {
  auto ide = new PythonIDE();
  TulipSettings::instance().beginGroup("pythonIDE");
  if (TulipSettings::instance().value("displayPythonIDEWarningDialog", true).toBool())
    // display a warning message on top of Python IDE
    QTimer::singleShot(100, [ide]() { showWarningDialog(ide); });
  TulipSettings::instance().endGroup();
  ide->setGraphsModel(model);
  return ide;
}

void PythonIDE::Builder::loadPlugins() {
  // side effect to ensure python plugins loading
  PythonInterpreter::getInstance();
}

// instantiate a PythonIDE::Builder to ensure GraphPerspective
// can create a PythonIDE
static PythonIDE::Builder builder;

PythonIDE::PythonIDE()
    : _ui(new Ui::PythonIDE), _pythonInterpreter(PythonInterpreter::getInstance()),
      _pythonPanel(new PythonPanel()), _dontTreatFocusIn(false), _graphsModel(nullptr),
      _scriptStopped(false) {
  _ui->setupUi(this);
  // fix display of QCheckBox and QRadioButton children
  tlpFixCBRBs(this);

  _ui->mainScriptsTabWidget->clear();
  _ui->modulesTabWidget->clear();
  _ui->pluginsTabWidget->clear();
  QVBoxLayout *layout = new QVBoxLayout();
  layout->addWidget(_pythonPanel);
  layout->setContentsMargins(0, 0, 0, 0);
  _ui->interpreterTab->setLayout(layout);

  // add pip command gui
  _pipFrame = new QFrame();
  auto hLayout = new QHBoxLayout(_pipFrame);
  hLayout->setContentsMargins(0, 0, 3, 0);
  hLayout->setSpacing(3);
  // add a vertical line to separate from tabs
  auto vLine = new QFrame();
  vLine->setFrameShape(QFrame::VLine);
  vLine->setFrameShadow(QFrame::Sunken);
  hLayout->addWidget(vLine);
  // indicate it involves the pip command
  auto label = new QLabel("<b>pip</b>");
  label->setToolTip(
      "pip is the package installer for Python; here's a simple graphical interface for using it\nto manage the packages available for the current Python environment.");
  hLayout->addWidget(label);
  // add QComboBox to choose pip sub-command
  _pipCombo = new QComboBox();
#ifdef __APPLE__
  _pipCombo->setMinimumContentsLength(9);
#endif
  _pipCombo->setToolTip(
      "Choose the pip command to execute:\n- install (a home directory package),\n- list (home directory packages only),\n- list all (installed packages),\n- show (package information)\n- uninstall (a package)\n- upgrade (a package).");
  hLayout->addWidget(_pipCombo);
  _pipCombo->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
  // add QLineEdit to give package name
  _pipPackage = new QLineEdit();
  _pipPackage->setPlaceholderText("package name");
  connect(_pipCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(pipCommandChanged(int)));
  for (auto pipCommand : pipCommands) {
    _pipCombo->addItem(pipCommand.name);
  }
  connect(_pipPackage, &QLineEdit::returnPressed, [this] {
    this->executePipCommand(this->_pipCombo->currentIndex(), this->_pipPackage->text());
  });
  hLayout->addWidget(_pipPackage);
  // add pip command gui
  _ui->consoleTab->setCornerWidget(_pipFrame, Qt::TopRightCorner);
  // show pip command gui only when 'Python output' is visible
  connect(_ui->consoleTab, &QTabWidget::currentChanged,
          [this](int index) { this->_pipFrame->setVisible(index == 0); });

  QList<int> sizes;
  sizes.push_back(550);
  sizes.push_back(150);
  _ui->splitter->setSizes(sizes);

  // customize tabWidget right corner
  auto tabWidgetCorner = new QFrame();
  hLayout = new QHBoxLayout(tabWidgetCorner);
  hLayout->setContentsMargins(0, 0, 3, 0);
  hLayout->setSpacing(3);
  label = new QLabel();
  QPixmap pythonLogo(":/tulip/gui/icons/python.png");
  label->setPixmap(pythonLogo.scaled(20, 20, Qt::KeepAspectRatio, Qt::SmoothTransformation));
  hLayout->addWidget(label);
  label =
      new QLabel(QString("Python ") + PythonInterpreter::getInstance()->getPythonFullVersionStr());
  hLayout->addWidget(label);
  _ui->tabWidget->setCornerWidget(tabWidgetCorner);

  _scriptEditorsWidget = _ui->tabWidget->widget(0);
  _scriptControlWidget = _ui->stackedWidget->widget(0);

  _pluginEditorsWidget = _ui->tabWidget->widget(1);
  _pluginControlWidget = _ui->stackedWidget->widget(1);

  _moduleEditorsWidget = _ui->tabWidget->widget(2);
  _moduleControlWidget = _ui->stackedWidget->widget(2);

  connect(_pythonInterpreter, SIGNAL(scriptExecutionPaused()), this, SLOT(currentScriptPaused()));
  _pythonInterpreter->runString(utilityFunctions);
  // init pyExe absolute path
  if (pyExe.empty()) {
    auto pyPath = _pythonInterpreter->getSysVariable("exec_prefix");
    if (QFile::exists(pyPath + "/bin"))
      pyPath.append("/bin");
#ifdef _WIN32
    // on windows we use pythonw exe to avoid the display of a command shell
    pyExe += "'" + QStringToTlpString(pyPath) + "/pythonw'";
#else
    pyExe += "'" + QStringToTlpString(pyPath) + "/python3'";
#endif
  }

#ifdef _WIN32
  // on windows we must ensure that pip is installed at this time
  // because Python may have been installed when installing tulip
  // and then we have no guarantee if pip installation was checked or not
  // in the Python gui installer.
  // So first get pip version to check if it installed
  std::string pipScript = beginPipScript();
  pipScript += R"(--version'], capture_output=True, text=True, env=exec_env)
if result.returncode != 0:
   subprocess.run([)";
  // if it is not, try to install it
  pipScript +=
      pyExe +
      ", '-m', 'ensurepip', '--default-pip'], capture_output=True, text=True, env=exec_env)";
  _pythonInterpreter->runString(pipScript.c_str());
#endif

  connect(_ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(currentTabChanged(int)));

  connect(_ui->newModuleButton, SIGNAL(clicked()), this, SLOT(newModule()));
  connect(_ui->loadModuleButton, SIGNAL(clicked()), this, SLOT(loadModule()));
  connect(_ui->saveModuleButton, SIGNAL(clicked()), this, SLOT(saveModule()));
  connect(_ui->saveAsModuleButton, SIGNAL(clicked()), this, SLOT(saveAsModule()));
  connect(_ui->newPluginButton, SIGNAL(clicked()), this, SLOT(newPythonPlugin()));
  connect(_ui->loadPluginButton, SIGNAL(clicked()), this, SLOT(loadPythonPlugin()));
  connect(_ui->savePluginButton, SIGNAL(clicked()), this, SLOT(savePythonPlugin()));
  connect(_ui->saveAsPluginButton, SIGNAL(clicked()), this, SLOT(saveAsPythonPlugin()));
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
  auto shortCut = new QShortcut(QKeySequence::ZoomOut, _ui->tabWidget);
  connect(shortCut, SIGNAL(activated()), this, SLOT(decreaseFontSize()));
  SET_TOOLTIP_WITH_CTRL_SHORTCUT(_ui->decreaseFontSizeButton, "decrease font size", "-");
  SET_TOOLTIP_WITH_CTRL_SHORTCUT(_ui->decreaseFontSizeButton_2, "decrease font size", "-");
  SET_TOOLTIP_WITH_CTRL_SHORTCUT(_ui->decreaseFontSizeButton_3, "decrease font size", "-");
  shortCut = new QShortcut(QKeySequence::ZoomIn, _ui->tabWidget);
  connect(shortCut, SIGNAL(activated()), this, SLOT(increaseFontSize()));
  SET_TOOLTIP_WITH_CTRL_SHORTCUT(_ui->increaseFontSizeButton, "increase font size", "-");
  SET_TOOLTIP_WITH_CTRL_SHORTCUT(_ui->increaseFontSizeButton_2, "increase font size", "-");
  SET_TOOLTIP_WITH_CTRL_SHORTCUT(_ui->increaseFontSizeButton_3, "increase font size", "-");

  connect(_ui->mainScriptsTabWidget, SIGNAL(fileSaved(int)), this, SLOT(scriptSaved(int)));
  connect(_ui->mainScriptsTabWidget, SIGNAL(fileEdited()), this, SLOT(fileEdited()));
  connect(_ui->modulesTabWidget, SIGNAL(fileSaved(int)), this, SLOT(moduleSaved(int)));
  connect(_ui->modulesTabWidget, SIGNAL(fileEdited()), this, SLOT(fileEdited()));
  connect(_ui->pluginsTabWidget, SIGNAL(fileSaved(int)), this, SLOT(pluginSaved(int)));
  connect(_ui->pluginsTabWidget, SIGNAL(fileEdited()), this, SLOT(fileEdited()));

  connect(_ui->runScriptButton, SIGNAL(clicked()), this, SLOT(executeCurrentScript()));
  connect(_ui->pauseScriptButton, SIGNAL(clicked()), this, SLOT(pauseCurrentScript()));
  connect(_ui->stopScriptButton, SIGNAL(clicked()), this, SLOT(stopCurrentScript()));
  _ui->progressBar->hide();

  connect(_ui->newMainScriptButton, SIGNAL(clicked()), this, SLOT(newScript()));
  connect(_ui->loadMainScriptButton, SIGNAL(clicked()), this, SLOT(loadScript()));
  connect(_ui->saveMainScriptButton, SIGNAL(clicked()), this, SLOT(saveScript()));
  connect(_ui->saveAsMainScriptButton, SIGNAL(clicked()), this, SLOT(saveAsScript()));
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

bool PythonIDE::saveModule(int tabIdx, bool saveAs) {
  if (tabIdx >= 0 && tabIdx < _ui->modulesTabWidget->count()) {
    auto moduleFileName = getModuleEditor(tabIdx)->getFileName();
    QFileInfo fileInfo(moduleFileName);
    QString moduleNameExt = _ui->modulesTabWidget->tabText(tabIdx);
    QString moduleName;
    auto pos = moduleNameExt.indexOf(".py");
    if (pos != -1)
      moduleName = moduleNameExt.mid(0, pos);

    if (saveAs || moduleFileName.isEmpty()) {
      QString dir = "";

      if (!moduleFileName.isEmpty()) {
        dir = fileInfo.dir().absolutePath();
      }
      auto fileName = QFileDialog::getSaveFileName(this, "Save module", dir, "Module (*.py)");
      if (fileName.isEmpty())
        return false;

      fileName = fileName.replace("&", "");
      getModuleEditor(tabIdx)->setFileName(fileName);
      fileInfo = QFileInfo(fileName);
      _pythonInterpreter->addModuleSearchPath(fileInfo.absolutePath());
    }

    // ensure module reimportation in _pythonInterpreter
    if (!moduleName.isEmpty())
      _pythonInterpreter->deleteModule(moduleName);

    // get module name in regard to the name of the attached file
    moduleNameExt = fileInfo.baseName();
    pos = moduleNameExt.indexOf(".py");
    moduleName = moduleNameExt.mid(0, pos);

    _ui->modulesTabWidget->setTabText(tabIdx, moduleName + ".py");

    if (getModuleEditor(tabIdx)->saveCodeToFile())
      _ui->modulesTabWidget->setTabToolTip(tabIdx, fileInfo.absoluteFilePath());

    getModuleEditor(tabIdx)->getCleanCode();
    checkUnsavedFiles(_ui->modulesTabWidget, true);

    return true;
  }
  return false;
}

void PythonIDE::newModule() {
  int editorId = addModuleEditor("");
  _ui->modulesTabWidget->setTabText(editorId, "[no file]");
  _ui->modulesTabWidget->setTabToolTip(
      editorId,
      "new module, do not forget to save the current module to a file to not lose your source code modifications.");
}

void PythonIDE::saveModule() {
  int curModule = _ui->modulesTabWidget->currentIndex();

  if (curModule == -1)
    return;

  saveModule(curModule);
}

void PythonIDE::saveAsModule() {
  int curModule = _ui->modulesTabWidget->currentIndex();

  if (curModule == -1)
    return;

  saveModule(curModule, true);
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
    QString pluginName = pluginCreationDialog.getPluginName();

    int editorId = addPluginEditor("");
    QString editorSID = QString("%1").arg(reinterpret_cast<intptr_t>(getPluginEditor(editorId)));
    _ui->pluginsTabWidget->setTabToolTip(editorId, pluginCreationDialog.getPluginName());
    _ui->pluginsTabWidget->setTabText(editorId,
                                      pluginCreationDialog.getPluginType() + " - " + pluginName);

    _editedPluginsClassName[editorSID] = pluginCreationDialog.getPluginClassName();
    _editedPluginsType[editorSID] = pluginCreationDialog.getPluginType();
    _editedPluginsName[editorSID] = pluginName;
    QString pluginSkeleton = getTulipPythonPluginSkeleton(
        pluginCreationDialog.getPluginClassName(), pluginCreationDialog.getPluginType(),
        pluginCreationDialog.getPluginName(), pluginCreationDialog.getPluginAuthor(),
        pluginCreationDialog.getPluginDate(), pluginCreationDialog.getPluginInfo(),
        pluginCreationDialog.getPluginRelease(), pluginCreationDialog.getPluginGroup());

    getPluginEditor(editorId)->setPlainText(pluginSkeleton);
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
    QRegularExpression rx("class ([a-zA-Z_][a-zA-Z0-9_]*)\\(([^,\\(\\)]+)\\)");
    QRegularExpressionMatch match;

    int pos = pluginCode.indexOf(rx, 0, &match);

    while (pos != -1) {
      pluginClassName = match.captured(1);
      pluginClass = match.captured(2);

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

      pos = pluginCode.indexOf(rx, pos + match.capturedLength(), &match);
    }

    rx.setPattern(".*registerPlugin.*\\(\\s*['\"]([^\"']+)[\"']\\s*,\\s*['\"]([^\"']+)[\"'].*$");
    match = rx.match(pluginCode);
    if (match.hasMatch()) {
      pluginName = match.captured(2);
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
  QTextStream in(&file);
  pluginCode = in.readAll();
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
    QString name = "\nName of the plugin: " + (pluginName.isEmpty() ? "missing" : pluginName);
    QString classname =
        "\nPlugin class name: " + (pluginClassName.isEmpty() ? "missing" : pluginClassName);
    QString type = "\nPlugin type: " + (pluginType.isEmpty() ? "missing" : pluginType);
    QMessageBox::critical(this, "Error",
                          "The file " + fileName +
                              " does not seem to be a valid Tulip Python plugin: \n " + name +
                              classname + type);
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

void PythonIDE::savePythonPlugin() {
  savePythonPlugin(_ui->pluginsTabWidget->currentIndex());
}

void PythonIDE::saveAsPythonPlugin() {
  savePythonPlugin(_ui->pluginsTabWidget->currentIndex(), true);
}

bool PythonIDE::savePythonPlugin(int tabIdx, bool saveAs) {
  if (tabIdx >= 0 && tabIdx < _ui->pluginsTabWidget->count()) {
    auto pluginFileName = getPluginEditor(tabIdx)->getFileName();
    if (saveAs || pluginFileName.isEmpty()) {
      QString dir = "";

      if (!pluginFileName.isEmpty()) {
        QFileInfo fileInfo(pluginFileName);
        dir = fileInfo.dir().absolutePath();
      }
      auto fileName =
          QFileDialog::getSaveFileName(this, "Save python plugin", dir, "Python plugin (*.py)");
      if (fileName.isEmpty())
        return false;
      getPluginEditor(tabIdx)->setFileName(fileName);
      PythonInterpreter::getInstance()->addModuleSearchPath(fileName);

      if (pluginFileName.isEmpty()) {
        pluginFileName = fileName;
        QString id = QString("%1").arg(reinterpret_cast<intptr_t>(getPluginEditor(tabIdx)));

        _editedPluginsClassName[fileName] = _editedPluginsClassName[id];
        _editedPluginsClassName.remove(id);
        _editedPluginsType[fileName] = _editedPluginsType[id];
        _editedPluginsType.remove(id);
        _editedPluginsName[fileName] = _editedPluginsName[id];
        _editedPluginsName.remove(id);
      }
    }

    getPluginEditor(tabIdx)->saveCodeToFile();

    _ui->pluginsTabWidget->setTabText(tabIdx, _editedPluginsType[pluginFileName] + " - " +
                                                  _editedPluginsName[pluginFileName]);
    _ui->pluginsTabWidget->setTabToolTip(tabIdx, getPluginEditor(tabIdx)->getFileName());

    getPluginEditor(tabIdx)->getCleanCode();
    checkUnsavedFiles(_ui->pluginsTabWidget, true);

    return true;
  }
  return false;
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

  oss << "import " << moduleName << Qt::endl;
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
  QRegularExpression rx("^.*File.*\"(.*)\".*line.*(\\d+).*in (.*)$");
  QRegularExpressionMatch match;

  QMap<QString, QVector<int>> errorLines;
  QString consoleOutput = _pythonInterpreter->getStandardErrorOutput();
  QStringList outputLines = consoleOutput.split("\n");

  for (int i = 0; i < outputLines.count() - 1; ++i) {
    int pos = 0;

    while ((pos = outputLines[i].indexOf(rx, pos, &match)) != -1) {
      QString file = match.captured(1);
#ifdef WIN32
      file.replace("\\", "/");
#endif
      int line = match.captured(2).toInt();
      errorLines[file].push_back(line);

      pos += match.capturedLength();
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

QString PythonIDE::readProjectFile(tlp::TulipProject *project, const QString &filePath) {
  QString content;

  if (project) {
    QIODevice *fs = project->fileStream(filePath, QIODevice::ReadOnly | QIODevice::Text);

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

bool PythonIDE::projectNeedsPythonIDE(tlp::TulipProject *project) {
  return (project->exists(PYTHON_MODULES_FILES) || project->exists(PYTHON_PLUGINS_FILES) ||
          project->exists(PYTHON_SCRIPTS_FILES));
}

bool PythonIDE::checkUnsavedFiles(PythonEditorsTabWidget *tabWidget, bool updateTabText) {
  for (int i = 0; i < tabWidget->count(); ++i) {
    QString tabText = tabWidget->tabText(i);
    if (tabText[tabText.size() - 1] == '*')
      return true;
  }
  if (updateTabText) {
    // remove '*' if needed
    auto current = _ui->tabWidget->currentIndex();
    auto tabText = _ui->tabWidget->tabText(current);
    if (tabText[tabText.size() - 1] == '*') {
      tabText = tabText.mid(0, tabText.size() - 1);
      _ui->tabWidget->setTabText(current, tabText);
    }
  }
  return false;
}

bool PythonIDE::hasUnsavedFiles() {
  int i = 0;
  auto tabWidgets = {_ui->mainScriptsTabWidget, _ui->pluginsTabWidget, _ui->modulesTabWidget};
  for (auto tabWidget : tabWidgets) {
    if (checkUnsavedFiles(tabWidget)) {
      // set it as current to ensure unsaved files
      // will be first shown
      _ui->tabWidget->setCurrentIndex(i);
      return true;
    }
    ++i;
  }
  return false;
}

void PythonIDE::readProject(tlp::TulipProject *project) {
  if (!project) {
    return;
  }

  if (project->exists(PYTHON_MODULES_FILES)) {
    QIODevice *fs =
        project->fileStream(PYTHON_MODULES_FILES, QIODevice::ReadOnly | QIODevice::Text);
    QString file = fs->readLine();

    while (!file.isEmpty()) {
      file = file.mid(0, file.size() - 1);

      if (!loadModule(file)) {
        QFileInfo fileInfo(file);
        QString projectFile = PYTHON_MODULES_PATH + "/" + fileInfo.fileName();

        if (project->exists(projectFile)) {
          QString code = readProjectFile(project, projectFile);
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

  if (project->exists(PYTHON_PLUGINS_FILES)) {
    QIODevice *fs =
        project->fileStream(PYTHON_PLUGINS_FILES, QIODevice::ReadOnly | QIODevice::Text);
    QString file = fs->readLine();

    while (!file.isEmpty()) {
      file = file.mid(0, file.size() - 1);

      if (!loadPythonPlugin(file)) {
        QFileInfo fileInfo(file);
        QString projectFile = PYTHON_PLUGINS_PATH + "/" + fileInfo.fileName();

        if (project->exists(projectFile)) {
          QString code = readProjectFile(project, projectFile);
          loadPythonPluginFromSrcCode(fileInfo.fileName(), code);
        }
      }

      file = fs->readLine();
    }

    fs->close();
    delete fs;
  }

  if (project->exists(PYTHON_SCRIPTS_FILES)) {
    QIODevice *fs =
        project->fileStream(PYTHON_SCRIPTS_FILES, QIODevice::ReadOnly | QIODevice::Text);
    QString file = fs->readLine();

    while (!file.isEmpty()) {
      file = file.mid(0, file.size() - 1);

      if (!loadScript(file)) {

        QFileInfo fileInfo(file);
        QString projectFile = PYTHON_SCRIPTS_PATH + "/" + fileInfo.fileName();

        if (project->exists(projectFile)) {
          QString code = readProjectFile(project, projectFile);
          QString fileName = fileInfo.fileName();
          int editorId = addMainScriptEditor();

          if (fileName.startsWith("[no file]")) {
            fileName = "[no file]";
            _ui->mainScriptsTabWidget->setTabToolTip(
                editorId,
                "\"main\" script, do not forget to save the current script to a file to not lose your source code modifications.");
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
}

void PythonIDE::scriptSaved(int /*idx*/) {}

void PythonIDE::pluginSaved(int /*idx*/) {}

void PythonIDE::moduleSaved(int /*idx*/) {}

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

bool PythonIDE::isCurrentScriptExecuting() {
  return getCurrentMainScriptEditor() && !_ui->runScriptButton->isEnabled();
}

void PythonIDE::newScript() {
  int editorId = addMainScriptEditor();
  QString defaultScriptCode =
      getDefaultScriptCode(_pythonInterpreter->getPythonVersionStr(), getSelectedGraph());
  getMainScriptEditor(editorId)->setPlainText(defaultScriptCode);
  _ui->mainScriptsTabWidget->setTabText(editorId, "[no file]");
  _ui->mainScriptsTabWidget->setTabToolTip(
      editorId,
      "\"main\" script, do not forget to save the current script to a file to not lose your source code modifications.");
}

void PythonIDE::loadScript() {
  QString fileName =
      QFileDialog::getOpenFileName(this, "Open \"main\" script", "", "Python script (*.py)");
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

void PythonIDE::saveAsScript() {
  saveScript(_ui->mainScriptsTabWidget->currentIndex(), true, true);
}

bool PythonIDE::saveScript(int tabIdx, bool clear, bool saveAs) {
  if (tabIdx >= 0 && tabIdx < _ui->mainScriptsTabWidget->count()) {
    QString fileName;
    QString mainScriptFileName = getMainScriptEditor(tabIdx)->getFileName();

    QString tabText = _ui->mainScriptsTabWidget->tabText(tabIdx);

    // workaround a Qt5 bug on linux
    tabText = tabText.replace("&", "");

    if (saveAs || mainScriptFileName.isEmpty()) {
      QString dir = "";

      if (!tabText.startsWith("[")) {
        dir = tabText;
        if (saveAs && dir.endsWith("*"))
          dir.remove(tabText.length() - 1, 1);
      }

      fileName =
          QFileDialog::getSaveFileName(this, tr("Save main script"), dir, "Python script (*.py)");
      if (fileName.isEmpty())
        return false;
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
    checkUnsavedFiles(_ui->mainScriptsTabWidget, true);
    return true;
  }
  return false;
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

void PythonIDE::executePipCommand(int command, const QString &packageName) {
  _ui->consoleWidget->clear();
  _pythonInterpreter->clearOutputBuffers();
  clearErrorIndicators();
  _pythonInterpreter->setConsoleWidget(_ui->consoleWidget);

  auto name = QStringToTlpString(packageName.trimmed());
  // nothing to do if package name is missing
  if (name.empty() && pipCommands[command].needPackage) {
    _pythonInterpreter->runString("print('Warning: you must specified a package name')");
    return;
  }

  // check internet access if needed
  if (pipCommands[command].needInternetAccess && !checkInternetAccess()) {
    _pythonInterpreter->runString(
        "print('Warning: there seems to be no internet access.\\nCheck your network configuration.')");
    return;
  }

  // construct the script to execute
  std::string pipScript = beginPipScript();
  // set the pip command to run
  pipScript += pipCommands[command].argList + "', '" + name + "'";
  // the end of the script
  pipScript += R"(], capture_output=True, text=True, env=exec_env)
print(result.stdout)
if result.returncode != 0:
    print(result.stderr))";
  if (pipCommands[command].name.indexOf("list") == 0)
    // display a message when the list is empty
    pipScript += R"(
elif result.stdout == '':
    print('no package'))";
  // if needed, show a progress message while executing the command
  QString progressMsg = pipCommands[command].progressMsg;
  QMessageBox *progress = progressMsg.isEmpty()
                              ? nullptr
                              : new QMessageBox(QMessageBox::Information, "Pip command execution",
                                                "", QMessageBox::Ok, this, Qt::Popup);
  if (progress) {
    for (auto button : progress->buttons())
      progress->removeButton(button);
    progress->setText(QString("%1 %2 ...").arg(progressMsg).arg(packageName));
    progress->show();
    QApplication::processEvents(QEventLoop::WaitForMoreEvents, 1000);
  }
  // execute script
  _pythonInterpreter->runString(pipScript.c_str());

  delete progress;
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
  _pipFrame->setEnabled(false);

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
  _pipFrame->setEnabled(true);
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

  if (curTabText[curTabText.size() - 1] == '*') {

    QMessageBox::StandardButton button = QMessageBox::question(
        QApplication::activeWindow(), QString("Save edited Python code"),
        QString("The code of ") + curTabText.mid(0, curTabText.size() - 1) +
            QString("\n has been edited but has not been saved to disk.\n"
                    "Do you want to save it?"),
        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel, QMessageBox::Save);

    if (button == QMessageBox::Save) {
      bool saved = false;
      if (tabWidget == _ui->mainScriptsTabWidget)
        saved = saveScript(idx, false);
      else if (tabWidget == _ui->pluginsTabWidget)
        saved = savePythonPlugin(idx);
      else if (tabWidget == _ui->modulesTabWidget)
        saved = saveModule(idx);
      return saved;
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

void PythonIDE::fileEdited() {
  // if needed mark current tab text
  // to indicate unsaved files exist
  auto current = _ui->tabWidget->currentIndex();
  auto curTabText = _ui->tabWidget->tabText(current);
  if (curTabText[curTabText.size() - 1] != '*') {
    curTabText += "*";
    _ui->tabWidget->setTabText(current, curTabText);
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
  if (closeEditorTabRequested(_ui->modulesTabWidget, idx)) {
    _ui->modulesTabWidget->closeTab(idx);
  }
}

void PythonIDE::closeScriptTabRequested(int idx) {
  if (closeEditorTabRequested(_ui->mainScriptsTabWidget, idx)) {
    _ui->mainScriptsTabWidget->closeTab(idx);

    if (_ui->mainScriptsTabWidget->count() == 0) {
      _ui->runScriptButton->setEnabled(false);
    }
  }
}

void PythonIDE::closePluginTabRequested(int idx) {
  QString pluginFile = getPluginEditor(idx)->getFileName();
  QFileInfo fileInfo(pluginFile);

  if (closeEditorTabRequested(_ui->pluginsTabWidget, idx)) {
    _editedPluginsClassName.remove(pluginFile);
    _editedPluginsType.remove(pluginFile);
    _editedPluginsName.remove(pluginFile);

    _ui->pluginsTabWidget->closeTab(idx);

    if (_ui->pluginsTabWidget->count() == 1) {
      _ui->registerPluginButton->setEnabled(false);
      _ui->removePluginButton->setEnabled(false);
    }
  }
}

void PythonIDE::pipCommandChanged(int index) {
  auto &command = pipCommands[index];
  _pipPackage->setToolTip(
      QString(command.needPackage
                  ? "Type the package name and hit [Enter]\nto execute the '%1' command."
                  : "Hit [Enter] to execute the '%1' command.")
          .arg(command.name));
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

void PythonIDE::clearPythonCodeEditors() {
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
