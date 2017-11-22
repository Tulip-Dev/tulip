/**
 *
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

#ifdef TULIP_BUILD_PYTHON_COMPONENTS
#include <tulip/PythonInterpreter.h>
#include <tulip/APIDataBase.h>
#include <tulip/PythonIDE.h>
#include "PythonPanel.h"
#endif

#include "GraphPerspective.h"

#include <QMessageBox>
#include <QDesktopServices>
#include <QFileDialog>
#include <QCloseEvent>
#include <QClipboard>
#include <QDropEvent>
#include <QUrl>
#include <QMimeData>
#include <QTimer>
#include <QVBoxLayout>
#include <QDialog>
#include <QByteArray>
#include <QStatusBar>
#include <QMainWindow>
#include <QApplication>
#include <QDesktopWidget>

#include <tulip/TlpTools.h>
#include <tulip/ImportModule.h>
#include <tulip/Graph.h>
#include <tulip/ExportModule.h>
#include <tulip/View.h>
#include <tulip/SimplePluginProgressWidget.h>
#include <tulip/GraphHierarchiesModel.h>
#include <tulip/CSVImportWizard.h>
#include <tulip/GraphModel.h>
#include <tulip/GraphTableItemDelegate.h>
#include <tulip/GraphPropertiesModel.h>
#include <tulip/GlMainView.h>
#include <tulip/GlMainWidget.h>
#include <tulip/GlGraphComposite.h>
#include <tulip/TulipSettings.h>
#include <tulip/PluginLister.h>
#include <tulip/TlpQtTools.h>
#include <tulip/TulipProject.h>
#include <tulip/GraphTools.h>
#include <tulip/ColorScaleConfigDialog.h>
#include <tulip/AboutTulipPage.h>
#include <tulip/ColorScalesManager.h>
#include <tulip/StableIterator.h>

#include "ui_GraphPerspectiveMainWindow.h"

#include "GraphPerspectiveLogger.h"
#include "ImportWizard.h"
#include "ExportWizard.h"
#include "PanelSelectionWizard.h"
#include "GraphHierarchiesEditor.h"
#include "PreferencesDialog.h"

#include <QDebug>

using namespace tlp;
using namespace std;

// checks if it exists a Tulip import plugin that can load the provided file based on its extension
static bool tulipCanOpenFile(const QString &path) {
  // Tulip project file does not use import / export plugin
  if (path.endsWith(".tlpx")) {
    return true;
  }

  std::list<std::string> imports = PluginLister::instance()->availablePlugins<ImportModule>();

  for (std::list<std::string>::const_iterator it = imports.begin(); it != imports.end(); ++it) {
    ImportModule *m = PluginLister::instance()->getPluginObject<ImportModule>(*it, nullptr);
    std::list<std::string> fileExtensions(m->allFileExtensions());

    for (std::list<std::string>::const_iterator itExt = fileExtensions.begin();
         itExt != fileExtensions.end(); ++itExt) {
      if (path.endsWith(tlpStringToQString(*itExt))) {
        delete m;
        return true;
      }
    }

    delete m;
  }

  return false;
}

GraphPerspective::GraphPerspective(const tlp::PluginContext *c)
    : Perspective(c), _ui(nullptr), _graphs(new GraphHierarchiesModel(this)),
      _recentDocumentsSettingsKey("perspective/recent_files"), _logger(nullptr) {
  Q_INIT_RESOURCE(GraphPerspective);

  if (c && static_cast<const PerspectiveContext *>(c)->parameters.contains("gui_testing")) {
    tlp::setGuiTestingMode(true);
    // we must ensure that choosing a file is relative to
    // the current directory to allow to run the gui tests
    // from any relative tests/gui directory
    _lastOpenLocation = QDir::currentPath();
  }

#ifdef TULIP_BUILD_PYTHON_COMPONENTS
  _pythonIDE = nullptr;
  _pythonIDEDialog = nullptr;
#endif
}

void GraphPerspective::reserveDefaultProperties() {
  registerReservedProperty("viewColor");
  registerReservedProperty("viewLabelColor");
  registerReservedProperty("viewLabelBorderColor");
  registerReservedProperty("viewLabelBorderWidth");
  registerReservedProperty("viewSize");
  registerReservedProperty("viewLabel");
  registerReservedProperty("viewLabelPosition");
  registerReservedProperty("viewShape");
  registerReservedProperty("viewRotation");
  registerReservedProperty("viewSelection");
  registerReservedProperty("viewFont");
  registerReservedProperty("viewIcon");
  registerReservedProperty("viewFontSize");
  registerReservedProperty("viewTexture");
  registerReservedProperty("viewBorderColor");
  registerReservedProperty("viewBorderWidth");
  registerReservedProperty("viewLayout");
  registerReservedProperty("viewSrcAnchorShape");
  registerReservedProperty("viewSrcAnchorSize");
  registerReservedProperty("viewTgtAnchorShape");
  registerReservedProperty("viewTgtAnchorSize");
  registerReservedProperty("viewAnimationFrame");
}

void GraphPerspective::buildRecentDocumentsMenu() {
  _ui->menuOpen_recent_file->clear();

  foreach (const QString &s, TulipSettings::instance().recentDocuments()) {
    if (!QFileInfo(s).exists() || !tulipCanOpenFile(s))
      continue;

    QAction *action = _ui->menuOpen_recent_file->addAction(
        QIcon(":/tulip/graphperspective/icons/16/archive.png"), s, this, SLOT(openRecentFile()));
    action->setData(s);
  }

  _ui->menuOpen_recent_file->addSeparator();

  foreach (const QString &s,
           TulipSettings::instance().value(_recentDocumentsSettingsKey).toStringList()) {
    if (!QFileInfo(s).exists() || !tulipCanOpenFile(s))
      continue;

    QAction *action = _ui->menuOpen_recent_file->addAction(
        QIcon(":/tulip/graphperspective/icons/16/empty-file.png"), s, this, SLOT(openRecentFile()));
    action->setData(s);
  }
}

void GraphPerspective::addRecentDocument(const QString &path) {
  QStringList recents = TulipSettings::instance().value(_recentDocumentsSettingsKey).toStringList();

  if (recents.contains(path) || !tulipCanOpenFile(path)) {
    return;
  }

  recents += path;

  if (recents.size() > 10)
    recents.pop_front();

  TulipSettings::instance().setValue(_recentDocumentsSettingsKey, recents);
  TulipSettings::instance().sync();
  buildRecentDocumentsMenu();
}

static void logMsgToStdErr(const QString &msg) {
  if (msg.startsWith("[Python")) {
// remove quotes around message added by Qt
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    QString msgClean = msg.mid(14).mid(2, msg.length() - 17);
#else
    QString msgClean = msg.mid(14).mid(2, msg.length() - 18);
#endif

    if (msg.startsWith("[PythonStdOut]")) {
      std::cout << QStringToTlpString(msgClean) << std::endl;
    } else {
      std::cerr << QStringToTlpString(msgClean) << std::endl;
    }
  } else {
    std::cerr << QStringToTlpString(msg) << std::endl;
  }
}

#define SET_TIPS(a, tt)                                                                            \
  a->setToolTip(QString(tt));                                                                      \
  a->setStatusTip(a->toolTip())
void GraphPerspective::updateLogIconsAndCounters() {
  GraphPerspectiveLogger::LogType logType = _logger->getLastLogType();
  QFrame *logIconCounterFrame = nullptr;
  QLabel *logIconLabel = nullptr;
  QLabel *logCounterLabel = nullptr;

  if (logType == GraphPerspectiveLogger::Info) {
    logIconCounterFrame = _ui->loggerFrameInfo;
    logIconLabel = _ui->loggerIconInfo;
    logCounterLabel = _ui->loggerMessageInfo;
  } else if (logType == GraphPerspectiveLogger::Warning) {
    logIconCounterFrame = _ui->loggerFrameWarning;
    logIconLabel = _ui->loggerIconWarning;
    logCounterLabel = _ui->loggerMessageWarning;
  } else if (logType == GraphPerspectiveLogger::Error) {
    logIconCounterFrame = _ui->loggerFrameError;
    logIconLabel = _ui->loggerIconError;
    logCounterLabel = _ui->loggerMessageError;
  } else {
    logIconCounterFrame = _ui->loggerFramePython;
    logIconLabel = _ui->loggerIconPython;
    logCounterLabel = _ui->loggerMessagePython;
  }

  logIconCounterFrame->setVisible(true);
  logIconLabel->setPixmap(_logger->icon(logType));
  SET_TIPS(logIconLabel, "Click here to show/hide the message log window");
  logCounterLabel->setText(QString::number(_logger->countByType(logType)));
  SET_TIPS(logCounterLabel, "Click here to show/hide the message log window");
}

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))

void graphPerspectiveLogger(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
  logMsgToStdErr(msg);
  static_cast<GraphPerspective *>(Perspective::instance())->log(type, context, msg);
}

void GraphPerspective::log(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
  _logger->log(type, context, msg);
  updateLogIconsAndCounters();
}

#else

void graphPerspectiveLogger(QtMsgType type, const char *msg) {
  logMsgToStdErr(msg);
  static_cast<GraphPerspective *>(Perspective::instance())->log(type, msg);
}

void GraphPerspective::log(QtMsgType type, const char *msg) {
  _logger->log(type, msg);
  updateLogIconsAndCounters();
}

#endif

GraphPerspective::~GraphPerspective() {
  // uninstall Qt message handler only if it is the current active perspective
  if (Perspective::instance() == this) {
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    qInstallMessageHandler(nullptr);
#else
    qInstallMsgHandler(0);
#endif
  }

  // ensure the opened views and interactors get deleted before the loaded graphs
  // to avoid possible segfaults when closing Tulip
  if (_ui && _ui->workspace)
    _ui->workspace->closeAll();

  // ensure all loaded graphs are deleted
  tlp::Graph *graph = nullptr;

  foreach (graph, _graphs->graphs()) { delete graph; }

#ifdef TULIP_BUILD_PYTHON_COMPONENTS
  delete _pythonIDEDialog;
#endif

  delete _ui;
}

bool GraphPerspective::terminated() {

#ifdef TULIP_BUILD_PYTHON_COMPONENTS
  _pythonIDE->savePythonFilesAndWriteToProject(true);
  _pythonIDEDialog->hide();
#endif

  if (_graphs->needsSaving() || mainWindow()->isWindowModified()) {
    QString message = trUtf8("The project has been modified. Do you want to save your changes?");
#ifdef TULIP_BUILD_PYTHON_COMPONENTS
    message = trUtf8("The project has been modified (loaded graphs or Python files opened in the "
                     "IDE). Do you want to save your changes?");
#endif
    QMessageBox::StandardButton answer = QMessageBox::question(
        _mainWindow, trUtf8("Save"), message,
        QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel | QMessageBox::Escape);

    if ((answer == QMessageBox::Yes && !save()) || (answer == QMessageBox::Cancel)) {
      return false;
    }
  }

  return true;
}

void GraphPerspective::logCleared() {
  _ui->loggerMessageInfo->clear();
  _ui->loggerIconInfo->clear();
  _ui->loggerFrameInfo->setVisible(false);
  _ui->loggerFrameWarning->setVisible(false);
  _ui->loggerFrameError->setVisible(false);
  _ui->loggerFramePython->setVisible(false);
}

void GraphPerspective::findPlugins() {
  _ui->algorithmRunner->findPlugins();
}

bool GraphPerspective::eventFilter(QObject *obj, QEvent *ev) {
  if (ev->type() == QEvent::DragEnter) {
    QDragEnterEvent *dragEvent = static_cast<QDragEnterEvent *>(ev);

    if (dragEvent->mimeData()->hasUrls()) {
      dragEvent->accept();
    }
  }

  if (ev->type() == QEvent::Drop) {
    QDropEvent *dropEvent = static_cast<QDropEvent *>(ev);

    foreach (const QUrl &url, dropEvent->mimeData()->urls()) { open(url.toLocalFile()); }
  }

  if (obj == _ui->loggerFrame && ev->type() == QEvent::MouseButtonPress)
    showHideLogger();

  if (obj == _mainWindow && _logger &&
      (ev->type() == QEvent::Resize || ev->type() == QEvent::Move ||
       ev->type() == QEvent::WindowStateChange)) {

    if (_logger->anchored()) {
      resetLoggerDialogPosition();
    }
  }

  return false;
}

void GraphPerspective::showLogger() {
  _logger->show();

  static bool firstTime = true;

  if (firstTime) {
    resetLoggerDialogPosition();
    firstTime = false;
  }
}

void GraphPerspective::showHideLogger() {

  if (!_logger->isVisible()) {
    showLogger();
  } else {
    _logger->hide();
  }
}

void GraphPerspective::redrawPanels(bool center) {
  _ui->workspace->redrawPanels(center);
}

#ifdef TULIP_BUILD_PYTHON_COMPONENTS
class PythonIDEDialog : public QDialog {

  QByteArray _windowGeometry;

public:
  PythonIDEDialog(QWidget *parent, Qt::WindowFlags flags) : QDialog(parent, flags) {}

protected:
  virtual void showEvent(QShowEvent *e) {
    QDialog::showEvent(e);

    if (!_windowGeometry.isEmpty()) {
      restoreGeometry(_windowGeometry);
    }
  }

  virtual void closeEvent(QCloseEvent *e) {
    _windowGeometry = saveGeometry();
    QDialog::closeEvent(e);
  }
};
#endif

#define SET_TOOLTIP(a, tt) a->setToolTip(QString(tt))

void GraphPerspective::start(tlp::PluginProgress *progress) {
  reserveDefaultProperties();
  _ui = new Ui::GraphPerspectiveMainWindowData;
  _ui->setupUi(_mainWindow);
#ifdef TULIP_BUILD_PYTHON_COMPONENTS
  _pythonPanel = new PythonPanel();
  QVBoxLayout *layout = new QVBoxLayout();
  layout->addWidget(_pythonPanel);
  layout->setContentsMargins(0, 0, 0, 0);
  _ui->pythonPanel->setLayout(layout);
  _pythonIDE = new PythonIDE();
  QVBoxLayout *dialogLayout = new QVBoxLayout();
  dialogLayout->addWidget(_pythonIDE);
  dialogLayout->setContentsMargins(0, 0, 0, 0);
  _pythonIDEDialog = new PythonIDEDialog(nullptr, Qt::Window);
  _pythonIDEDialog->setStyleSheet(_mainWindow->styleSheet());
  _pythonIDEDialog->setWindowIcon(_mainWindow->windowIcon());
  _pythonIDEDialog->setLayout(dialogLayout);
  _pythonIDEDialog->resize(800, 600);
  _pythonIDEDialog->setWindowTitle("Tulip Python IDE");
#else
  _ui->pythonButton->setVisible(false);
  _ui->developButton->setVisible(false);
  _ui->actionPython_IDE->setVisible(false);
#endif
  currentGraphChanged(nullptr);
  // set win/Mac dependent tooltips with ctrl shortcut
  SET_TIPS_WITH_CTRL_SHORTCUT(_ui->exposeModeButton, "Toggle the Expose mode", "E");
  SET_TIPS_WITH_CTRL_SHORTCUT(_ui->searchButton, "Show/hide the graph's elements search panel",
                              "F");
  SET_TIPS_WITH_CTRL_SHORTCUT(_ui->pythonButton,
                              "Show/hide the Python interpreter (Read Eval Print Loop) panel",
                              "Shift+P");
  SET_TIPS_WITH_CTRL_SHORTCUT(_ui->previousPageButton, "Show previous panel", "Shift+Left");
  SET_TIPS_WITH_CTRL_SHORTCUT(_ui->nextPageButton, "Show next panel", "Shift+Right");
  SET_TOOLTIP_WITH_CTRL_SHORTCUT(_ui->actionNewProject, "Open a new  empty Tulip perspective",
                                 "Shift+N");
  SET_TOOLTIP_WITH_CTRL_SHORTCUT(
      _ui->actionSave_Project,
      "Save the current project (current graphs with current views) in the attached file", "S");
  SET_TOOLTIP_WITH_CTRL_SHORTCUT(_ui->actionSave_Project_as, "Save Project as a new file name",
                                 "Shift+S");
  SET_TOOLTIP_WITH_CTRL_SHORTCUT(_ui->actionImport, "Display the Graph importing wizard",
                                 "Shift+O");
  SET_TOOLTIP_WITH_CTRL_SHORTCUT(_ui->actionExit, "Exit from Tulip perspective", "Q");
  SET_TOOLTIP_WITH_CTRL_SHORTCUT(_ui->actionUndo, "Undo the latest update of the current graph",
                                 "Z");
  SET_TOOLTIP_WITH_CTRL_SHORTCUT(_ui->actionRedo, "Redo the latest update of the current graph",
                                 "Y");
  SET_TOOLTIP_WITH_CTRL_SHORTCUT(
      _ui->actionCut, "Move the selected elements of the current graph into the clipboard", "X");
  SET_TOOLTIP_WITH_CTRL_SHORTCUT(
      _ui->actionCopy, "Copy the selected elements of the current graph into the clipboard", "C");
  SET_TOOLTIP_WITH_CTRL_SHORTCUT(_ui->actionPaste,
                                 "Paste the clipboard elements into the current graph", "V");
  SET_TOOLTIP_WITH_CTRL_SHORTCUT(_ui->actionSelect_All, "Select all elements of the current graph",
                                 "A");
  SET_TOOLTIP_WITH_CTRL_SHORTCUT(_ui->actionInvert_selection, "Invert the selection of the current "
                                                              "graph elements, deselect if "
                                                              "selected and select if not selected",
                                 "I");
  SET_TOOLTIP_WITH_CTRL_SHORTCUT(_ui->actionCancel_selection,
                                 "Deselect all selected elements of the current grap", "Shift+A");
  SET_TOOLTIP_WITH_CTRL_SHORTCUT(_ui->actionGroup_elements,
                                 "Create a meta-node representing a newly created sub-graph "
                                 "containing all selected elements of the current graph",
                                 "G");
  SET_TOOLTIP_WITH_CTRL_SHORTCUT(
      _ui->actionCreate_sub_graph,
      "Create a sub-graph containing all selected elements of the current graph", "Shift+G");
  SET_TOOLTIP_WITH_CTRL_SHORTCUT(_ui->actionPreferences, "Show Tulip preferences dialog", ",");
  SET_TOOLTIP_WITH_CTRL_SHORTCUT(_ui->actionShowUserDocumentation,
                                 "Display the User handbook in a navigator", "?");
  SET_TOOLTIP_WITH_CTRL_SHORTCUT(_ui->actionShowDevelDocumentation,
                                 "Display the Developer handbook in a navigator", "D");
  SET_TOOLTIP_WITH_CTRL_SHORTCUT(_ui->actionShowPythonDocumentation,
                                 "Display the Tulip python documentation in a navigator", "P");
  SET_TOOLTIP_WITH_CTRL_SHORTCUT(_ui->actionMessages_log, "Show the message log", "M");
  SET_TOOLTIP_WITH_CTRL_SHORTCUT(_ui->actionPython_IDE, "Show the Python IDE", "Alt+P");
  SET_TOOLTIP_WITH_CTRL_SHORTCUT(_ui->actionExport, "Show the Graph exporting wizard", "E");
  SET_TOOLTIP_WITH_CTRL_SHORTCUT(_ui->actionOpen_Project, "Open a graph file", "O");
  SET_TOOLTIP_WITH_CTRL_SHORTCUT(_ui->actionFind_plugins,
                                 "Allow to find algorithms in typing text in a search field",
                                 "Alt+H");
  SET_TOOLTIP_WITH_CTRL_SHORTCUT(_ui->actionNew_graph, "Create a new empty graph", "N");
  // set portable tooltips
  SET_TIPS(_ui->undoButton, "Undo the latest update of the current graph");
  SET_TIPS(_ui->redoButton, "Redo the latest undone update of the current graph");
  _ui->workspaceButton->setToolTip(QString("Display the existing graph views"));
  SET_TIPS(_ui->developButton, "Display the Tulip Python IDE for developing scripts and plugins to "
                               "execute on the loaded graphs");
  _ui->loggerMessageInfo->setToolTip(QString("Show/Hide the Messages log panel"));
  _ui->loggerMessagePython->setToolTip(_ui->loggerMessageInfo->toolTip());
  _ui->loggerMessageWarning->setToolTip(_ui->loggerMessageInfo->toolTip());
  _ui->loggerMessageError->setToolTip(_ui->loggerMessageInfo->toolTip());
  SET_TIPS(_ui->exportButton, "Display the Graph exporting wizard");
  SET_TIPS(_ui->csvImportButton, "Import data in the current graph using a csv formatted file");
  SET_TIPS(_ui->importButton, "Display the Graph importing wizard");
  SET_TIPS(_ui->pluginsButton, "Display the Plugin center");
  SET_TIPS(_ui->sidebarButton, "Hide Sidebar");
  SET_TIPS(_ui->statusbarButton, "Hide Status bar");
  SET_TIPS(_ui->addPanelButton, "Open a new visualization panel on the current graph");
  SET_TIPS(_ui->singleModeButton, "Switch to 1-panel mode");
  SET_TIPS(_ui->splitModeButton, "Switch to 2-panels mode");
  SET_TIPS(_ui->splitHorizontalModeButton, "Switch to 2-panels mode");
  SET_TIPS(_ui->split3ModeButton, "Switch to 2-top-1-bottom-panels mode");
  SET_TIPS(_ui->split32ModeButton, "Switch to 1-left-2-right-panels mode");
  SET_TIPS(_ui->split33ModeButton, "Switch to 2-left-1-right-panels mode");
  SET_TIPS(_ui->gridModeButton, "Switch to 4-panels mode");
  SET_TIPS(_ui->sixModeButton, "Switch to 6-panels mode");
  SET_TIPS(_ui->menuDelete, "Delete elements of the current graph");
  _ui->menuOpen_recent_file->setToolTip(
      QString("Choose a file to open among the recently opened/saved graphs or projects"));
  SET_TOOLTIP(_ui->actionDelete, "Delete the selected elements from the current graph [Del]");
  SET_TOOLTIP(_ui->actionFull_screen, "Display the Tulip perspective in full screen [F11]");
  SET_TOOLTIP(_ui->actionAbout_us, "Display the 'About Tulip' information dialog [F1]");
  SET_TOOLTIP(_ui->actionPlugins_Center, _ui->pluginsButton->toolTip());
  SET_TOOLTIP(_ui->actionImport_CSV, _ui->csvImportButton->toolTip());
  SET_TOOLTIP(_ui->actionSave_graph_to_file, "Write the current graph into a file");
  SET_TOOLTIP(_ui->actionCreate_empty_sub_graph, "Create an empty sub-graph of the current graph");
  SET_TOOLTIP(_ui->actionClone_sub_graph,
              "Create a sub-graph containing the same elements as the current graph");
  SET_TOOLTIP(_ui->action_Close_All, "Close all opened workspace views");
  SET_TOOLTIP(_ui->actionColor_scales_management, "Manage Tulip color scales");
  SET_TOOLTIP(_ui->actionMake_selection_a_graph,
              "Add the non selected ends of the selected edges to the current graph selection");
  SET_TOOLTIP(_ui->actionDelete_from_the_root_graph, "Delete the selected elements from the whole "
                                                     "graph hierarchy (not only from the current "
                                                     "graph) [Shift+Del]");
  SET_TOOLTIP(_ui->actionReverse_selected_edges,
              "Reverse the source and target nodes of the selected edges");
  SET_TOOLTIP(_ui->actionDelete_all, "Delete all elements of the current graph");
  SET_TOOLTIP(_ui->actionSelect_All_Nodes, "Select all nodes of the current graph");
  SET_TOOLTIP(_ui->actionSelect_All_Edges, "Select all edges of the current graph");

  _ui->singleModeButton->setEnabled(false);
  _ui->singleModeButton->hide();
  _ui->workspace->setSingleModeSwitch(_ui->singleModeButton);
  _ui->workspace->setFocusedPanelHighlighting(true);
  _ui->splitModeButton->setEnabled(false);
  _ui->splitModeButton->hide();
  _ui->workspace->setSplitModeSwitch(_ui->splitModeButton);
  _ui->splitHorizontalModeButton->setEnabled(false);
  _ui->splitHorizontalModeButton->hide();
  _ui->workspace->setSplitHorizontalModeSwitch(_ui->splitHorizontalModeButton);
  _ui->split3ModeButton->setEnabled(false);
  _ui->split3ModeButton->hide();
  _ui->workspace->setSplit3ModeSwitch(_ui->split3ModeButton);
  _ui->split32ModeButton->setEnabled(false);
  _ui->split32ModeButton->hide();
  _ui->workspace->setSplit32ModeSwitch(_ui->split32ModeButton);
  _ui->split33ModeButton->setEnabled(false);
  _ui->split33ModeButton->hide();
  _ui->workspace->setSplit33ModeSwitch(_ui->split33ModeButton);
  _ui->gridModeButton->setEnabled(false);
  _ui->gridModeButton->hide();
  _ui->workspace->setGridModeSwitch(_ui->gridModeButton);
  _ui->sixModeButton->setEnabled(false);
  _ui->sixModeButton->hide();
  _ui->workspace->setSixModeSwitch(_ui->sixModeButton);
  _ui->workspace->setPageCountLabel(_ui->pageCountLabel);
  _ui->workspace->setExposeModeSwitch(_ui->exposeModeButton);
  _ui->outputFrame->hide();
  _logger = new GraphPerspectiveLogger(_mainWindow);
  _ui->loggerFrame->installEventFilter(this);
  _mainWindow->installEventFilter(this);
  _mainWindow->setAcceptDrops(true);
  _mainWindow->statusBar();

  if (tlp::inGuiTestingMode() || !TulipSettings::instance().showStatusBar())
    _mainWindow->statusBar()->hide();

  connect(_logger, SIGNAL(cleared()), this, SLOT(logCleared()));
  connect(_logger, SIGNAL(resetLoggerPosition()), this, SLOT(resetLoggerDialogPosition()));

  _colorScalesDialog =
      new ColorScaleConfigDialog(ColorScalesManager::getLatestColorScale(), mainWindow());

  // redirection of various output
  redirectDebugOutputToQDebug();
  redirectWarningOutputToQWarning();
  redirectErrorOutputToQCritical();

  // redirect statusTip or toolTip of menu actions in status bar
  Perspective::redirectStatusTipOfMenu(_ui->menuFile);
  Perspective::redirectStatusTipOfMenu(_ui->menuEdit);
  Perspective::redirectStatusTipOfMenu(_ui->menuHelp);
  Perspective::redirectStatusTipOfMenu(_ui->menuWindow);

  TulipSettings::instance().synchronizeViewSettings();

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
  qInstallMessageHandler(graphPerspectiveLogger);
#else
  qInstallMsgHandler(graphPerspectiveLogger);
#endif

  connect(_ui->workspaceButton, SIGNAL(clicked()), this, SLOT(workspaceButtonClicked()));
  connect(_ui->workspace, SIGNAL(addPanelRequest(tlp::Graph *)), this,
          SLOT(createPanel(tlp::Graph *)));
  connect(_ui->workspace, SIGNAL(focusedPanelSynchronized()), this,
          SLOT(focusedPanelSynchronized()));
  connect(_graphs, SIGNAL(currentGraphChanged(tlp::Graph *)), this,
          SLOT(currentGraphChanged(tlp::Graph *)));
  connect(_graphs, SIGNAL(currentGraphChanged(tlp::Graph *)), _ui->algorithmRunner,
          SLOT(setGraph(tlp::Graph *)));
  connect(_ui->graphHierarchiesEditor, SIGNAL(changeSynchronization(bool)), this,
          SLOT(changeSynchronization(bool)));

  // Connect actions
  connect(_ui->actionMessages_log, SIGNAL(triggered()), this, SLOT(showLogger()));
  connect(_ui->actionPython_IDE, SIGNAL(triggered()), this, SLOT(showPythonIDE()));
  connect(_ui->actionFull_screen, SIGNAL(triggered(bool)), this, SLOT(showFullScreen(bool)));
  connect(_ui->actionImport, SIGNAL(triggered()), this, SLOT(importGraph()));
  connect(_ui->actionExport, SIGNAL(triggered()), this, SLOT(exportGraph()));
  connect(_ui->actionSave_graph_to_file, SIGNAL(triggered()), this,
          SLOT(saveGraphHierarchyInTlpFile()));
  connect(_ui->workspace, SIGNAL(panelFocused(tlp::View *)), this, SLOT(panelFocused(tlp::View *)));
  connect(_ui->actionSave_Project, SIGNAL(triggered()), this, SLOT(save()));
  connect(_ui->actionSave_Project_as, SIGNAL(triggered()), this, SLOT(saveAs()));
  connect(_ui->actionOpen_Project, SIGNAL(triggered()), this, SLOT(open()));
  connect(_ui->actionDelete, SIGNAL(triggered()), this, SLOT(deleteSelectedElements()));
  connect(_ui->actionDelete_from_the_root_graph, SIGNAL(triggered()), this,
          SLOT(deleteSelectedElementsFromRootGraph()));
  connect(_ui->actionDelete_all, SIGNAL(triggered()), this, SLOT(clearGraph()));
  connect(_ui->actionInvert_selection, SIGNAL(triggered()), this, SLOT(invertSelection()));
  connect(_ui->actionCancel_selection, SIGNAL(triggered()), this, SLOT(cancelSelection()));
  connect(_ui->actionReverse_selected_edges, SIGNAL(triggered()), this,
          SLOT(reverseSelectedEdges()));
  connect(_ui->actionMake_selection_a_graph, SIGNAL(triggered()), this, SLOT(make_graph()));
  connect(_ui->actionSelect_All, SIGNAL(triggered()), this, SLOT(selectAll()));
  connect(_ui->actionSelect_All_Nodes, SIGNAL(triggered()), this, SLOT(selectAllNodes()));
  connect(_ui->actionSelect_All_Edges, SIGNAL(triggered()), this, SLOT(selectAllEdges()));
  connect(_ui->actionUndo, SIGNAL(triggered()), this, SLOT(undo()));
  connect(_ui->actionRedo, SIGNAL(triggered()), this, SLOT(redo()));
  connect(_ui->actionCut, SIGNAL(triggered()), this, SLOT(cut()));
  connect(_ui->actionPaste, SIGNAL(triggered()), this, SLOT(paste()));
  connect(_ui->actionCopy, SIGNAL(triggered()), this, SLOT(copy()));
  connect(_ui->actionGroup_elements, SIGNAL(triggered()), this, SLOT(group()));
  connect(_ui->actionCreate_sub_graph, SIGNAL(triggered()), this, SLOT(createSubGraph()));
  connect(_ui->actionClone_sub_graph, SIGNAL(triggered()), this, SLOT(cloneSubGraph()));
  connect(_ui->actionCreate_empty_sub_graph, SIGNAL(triggered()), this, SLOT(addEmptySubGraph()));
  connect(_ui->actionImport_CSV, SIGNAL(triggered()), this, SLOT(CSVImport()));
  connect(_ui->actionFind_plugins, SIGNAL(triggered()), this, SLOT(findPlugins()));
  connect(_ui->actionNew_graph, SIGNAL(triggered()), this, SLOT(addNewGraph()));
  connect(_ui->actionNewProject, SIGNAL(triggered()), this, SLOT(newProject()));
  connect(_ui->actionPreferences, SIGNAL(triggered()), this, SLOT(openPreferences()));
  connect(_ui->searchButton, SIGNAL(clicked(bool)), this, SLOT(setSearchOutput(bool)));
  connect(_ui->workspace, SIGNAL(importGraphRequest()), this, SLOT(importGraph()));
  connect(_ui->action_Close_All, SIGNAL(triggered()), _ui->workspace, SLOT(closeAll()));
  connect(_ui->addPanelButton, SIGNAL(clicked()), this, SLOT(createPanel()));
  connect(_ui->actionColor_scales_management, SIGNAL(triggered()), this,
          SLOT(displayColorScalesDialog()));
  connect(_ui->exportButton, SIGNAL(clicked()), this, SLOT(exportGraph()));

  // Agent actions
  connect(_ui->actionPlugins_Center, SIGNAL(triggered()), this, SLOT(showPluginsCenter()));
  connect(_ui->actionAbout_us, SIGNAL(triggered()), this, SLOT(showAboutPage()));
  connect(_ui->actionAbout_us, SIGNAL(triggered()), this, SLOT(showAboutTulipPage()));

  if (QFile(tlpStringToQString(tlp::TulipShareDir) + "../doc/tulip/tulip-user/html/index.html")
          .exists()) {
    connect(_ui->actionShowUserDocumentation, SIGNAL(triggered()), this,
            SLOT(showUserDocumentation()));
    connect(_ui->actionShowDevelDocumentation, SIGNAL(triggered()), this,
            SLOT(showDevelDocumentation()));
#ifdef TULIP_BUILD_PYTHON_COMPONENTS
    connect(_ui->actionShowPythonDocumentation, SIGNAL(triggered()), this,
            SLOT(showPythonDocumentation()));
#else
    _ui->actionShowPythonDocumentation->setVisible(false);
#endif
  } else {
    _ui->actionShowUserDocumentation->setVisible(false);
    _ui->actionShowDevelDocumentation->setVisible(false);
    _ui->actionShowPythonDocumentation->setVisible(false);
  }

  if (QFile(tlpStringToQString(tlp::TulipShareDir) + "../doc/tulip/doxygen/html/index.html")
          .exists()) {
    connect(_ui->actionShowAPIDocumentation, SIGNAL(triggered()), this,
            SLOT(showAPIDocumentation()));
  } else {
    _ui->actionShowAPIDocumentation->setVisible(false);
  }

  // Setting initial sizes for splitters
  _ui->mainSplitter->setSizes(QList<int>() << 350 << 850);
  _ui->mainSplitter->setStretchFactor(0, 0);
  _ui->mainSplitter->setStretchFactor(1, 1);
  _ui->mainSplitter->setCollapsible(1, false);

  // Open project with model
  QMap<QString, tlp::Graph *> rootIds;

  if (!_project->projectFile().isEmpty()) {
    rootIds = _graphs->readProject(_project, progress);

    if (rootIds.empty())
      QMessageBox::critical(_mainWindow,
                            QString("Error while loading project ").append(_project->projectFile()),
                            QString("The Tulip project file is probably corrupted.<br>") +
                                tlpStringToQString(progress->getError()));
  }

  // these ui initializations are needed here
  // in case of a call to showStartPanels in the open method
  _ui->graphHierarchiesEditor->setModel(_graphs);
  _ui->workspace->setModel(_graphs);

  if (!rootIds.empty())
    _ui->workspace->readProject(_project, rootIds, progress);

  _ui->searchPanel->setModel(_graphs);

#ifdef TULIP_BUILD_PYTHON_COMPONENTS
  connect(_ui->pythonButton, SIGNAL(clicked(bool)), this, SLOT(setPythonPanel(bool)));
  connect(_ui->developButton, SIGNAL(clicked()), this, SLOT(showPythonIDE()));
  _pythonPanel->setModel(_graphs);
  _pythonIDE->setGraphsModel(_graphs);
  tlp::PluginLister::instance()->addListener(this);
  QTimer::singleShot(100, this, SLOT(initPythonIDE()));
#endif

  if (!_externalFile.isEmpty() && QFileInfo(_externalFile).exists()) {
    open(_externalFile);
  }

  foreach (HeaderFrame *h, _ui->docksSplitter->findChildren<HeaderFrame *>()) {
    connect(h, SIGNAL(expanded(bool)), this, SLOT(refreshDockExpandControls()));
  }

  connect(_ui->sidebarButton, SIGNAL(clicked()), this, SLOT(showHideSideBar()));
  connect(_ui->statusbarButton, SIGNAL(clicked()), this, SLOT(showHideStatusBar()));

#if !defined(__APPLE__) && !defined(_WIN32)
  // Hide plugins center when not on MacOS or Windows
  _ui->pluginsButton->hide();
  _ui->menuHelp->removeAction(_ui->actionPlugins_Center);
#else
  // show the 'Plugins center' menu entry and button only if connected to the Tulip agent
  _ui->pluginsButton->setVisible(checkSocketConnected());
  _ui->actionPlugins_Center->setVisible(checkSocketConnected());
#endif

  // fill menu with recent documents
  buildRecentDocumentsMenu();

  showTrayMessage("GraphPerspective started");

#ifdef TULIP_BUILD_PYTHON_COMPONENTS
  // for 5.0 show message to indicate that
  // Python Script view no longer exist
  unsigned int mm_version = TULIP_INT_MM_VERSION;

  if ((mm_version == 50) && TulipSettings::instance().isFirstTulipMMRun()) {
    QTimer::singleShot(100, this, SLOT(showStartMessage()));
  }

#endif
  logCleared();
}

void GraphPerspective::showStartMessage() {
  QMessageBox::information(
      _mainWindow, QString("About Tulip Python IDE"),
      QString("<html><body><p>Be aware that the <b>Python Script View</b> no longer exists. The "
              "coding of python scripts is now available in using the <b>Tulip Python "
              "IDE</b>.<br/>Click on the <img "
              "src=\":/tulip/graphperspective/icons/16/python.png\">&nbsp;<b>Python IDE</b> button "
              "(enabled when a graph is loaded) to launch it.</p></body></html>"));
}

void GraphPerspective::openExternalFile() {
  open(_externalFile);
}

tlp::GraphHierarchiesModel *GraphPerspective::model() const {
  return _graphs;
}

void GraphPerspective::refreshDockExpandControls() {
  QList<HeaderFrame *> expandedHeaders, collapsedHeaders;

  foreach (HeaderFrame *h, _ui->docksSplitter->findChildren<HeaderFrame *>()) {
    h->expandControl()->setEnabled(true);

    if (h->isExpanded())
      expandedHeaders.push_back(h);
    else
      collapsedHeaders.push_back(h);
  }

  if (expandedHeaders.size() == 1)
    expandedHeaders[0]->expandControl()->setEnabled(false);
}

void GraphPerspective::exportGraph(Graph *g) {
  if (g == nullptr)
    g = _graphs->currentGraph();

  if (g == nullptr)
    return;

  static QString exportFile;
  ExportWizard wizard(g, exportFile, _mainWindow);
  wizard.setWindowTitle(QString("Exporting graph \"") + tlpStringToQString(g->getName()) + '"');

  if (wizard.exec() != QDialog::Accepted || wizard.algorithm().isEmpty() ||
      wizard.outputFile().isEmpty())
    return;

  std::string filename = QStringToTlpString(exportFile = wizard.outputFile());
  std::string exportPluginName = QStringToTlpString(wizard.algorithm());

  DataSet data = wizard.parameters();
  PluginProgress *prg = progress(NoProgressOption);
  prg->setTitle(exportPluginName);
  // take time before run
  QTime start = QTime::currentTime();
  bool result = tlp::saveGraph(g, filename, prg, &data);

  if (!result) {
    QMessageBox::critical(_mainWindow, trUtf8("Export error"),
                          QString("<i>") + wizard.algorithm() +
                              trUtf8("</i> failed to export graph.<br/><br/><b>") +
                              tlp::tlpStringToQString(prg->getError()) + "</b>");
  } else {
    // log export plugin call
    if (TulipSettings::instance().logPluginCall() != TulipSettings::NoLog) {
      std::stringstream log;
      log << exportPluginName.c_str() << " - " << data.toString().c_str();

      if (TulipSettings::instance().logPluginCall() == TulipSettings::LogCallWithExecutionTime)
        log << ": " << start.msecsTo(QTime::currentTime()) << "ms";

      qDebug() << log.str().c_str();
    }

    addRecentDocument(wizard.outputFile());
  }

  delete prg;
}

void GraphPerspective::saveGraphHierarchyInTlpFile(Graph *g) {
  if (g == nullptr)
    g = _graphs->currentGraph();

  if (g == nullptr)
    return;

  static QString savedFile;
  QString filter("TLP format (*.tlp *.tlp.gz *.tlpz);;TLPB format (*.tlpb *.tlpb.gz *.tlpbz)");
  QString filename = QFileDialog::getSaveFileName(
      _mainWindow, tr("Save graph hierarchy in tlp/tlpb file"), savedFile, filter);

  if (!filename.isEmpty()) {
    bool result = tlp::saveGraph(g, tlp::QStringToTlpString(filename));

    if (!result)
      QMessageBox::critical(_mainWindow, trUtf8("Save error"),
                            trUtf8("Failed to save graph hierarchy"));
    else {
      savedFile = filename;
      addRecentDocument(filename);
    }
  }
}

void GraphPerspective::importGraph(const std::string &module, DataSet &data) {
  Graph *g;

  if (!module.empty()) {
    PluginProgress *prg = progress(IsStoppable | IsCancellable);
    prg->setTitle(module);
    // take time before run
    QTime start = QTime::currentTime();
    g = tlp::importGraph(module, data, prg);

    if (g == nullptr) {
      QMessageBox::critical(_mainWindow, trUtf8("Import error"),
                            QString("<i>") + tlp::tlpStringToQString(module) +
                                trUtf8("</i> failed to import data.<br/><br/><b>") +
                                tlp::tlpStringToQString(prg->getError()) + "</b>");
      delete prg;
      return;
    }

    delete prg;

    // log import plugin call
    if (TulipSettings::instance().logPluginCall() != TulipSettings::NoLog) {
      std::stringstream log;
      log << module.c_str() << " import - " << data.toString().c_str();

      if (TulipSettings::instance().logPluginCall() == TulipSettings::LogCallWithExecutionTime)
        log << ": " << start.msecsTo(QTime::currentTime()) << "ms";

      qDebug() << log.str().c_str();
    }

    if (g->getName().empty()) {
      QString n =
          tlp::tlpStringToQString(module) + " - " + tlp::tlpStringToQString(data.toString());
      n.replace(QRegExp("[\\w]*::"), ""); // remove words before "::"
      g->setName(tlp::QStringToTlpString(n));
    }
  } else {
    g = tlp::newGraph();
  }

  _graphs->addGraph(g);
  std::string fileName;

  if (data.get("file::filename", fileName))
    // set current directory to the directory of the loaded file
    // to ensure a correct loading of the associated texture files if any
    QDir::setCurrent(QFileInfo(tlpStringToQString(fileName)).absolutePath());

  applyRandomLayout(g);
  showStartPanels(g);
}

void GraphPerspective::importGraph() {
  ImportWizard wizard(_mainWindow);

  if (wizard.exec() == QDialog::Accepted) {
    DataSet data = wizard.parameters();
    importGraph(tlp::QStringToTlpString(wizard.algorithm()), data);
  }
}

void GraphPerspective::createPanel(tlp::Graph *g) {
  if (_graphs->empty())
    return;

  PanelSelectionWizard wizard(_graphs, _mainWindow);

  if (g != nullptr)
    wizard.setSelectedGraph(g);
  else
    wizard.setSelectedGraph(_graphs->currentGraph());

  int result = wizard.exec();

  if (result == QDialog::Accepted && wizard.panel() != nullptr) {
    // expose mode is not safe to add a new panel
    // so hide it if needed
    _ui->workspace->hideExposeMode();
    _ui->workspace->addPanel(wizard.panel());
    _ui->workspace->setActivePanel(wizard.panel());
    wizard.panel()->applySettings();
  }
}

void GraphPerspective::panelFocused(tlp::View *view) {
  disconnect(this, SLOT(focusedPanelGraphSet(tlp::Graph *)));

  if (!_ui->graphHierarchiesEditor->synchronized())
    return;

  connect(view, SIGNAL(graphSet(tlp::Graph *)), this, SLOT(focusedPanelGraphSet(tlp::Graph *)));
  focusedPanelGraphSet(view->graph());
}

void GraphPerspective::changeSynchronization(bool s) {
  _ui->workspace->setFocusedPanelHighlighting(s);
}

void GraphPerspective::focusedPanelGraphSet(Graph *g) {
  _graphs->setCurrentGraph(g);
}

void GraphPerspective::focusedPanelSynchronized() {
  _ui->workspace->setGraphForFocusedPanel(_graphs->currentGraph());
}

bool GraphPerspective::save() {
  return saveAs(_project->projectFile());
}

bool GraphPerspective::saveAs(const QString &path) {
  if (_graphs->empty())
    return false;

  if (path.isEmpty()) {
    QString path = QFileDialog::getSaveFileName(_mainWindow, trUtf8("Save project"), QString(),
                                                "Tulip Project (*.tlpx)");

    if (!path.isEmpty()) {
      if (!path.endsWith(".tlpx"))
        path += ".tlpx";

      _project->setProjectFile(path);
      return saveAs(path);
    }

    return false;
  }

  SimplePluginProgressDialog progress(_mainWindow);
  progress.showPreview(false);
  progress.show();
  QMap<Graph *, QString> rootIds = _graphs->writeProject(_project, &progress);
  _ui->workspace->writeProject(_project, rootIds, &progress);
#ifdef TULIP_BUILD_PYTHON_COMPONENTS
  _pythonIDE->savePythonFilesAndWriteToProject();
#endif
  bool ret = _project->write(path, &progress);

  if (ret)
    TulipSettings::instance().addToRecentDocuments(path);

  return ret;
}

void GraphPerspective::open(QString fileName) {
  QMap<std::string, std::string> modules;
  std::list<std::string> imports = PluginLister::instance()->availablePlugins<ImportModule>();

  std::string filters("Tulip project (*.tlpx);;");
  std::string filterAny("Any supported format (");

  for (std::list<std::string>::const_iterator it = imports.begin(); it != imports.end(); ++it) {
    ImportModule *m = PluginLister::instance()->getPluginObject<ImportModule>(*it, nullptr);
    std::list<std::string> fileExtension(m->allFileExtensions());

    std::string currentFilter;

    for (std::list<std::string>::const_iterator listIt = fileExtension.begin();
         listIt != fileExtension.end(); ++listIt) {

      if (listIt->empty())
        continue;

      filterAny += "*." + *listIt + " ";
      currentFilter += "*." + *listIt + " ";

      modules[*listIt] = *it;
    }

    if (!currentFilter.empty())
      filters += *it + "(" + currentFilter + ");;";

    delete m;
  }

  filterAny += " *.tlpx);;";

  filters += "All files (*)";
  filters.insert(0, filterAny);

  if (fileName.isEmpty()) // If open() was called without a parameter, open the file dialog
    fileName = QFileDialog::getOpenFileName(_mainWindow, tr("Open graph"), _lastOpenLocation,
                                            filters.c_str());

  if (!fileName.isEmpty()) {
    QFileInfo fileInfo(fileName);

    // we must ensure that choosing a file is relative to
    // the current directory to allow to run the gui tests
    // from any relative tests/gui directory
    if (!tlp::inGuiTestingMode())
      _lastOpenLocation = fileInfo.absolutePath();

    foreach (const std::string &extension, modules.keys()) {
      if (fileName.endsWith(".tlpx")) {
        openProjectFile(fileName);
        TulipSettings::instance().addToRecentDocuments(fileInfo.absoluteFilePath());
        break;
      } else if (fileName.endsWith(QString::fromStdString(extension))) {
        DataSet params;
        params.set("file::filename", QStringToTlpString(fileName));
        addRecentDocument(fileName);
        importGraph(modules[extension], params);
        break;
      }
    }
  }
}

void GraphPerspective::openProjectFile(const QString &path) {
  if (_graphs->empty()) {
    PluginProgress *prg = progress(NoProgressOption);
    _project->openProjectFile(path, prg);
    QMap<QString, tlp::Graph *> rootIds = _graphs->readProject(_project, prg);
    _ui->workspace->readProject(_project, rootIds, prg);
#ifdef TULIP_BUILD_PYTHON_COMPONENTS
    QTimer::singleShot(100, this, SLOT(initPythonIDE()));
#endif
    delete prg;
  } else {
    Perspective::openProjectFile(path);
  }
}

#ifdef TULIP_BUILD_PYTHON_COMPONENTS
void GraphPerspective::initPythonIDE() {
  _pythonIDE->setProject(_project);
}
#endif

void GraphPerspective::deleteSelectedElementsFromRootGraph() {
  deleteSelectedElements(true);
}

void GraphPerspective::clearGraph() {
  if (QMessageBox::question(_mainWindow, "Clear graph content",
                            "Do you really want to remove all nodes and edges from the current "
                            "graph. This action cannot be undone",
                            QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
    _graphs->currentGraph()->clear();
}

void GraphPerspective::deleteSelectedElements(bool fromRoot) {
  Observable::holdObservers();
  tlp::Graph *graph = _graphs->currentGraph();
  tlp::BooleanProperty *selection = graph->getProperty<BooleanProperty>("viewSelection");

  tlp::Iterator<edge> *itEdges = new StableIterator<edge>(selection->getEdgesEqualTo(true, graph));
  bool hasPush = itEdges->hasNext();

  if (hasPush) {
    graph->push();
    graph->delEdges(itEdges, fromRoot);
  }

  delete itEdges;

  tlp::Iterator<node> *itNodes = new StableIterator<node>(selection->getNodesEqualTo(true, graph));

  if (itNodes->hasNext()) {
    if (!hasPush)
      graph->push();

    graph->delNodes(itNodes, fromRoot);
  }

  delete itNodes;

  Observable::unholdObservers();
}

void GraphPerspective::invertSelection() {
  Observable::holdObservers();
  tlp::Graph *graph = _graphs->currentGraph();
  tlp::BooleanProperty *selection = graph->getProperty<BooleanProperty>("viewSelection");
  graph->push();
  selection->reverse(graph);
  Observable::unholdObservers();
}

void GraphPerspective::reverseSelectedEdges() {
  Observable::holdObservers();
  tlp::Graph *graph = _graphs->currentGraph();
  tlp::BooleanProperty *selection = graph->getProperty<BooleanProperty>("viewSelection");
  graph->push();
  selection->reverseEdgeDirection(graph);
  graph->popIfNoUpdates();
  Observable::unholdObservers();
}

void GraphPerspective::cancelSelection() {
  Observable::holdObservers();
  tlp::Graph *graph = _graphs->currentGraph();
  tlp::BooleanProperty *selection = graph->getProperty<BooleanProperty>("viewSelection");
  graph->push();
  node n;
  forEach(n, selection->getNodesEqualTo(true, graph)) {
    selection->setNodeValue(n, false);
  }
  edge e;
  forEach(e, selection->getEdgesEqualTo(true, graph)) {
    selection->setEdgeValue(e, false);
  }
  graph->popIfNoUpdates();
  Observable::unholdObservers();
}

void GraphPerspective::selectAll(bool nodes, bool edges) {
  Observable::holdObservers();
  tlp::Graph *graph = _graphs->currentGraph();
  tlp::BooleanProperty *selection = graph->getProperty<BooleanProperty>("viewSelection");
  graph->push();
  selection->setAllNodeValue(false);
  selection->setAllEdgeValue(false);

  if (nodes) {
    selection->setValueToGraphNodes(true, graph);
  }

  if (edges) {
    selection->setValueToGraphEdges(true, graph);
  }

  Observable::unholdObservers();
}

void GraphPerspective::selectAllEdges() {
  selectAll(false, true);
}

void GraphPerspective::selectAllNodes() {
  selectAll(true, false);
}

void GraphPerspective::undo() {
  Observable::holdObservers();
  tlp::Graph *graph = _graphs->currentGraph();

  if (graph != nullptr)
    graph->pop();

  Observable::unholdObservers();

  foreach (View *v, _ui->workspace->panels()) {
    if (v->graph() == graph)
      v->undoCallback();
  }
}

void GraphPerspective::redo() {
  Observable::holdObservers();
  tlp::Graph *graph = _graphs->currentGraph();

  if (graph != nullptr)
    graph->unpop();

  Observable::unholdObservers();

  foreach (View *v, _ui->workspace->panels()) {
    if (v->graph() == graph)
      v->undoCallback();
  }
}

void GraphPerspective::cut() {
  copy(_graphs->currentGraph(), true);
}

void GraphPerspective::paste() {
  if (_graphs->currentGraph() == nullptr)
    return;

  Graph *outGraph = _graphs->currentGraph();
  std::stringstream ss;
  ss << QStringToTlpString(QApplication::clipboard()->text());

  Observable::holdObservers();
  outGraph->push();
  DataSet data;
  data.set("file::data", ss.str());
  Graph *inGraph = tlp::importGraph("TLP Import", data);
  tlp::copyToGraph(outGraph, inGraph);
  outGraph->popIfNoUpdates();
  delete inGraph;
  Observable::unholdObservers();
  centerPanelsForGraph(outGraph);
}

void GraphPerspective::copy() {
  copy(_graphs->currentGraph());
}

void GraphPerspective::copy(Graph *g, bool deleteAfter) {
  if (g == nullptr)
    return;

  Observable::holdObservers();
  g->push();

  BooleanProperty *selection = g->getProperty<BooleanProperty>("viewSelection");

  Graph *copyGraph = tlp::newGraph();
  tlp::copyToGraph(copyGraph, g, selection);

  std::stringstream ss;
  DataSet data;
  tlp::exportGraph(copyGraph, ss, "TLP Export", data);
  QApplication::clipboard()->setText(tlpStringToQString(ss.str()));

  if (deleteAfter) {
    tlp::node n;
    stableForEach(n, selection->getNodesEqualTo(true)) g->delNode(n);
  }

  g->popIfNoUpdates();

  delete copyGraph;

  Observable::unholdObservers();
}

void GraphPerspective::group() {
  Observable::holdObservers();
  tlp::Graph *graph = _graphs->currentGraph();
  tlp::BooleanProperty *selection = graph->getProperty<BooleanProperty>("viewSelection");
  std::vector<node> groupedNodes;
  node n;
  forEach(n, selection->getNodesEqualTo(true)) {
    if (graph->isElement(n))
      groupedNodes.push_back(n);
  }

  if (groupedNodes.empty()) {
    Observable::unholdObservers();
    qCritical() << "[Group] Cannot create meta-nodes from empty selection";
    return;
  }

  graph->push();

  bool changeGraph = false;

  if (graph == graph->getRoot()) {
    qWarning() << "[Group] Grouping can not be done on the root graph. A subgraph has "
                  "automatically been created";
    graph = graph->addCloneSubGraph("groups");
    changeGraph = true;
  }

  graph->createMetaNode(groupedNodes, false);

  selection->setAllNodeValue(false);
  selection->setAllEdgeValue(false);

  Observable::unholdObservers();

  if (!changeGraph)
    return;

  foreach (View *v, _ui->workspace->panels()) {
    if (v->graph() == graph->getRoot())
      v->setGraph(graph);
  }
}

void GraphPerspective::make_graph() {
  Graph *graph = _graphs->currentGraph();
  unsigned int added = makeSelectionGraph(_graphs->currentGraph(),
                                          graph->getProperty<BooleanProperty>("viewSelection"));
  stringstream msg;
  msg << "Make selection a graph: " << added << " elements added to the selection.";
  Perspective::showStatusMessage(msg.str());
}

Graph *GraphPerspective::createSubGraph(Graph *graph) {
  if (graph == nullptr)
    return nullptr;

  graph->push();
  Observable::holdObservers();
  BooleanProperty *selection = graph->getProperty<BooleanProperty>("viewSelection");
  makeSelectionGraph(graph, selection);
  Graph *result = graph->addSubGraph(selection, "selection sub-graph");
  Observable::unholdObservers();
  return result;
}

void GraphPerspective::createSubGraph() {
  createSubGraph(_graphs->currentGraph());
}

void GraphPerspective::cloneSubGraph() {
  if (_graphs->currentGraph() == nullptr)
    return;

  tlp::BooleanProperty prop(_graphs->currentGraph());
  prop.setAllNodeValue(true);
  prop.setAllEdgeValue(true);
  _graphs->currentGraph()->push();
  _graphs->currentGraph()->addSubGraph(&prop, "clone sub-graph");
}

void GraphPerspective::addEmptySubGraph() {
  if (_graphs->currentGraph() == nullptr)
    return;

  _graphs->currentGraph()->push();
  _graphs->currentGraph()->addSubGraph(nullptr, "empty sub-graph");
}

void GraphPerspective::currentGraphChanged(Graph *graph) {
  bool enabled(graph != nullptr);
  _ui->actionUndo->setEnabled(enabled);
  _ui->actionRedo->setEnabled(enabled);
  _ui->actionCut->setEnabled(enabled);
  _ui->actionCopy->setEnabled(enabled);
  _ui->actionPaste->setEnabled(enabled);
  _ui->actionDelete->setEnabled(enabled);
  _ui->actionDelete_from_the_root_graph->setEnabled(enabled && (graph != graph->getRoot()));
  _ui->actionDelete_all->setEnabled(enabled);
  _ui->actionInvert_selection->setEnabled(enabled);
  _ui->actionReverse_selected_edges->setEnabled(enabled);
  _ui->actionSelect_All->setEnabled(enabled);
  _ui->actionSelect_All_Nodes->setEnabled(enabled);
  _ui->actionSelect_All_Edges->setEnabled(enabled);
  _ui->actionCancel_selection->setEnabled(enabled);
  _ui->actionMake_selection_a_graph->setEnabled(enabled);
  _ui->actionGroup_elements->setEnabled(enabled);
  _ui->actionCreate_sub_graph->setEnabled(enabled);
  _ui->actionCreate_empty_sub_graph->setEnabled(enabled);
  _ui->actionClone_sub_graph->setEnabled(enabled);
  _ui->actionExport->setEnabled(enabled);
  _ui->singleModeButton->setEnabled(enabled);
  _ui->splitModeButton->setEnabled(enabled);
  _ui->splitHorizontalModeButton->setEnabled(enabled);
  _ui->split3ModeButton->setEnabled(enabled);
  _ui->split32ModeButton->setEnabled(enabled);
  _ui->split33ModeButton->setEnabled(enabled);
  _ui->gridModeButton->setEnabled(enabled);
  _ui->sixModeButton->setEnabled(enabled);
  _ui->exposeModeButton->setEnabled(enabled);
  _ui->searchButton->setEnabled(enabled);
  _ui->pythonButton->setEnabled(enabled);
  _ui->exportButton->setEnabled(enabled);
  _ui->previousPageButton->setVisible(enabled);
  _ui->pageCountLabel->setVisible(enabled);
  _ui->nextPageButton->setVisible(enabled);
  _ui->actionSave_Project->setEnabled(enabled);
  _ui->actionSave_Project_as->setEnabled(enabled);

  if (graph == nullptr) {
    _ui->workspace->switchToStartupMode();
    _ui->exposeModeButton->setChecked(false);
    _ui->searchButton->setChecked(false);
    _ui->pythonButton->setChecked(false);
    setSearchOutput(false);
    _ui->actionSave_Project->setEnabled(false);
    _ui->actionSave_Project_as->setEnabled(false);
  } else {
    _ui->workspace->setGraphForFocusedPanel(graph);
  }

#ifdef TULIP_BUILD_PYTHON_COMPONENTS

  if (_graphs->empty()) {
    _pythonIDE->clearPythonCodeEditors();
    _pythonIDEDialog->hide();
    _ui->developButton->setEnabled(false);
    _ui->actionPython_IDE->setEnabled(false);
  } else {
    _ui->developButton->setEnabled(true);
    _ui->actionPython_IDE->setEnabled(true);
  }

#endif
}

void GraphPerspective::CSVImport() {
  bool mustDeleteGraph = false;

  if (_graphs->empty()) {
    _graphs->addGraph(tlp::newGraph());
    mustDeleteGraph = true;
  }

  Graph *g = _graphs->currentGraph();

  if (g == nullptr)
    return;

  CSVImportWizard wizard(_mainWindow);

  if (mustDeleteGraph) {
    wizard.setWindowTitle("Import CSV data into a new graph");
    wizard.setButtonText(QWizard::FinishButton, QString("Import into a new graph"));
  } else {
    wizard.setWindowTitle(QString("Import CSV data into current graph: ") + g->getName().c_str());
    wizard.setButtonText(QWizard::FinishButton, QString("Import into current graph"));
  }

  // get the number of line displayed in the logger
  unsigned int nbLogsBefore = _logger->countByType(GraphPerspectiveLogger::Error);
  nbLogsBefore += _logger->countByType(GraphPerspectiveLogger::Warning);

  wizard.setGraph(g);

  tlp::ObserverHolder oh;

  g->push();
  int result = wizard.exec();

  if (result == QDialog::Rejected) {
    if (mustDeleteGraph) {
      _graphs->removeGraph(g);
      delete g;
    } else {
      g->pop();
    }

    return;
  } else {
    unsigned int nbLogsAfter = _logger->countByType(GraphPerspectiveLogger::Error);
    nbLogsAfter += _logger->countByType(GraphPerspectiveLogger::Warning);
    applyRandomLayout(g);
    bool openPanels = true;

    foreach (View *v, _ui->workspace->panels()) {
      if (v->graph() == g) {
        openPanels = false;
        break;
      }
    }

    if (openPanels)
      showStartPanels(g);
    else
      centerPanelsForGraph(g);

    if ((nbLogsAfter != nbLogsBefore) &&
        (QMessageBox::question(_mainWindow, "CSV Parse errors found",
                               QString("When parsing your CSV file,<br/> %1 error(s) has been "
                                       "encountered.<br/>Do you want to see them?")
                                   .arg(nbLogsAfter - nbLogsBefore),
                               QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes))
      showLogger();

    g->popIfNoUpdates();
  }
}

void GraphPerspective::showStartPanels(Graph *g) {
  if (TulipSettings::instance().displayDefaultViews() == false)
    return;

  // expose mode is not safe to add a new panel
  // so hide it if needed
  _ui->workspace->hideExposeMode();
  View *firstPanel = nullptr;
  View *secondPanel = nullptr;

  foreach (const QString &panelName, QStringList() << "Spreadsheet view"
                                                   << "Node Link Diagram view") {
    View *view =
        PluginLister::instance()->getPluginObject<View>(QStringToTlpString(panelName), nullptr);

    if (firstPanel == nullptr)
      firstPanel = view;
    else
      secondPanel = view;

    view->setupUi();
    view->setGraph(g);
    view->setState(DataSet());
    _ui->workspace->addPanel(view);
  }

  _ui->workspace->setActivePanel(firstPanel);
  _ui->workspace->switchToSplitMode();
  secondPanel->centerView(false);
}

void GraphPerspective::applyRandomLayout(Graph *g) {
  Observable::holdObservers();
  LayoutProperty *viewLayout = g->getProperty<LayoutProperty>("viewLayout");

  if (viewLayout->numberOfNonDefaultValuatedNodes(g) == 0) {
    std::string str;
    g->applyPropertyAlgorithm("Random layout", viewLayout, str);
  }

  Observable::unholdObservers();
}

void GraphPerspective::centerPanelsForGraph(tlp::Graph *g, bool graphChanged, bool onlyGlMainView) {
  foreach (View *v, _ui->workspace->panels()) {
    if ((v->graph() == g) && (!onlyGlMainView || dynamic_cast<tlp::GlMainView *>(v)))
      v->centerView(graphChanged);
  }
}

void GraphPerspective::closePanelsForGraph(tlp::Graph *g) {
  list<View *> viewsToDelete;

  foreach (View *v, _ui->workspace->panels()) {
    if (v->graph() == g || g->isDescendantGraph(v->graph()))
      viewsToDelete.push_back(v);
  }

  if (!viewsToDelete.empty()) {
    // expose mode is not safe when deleting a panel
    // so hide it first
    _ui->workspace->hideExposeMode();
    qDeleteAll(viewsToDelete);
  }
}

bool GraphPerspective::setGlMainViewPropertiesForGraph(
    tlp::Graph *g, const std::map<std::string, tlp::PropertyInterface *> &propsMap) {
  bool result = false;

  foreach (View *v, _ui->workspace->panels()) {
    GlMainView *glMainView = dynamic_cast<tlp::GlMainView *>(v);

    if (v->graph() == g && glMainView != nullptr) {
      if (glMainView->getGlMainWidget()
              ->getScene()
              ->getGlGraphComposite()
              ->getInputData()
              ->installProperties(propsMap))
        result = true;
    }
  }

  return result;
}

void GraphPerspective::setSearchOutput(bool f) {
  if (f) {
    _ui->outputFrame->setCurrentWidget(_ui->searchPanel);
    _ui->pythonButton->setChecked(false);
  }

  _ui->outputFrame->setVisible(f);
}

void GraphPerspective::setPythonPanel(bool f) {
  if (f) {
    _ui->outputFrame->setCurrentWidget(_ui->pythonPanel);
    _ui->searchButton->setChecked(false);
  }

  _ui->outputFrame->setVisible(f);
}

void GraphPerspective::openPreferences() {
  PreferencesDialog dlg(_ui->mainWidget);
  dlg.readSettings();

  if (dlg.exec() == QDialog::Accepted) {
    dlg.writeSettings();

    foreach (tlp::View *v, _ui->workspace->panels()) {
      GlMainView *glMainView = dynamic_cast<tlp::GlMainView *>(v);

      if (glMainView != nullptr) {
        if (glMainView->getGlMainWidget() != nullptr) {
          glMainView->getGlMainWidget()
              ->getScene()
              ->getGlGraphComposite()
              ->getRenderingParametersPointer()
              ->setSelectionColor(TulipSettings::instance().defaultSelectionColor());
          glMainView->redraw();
        }
      }
    }
  }
}

void GraphPerspective::setAutoCenterPanelsOnDraw(bool f) {
  _ui->workspace->setAutoCenterPanelsOnDraw(f);
}

void GraphPerspective::pluginsListChanged() {
  _ui->algorithmRunner->refreshPluginsList();
}

void GraphPerspective::addNewGraph() {
  Graph *g = tlp::newGraph();
  _graphs->addGraph(g);
  showStartPanels(g);
}

void GraphPerspective::newProject() {
  createPerspective(name().c_str());
}

void GraphPerspective::openRecentFile() {
  QAction *action = static_cast<QAction *>(sender());
  open(action->data().toString());
}

void GraphPerspective::treatEvent(const tlp::Event &ev) {
  if (dynamic_cast<const tlp::PluginEvent *>(&ev)) {
    pluginsListChanged();
  }
}

void GraphPerspective::showPythonIDE() {
#ifdef TULIP_BUILD_PYTHON_COMPONENTS
  _pythonIDEDialog->show();
  _pythonIDEDialog->raise();
#endif
}

void GraphPerspective::showUserDocumentation() {
  QDesktopServices::openUrl(QUrl::fromLocalFile(tlpStringToQString(tlp::TulipShareDir) +
                                                "../doc/tulip/tulip-user/html/index.html"));
}

void GraphPerspective::showDevelDocumentation() {
  QDesktopServices::openUrl(QUrl::fromLocalFile(tlpStringToQString(tlp::TulipShareDir) +
                                                "../doc/tulip/tulip-dev/html/index.html"));
}

void GraphPerspective::showPythonDocumentation() {
  QDesktopServices::openUrl(QUrl::fromLocalFile(tlpStringToQString(tlp::TulipShareDir) +
                                                "../doc/tulip/tulip-python/html/index.html"));
}

void GraphPerspective::showAPIDocumentation() {
  QDesktopServices::openUrl(QUrl::fromLocalFile(tlpStringToQString(tlp::TulipShareDir) +
                                                "../doc/tulip/doxygen/html/index.html"));
}

void GraphPerspective::showHideSideBar() {
  if (_ui->docksWidget->isVisible()) {
    _ui->docksWidget->setVisible(false);
    SET_TIPS(_ui->sidebarButton, "Show Sidebar");
  } else {
    _ui->docksWidget->setVisible(true);
    SET_TIPS(_ui->sidebarButton, "Hide Sidebar");
  }

  if (_logger->anchored()) {
    resetLoggerDialogPosition();
  }
}

void GraphPerspective::showHideStatusBar() {
  QStatusBar *stsBar = _mainWindow->statusBar();

  if (stsBar->isVisible()) {
    stsBar->setVisible(false);
    SET_TIPS(_ui->statusbarButton, "Show Status bar");
  } else {
    stsBar->setVisible(true);
    SET_TIPS(_ui->statusbarButton, "Hide Status bar");
  }

  TulipSettings::instance().setShowStatusBar(stsBar->isVisible());
}

void GraphPerspective::displayColorScalesDialog() {
  _colorScalesDialog->show();
}

void GraphPerspective::showAboutTulipPage() {
  if (!checkSocketConnected()) {
    tlp::AboutTulipPage *aboutPage = new tlp::AboutTulipPage;
    QDialog aboutDialog(mainWindow(), Qt::Window);
    aboutDialog.setWindowTitle("About Tulip");
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(aboutPage);
    layout->setContentsMargins(0, 0, 0, 0);
    aboutDialog.setLayout(layout);
    aboutDialog.resize(800, 600);
    aboutDialog.exec();
  }
}

void GraphPerspective::workspaceButtonClicked() {
  _ui->workspaceButton->setChecked(true);
}

void GraphPerspective::resetLoggerDialogPosition() {
  QPoint pos = _mainWindow->mapToGlobal(_ui->exportButton->pos());
  pos.setX(pos.x() + _ui->loggerFrame->width());

  if (_logger->isVisible()) {
    _logger->showNormal();
  }

  // extend the logger frame width until reaching the right side of the main window
  _logger->setGeometry(
      pos.x(), pos.y(), _mainWindow->width() - _ui->loggerFrame->width(),
      _mainWindow->mapToGlobal(QPoint(0, 0)).y() + _mainWindow->height() - pos.y() - 2 -
          (_mainWindow->statusBar()->isVisible() ? _mainWindow->statusBar()->height() : 0));
}

PLUGIN(GraphPerspective)
