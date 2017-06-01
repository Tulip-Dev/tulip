/**
 *
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

  for(std::list<std::string>::const_iterator it = imports.begin(); it != imports.end(); ++it) {
    ImportModule* m = PluginLister::instance()->getPluginObject<ImportModule>(*it, NULL);
    std::list<std::string> fileExtensions(m->allFileExtensions());

    for(std::list<std::string>::const_iterator itExt = fileExtensions.begin(); itExt != fileExtensions.end(); ++itExt) {
      if (path.endsWith(tlpStringToQString(*itExt))) {
        delete m;
        return true;
      }
    }

    delete m;
  }

  return false;
}

GraphPerspective::GraphPerspective(const tlp::PluginContext* c): Perspective(c), _ui(NULL), _graphs(new GraphHierarchiesModel(this)), _recentDocumentsSettingsKey("perspective/recent_files"), _logger(NULL) {
  Q_INIT_RESOURCE(GraphPerspective);

  if (c && ((PerspectiveContext *) c)->parameters.contains("gui_testing")) {
    tlp::setGuiTestingMode(true);
    // we must ensure that choosing a file is relative to
    // the current directory to allow to run the gui tests
    // from any relative unit_test/gui directory
    _lastOpenLocation = QDir::currentPath();
  }

#ifdef TULIP_BUILD_PYTHON_COMPONENTS
  _pythonIDE = NULL;
  _pythonIDEDialog = NULL;
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

  foreach(const QString& s, TulipSettings::instance().recentDocuments()) {
    if (!QFileInfo(s).exists() || !tulipCanOpenFile(s))
      continue;

    QAction* action = _ui->menuOpen_recent_file->addAction(QIcon(":/tulip/graphperspective/icons/16/archive.png"),s,this,SLOT(openRecentFile()));
    action->setData(s);
  }

  _ui->menuOpen_recent_file->addSeparator();

  foreach(const QString& s, TulipSettings::instance().value(_recentDocumentsSettingsKey).toStringList()) {
    if (!QFileInfo(s).exists() || !tulipCanOpenFile(s))
      continue;

    QAction* action = _ui->menuOpen_recent_file->addAction(QIcon(":/tulip/graphperspective/icons/16/empty-file.png"),s,this,SLOT(openRecentFile()));
    action->setData(s);
  }
}

void GraphPerspective::addRecentDocument(const QString& path) {
  QStringList recents = TulipSettings::instance().value(_recentDocumentsSettingsKey).toStringList();

  if (recents.contains(path) || !tulipCanOpenFile(path)) {
    return;
  }

  recents += path;

  if (recents.size()>10)
    recents.pop_front();

  TulipSettings::instance().setValue(_recentDocumentsSettingsKey,recents);
  TulipSettings::instance().sync();
  buildRecentDocumentsMenu();
}

static void logMsgToStdErr(const QString &msg) {
  if (msg.startsWith("[Python")) {
    // remove quotes around message added by Qt
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    QString msgClean = msg.mid(14).mid(2, msg.length()-17);
#else
    QString msgClean = msg.mid(14).mid(2, msg.length()-18);
#endif

    if (msg.startsWith("[PythonStdOut]")) {
      std::cout << QStringToTlpString(msgClean) << std::endl;
    }
    else {
      std::cerr << QStringToTlpString(msgClean) << std::endl;
    }
  }
  else {
    std::cerr << QStringToTlpString(msg) << std::endl;
  }
}

void GraphPerspective::updateLogIconsAndCounters() {
  GraphPerspectiveLogger::LogType logType = _logger->getLastLogType();
  QFrame *logIconCounterFrame = NULL;
  QLabel *logIconLabel = NULL;
  QLabel *logCounterLabel = NULL;
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
  logCounterLabel->setText(QString::number(_logger->countByType(logType)));
}

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))

void graphPerspectiveLogger(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
  logMsgToStdErr(msg);
  static_cast<GraphPerspective*>(Perspective::instance())->log(type, context, msg);
}

void GraphPerspective::log(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
  _logger->log(type, context, msg);
  updateLogIconsAndCounters();
}

#else

void graphPerspectiveLogger(QtMsgType type, const char* msg) {
  logMsgToStdErr(msg);
  static_cast<GraphPerspective*>(Perspective::instance())->log(type,msg);
}

void GraphPerspective::log(QtMsgType type, const char* msg) {
  _logger->log(type,msg);
  updateLogIconsAndCounters();
}

#endif

GraphPerspective::~GraphPerspective() {
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
  qInstallMessageHandler(0);
#else
  qInstallMsgHandler(0);
#endif

  delete _ui;
}

bool GraphPerspective::terminated() {
  if(_graphs->needsSaving()) {
    QMessageBox::StandardButton answer = QMessageBox::question(_mainWindow, trUtf8("Save"), trUtf8("The project has been modified. Do you want to save your changes?"),QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel | QMessageBox::Escape);

    if ((answer == QMessageBox::Yes && !save()) ||
        (answer == QMessageBox::Cancel)) {
      return false;
    }
  }

#ifdef TULIP_BUILD_PYTHON_COMPONENTS
  _pythonIDE->clearPythonCodeEditors();
  delete _pythonIDEDialog;
#endif

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

bool GraphPerspective::eventFilter(QObject* obj, QEvent* ev) {
  if(ev->type() == QEvent::DragEnter) {
    QDragEnterEvent* dragEvent = dynamic_cast<QDragEnterEvent*>(ev);

    if(dragEvent->mimeData()->hasUrls()) {
      dragEvent->accept();
    }
  }

  if(ev->type() == QEvent::Drop) {
    QDropEvent* dropEvent = dynamic_cast<QDropEvent*>(ev);

    foreach(const QUrl& url, dropEvent->mimeData()->urls()) {
      open(url.toLocalFile());
    }
  }

  if (obj == _ui->loggerFrame && ev->type() == QEvent::MouseButtonPress)
    showLogger();

  return false;
}

void GraphPerspective::showLogger() {
  if (_logger->count()==0)
    return;

  QPoint pos = _mainWindow->mapToGlobal(_ui->loggerFrame->pos());
  pos.setX(pos.x()+_ui->loggerFrame->width());
  pos.setY(std::min<int>(_mainWindow->mapToGlobal(QPoint(0,0)).y()+mainWindow()->height()-_logger->height(),pos.y()));
  _logger->move(pos);
  // extend the logger frame width until reaching the right side of the main window
  _logger->resize(_mainWindow->mapToGlobal(_mainWindow->pos()).x()+mainWindow()->width()-_mainWindow->mapToGlobal(_logger->pos()).x(),
                  _mainWindow->mapToGlobal(QPoint(0,0)).y()+mainWindow()->height() - pos.y() - 2);
  _logger->show();
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

void GraphPerspective::start(tlp::PluginProgress *progress) {
  reserveDefaultProperties();
  _ui = new Ui::GraphPerspectiveMainWindowData;
  _ui->setupUi(_mainWindow);
#ifdef TULIP_BUILD_PYTHON_COMPONENTS
  _pythonPanel = new PythonPanel();
  QVBoxLayout *layout = new QVBoxLayout();
  layout->addWidget(_pythonPanel);
  layout->setContentsMargins(0,0,0,0);
  _ui->pythonPanel->setLayout(layout);
  _pythonIDE = new PythonIDE();
  QVBoxLayout *dialogLayout = new QVBoxLayout();
  dialogLayout->addWidget(_pythonIDE);
  dialogLayout->setContentsMargins(0,0,0,0);
  _pythonIDEDialog = new PythonIDEDialog(NULL, Qt::Window);
  _pythonIDEDialog->setStyleSheet(_mainWindow->styleSheet());
  _pythonIDEDialog->setWindowIcon(_mainWindow->windowIcon());
  _pythonIDEDialog->setLayout(dialogLayout);
  _pythonIDEDialog->resize(800, 600);
  _pythonIDEDialog->setWindowTitle("Tulip Python IDE");
#else
  _ui->pythonButton->setVisible(false);
  _ui->developButton->setVisible(false);
#endif
  currentGraphChanged(NULL);
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

  if (tlp::inGuiTestingMode())
    _mainWindow->statusBar()->hide();

  connect(_logger,SIGNAL(cleared()),this,SLOT(logCleared()));

  _colorScalesDialog = new ColorScaleConfigDialog(ColorScalesManager::getLatestColorScale(), mainWindow());

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
  connect(_ui->workspace,SIGNAL(addPanelRequest(tlp::Graph*)),this,SLOT(createPanel(tlp::Graph*)));
  connect(_ui->workspace,SIGNAL(focusedPanelSynchronized()),this,SLOT(focusedPanelSynchronized()));
  connect(_graphs,SIGNAL(currentGraphChanged(tlp::Graph*)),this,SLOT(currentGraphChanged(tlp::Graph*)));
  connect(_graphs,SIGNAL(currentGraphChanged(tlp::Graph*)),_ui->algorithmRunner,SLOT(setGraph(tlp::Graph*)));
  connect(_ui->graphHierarchiesEditor,SIGNAL(changeSynchronization(bool)),this,SLOT(changeSynchronization(bool)));

  // Connect actions
  connect(_ui->actionMessages_log,SIGNAL(triggered()),this,SLOT(showLogger()));
  connect(_ui->actionFull_screen,SIGNAL(triggered(bool)),this,SLOT(showFullScreen(bool)));
  connect(_ui->actionImport,SIGNAL(triggered()),this,SLOT(importGraph()));
  connect(_ui->actionExport,SIGNAL(triggered()),this,SLOT(exportGraph()));
  connect(_ui->actionSave_graph_to_file,SIGNAL(triggered()),this,SLOT(saveGraphHierarchyInTlpFile()));
  connect(_ui->workspace,SIGNAL(panelFocused(tlp::View*)),this,SLOT(panelFocused(tlp::View*)));
  connect(_ui->actionSave_Project,SIGNAL(triggered()),this,SLOT(save()));
  connect(_ui->actionSave_Project_as,SIGNAL(triggered()),this,SLOT(saveAs()));
  connect(_ui->actionOpen_Project,SIGNAL(triggered()),this,SLOT(open()));
  connect(_ui->actionDelete,SIGNAL(triggered()),this,SLOT(deleteSelectedElements()));
  connect(_ui->actionDelete_from_the_root_graph,SIGNAL(triggered()),this,SLOT(deleteSelectedElementsFromRootGraph()));
  connect(_ui->actionDelete_all, SIGNAL(triggered()), this, SLOT(clearGraph()));
  connect(_ui->actionInvert_selection,SIGNAL(triggered()),this,SLOT(invertSelection()));
  connect(_ui->actionCancel_selection,SIGNAL(triggered()),this,SLOT(cancelSelection()));
  connect(_ui->actionReverse_selected_edges, SIGNAL(triggered()), this, SLOT(reverseSelectedEdges()));
  connect(_ui->actionMake_selection_a_graph, SIGNAL(triggered()), this, SLOT(make_graph()));
  connect(_ui->actionSelect_All,SIGNAL(triggered()),this,SLOT(selectAll()));
  connect(_ui->actionSelect_All_Nodes,SIGNAL(triggered()),this,SLOT(selectAllNodes()));
  connect(_ui->actionSelect_All_Edges,SIGNAL(triggered()),this,SLOT(selectAllEdges()));
  connect(_ui->actionUndo,SIGNAL(triggered()),this,SLOT(undo()));
  connect(_ui->actionRedo,SIGNAL(triggered()),this,SLOT(redo()));
  connect(_ui->actionCut,SIGNAL(triggered()),this,SLOT(cut()));
  connect(_ui->actionPaste,SIGNAL(triggered()),this,SLOT(paste()));
  connect(_ui->actionCopy,SIGNAL(triggered()),this,SLOT(copy()));
  connect(_ui->actionGroup_elements,SIGNAL(triggered()),this,SLOT(group()));
  connect(_ui->actionCreate_sub_graph,SIGNAL(triggered()),this,SLOT(createSubGraph()));
  connect(_ui->actionClone_sub_graph,SIGNAL(triggered()),this,SLOT(cloneSubGraph()));
  connect(_ui->actionCreate_empty_sub_graph,SIGNAL(triggered()),this,SLOT(addEmptySubGraph()));
  connect(_ui->actionImport_CSV,SIGNAL(triggered()),this,SLOT(CSVImport()));
  connect(_ui->actionFind_plugins,SIGNAL(triggered()),this,SLOT(findPlugins()));
  connect(_ui->actionNew_graph, SIGNAL(triggered()), this, SLOT(addNewGraph()));
  connect(_ui->actionNewProject, SIGNAL(triggered()), this, SLOT(newProject()));
  connect(_ui->actionPreferences,SIGNAL(triggered()),this,SLOT(openPreferences()));
  connect(_ui->searchButton,SIGNAL(clicked(bool)),this,SLOT(setSearchOutput(bool)));
  connect(_ui->workspace,SIGNAL(importGraphRequest()),this,SLOT(importGraph()));
  connect(_ui->action_Close_All, SIGNAL(triggered()), _ui->workspace, SLOT(closeAll()));
  connect(_ui->addPanelButton, SIGNAL(clicked()), this, SLOT(createPanel()));
  connect(_ui->actionColor_scales_management, SIGNAL(triggered()), this, SLOT(displayColorScalesDialog()));
  connect(_ui->exportButton, SIGNAL(clicked()), this, SLOT(exportGraph()));

  // Agent actions
  connect(_ui->actionPlugins_Center,SIGNAL(triggered()),this,SLOT(showPluginsCenter()));
  connect(_ui->actionAbout_us,SIGNAL(triggered()),this,SLOT(showAboutPage()));
  connect(_ui->actionAbout_us,SIGNAL(triggered()),this,SLOT(showAboutTulipPage()));

  if (QFile(tlpStringToQString(tlp::TulipShareDir) +
            "doc/tulip-user/html/index.html").exists()) {
    connect(_ui->actionShowUserDocumentation,SIGNAL(triggered()),this,SLOT(showUserDocumentation()));
    connect(_ui->actionShowDevelDocumentation,SIGNAL(triggered()),this,SLOT(showDevelDocumentation()));
#ifdef TULIP_BUILD_PYTHON_COMPONENTS
    connect(_ui->actionShowPythonDocumentation,SIGNAL(triggered()),this,SLOT(showPythonDocumentation()));
#else
    _ui->actionShowPythonDocumentation->setVisible(false);
#endif
  }
  else {
    _ui->actionShowUserDocumentation->setVisible(false);
    _ui->actionShowDevelDocumentation->setVisible(false);
    _ui->actionShowPythonDocumentation->setVisible(false);
  }

  // Setting initial sizes for splitters
  _ui->mainSplitter->setSizes(QList<int>() << 350 << 850);
  _ui->mainSplitter->setStretchFactor(0,0);
  _ui->mainSplitter->setStretchFactor(1,1);
  _ui->mainSplitter->setCollapsible(1, false);

  // Open project with model
  QMap<QString,tlp::Graph*> rootIds;

  if(!_project->projectFile().isEmpty()) {
    rootIds = _graphs->readProject(_project,progress);

    if (rootIds.empty())
      QMessageBox::critical(_mainWindow, QString("Error while loading project ").append(_project->projectFile()), QString("The Tulip project file is probably corrupted.<br>")+tlpStringToQString(progress->getError()));
  }

  // these ui initializations are needed here
  // in case of a call to showStartPanels in the open method
  _ui->graphHierarchiesEditor->setModel(_graphs);
  _ui->workspace->setModel(_graphs);

  if (!rootIds.empty())
    _ui->workspace->readProject(_project,rootIds,progress);

  _ui->searchPanel->setModel(_graphs);

#ifdef TULIP_BUILD_PYTHON_COMPONENTS
  connect(_ui->pythonButton,SIGNAL(clicked(bool)),this,SLOT(setPythonPanel(bool)));
  connect(_ui->developButton, SIGNAL(clicked()), this, SLOT(showPythonIDE()));
  _pythonPanel->setModel(_graphs);
  _pythonIDE->setGraphsModel(_graphs);
  tlp::PluginLister::instance()->addListener(this);
  _pythonIDE->setProject(_project);
#endif

  if (!_externalFile.isEmpty() && QFileInfo(_externalFile).exists()) {
    open(_externalFile);
  }

  foreach(HeaderFrame *h, _ui->docksSplitter->findChildren<HeaderFrame *>()) {
    connect(h,SIGNAL(expanded(bool)),this,SLOT(refreshDockExpandControls()));
  }

  connect(_ui->sidebarButton, SIGNAL(clicked()), this, SLOT(showHideSideBar()));

#if !defined(__APPLE__) && !defined(_WIN32)
  // Hide plugins center when not on MacOS or Windows
  _ui->pluginsButton->hide();
  _ui->menuHelp->removeAction(_ui->actionPlugins_Center);
#else
  // show the 'Plugins center' menu entry and button only if connected to the Tulip agent
  _ui->pluginsButton->setVisible(checkSocketConnected());
  _ui->actionPlugins_Center->setVisible(checkSocketConnected());
#endif

  // show the 'Find plugins' menu entry only if connected to the Tulip agent
  _ui->actionFind_plugins->setVisible(checkSocketConnected());

  // fill menu with recent documents
  buildRecentDocumentsMenu();

  showTrayMessage("GraphPerspective started");

#ifdef TULIP_BUILD_PYTHON_COMPONENTS
  // for 4.11 show message to indicate that
  // Python Script view no longer exist
  unsigned int mm_version = TULIP_INT_MM_VERSION;

  if ((mm_version == 411) &&
      TulipSettings::instance().isFirstTulipMMRun()) {
    QTimer::singleShot(100, this, SLOT(showStartMessage()));
  }

#endif
  logCleared();
}

void GraphPerspective::showStartMessage() {
  QMessageBox::information(_mainWindow, QString("About Tulip Python IDE"), QString("<html><body><p>Be aware that the <b>Python Script View</b> no longer exists. The coding of python scripts is now available in using the <b>Tulip Python IDE</b>.<br/>Click on the <img src=\":/tulip/graphperspective/icons/16/python.png\">&nbsp;<b>Python IDE</b> button to launch it.</p></body></html>"));
}

void GraphPerspective::openExternalFile() {
  open(_externalFile);
}

tlp::GraphHierarchiesModel* GraphPerspective::model() const {
  return _graphs;
}

void GraphPerspective::refreshDockExpandControls() {
  QList<HeaderFrame *> expandedHeaders, collapsedHeaders;

  foreach(HeaderFrame *h, _ui->docksSplitter->findChildren<HeaderFrame *>()) {
    h->expandControl()->setEnabled(true);

    if (h->isExpanded())
      expandedHeaders.push_back(h);
    else
      collapsedHeaders.push_back(h);
  }

  if (expandedHeaders.size() == 1)
    expandedHeaders[0]->expandControl()->setEnabled(false);
}

void GraphPerspective::exportGraph(Graph* g) {
  if (g == NULL)
    g = _graphs->currentGraph();

  if (g == NULL)
    return;

  static QString exportFile;
  ExportWizard wizard(g, exportFile, _mainWindow);
  wizard.setWindowTitle(QString("Exporting graph \"") + tlpStringToQString(g->getName()) + '"');

  if (wizard.exec() != QDialog::Accepted || wizard.algorithm().isEmpty() || wizard.outputFile().isEmpty())
    return;

  std::string filename = QStringToTlpString(exportFile = wizard.outputFile());
  std::string exportPluginName = QStringToTlpString(wizard.algorithm());

  DataSet data = wizard.parameters();
  PluginProgress* prg = progress(NoProgressOption);
  prg->setTitle(exportPluginName);
  // take time before run
  QTime start = QTime::currentTime();
  bool result = tlp::saveGraph(g, filename,prg,&data);

  if (!result) {
    QMessageBox::critical(_mainWindow,trUtf8("Export error"),QString("<i>") + wizard.algorithm() + trUtf8("</i> failed to export graph.<br/><br/><b>") + tlp::tlpStringToQString(prg->getError()) + "</b>");
  }
  else {
    // log export plugin call
    if (TulipSettings::instance().logPluginCall() != TulipSettings::NoLog) {
      std::stringstream log;
      log << exportPluginName.c_str() << " - " << data.toString().c_str();

      if (TulipSettings::instance().logPluginCall() == TulipSettings::LogCallWithExecutionTime)
        log << ": " << start.msecsTo(QTime::currentTime()) << "ms";

      qDebug() << tlpStringToQString(log.str());
    }

    addRecentDocument(wizard.outputFile());
  }

  delete prg;
}

void GraphPerspective::saveGraphHierarchyInTlpFile(Graph *g) {
  if (g == NULL)
    g = _graphs->currentGraph();

  if (g == NULL)
    return;

  static QString savedFile;
  QString filter("TLP format (*.tlp *.tlp.gz *.tlpz);;TLPB format (*.tlpb *.tlpb.gz *.tlpbz)");
  QString filename =
    QFileDialog::getSaveFileName(_mainWindow, tr("Save graph hierarchy in tlp/tlpb file"),savedFile, filter);

  if(!filename.isEmpty()) {
    bool result=tlp::saveGraph(g, tlp::QStringToTlpString(filename));

    if (!result)
      QMessageBox::critical(_mainWindow,trUtf8("Save error"),trUtf8("Failed to save graph hierarchy"));
    else {
      savedFile=filename;
      addRecentDocument(filename);
    }
  }
}

void GraphPerspective::importGraph(const std::string& module,
                                   DataSet& data) {
  Graph* g;

  if (!module.empty()) {
    PluginProgress* prg = progress(IsStoppable | IsCancellable);
    prg->setTitle(module);
    // take time before run
    QTime start = QTime::currentTime();
    g = tlp::importGraph(module,data,prg);

    if (g == NULL) {
      QMessageBox::critical(_mainWindow,trUtf8("Import error"),
                            QString("<i>") + tlp::tlpStringToQString(module) + trUtf8("</i> failed to import data.<br/><br/><b>") + tlp::tlpStringToQString(prg->getError()) + "</b>");
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

      qDebug() << tlpStringToQString(log.str());
    }

    if (g->getName().empty()) {
      QString n = tlp::tlpStringToQString(module) + " - " + tlp::tlpStringToQString(data.toString());
      n.replace(QRegExp("[\\w]*::"),""); // remove words before "::"
      g->setName(tlp::QStringToTlpString(n));
    }
  }
  else {
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

void GraphPerspective::createPanel(tlp::Graph* g) {
  if (_graphs->empty())
    return;

  PanelSelectionWizard wizard(_graphs,_mainWindow);

  if (g != NULL)
    wizard.setSelectedGraph(g);
  else
    wizard.setSelectedGraph(_graphs->currentGraph());


  int result = wizard.exec();

  if (result == QDialog::Accepted && wizard.panel() != NULL) {
    // expose mode is not safe to add a new panel
    // so hide it if needed
    _ui->workspace->hideExposeMode();
    _ui->workspace->addPanel(wizard.panel());
    _ui->workspace->setActivePanel(wizard.panel());
    wizard.panel()->applySettings();
  }
}

void GraphPerspective::panelFocused(tlp::View* view) {
  disconnect(this,SLOT(focusedPanelGraphSet(tlp::Graph*)));

  if (!_ui->graphHierarchiesEditor->synchronized())
    return;

  connect(view,SIGNAL(graphSet(tlp::Graph*)),this,SLOT(focusedPanelGraphSet(tlp::Graph*)));
  focusedPanelGraphSet(view->graph());
}

void GraphPerspective::changeSynchronization(bool s) {
  _ui->workspace->setFocusedPanelHighlighting(s);
}

void GraphPerspective::focusedPanelGraphSet(Graph* g) {
  _graphs->setCurrentGraph(g);
}

void GraphPerspective::focusedPanelSynchronized() {
  _ui->workspace->setGraphForFocusedPanel(_graphs->currentGraph());
}

bool GraphPerspective::save() {
  return saveAs(_project->projectFile());
}

bool GraphPerspective::saveAs(const QString& path) {
  if(_graphs->empty())
    return false;

  if(path.isEmpty()) {
    QString path = QFileDialog::getSaveFileName(_mainWindow,trUtf8("Save project"),QString(),"Tulip Project (*.tlpx)");

    if (!path.isEmpty()) {
      if (!path.endsWith(".tlpx"))
        path+=".tlpx";

      _project->setProjectFile(path);
      return saveAs(path);
    }

    return false;
  }

  SimplePluginProgressDialog progress(_mainWindow);
  progress.showPreview(false);
  progress.show();
  QMap<Graph*,QString> rootIds = _graphs->writeProject(_project,&progress);
  _ui->workspace->writeProject(_project,rootIds,&progress);
#ifdef TULIP_BUILD_PYTHON_COMPONENTS
  _pythonIDE->savePythonFilesAndWriteToProject();
#endif
  bool ret = _project->write(path,&progress);

  if(ret)
    TulipSettings::instance().addToRecentDocuments(path);

  return ret;
}

void GraphPerspective::open(QString fileName) {
  QMap<std::string, std::string> modules;
  std::list<std::string> imports = PluginLister::instance()->availablePlugins<ImportModule>();

  std::string filters("Tulip project (*.tlpx);;");
  std::string filterAny("Any supported format (");

  for(std::list<std::string>::const_iterator it = imports.begin(); it != imports.end(); ++it) {
    ImportModule* m = PluginLister::instance()->getPluginObject<ImportModule>(*it, NULL);
    std::list<std::string> fileExtension(m->allFileExtensions());

    std::string currentFilter;

    for(std::list<std::string>::const_iterator listIt = fileExtension.begin(); listIt != fileExtension.end(); ++listIt) {

      if(listIt->empty())
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
    fileName = QFileDialog::getOpenFileName(_mainWindow, tr("Open graph"), _lastOpenLocation, filters.c_str());

  if(!fileName.isEmpty()) {
    QFileInfo fileInfo(fileName);

    // we must ensure that choosing a file is relative to
    // the current directory to allow to run the gui tests
    // from any relative unit_test/gui directory
    if (!tlp::inGuiTestingMode())
      _lastOpenLocation = fileInfo.absolutePath();

    foreach(const std::string& extension, modules.keys()) {
      if (fileName.endsWith(".tlpx")) {
        openProjectFile(fileName);
        TulipSettings::instance().addToRecentDocuments(fileInfo.absoluteFilePath());
        break;
      }
      else if(fileName.endsWith(QString::fromStdString(extension))) {
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
    PluginProgress* prg = progress(NoProgressOption);
    _project->openProjectFile(path,prg);
    QMap<QString,tlp::Graph*> rootIds = _graphs->readProject(_project,prg);
    _ui->workspace->readProject(_project,rootIds,prg);
#ifdef TULIP_BUILD_PYTHON_COMPONENTS
    _pythonIDE->setProject(_project);
#endif
    delete prg;
  }
  else {
    Perspective::openProjectFile(path);
  }
}

void GraphPerspective::deleteSelectedElementsFromRootGraph() {
  deleteSelectedElements(true);
}

void GraphPerspective::clearGraph() {
  if(QMessageBox::question(_mainWindow, "Clear graph content", "Do you really want to remove all nodes and edges from the current graph. This action cannot be undone", QMessageBox::Yes|QMessageBox::No)==QMessageBox::Yes)
    _graphs->currentGraph()->clear();
}

void GraphPerspective::deleteSelectedElements(bool fromRoot) {
  Observable::holdObservers();
  tlp::Graph* graph = _graphs->currentGraph();
  tlp::BooleanProperty* selection = graph->getProperty<BooleanProperty>("viewSelection");

  graph->push();
  tlp::Iterator<edge>* itEdges =
    new StableIterator<edge>(selection->getEdgesEqualTo(true, graph));
  graph->delEdges(itEdges, fromRoot);
  delete itEdges;

  tlp::Iterator<node>* itNodes =
    new StableIterator<node>(selection->getNodesEqualTo(true,graph));
  graph->delNodes(itNodes, fromRoot);
  delete itNodes;

  Observable::unholdObservers();
}

void GraphPerspective::invertSelection() {
  Observable::holdObservers();
  tlp::Graph* graph = _graphs->currentGraph();
  tlp::BooleanProperty* selection = graph->getProperty<BooleanProperty>("viewSelection");
  graph->push();
  selection->reverse(graph);
  Observable::unholdObservers();
}

void GraphPerspective::reverseSelectedEdges() {
  Observable::holdObservers();
  tlp::Graph* graph = _graphs->currentGraph();
  tlp::BooleanProperty* selection = graph->getProperty<BooleanProperty>("viewSelection");
  graph->push();
  selection->reverseEdgeDirection(graph);
  Observable::unholdObservers();
}


void GraphPerspective::cancelSelection() {
  Observable::holdObservers();
  tlp::Graph* graph = _graphs->currentGraph();
  tlp::BooleanProperty* selection = graph->getProperty<BooleanProperty>("viewSelection");
  graph->push();
  node n;
  forEach(n, selection->getNodesEqualTo(true,graph)) {
    selection->setNodeValue(n, false);
  }
  edge e;
  forEach(e, selection->getEdgesEqualTo(true,graph)) {
    selection->setEdgeValue(e, false);
  }
  Observable::unholdObservers();
}

void GraphPerspective::selectAll(bool nodes, bool edges) {
  Observable::holdObservers();
  tlp::Graph* graph = _graphs->currentGraph();
  tlp::BooleanProperty* selection = graph->getProperty<BooleanProperty>("viewSelection");
  graph->push();
  selection->setAllNodeValue(false);
  selection->setAllEdgeValue(false);

  if(nodes) {
    selection->setValueToGraphNodes(true, graph);
  }

  if(edges) {
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
  tlp::Graph* graph = _graphs->currentGraph();

  if (graph != NULL)
    graph->pop();

  Observable::unholdObservers();

  foreach(View* v, _ui->workspace->panels()) {
    if (v->graph() == graph)
      v->undoCallback();
  }
}

void GraphPerspective::redo() {
  Observable::holdObservers();
  tlp::Graph* graph = _graphs->currentGraph();

  if (graph != NULL)
    graph->unpop();

  Observable::unholdObservers();

  foreach(View* v, _ui->workspace->panels()) {
    if (v->graph() == graph)
      v->undoCallback();
  }
}

void GraphPerspective::cut() {
  copy(_graphs->currentGraph(),true);
}

void GraphPerspective::paste() {
  if (_graphs->currentGraph() == NULL)
    return;

  Graph* outGraph = _graphs->currentGraph();
  std::stringstream ss;
  ss << QStringToTlpString(QApplication::clipboard()->text());

  Observable::holdObservers();
  outGraph->push();
  DataSet data;
  data.set("file::data", ss.str());
  Graph* inGraph = tlp::importGraph("TLP Import",data);
  tlp::copyToGraph(outGraph,inGraph);
  delete inGraph;
  Observable::unholdObservers();
  centerPanelsForGraph(outGraph);
}

void GraphPerspective::copy() {
  copy(_graphs->currentGraph());
}

void GraphPerspective::copy(Graph* g, bool deleteAfter) {
  if (g == NULL)
    return;

  Observable::holdObservers();
  g->push();

  BooleanProperty* selection = g->getProperty<BooleanProperty>("viewSelection");

  Graph* copyGraph = tlp::newGraph();
  tlp::copyToGraph(copyGraph,g,selection);

  std::stringstream ss;
  DataSet data;
  tlp::exportGraph(copyGraph,ss,"TLP Export",data);
  QApplication::clipboard()->setText(tlpStringToQString(ss.str()));


  if (deleteAfter) {
    tlp::node n;
    stableForEach(n, selection->getNodesEqualTo(true))
    g->delNode(n);
  }

  delete copyGraph;

  Observable::unholdObservers();
}

void GraphPerspective::group() {
  Observable::holdObservers();
  tlp::Graph* graph = _graphs->currentGraph();
  tlp::BooleanProperty* selection = graph->getProperty<BooleanProperty>("viewSelection");
  std::vector<node> groupedNodes;
  node n;
  forEach(n, selection->getNodesEqualTo(true)) {
    if(graph->isElement(n))
      groupedNodes.push_back(n);
  }

  if (groupedNodes.empty()) {
    Observable::unholdObservers();
    qCritical() << trUtf8("[Group] Cannot create meta-nodes from empty selection");
    return;
  }

  graph->push();

  bool changeGraph=false;

  if (graph == graph->getRoot()) {
    qWarning() << trUtf8("[Group] Grouping can not be done on the root graph. A subgraph has automatically been created");
    graph = graph->addCloneSubGraph("groups");
    changeGraph = true;
  }

  graph->createMetaNode(groupedNodes, false);

  selection->setAllNodeValue(false);
  selection->setAllEdgeValue(false);

  Observable::unholdObservers();

  if (!changeGraph)
    return;

  foreach(View* v, _ui->workspace->panels()) {
    if (v->graph() == graph->getRoot())
      v->setGraph(graph);
  }
}

void GraphPerspective::make_graph() {
  Graph* graph = _graphs->currentGraph();
  unsigned added = makeSelectionGraph(_graphs->currentGraph(), graph->getProperty<BooleanProperty>("viewSelection"));
  tlp::debug() << "Make selection a graph: " << added << " elements added to the selection.";
}

Graph *GraphPerspective::createSubGraph(Graph *graph) {
  if (graph == NULL)
    return NULL;

  graph->push();
  Observable::holdObservers();
  BooleanProperty* selection = graph->getProperty<BooleanProperty>("viewSelection");
  makeSelectionGraph(graph, selection);
  Graph* result = graph->addSubGraph(selection, "selection sub-graph");
  Observable::unholdObservers();
  return result;
}

void GraphPerspective::createSubGraph() {
  createSubGraph(_graphs->currentGraph());
}

void GraphPerspective::cloneSubGraph() {
  if (_graphs->currentGraph() == NULL)
    return;

  tlp::BooleanProperty prop(_graphs->currentGraph());
  prop.setAllNodeValue(true);
  prop.setAllEdgeValue(true);
  _graphs->currentGraph()->push();
  _graphs->currentGraph()->addSubGraph(&prop, "clone sub-graph");
}

void GraphPerspective::addEmptySubGraph() {
  if (_graphs->currentGraph() == NULL)
    return;

  _graphs->currentGraph()->push();
  _graphs->currentGraph()->addSubGraph(NULL, "empty sub-graph");
}

void GraphPerspective::currentGraphChanged(Graph *graph) {
  bool enabled(graph != NULL);
  _ui->actionUndo->setEnabled(enabled);
  _ui->actionRedo->setEnabled(enabled);
  _ui->actionCut->setEnabled(enabled);
  _ui->actionCopy->setEnabled(enabled);
  _ui->actionPaste->setEnabled(enabled);
  _ui->actionDelete->setEnabled(enabled);
  _ui->actionDelete_from_the_root_graph->setEnabled(enabled&&(graph!=graph->getRoot()));
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

  if (graph == NULL) {
    _ui->workspace->switchToStartupMode();
    _ui->exposeModeButton->setChecked(false);
    _ui->searchButton->setChecked(false);
    _ui->pythonButton->setChecked(false);
    setSearchOutput(false);
    _ui->actionSave_Project->setEnabled(false);
    _ui->actionSave_Project_as->setEnabled(false);
  }
  else {
    _ui->workspace->setGraphForFocusedPanel(graph);
  }


#ifdef TULIP_BUILD_PYTHON_COMPONENTS

  if (_graphs->empty()) {
    _pythonIDE->clearPythonCodeEditors();
    _pythonIDEDialog->hide();
    _ui->developButton->setEnabled(false);
  }
  else {
    _ui->developButton->setEnabled(true);
  }

#endif
}

void GraphPerspective::CSVImport() {
  bool mustDeleteGraph = false;

  if (_graphs->empty()) {
    _graphs->addGraph(tlp::newGraph());
    mustDeleteGraph = true;
  }

  Graph* g = _graphs->currentGraph();

  if (g == NULL)
    return;

  CSVImportWizard wizard(_mainWindow);

  if (mustDeleteGraph) {
    wizard.setWindowTitle("Import CSV data into a new graph");
    wizard.setButtonText(QWizard::FinishButton, QString("Import into a new graph"));
  }
  else {
    wizard.setWindowTitle(QString("Import CSV data into current graph: ") +  g->getName().c_str());
    wizard.setButtonText(QWizard::FinishButton, QString("Import into current graph"));
  }

  // get the number of line displayed in the logger
  unsigned int nbLogsBefore = _logger->count();

  wizard.setGraph(g);

  g->push();
  Observable::holdObservers();
  int result = wizard.exec();

  if (result == QDialog::Rejected) {
    if (mustDeleteGraph) {
      _graphs->removeGraph(g);
      delete g;
    }
    else {
      g->pop();
    }
  }
  else {
    unsigned int nbLogsAfter = _logger->count();
    applyRandomLayout(g);
    bool openPanels = true;

    foreach(View* v, _ui->workspace->panels()) {
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
        (QMessageBox::question(_mainWindow, "CSV Parse errors found", QString("When parsing your CSV file,<br/> %1 error(s) has been encountered.<br/>Do you want to see them?").arg(nbLogsAfter - nbLogsBefore), QMessageBox::Yes|QMessageBox::No)==QMessageBox::Yes))
      showLogger();
  }

  Observable::unholdObservers();
}

void GraphPerspective::showStartPanels(Graph *g) {
  if (TulipSettings::instance().displayDefaultViews() == false)
    return;

  // expose mode is not safe to add a new panel
  // so hide it if needed
  _ui->workspace->hideExposeMode();
  View* firstPanel = NULL;
  View* secondPanel = NULL;

  foreach(const QString& panelName, QStringList() << "Spreadsheet view" << "Node Link Diagram view") {
    View* view = PluginLister::instance()->getPluginObject<View>(QStringToTlpString(panelName),NULL);

    if (firstPanel == NULL)
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

void GraphPerspective::applyRandomLayout(Graph* g) {
  Observable::holdObservers();
  LayoutProperty* viewLayout = g->getProperty<LayoutProperty>("viewLayout");

  if (viewLayout->numberOfNonDefaultValuatedNodes(g)==0) {
    std::string str;
    g->applyPropertyAlgorithm("Random layout", viewLayout, str);
  }

  Observable::unholdObservers();
}

void GraphPerspective::centerPanelsForGraph(tlp::Graph* g, bool graphChanged,
    bool onlyGlMainView) {
  foreach(View* v, _ui->workspace->panels()) {
    if ((v->graph() == g) &&
        (!onlyGlMainView || dynamic_cast<tlp::GlMainView*>(v)))
      v->centerView(graphChanged);
  }
}

void GraphPerspective::closePanelsForGraph(tlp::Graph* g) {
  QVector<View*> viewsToDelete;

  foreach(View* v, _ui->workspace->panels()) {
    if (v->graph() == g || g->isDescendantGraph(v->graph()))
      viewsToDelete+=v;
  }

  if (!viewsToDelete.empty()) {
    // expose mode is not safe to add a delete a panel
    // so hide it if needed
    _ui->workspace->hideExposeMode();

    foreach(View* v, viewsToDelete) {
      _ui->workspace->delView(v);
    }
  }
}

bool GraphPerspective::setGlMainViewPropertiesForGraph(tlp::Graph* g, const std::map<std::string, tlp::PropertyInterface*>& propsMap) {
  bool result = false;

  foreach(View* v, _ui->workspace->panels()) {
    GlMainView* glMainView = dynamic_cast<tlp::GlMainView*>(v);

    if (v->graph() == g && glMainView != NULL) {
      if (glMainView->getGlMainWidget()->getScene()->getGlGraphComposite()->getInputData()->installProperties(propsMap))
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

    foreach(tlp::View* v, _ui->workspace->panels()) {
      GlMainView* glMainView = dynamic_cast<tlp::GlMainView*>(v);

      if (glMainView != NULL) {
        if (glMainView->getGlMainWidget() != NULL) {
          glMainView->getGlMainWidget()->getScene()->getGlGraphComposite()->getRenderingParametersPointer()->setSelectionColor(TulipSettings::instance().defaultSelectionColor());
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
  Graph* g = tlp::newGraph();
  _graphs->addGraph(g);
  showStartPanels(g);
}

void GraphPerspective::newProject() {
  createPerspective(name().c_str());
}

void GraphPerspective::openRecentFile() {
  QAction* action = static_cast<QAction*>(sender());
  open(action->data().toString());
}

void GraphPerspective::treatEvent(const tlp::Event &ev) {
  if (dynamic_cast<const tlp::PluginEvent*>(&ev)) {
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
  QDesktopServices::openUrl(QUrl::fromLocalFile(tlpStringToQString(tlp::TulipShareDir) + "doc/tulip-user/html/index.html"));
}

void GraphPerspective::showDevelDocumentation() {
  QDesktopServices::openUrl(QUrl::fromLocalFile(tlpStringToQString(tlp::TulipShareDir) + "doc/tulip-dev/html/index.html"));
}

void GraphPerspective::showPythonDocumentation() {
  QDesktopServices::openUrl(QUrl::fromLocalFile(tlpStringToQString(tlp::TulipShareDir) + "doc/tulip-python/html/index.html"));
}

void GraphPerspective::showHideSideBar() {
  if (_ui->docksWidget->isVisible()) {
    _ui->docksWidget->setVisible(false);
    _ui->sidebarButton->setToolTip("Show Sidebar");
  }
  else {
    _ui->docksWidget->setVisible(true);
    _ui->sidebarButton->setToolTip("Hide Sidebar");
  }
}

void GraphPerspective::displayColorScalesDialog() {
  _colorScalesDialog->show();
}

void GraphPerspective::showAboutTulipPage() {
  if(!checkSocketConnected()) {
    tlp::AboutTulipPage *aboutPage = new tlp::AboutTulipPage;
    QDialog aboutDialog(mainWindow(), Qt::Window);
    aboutDialog.setWindowTitle("About Tulip");
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(aboutPage);
    layout->setContentsMargins(0,0,0,0);
    aboutDialog.setLayout(layout);
    aboutDialog.resize(800, 600);
    aboutDialog.exec();
  }
}

void GraphPerspective::workspaceButtonClicked() {
  _ui->workspaceButton->setChecked(true);
}

PLUGIN(GraphPerspective)
