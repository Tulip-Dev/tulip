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
#include <tulip/PythonCodeEditor.h>
#endif

#include <list>
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
#include <tulip/TreeTest.h>

#include "ui_GraphPerspectiveMainWindow.h"

#include "GraphPerspectiveLogger.h"
#include "ImportWizard.h"
#include "ExportWizard.h"
#include "PanelSelectionWizard.h"
#include "GraphHierarchiesEditor.h"
#include "PreferencesDialog.h"
#include "SearchWidget.h"

#include <QDebug>

using namespace tlp;
using namespace std;

// checks if it exists a Tulip import plugin that can load the provided file based on its extension
static bool tulipCanOpenFile(const QString &path) {
  // Tulip project file does not use import / export plugin
  if (path.endsWith(".tlpx")) {
    return true;
  }

  std::list<std::string> imports = PluginLister::availablePlugins<ImportModule>();

  for (auto &import : imports) {
    ImportModule *m = PluginLister::getPluginObject<ImportModule>(import);
    std::list<std::string> fileExtensions(m->allFileExtensions());

    for (auto &ext : fileExtensions) {
      if (path.endsWith(tlpStringToQString(ext))) {
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
      _recentDocumentsSettingsKey("perspective/recent_files"), _logger(nullptr),
      _searchDialog(nullptr) {
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
  _pythonPanel = nullptr;
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

  for (const QString &s : TulipSettings::instance().recentDocuments()) {
    if (!QFileInfo(s).exists() || !tulipCanOpenFile(s))
      continue;

    QAction *action = _ui->menuOpen_recent_file->addAction(
        QIcon(":/tulip/graphperspective/icons/16/archive.png"), s, this, SLOT(openRecentFile()));
    action->setData(s);
  }

  _ui->menuOpen_recent_file->addSeparator();

  for (const QString &s :
       TulipSettings::instance().value(_recentDocumentsSettingsKey).toStringList()) {
    if (!QFileInfo(s).exists() || !tulipCanOpenFile(s))
      continue;

    QAction *action = _ui->menuOpen_recent_file->addAction(
        QIcon(":/tulip/graphperspective/icons/16/empty-file.png"), s, this, SLOT(openRecentFile()));
    action->setData(s);
  }
  _ui->menuOpen_recent_file->setEnabled(!_ui->menuOpen_recent_file->isEmpty());
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

#define SET_TIPS(a, tt)                                                                            \
  a->setToolTip(tt);                                                                               \
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

  logIconCounterFrame->setVisible(_logger->countByType(logType) != 0);
  logIconLabel->setPixmap(_logger->icon(logType));
  SET_TIPS(logIconLabel, "Click here to show/hide the message log window");
  logCounterLabel->setText(QString::number(_logger->countByType(logType)));
  SET_TIPS(logCounterLabel, "Click here to show/hide the message log window");
}

void GraphPerspective::logMessage(QtMsgType type, const QMessageLogContext &context,
                                  const QString &msg) {
  if (type == QtFatalMsg) {
    std::cerr << tlp::QStringToTlpString(msg) << std::endl;
    abort();
  }

  if (type == QtWarningMsg && !tlp::isWarningOutputEnabled())
    return;

  // on some windows systems
  // "No errors." messages may be logged coming from QGLShader::link
  // we try to hide them
  if (msg.indexOf("No errors.") != -1 || msg.isEmpty())
    return;

#ifdef __APPLE__
  // hide numerous messages generated by geographic view
  if (msg.indexOf("QMacCGContext:: Unsupported painter ") != -1)
    return;
#endif

  // redirect msg to standard output
  if (msg.startsWith("[Python")) {
    // remove quotes around message added by Qt
    QString msgClean(msg.mid(14).mid(2, msg.length() - 17));
    if (msg.startsWith("[PythonStdOut]"))
      std::cout << QStringToTlpString(msgClean) << std::endl;
    else
      std::cerr << QStringToTlpString(msgClean) << std::endl;
    _logger->log(type, context, msgClean, true);
  } else {
    if (type < QtCriticalMsg)
      std::cout << QStringToTlpString(msg) << std::endl;
    else
      std::cerr << QStringToTlpString(msg) << std::endl;
    // redirect to GraphPerpectiveLogger
    _logger->log(type, context, msg, false);
  }

  updateLogIconsAndCounters();
}

GraphPerspective::~GraphPerspective() {
  // uninstall Qt message handler only if it is the current active perspective
  if (Perspective::instance() == this) {
    qInstallMessageHandler(nullptr);
  }

  // ensure all loaded graphs are deleted
  for (auto graph : _graphs->graphs()) {
    delete graph;
  }

#ifdef TULIP_BUILD_PYTHON_COMPONENTS
  delete _pythonIDEDialog;
  if (Perspective::instance() == this) {
    PythonCodeEditor::deleteStaticResources();
  }
#endif

  delete _searchDialog;
  delete _ui;
}

void GraphPerspective::destroyWorkspace() {
  // disconnect to avoid any possible segfaults when deleting graphs
  disconnect(_graphs, SIGNAL(currentGraphChanged(tlp::Graph *)), this,
             SLOT(currentGraphChanged(tlp::Graph *)));

  // delete the workspace, which causes views deletion, before the graphs
  // to avoid any possible segfaults when closing Tulip
  if (_ui) {
    delete _ui->workspace;
    _ui->workspace = nullptr;
    // more disconnection
    disconnect(_graphs, SIGNAL(currentGraphChanged(tlp::Graph *)), _ui->algorithmRunner,
               SLOT(setGraph(tlp::Graph *)));
  }
}

bool GraphPerspective::terminated() {

#ifdef TULIP_BUILD_PYTHON_COMPONENTS
  if (_pythonIDE) {
    _pythonIDE->savePythonFilesAndWriteToProject(true);
    _pythonIDEDialog->hide();
  }
#endif

  if (_graphs->needsSaving() || mainWindow()->isWindowModified()) {
#ifdef TULIP_BUILD_PYTHON_COMPONENTS
    QString message("The project has been modified (loaded graphs or Python files opened in the "
                    "IDE).\nDo you want to save your changes?");
#else
    QString message("The project has been modified.\nDo you want to save your changes?");
#endif
    QMessageBox::StandardButton answer = QMessageBox::question(
        _mainWindow, "Save", message,
        QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel | QMessageBox::Escape);

    if ((answer == QMessageBox::Yes && !save()) || (answer == QMessageBox::Cancel)) {
      return false;
    }
  }

  // force workspace and views destruction here to avoid hanging on exit
  // when linking against QtWebEngine binaries provided by qt.io
  destroyWorkspace();
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

bool GraphPerspective::eventFilter(QObject *obj, QEvent *ev) {
  if (ev->type() == QEvent::DragEnter) {
    QDragEnterEvent *dragEvent = static_cast<QDragEnterEvent *>(ev);

    if (dragEvent->mimeData()->hasUrls()) {
      dragEvent->accept();
    }
  }

  if (ev->type() == QEvent::Drop) {
    QDropEvent *dropEvent = static_cast<QDropEvent *>(ev);

    for (const QUrl &url : dropEvent->mimeData()->urls()) {
      open(url.toLocalFile());
    }
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

void GraphPerspective::displayLogMessages() {
  _logger->show();

  static bool firstTime = true;

  if (firstTime) {
    resetLoggerDialogPosition();
    firstTime = false;
  }
}

void GraphPerspective::showHideLogger() {

  if (!_logger->isVisible()) {
    displayLogMessages();
  } else {
    _logger->hide();
  }
}

void GraphPerspective::redrawPanels(bool center) {
  _ui->workspace->redrawPanels(center);
}

class GraphPerspectiveDialog : public QDialog {

  QMainWindow *_mainWindow;
  QByteArray _windowGeometry;
  bool _mainWindowHidden;
  std::list<QDialog *> childrenToShow;

public:
  GraphPerspectiveDialog(QString title)
      : QDialog(nullptr, Qt::Tool | Qt::CustomizeWindowHint | Qt::WindowTitleHint |
                             Qt::WindowCloseButtonHint),
        _mainWindow(Perspective::instance()->mainWindow()), _mainWindowHidden(false) {
    setStyleSheet(_mainWindow->styleSheet());
    setWindowIcon(_mainWindow->windowIcon());
    QString tlpTitle("Tulip ");

    // show patch number only if needed
    if (TULIP_INT_VERSION % 10)
      tlpTitle += TULIP_VERSION;
    else
      tlpTitle += TULIP_MM_VERSION;

    setWindowTitle(tlpTitle.append(" - %1").arg(title));
    _mainWindow->installEventFilter(this);
  }

  void hideEvent(QHideEvent *) override {
    if (!_mainWindowHidden) {
      for (auto child : findChildren<QDialog *>()) {
        if (!child->isHidden()) {
          child->reject();
        }
      }
    }
  }

  bool eventFilter(QObject *, QEvent *event) override {
    if (event->type() == QEvent::Hide && !isHidden() && _mainWindow->isMinimized()) {
      _mainWindowHidden = true;
      _windowGeometry = saveGeometry();
      // ensure visible children dialogs
      // will be hidden
      childrenToShow.clear();
      for (auto child : findChildren<QDialog *>()) {
        if (!child->isHidden()) {
          // only non modal will be re-displayed
          if (!child->isModal())
            childrenToShow.push_back(child);
          child->reject();
        }
      }
      // hide current dialog
      hide();
      return true;
    } else if (event->type() == QEvent::Show && _mainWindowHidden) {
      _mainWindowHidden = false;
      // show current dialog
      show();
      // re-display non modal children
      for (auto child : childrenToShow)
        child->show();
      return true;
    }
    return false;
  }

protected:
  void showEvent(QShowEvent *e) override {
    QDialog::showEvent(e);
    if (!_windowGeometry.isEmpty()) {
      restoreGeometry(_windowGeometry);
    }
  }

  void closeEvent(QCloseEvent *e) override {
    _windowGeometry = saveGeometry();
    QDialog::closeEvent(e);
  }
};

#define SET_TOOLTIP(a, tt) a->setToolTip(QString(tt))

void GraphPerspective::buildPythonIDE() {
#ifdef TULIP_BUILD_PYTHON_COMPONENTS
  if (_pythonIDE == nullptr) {
    _pythonIDE = new PythonIDE();
    _pythonIDE->setGraphsModel(_graphs);
    QVBoxLayout *dialogLayout = new QVBoxLayout();
    dialogLayout->addWidget(_pythonIDE);
    dialogLayout->setContentsMargins(0, 0, 0, 0);
    _pythonIDEDialog = new GraphPerspectiveDialog("Python IDE");
    _pythonIDEDialog->setLayout(dialogLayout);
    _pythonIDEDialog->resize(800, 600);
  }
#endif
}

void GraphPerspective::start(tlp::PluginProgress *progress) {
  reserveDefaultProperties();
  _ui = new Ui::GraphPerspectiveMainWindowData;
  _ui->setupUi(_mainWindow);
#ifdef TULIP_BUILD_PYTHON_COMPONENTS
  // ensure the loading of python plugins
  // before instantiating PythonIDE
  PythonInterpreter::getInstance();
  pluginsListChanged();
#else
  _ui->developButton->setVisible(false);
  _ui->actionPython_IDE->setVisible(false);
#endif
  currentGraphChanged(nullptr);
  // set win/Mac dependent tooltips with ctrl shortcut
  SET_TIPS_WITH_CTRL_SHORTCUT(_ui->previousPageButton, "Show previous panel", "Shift+Left");
  SET_TIPS_WITH_CTRL_SHORTCUT(_ui->nextPageButton, "Show next panel", "Shift+Right");
  SET_TOOLTIP_WITH_CTRL_SHORTCUT(_ui->actionNewProject, "Open a new empty Tulip perspective",
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
  SET_TOOLTIP_WITH_CTRL_SHORTCUT(_ui->actionRedo,
                                 "Redo the latest undone update of the current graph", "Y");
  SET_TOOLTIP_WITH_CTRL_SHORTCUT(_ui->actionCut,
                                 "Move the selected elements of the current graph into the "
                                 "clipboard (the selected edges ends are selected too)",
                                 "X");
  SET_TOOLTIP_WITH_CTRL_SHORTCUT(_ui->actionCopy,
                                 "Copy the selected elements of the current graph into the "
                                 "clipboard (the selected edges ends are selected too)",
                                 "C");
  SET_TOOLTIP_WITH_CTRL_SHORTCUT(_ui->actionPaste,
                                 "Paste the clipboard elements into the current graph", "V");
  SET_TOOLTIP_WITH_CTRL_SHORTCUT(_ui->actionSelect_All, "Select all elements of the current graph",
                                 "A");
  SET_TOOLTIP_WITH_CTRL_SHORTCUT(_ui->actionInvert_selection,
                                 "Invert the selection of the current "
                                 "graph elements, deselect if "
                                 "selected and select if not selected",
                                 "I");
  SET_TOOLTIP_WITH_CTRL_SHORTCUT(_ui->actionCancel_selection,
                                 "Deselect all selected elements of the current graph", "Shift+A");
  SET_TOOLTIP_WITH_CTRL_SHORTCUT(_ui->actionGroup_elements,
                                 "Create a meta-node representing a newly created subgraph "
                                 "containing all selected elements of the current graph",
                                 "G");
  SET_TOOLTIP_WITH_CTRL_SHORTCUT(
      _ui->actionCreate_sub_graph,
      "Create a subgraph containing all selected elements of the current graph", "Shift+G");
  SET_TOOLTIP_WITH_CTRL_SHORTCUT(_ui->actionPreferences, "Show Tulip preferences dialog", ",");
  SET_TOOLTIP_WITH_CTRL_SHORTCUT(_ui->actionShowUserDocumentation,
                                 "Display the User handbook in a navigator", "?");
  SET_TOOLTIP_WITH_CTRL_SHORTCUT(_ui->actionShowDevelDocumentation,
                                 "Display the Developer handbook in a navigator", "D");
  SET_TOOLTIP_WITH_CTRL_SHORTCUT(_ui->actionShowPythonDocumentation,
                                 "Display the Tulip python documentation in a navigator", "P");
  SET_TOOLTIP_WITH_CTRL_SHORTCUT(_ui->actionMessages_log, "Show the message log", "M");
  SET_TOOLTIP_WITH_CTRL_SHORTCUT(_ui->actionPython_IDE,
                                 "Display the Tulip Python IDE for developing scripts and plugins "
                                 "to execute on the loaded graphs",
                                 "Alt+P");
  SET_TOOLTIP_WITH_CTRL_SHORTCUT(_ui->actionOpen_Project, "Open a graph file", "O");
  SET_TOOLTIP_WITH_CTRL_SHORTCUT(_ui->actionNew_graph, "Create a new empty graph", "N");
  SET_TOOLTIP_WITH_CTRL_SHORTCUT(_ui->actionExposePanels,
                                 "Toggle the 'Expose' all visualisation panels mode", "E");
  SET_TIPS_WITH_CTRL_SHORTCUT(_ui->actionSearch, "Display the graph's elements search dialog", "F");

  // set portable tooltips
  SET_TIPS(_ui->undoButton, _ui->actionUndo->toolTip());
  SET_TIPS(_ui->redoButton, _ui->actionRedo->toolTip());
  SET_TIPS(_ui->exposePanelsButton, _ui->actionExposePanels->toolTip());
  SET_TIPS(_ui->searchButton, _ui->actionSearch->toolTip());
  SET_TIPS(_ui->developButton, _ui->actionPython_IDE->toolTip());
  _ui->loggerMessageInfo->setToolTip(QString("Show/Hide the Messages log panel"));
  _ui->loggerMessagePython->setToolTip(_ui->loggerMessageInfo->toolTip());
  _ui->loggerMessageWarning->setToolTip(_ui->loggerMessageInfo->toolTip());
  _ui->loggerMessageError->setToolTip(_ui->loggerMessageInfo->toolTip());
  SET_TIPS(_ui->csvImportButton, "Import data in the current graph using a csv formatted file");
  SET_TIPS(_ui->importButton, "Display the Graph importing wizard");
  SET_TIPS(_ui->pluginsButton, "Display the Plugin center");
  SET_TIPS(_ui->sidebarButton, "Hide Sidebar");
  SET_TIPS(_ui->menubarButton, "Hide Menubar");
  SET_TIPS(_ui->singleModeButton, "Switch to 1 panel mode");
  SET_TIPS(_ui->splitModeButton, "Switch to 2 panels mode");
  SET_TIPS(_ui->splitHorizontalModeButton, "Switch to 2 panels mode");
  SET_TIPS(_ui->split3ModeButton, "Switch to 2-top 1-bottom panels mode");
  SET_TIPS(_ui->split32ModeButton, "Switch to 1-left 2-right panels mode");
  SET_TIPS(_ui->split33ModeButton, "Switch to 2-left 1-right panels mode");
  SET_TIPS(_ui->gridModeButton, "Switch to 4 panels mode");
  SET_TIPS(_ui->sixModeButton, "Switch to 6 panels mode");
  SET_TIPS(_ui->menuDelete, "Delete elements of the current graph");
  _ui->menuOpen_recent_file->setToolTip(
      QString("Choose a file to open among the recently opened/saved graphs or projects"));
  SET_TOOLTIP(_ui->actionDelete, "Delete the selected elements from the current graph [Del]");
  SET_TOOLTIP(_ui->actionFull_screen, "Display the Tulip perspective in full screen [F11]");
  SET_TOOLTIP(_ui->actionShow_Menubar, "Show/Hide the main menu bar [Ctrl+Shift+M]");
  SET_TOOLTIP(_ui->actionAbout_us, "Display the 'About Tulip' information dialog [F1]");
  SET_TOOLTIP(_ui->actionPlugins_Center, _ui->pluginsButton->toolTip());
  SET_TOOLTIP(_ui->actionImport_CSV, _ui->csvImportButton->toolTip());
  SET_TOOLTIP(_ui->actionSave_graph_to_file, "Write the current graph into a file");
  SET_TOOLTIP(_ui->actionCreate_empty_sub_graph, "Create an empty subgraph of the current graph");
  SET_TOOLTIP(_ui->actionClone_sub_graph,
              "Create a subgraph containing the same elements as the current graph");
  SET_TOOLTIP(_ui->action_Remove_All, "Remove all visualisation panels");
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
  _ui->workspace->setExposeModeSwitch(_ui->exposePanelsButton);
  _logger = new GraphPerspectiveLogger(_mainWindow);
  _ui->loggerFrame->installEventFilter(this);
  _mainWindow->installEventFilter(this);
  _mainWindow->setAcceptDrops(true);

  if (tlp::inGuiTestingMode()) {
    _ui->undoButton->setMinimumSize(75, 75);
    _ui->redoButton->setMinimumSize(75, 75);
    _ui->exposePanelsButton->setMinimumSize(75, 75);
    _ui->developButton->setMinimumSize(75, 75);
    _ui->csvImportButton->setMinimumSize(75, 75);
    _ui->importButton->setMinimumSize(75, 75);
    _ui->pluginsButton->setMinimumSize(75, 75);
  }

  connect(_logger, SIGNAL(cleared()), this, SLOT(logCleared()));
  connect(_logger, SIGNAL(itemRemoved()), this, SLOT(updateLogIconsAndCounters()));
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

  qInstallMessageHandler(Perspective::showLogMessage);

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
  connect(_ui->actionMessages_log, SIGNAL(triggered()), this, SLOT(displayLogMessages()));
  connect(_ui->actionPython_IDE, SIGNAL(triggered()), this, SLOT(showPythonIDE()));
  connect(_ui->actionFull_screen, SIGNAL(triggered(bool)), this, SLOT(showFullScreen(bool)));
  connect(_ui->actionImport, SIGNAL(triggered()), this, SLOT(importGraph()));
  connect(_ui->actionExport, SIGNAL(triggered()), this, SLOT(exportGraph()));
  connect(_ui->actionCreate_panel, SIGNAL(triggered()), this, SLOT(createPanel()));
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
  connect(_ui->actionNew_graph, SIGNAL(triggered()), this, SLOT(addNewGraph()));
  connect(_ui->actionNewProject, SIGNAL(triggered()), this, SLOT(newProject()));
  connect(_ui->actionPreferences, SIGNAL(triggered()), this, SLOT(openPreferences()));
  connect(_ui->actionSearch, SIGNAL(triggered()), this, SLOT(showSearchDialog()));
  connect(_ui->workspace, SIGNAL(importGraphRequest()), this, SLOT(importGraph()));
  connect(_ui->action_Remove_All, SIGNAL(triggered()), _ui->workspace, SLOT(closeAll()));
  _ui->addPanelButton->setDefaultAction(_ui->actionCreate_panel);
  connect(_ui->actionColor_scales_management, SIGNAL(triggered()), this,
          SLOT(displayColorScalesDialog()));
  _ui->exportButton->setDefaultAction(_ui->actionExport);

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
  if (tlp::inGuiTestingMode())
    _ui->mainSplitter->setSizes(QList<int>() << 350 << 850);
  else
    _ui->mainSplitter->setSizes(QList<int>() << 300 << 900);
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

#ifdef TULIP_BUILD_PYTHON_COMPONENTS
  connect(_ui->developButton, SIGNAL(clicked()), this, SLOT(showPythonIDE()));
  tlp::PluginEvent::addListener(this);
  if (_pythonIDE || PythonIDE::projectNeedsPythonIDE(_project))
    QTimer::singleShot(100, this, SLOT(initPythonIDE()));
#endif

  if (!_externalFile.isEmpty() && QFileInfo(_externalFile).exists()) {
    open(_externalFile);
  }

  for (auto h : _ui->docksSplitter->findChildren<HeaderFrame *>()) {
    connect(h, SIGNAL(expanded(bool)), this, SLOT(refreshDockExpandControls()));
  }

  connect(_ui->sidebarButton, SIGNAL(clicked()), this, SLOT(showHideSideBar()));
  connect(_ui->menubarButton, SIGNAL(clicked()), this, SLOT(showHideMenuBar()));

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

  // for former user who has never launched Tulip 5.3
  // we show a message to ask him if he wants to use
  // tlpb as default graph file format
  if (TulipSettings::instance().isFirstTulipMMRun() &&
      !TulipSettings::instance().userHasLaunchedTulipMM("5.3") &&
      !TulipSettings::instance().isFirstRun() && !TulipSettings::instance().isUseTlpbFileFormat()) {
    QTimer::singleShot(500, this, SLOT(showStartMessage()));
  }

  logCleared();
}

void GraphPerspective::showStartMessage() {
  if (QMessageBox::question(
          _mainWindow, QString("About graph file format in Tulip projects"),
          QString(
              "<html><body><p>Since Tulip 5.0, the <b>tlpb</b> (Tulip binary) file format<br/>can "
              "be choosed to save graphs in project files.<br/>This format speeds up the save/load "
              "of graphs but is not human readable.<br/>The <b>Preferences</b> dialog allows to "
              "choose this format, but you can click on <b>Apply</b>, if you want to use it as of "
              "now for the save of graphs in your project files.</p></body></html>"),
          QMessageBox::Apply | QMessageBox::Close, QMessageBox::Close) == QMessageBox::Apply)
    TulipSettings::instance().setUseTlpbFileFormat(true);
}

void GraphPerspective::openExternalFile() {
  open(_externalFile);
}

tlp::GraphHierarchiesModel *GraphPerspective::model() const {
  return _graphs;
}

void GraphPerspective::refreshDockExpandControls() {
  QList<HeaderFrame *> expandedHeaders, collapsedHeaders;

  for (auto h : _ui->docksSplitter->findChildren<HeaderFrame *>()) {
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
    QMessageBox::critical(_mainWindow, "Export error",
                          QString("<i>") + wizard.algorithm() +
                              "</i> failed to export graph.<br/><br/><b>" +
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

QAction *GraphPerspective::exportAction() {
  return _ui->actionExport;
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
      QMessageBox::critical(_mainWindow, "Save error", "Failed to save graph hierarchy");
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
      QMessageBox::critical(_mainWindow, "Import error",
                            QString("<i>") + tlp::tlpStringToQString(module) +
                                "</i> failed to import data.<br/><br/><b>" +
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

  applyDefaultLayout(g);
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

QAction *GraphPerspective::createPanelAction() {
  return _ui->actionCreate_panel;
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
    QString path = QFileDialog::getSaveFileName(_mainWindow, "Save project", QString(),
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
  if (_pythonIDE)
    _pythonIDE->savePythonFilesAndWriteToProject();
#endif
  bool ret = _project->write(path, &progress);

  if (ret)
    TulipSettings::instance().addToRecentDocuments(path);

  return ret;
}

void GraphPerspective::open(QString fileName) {
  QMap<std::string, std::string> modules;
  std::list<std::string> imports = PluginLister::availablePlugins<ImportModule>();

  std::string filters("Tulip project (*.tlpx);;");
  std::string filterAny("Any supported format (");

  for (auto &import : imports) {
    ImportModule *m = PluginLister::getPluginObject<ImportModule>(import);
    std::list<std::string> fileExtension(m->allFileExtensions());

    std::string currentFilter;

    for (auto &ext : fileExtension) {

      if (ext.empty())
        continue;

      filterAny += "*." + ext + " ";
      currentFilter += "*." + ext + " ";

      modules[ext] = import;
    }

    if (!currentFilter.empty())
      filters += import + "(" + currentFilter + ");;";

    delete m;
  }

  filterAny += " *.tlpx);;";

  filters += "All files (*)";
  filters.insert(0, filterAny);

  if (fileName.isEmpty()) // If open() was called without a parameter, open the file dialog
    fileName = QFileDialog::getOpenFileName(
        _mainWindow, tr("Open graph"), _lastOpenLocation, filters.c_str(), nullptr,
        // ensure predictable behavior
        // needed by gui tests
        inGuiTestingMode() ? QFileDialog::DontUseNativeDialog : QFileDialog::Options());

  if (!fileName.isEmpty()) {
    QFileInfo fileInfo(fileName);

    // we must ensure that choosing a file is relative to
    // the current directory to allow to run the gui tests
    // from any relative tests/gui directory
    if (!tlp::inGuiTestingMode())
      _lastOpenLocation = fileInfo.absolutePath();

    for (const std::string &extension : modules.keys()) {
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
    if (_project->openProjectFile(path, prg)) {
      QMap<QString, tlp::Graph *> rootIds = _graphs->readProject(_project, prg);
      _ui->workspace->readProject(_project, rootIds, prg);
#ifdef TULIP_BUILD_PYTHON_COMPONENTS
      if (_pythonIDE || PythonIDE::projectNeedsPythonIDE(_project))
        QTimer::singleShot(100, this, SLOT(initPythonIDE()));
#endif
    } else {
      QMessageBox::critical(_mainWindow,
                            QString("Error while loading project ").append(_project->projectFile()),
                            QString("The Tulip project file is probably corrupted:<br>") +
                                tlpStringToQString(prg->getError()));
    }
    delete prg;
  } else {
    Perspective::openProjectFile(path);
  }
}

#ifdef TULIP_BUILD_PYTHON_COMPONENTS
void GraphPerspective::initPythonIDE() {
  buildPythonIDE();
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

  vector<tlp::edge> edgesToDelete = iteratorVector(selection->getEdgesEqualTo(true, graph));
  bool hasPush = !edgesToDelete.empty();

  if (hasPush) {
    graph->push();
    graph->delEdges(edgesToDelete, fromRoot);
  }

  vector<tlp::node> nodesToDelete = iteratorVector(selection->getNodesEqualTo(true, graph));

  if (!hasPush && !nodesToDelete.empty())
    graph->push();

  graph->delNodes(nodesToDelete, fromRoot);

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
  selection->setValueToGraphNodes(false, graph);
  selection->setValueToGraphEdges(false, graph);
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

  for (auto v : _ui->workspace->panels()) {
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

  for (auto v : _ui->workspace->panels()) {
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

  BooleanProperty *selection = g->getProperty<BooleanProperty>("viewSelection");

  Graph *copyGraph = tlp::newGraph();
  tlp::copyToGraph(copyGraph, g, selection);

  if (!copyGraph->isEmpty()) {
    std::stringstream ss;
    DataSet data;
    tlp::exportGraph(copyGraph, ss, "TLP Export", data);
    delete copyGraph;
    QApplication::clipboard()->setText(tlpStringToQString(ss.str()));

    if (deleteAfter) {
      Observable::holdObservers();
      g->push();

      for (auto n : stableIterator(selection->getNonDefaultValuatedNodes(g))) {
        g->delNode(n);
      }
      Observable::unholdObservers();
    }
  }
}

void GraphPerspective::group() {
  Observable::holdObservers();
  tlp::Graph *graph = _graphs->currentGraph();
  tlp::BooleanProperty *selection = graph->getProperty<BooleanProperty>("viewSelection");
  std::vector<node> groupedNodes;
  for (auto n : selection->getNodesEqualTo(true, graph)) {
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

  for (auto v : _ui->workspace->panels()) {
    if (v->graph() == graph->getRoot())
      v->setGraph(graph);
  }
}

void GraphPerspective::make_graph() {
  Graph *graph = _graphs->currentGraph();
  unsigned int added = makeSelectionGraph(_graphs->currentGraph(),
                                          graph->getProperty<BooleanProperty>("viewSelection"));
  stringstream msg;
  msg << "Make selection a graph: ";
  if (added) {
    if (added == 1)
      msg << "one node added";
    else
      msg << added << " nodes added";
  } else
    msg << "no nodes added.";
  Perspective::showStatusMessage(msg.str());
}

Graph *GraphPerspective::createSubGraph(Graph *graph) {
  if (graph == nullptr)
    return nullptr;
  BooleanProperty *selection = graph->getProperty<BooleanProperty>("viewSelection");
  if (!selection->hasNonDefaultValuatedEdges() && !selection->hasNonDefaultValuatedNodes()) {
    if (QMessageBox::question(_mainWindow, "Empty selection",
                              "No nodes/edges selected.<br/> The new subgraph will be empty.<br/> \
                               Do you want to continue?",
                              QMessageBox::Yes | QMessageBox::No) == QMessageBox::No) {
      return nullptr;
    }
  }
  graph->push();
  Observable::holdObservers();
  makeSelectionGraph(graph, selection);
  Graph *result = graph->addSubGraph(selection, "selection subgraph");
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
  _graphs->currentGraph()->addSubGraph(&prop, "clone subgraph");
}

void GraphPerspective::addEmptySubGraph() {
  if (_graphs->currentGraph() == nullptr)
    return;

  _graphs->currentGraph()->push();
  _graphs->currentGraph()->addSubGraph(nullptr, "empty subgraph");
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
  _ui->exposePanelsButton->setEnabled(enabled);
  _ui->searchButton->setEnabled(enabled);
  _ui->previousPageButton->setVisible(enabled);
  _ui->pageCountLabel->setVisible(enabled);
  _ui->nextPageButton->setVisible(enabled);
  _ui->actionSave_Project->setEnabled(enabled);
  _ui->actionSave_Project_as->setEnabled(enabled);
  _ui->actionSearch->setEnabled(enabled);

  if (graph == nullptr) {
    _ui->workspace->switchToStartupMode();
    _ui->exposePanelsButton->setChecked(false);
    showSearchDialog(false);
    _ui->actionSave_Project->setEnabled(false);
    _ui->actionSave_Project_as->setEnabled(false);
  } else {
    _ui->workspace->setGraphForFocusedPanel(graph);
  }

  _ui->actionExposePanels->setEnabled(!_ui->workspace->empty());

#ifdef TULIP_BUILD_PYTHON_COMPONENTS

  if (_graphs->empty()) {
    if (_pythonIDE) {
      _pythonIDE->clearPythonCodeEditors();
      _pythonIDEDialog->hide();
    }
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
    applyDefaultLayout(g);

    // create start panels only for a newly created graph
    if (mustDeleteGraph)
      showStartPanels(g);
    else
      centerPanelsForGraph(g);

    unsigned nb_error = nbLogsAfter - nbLogsBefore;
    if ((nb_error == 1) &&
        (QMessageBox::question(_mainWindow, "CSV parse error",
                               "When parsing your CSV file,<br/> one error has been "
                               "encountered.<br/>Do you want to see it?",
                               QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes))
      displayLogMessages();
    if ((nb_error > 1) &&
        (QMessageBox::question(_mainWindow, "CSV parse errors",
                               QString("When parsing your CSV file,<br/> %1 errors have been "
                                       "encountered.<br/>Do you want to see them?")
                                   .arg(nb_error),
                               QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes))
      displayLogMessages();

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

  for (auto panelName : {"Spreadsheet view", "Node Link Diagram view"}) {
    View *view = PluginLister::getPluginObject<View>(panelName);

    if (firstPanel == nullptr) {
      firstPanel = view;
    }

    view->setupUi();
    view->setGraph(g);
    view->setState(DataSet());
    _ui->workspace->addPanel(view);
  }

  _ui->workspace->setActivePanel(firstPanel);
  _ui->workspace->switchToSplitMode();
}

void GraphPerspective::applyDefaultLayout(Graph *g) {
  Observable::holdObservers();
  LayoutProperty *viewLayout = g->getProperty<LayoutProperty>("viewLayout");

  if (!viewLayout->hasNonDefaultValuatedNodes(g)) {
    std::string str;
    if (g->numberOfEdges() > 0) {
      if (TreeTest::isTree(g))
        g->applyPropertyAlgorithm("Tree Radial", viewLayout, str);
      else
        g->applyPropertyAlgorithm("FM^3 (OGDF)", viewLayout, str);
    } else
      g->applyPropertyAlgorithm("Random layout", viewLayout, str);
  }

  Observable::unholdObservers();
}

void GraphPerspective::centerPanelsForGraph(tlp::Graph *g, bool graphChanged, bool onlyGlMainView) {
  for (auto v : _ui->workspace->panels()) {
    if ((v->graph() == g) && (!onlyGlMainView || dynamic_cast<tlp::GlMainView *>(v)))
      v->centerView(graphChanged);
  }
}

void GraphPerspective::closePanelsForGraph(tlp::Graph *g) {
  list<View *> viewsToDelete;

  for (auto v : _ui->workspace->panels()) {
    if (v->graph() == g || g->isDescendantGraph(v->graph()))
      viewsToDelete.push_back(v);
  }

  if (!viewsToDelete.empty()) {
    // expose mode is not safe when deleting a panel
    // so hide it first
    _ui->workspace->hideExposeMode();
    for (auto v : viewsToDelete) {
      _ui->workspace->delView(v);
    }
  }
}

bool GraphPerspective::setGlMainViewPropertiesForGraph(
    tlp::Graph *g, const std::map<std::string, tlp::PropertyInterface *> &propsMap) {
  bool result = false;

  for (auto v : _ui->workspace->panels()) {
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

void GraphPerspective::showSearchDialog(bool f) {
  if (f) {
    if (_searchDialog == nullptr) {
      _searchDialog = new GraphPerspectiveDialog("Search graph elements");
      auto searchPanel = new SearchWidget(_searchDialog);
      searchPanel->setModel(_graphs);
      QVBoxLayout *layout = new QVBoxLayout;
      _searchDialog->setMinimumWidth(600);
      _searchDialog->setMinimumHeight(150);
      layout->addWidget(searchPanel);
      layout->setContentsMargins(0, 0, 0, 0);
      _searchDialog->setLayout(layout);
    }
    _searchDialog->hide();
    _ui->searchButton->setChecked(true);
    _searchDialog->show();
    _searchDialog->raise();
    _searchDialog->activateWindow();
  } else if (_searchDialog) {
    _searchDialog->hide();
    _ui->searchButton->setChecked(false);
    _ui->actionSearch->setChecked(false);
  }
}

void GraphPerspective::openPreferences() {
  PreferencesDialog dlg(_ui->mainWidget);
  dlg.readSettings();

  if (dlg.exec() == QDialog::Accepted) {
    dlg.writeSettings();

    for (auto v : _ui->workspace->panels()) {
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
  buildPythonIDE();
  _pythonIDEDialog->show();
  _pythonIDEDialog->raise();
#endif
}

#ifdef APPIMAGE_BUILD
// When running the appimage
// the LD_LIBRARY_PATH variable must be unset to ensure a successful launch
// of the default web browser to show the Tulip documentation
#define UNSET_LD_LIBRARY_PATH()                                                                    \
  auto ldPath = qgetenv("LD_LIBRARY_PATH");                                                        \
  qunsetenv("LD_LIBRARY_PATH")
#define RESTORE_LD_LIBRARY_PATH() qputenv("LD_LIBRARY_PATH", ldPath);
#else
#define UNSET_LD_LIBRARY_PATH()
#define RESTORE_LD_LIBRARY_PATH()
#endif

void GraphPerspective::showUserDocumentation() {
  UNSET_LD_LIBRARY_PATH();
  QDesktopServices::openUrl(QUrl::fromLocalFile(tlpStringToQString(tlp::TulipShareDir) +
                                                "../doc/tulip/tulip-user/html/index.html"));
  RESTORE_LD_LIBRARY_PATH();
}

void GraphPerspective::showDevelDocumentation() {
  UNSET_LD_LIBRARY_PATH();
  QDesktopServices::openUrl(QUrl::fromLocalFile(tlpStringToQString(tlp::TulipShareDir) +
                                                "../doc/tulip/tulip-dev/html/index.html"));
  RESTORE_LD_LIBRARY_PATH();
}

void GraphPerspective::showPythonDocumentation() {
  UNSET_LD_LIBRARY_PATH();
  QDesktopServices::openUrl(QUrl::fromLocalFile(tlpStringToQString(tlp::TulipShareDir) +
                                                "../doc/tulip/tulip-python/html/index.html"));
  RESTORE_LD_LIBRARY_PATH();
}

void GraphPerspective::showAPIDocumentation() {
  UNSET_LD_LIBRARY_PATH();
  QDesktopServices::openUrl(QUrl::fromLocalFile(tlpStringToQString(tlp::TulipShareDir) +
                                                "../doc/tulip/doxygen/html/index.html"));
  RESTORE_LD_LIBRARY_PATH();
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

void GraphPerspective::showHideMenuBar() {
  if (_mainWindow->menuBar()->isVisible()) {
    _mainWindow->menuBar()->setVisible(false);
    SET_TIPS(_ui->menubarButton, "Show Menubar");
  } else {
    _mainWindow->menuBar()->setVisible(true);
    SET_TIPS(_ui->menubarButton, "Hide Menubar");
  }
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

void GraphPerspective::resetLoggerDialogPosition() {
  QPoint pos = _mainWindow->mapToGlobal(_ui->exportButton->pos());
  pos.setX(pos.x() + _ui->loggerFrame->width());

  if (_logger->isVisible()) {
    _logger->showNormal();
  }

  // extend the logger frame width until reaching the right side of the main window
  _logger->setGeometry(pos.x(), pos.y(), _mainWindow->width() - _ui->loggerFrame->width(),
                       _mainWindow->mapToGlobal(QPoint(0, 0)).y() + _mainWindow->height() -
                           pos.y() - 2);
}

void GraphPerspective::displayStatusMessage(const QString &msg) {
  _ui->statusLabel->setText(msg);
}

void GraphPerspective::clearStatusMessage() {
  _ui->statusLabel->setText("");
}

PLUGIN(GraphPerspective)
