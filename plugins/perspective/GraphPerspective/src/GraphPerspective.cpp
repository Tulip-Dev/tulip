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

#ifdef BUILD_PYTHON_COMPONENTS
#include <tulip/PythonInterpreter.h>
#include <tulip/APIDataBase.h>
#include "PythonPanel.h"
#include "PythonPluginsIDE.h"
#endif

#include "GraphPerspective.h"
#include "DocumentationNavigator.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QCloseEvent>
#include <QMessageBox>
#include <QClipboard>
#include <QDropEvent>
#include <QUrl>
#include <QMimeData>

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

GraphPerspective::GraphPerspective(const tlp::PluginContext* c): Perspective(c), _ui(NULL), _graphs(new GraphHierarchiesModel(this)), _recentDocumentsSettingsKey("perspective/recent_files"), _logger(NULL) {
  Q_INIT_RESOURCE(GraphPerspective);

  if (c && ((PerspectiveContext *) c)->parameters.contains("gui_testing")) {
    tlp::setGuiTestingMode(true);
    // we must ensure that choosing a file is relative to
    // the current directory to allow to run the gui tests
    // from any relative unit_test/gui directory
    _lastOpenLocation = QDir::currentPath();
  }
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
  foreach(QString s, TulipSettings::instance().recentDocuments()) {
    if (!QFileInfo(s).exists())
      continue;

    _ui->menuOpen_recent_file->addAction(QIcon(":/tulip/graphperspective/icons/16/archive.png"),s,this,SLOT(openRecentFile()));
  }
  _ui->menuOpen_recent_file->addSeparator();
  foreach(QString s, TulipSettings::instance().value(_recentDocumentsSettingsKey).toStringList()) {
    if (!QFileInfo(s).exists())
      continue;

    _ui->menuOpen_recent_file->addAction(QIcon(":/tulip/graphperspective/icons/16/empty-file.png"),s,this,SLOT(openRecentFile()));
  }
}

void GraphPerspective::addRecentDocument(const QString& path) {
  QStringList recents = TulipSettings::instance().value(_recentDocumentsSettingsKey).toStringList();

  if (recents.contains(path))
    return;

  recents += path;

  if (recents.size()>10)
    recents.pop_front();

  TulipSettings::instance().setValue(_recentDocumentsSettingsKey,recents);
  TulipSettings::instance().sync();
  buildRecentDocumentsMenu();
}

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))

void graphPerspectiveLogger(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
  std::cerr << msg.toStdString() << std::endl;
  static_cast<GraphPerspective*>(Perspective::instance())->log(type, context, msg);
}

void GraphPerspective::log(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
  _logger->log(type, context, msg);
  _ui->loggerIcon->setPixmap(_logger->icon());
  _ui->loggerMessage->setText(QString::number(_logger->count()));
}

#else

void graphPerspectiveLogger(QtMsgType type, const char* msg) {
  static_cast<GraphPerspective*>(Perspective::instance())->log(type,msg);
}

void GraphPerspective::log(QtMsgType type, const char* msg) {
  _logger->log(type,msg);
  _ui->loggerIcon->setPixmap(_logger->icon());
  _ui->loggerMessage->setText(QString::number(_logger->count()));
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

void GraphPerspective::logCleared() {
  _ui->loggerMessage->setText("");
  _ui->loggerIcon->setPixmap(QPixmap());
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

  if(obj == _mainWindow && ev->type() == QEvent::Close) {
    if(_graphs->needsSaving()) {
      QMessageBox::StandardButton answer = QMessageBox::question(_mainWindow, trUtf8("Save"), trUtf8("The project has been modified, do you want to save your changes ?"),QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel | QMessageBox::Escape);

      if ((answer == QMessageBox::Yes && !save()) ||
          (answer == QMessageBox::Cancel)) {
        ev->ignore();
        return true;
      }
    }
  }

  return false;
}

void GraphPerspective::showLogger() {
  if (_logger->count()==0)
    return;

  QPoint pos = _mainWindow->mapToGlobal(_ui->loggerFrame->pos());
  pos.setX(pos.x()+_ui->loggerFrame->width());
  pos.setY(std::min<int>(_mainWindow->mapToGlobal(_mainWindow->pos()).y()+mainWindow()->height()-_logger->height(),pos.y()));
  _logger->move(pos);
  _logger->show();
}

void GraphPerspective::redrawPanels(bool center) {
  _ui->workspace->redrawPanels(center);
}

void GraphPerspective::start(tlp::PluginProgress *progress) {
  reserveDefaultProperties();
  _ui = new Ui::GraphPerspectiveMainWindowData;
  _ui->setupUi(_mainWindow);
#ifdef BUILD_PYTHON_COMPONENTS
  _pythonPanel = new PythonPanel();
  QVBoxLayout *layout = new QVBoxLayout();
  layout->addWidget(_pythonPanel);
  layout->setContentsMargins(0,0,0,0);
  _ui->pythonPanel->setLayout(layout);
  _developFrame = new PythonPluginsIDE();
  layout = new QVBoxLayout();
  layout->addWidget(_developFrame);
  layout->setContentsMargins(0,0,0,0);
  _ui->developFrame->setLayout(layout);
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
  connect(_logger,SIGNAL(cleared()),this,SLOT(logCleared()));

  // redirection of various output
  redirectDebugOutputToQDebug();
  redirectWarningOutputToQWarning();
  redirectErrorOutputToQCritical();

  TulipSettings::instance().synchronizeViewSettings();

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
  qInstallMessageHandler(graphPerspectiveLogger);
#else
  qInstallMsgHandler(graphPerspectiveLogger);
#endif

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
  connect(_ui->actionInvert_selection,SIGNAL(triggered()),this,SLOT(invertSelection()));
  connect(_ui->actionCancel_selection,SIGNAL(triggered()),this,SLOT(cancelSelection()));
  connect(_ui->actionSelect_All,SIGNAL(triggered()),this,SLOT(selectAll()));
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
  connect(_ui->workspaceButton, SIGNAL(clicked()), this, SLOT(setWorkspaceMode()));
  connect(_ui->action_Close_All, SIGNAL(triggered()), _ui->workspace, SLOT(closeAll()));

  // Agent actions
  connect(_ui->actionPlugins_Center,SIGNAL(triggered()),this,SLOT(showPluginsCenter()));
  connect(_ui->actionAbout_us,SIGNAL(triggered()),this,SLOT(showAboutPage()));

  if (DocumentationNavigator::hasDocumentation())
    connect(_ui->actionShowDocumentation,SIGNAL(triggered()),this,SLOT(showDocumentation()));
  else
    _ui->actionShowDocumentation->setVisible(false);

  // Setting initial sizes for splitters
  _ui->mainSplitter->setSizes(QList<int>() << 350 << 850);
  _ui->mainSplitter->setStretchFactor(0,0);
  _ui->mainSplitter->setStretchFactor(1,1);
  _ui->mainSplitter->setCollapsible(1, false);

  // Open project with model
  QMap<QString,tlp::Graph*> rootIds;

  if(!_project->projectFile().isEmpty()) {
    rootIds = _graphs->readProject(_project,progress);
  }

  // these ui initializations are needed here
  // in case of a call to showStartPanels in the open method
  _ui->graphHierarchiesEditor->setModel(_graphs);
  _ui->workspace->setModel(_graphs);
  _ui->workspace->readProject(_project,rootIds,progress);
  _ui->searchPanel->setModel(_graphs);

#ifdef BUILD_PYTHON_COMPONENTS
  connect(_ui->pythonButton,SIGNAL(clicked(bool)),this,SLOT(setPythonPanel(bool)));
  connect(_ui->developButton, SIGNAL(clicked()), this, SLOT(setDevelopMode()));
  _pythonPanel->setModel(_graphs);
  _developFrame->setProject(_project);
  _pythonPanel->setPanelButton(_ui->pythonButton);
  tlp::PluginLister::instance()->addListener(this);
  PythonInterpreter::getInstance()->setDefaultConsoleWidget(_pythonPanel->consoleWidget());

  APIDataBase::getInstance()->loadApiFile(tlpStringToQString(tlp::TulipShareDir) + "/apiFiles/tulip.api");
  APIDataBase::getInstance()->loadApiFile(tlpStringToQString(tlp::TulipShareDir) + "/apiFiles/Python-" + PythonInterpreter::getInstance()->getPythonVersionStr() + ".api");
  APIDataBase::getInstance()->loadApiFile(tlpStringToQString(tlp::TulipShareDir) + "/apiFiles/tulipogl.api");
  APIDataBase::getInstance()->loadApiFile(tlpStringToQString(tlp::TulipShareDir) + "/apiFiles/tulipgui.api");

  PythonInterpreter::getInstance()->setOutputEnabled(false);

  if (PythonInterpreter::getInstance()->runString("import PyQt4")) {
    APIDataBase::getInstance()->loadApiFile(tlpStringToQString(tlp::TulipShareDir) + "/apiFiles/PyQt4.api");
  }

  PythonInterpreter::getInstance()->setOutputEnabled(true);
#endif

  if (!_externalFile.isEmpty() && QFileInfo(_externalFile).exists()) {
    open(_externalFile);
  }

  foreach(HeaderFrame *h, _ui->docksSplitter->findChildren<HeaderFrame *>()) {
    connect(h,SIGNAL(expanded(bool)),this,SLOT(refreshDockExpandControls()));
  }

#if !defined(__APPLE__) && !defined(_WIN32)
  // Hide plugins center when not on MacOS or Windows
  _ui->pluginsButton->hide();
  _ui->menuHelp->removeAction(_ui->actionPlugins_Center);
#endif

  // fill menu with recent documents
  buildRecentDocumentsMenu();

  showTrayMessage("GraphPerspective started");
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
  wizard.setWindowTitle(QString("Export of graph \"") + g->getName().c_str() + '"');

  if (wizard.exec() != QDialog::Accepted || wizard.algorithm().isNull() || wizard.outputFile().isEmpty())
    return;

  std::ostream *os;
  std::string filename = (exportFile = wizard.outputFile()).toUtf8().data();
  std::string exportPluginName = wizard.algorithm().toStdString();

  if (filename.rfind(".gz") == (filename.length() - 3))
    os = tlp::getOgzstream(filename);
  else {
    if (exportPluginName == "TLPB Export")
      os = tlp::getOutputFileStream(filename,
                                    std::ios::out | std::ios::binary);
    else
      os = tlp::getOutputFileStream(filename);
  }

  if (os->fail()) {
    QMessageBox::critical(_mainWindow,trUtf8("File error"),trUtf8("Cannot open output file for writing: ") + wizard.outputFile());
    delete os;
    return;
  }

  DataSet data = wizard.parameters();
  PluginProgress* prg = progress(NoProgressOption);
  prg->setTitle(exportPluginName);
  bool result = tlp::exportGraph(g,*os,exportPluginName,data,prg);
  delete os;

  if (!result) {
    QMessageBox::critical(_mainWindow,trUtf8("Export error"),trUtf8("Failed to export to format") + wizard.algorithm());
  }
  else {
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
  QString filter("TLP (*.tlp *.tlp.gz)");
  std::string filename =
    QFileDialog::getSaveFileName(_mainWindow, tr("Save graph hierarchy in tlp file"),
                                 savedFile, filter).toUtf8().data();

  if(!filename.empty()) {
    std::ostream *os;

    if (filename.rfind(".tlp.gz") == (filename.length() - 7))
      os = tlp::getOgzstream(filename);
    else {
      if (filename.rfind(".tlp") != (filename.length() - 4))
        // force file extension
        filename += ".tlp";

      os = tlp::getOutputFileStream(filename);
    }

    if (os->fail()) {
      QMessageBox::critical(_mainWindow,trUtf8("File error"),trUtf8("Cannot open output file for writing: ") + QString::fromUtf8(filename.c_str()));
      delete os;
      return;
    }

    // remember for the next call
    savedFile = QString::fromUtf8(filename.c_str());

    DataSet params;
    params.set("file", filename);
    bool result = tlp::exportGraph(g, *os, "TLP Export", params);

    if (!result)
      QMessageBox::critical(_mainWindow,trUtf8("Save error"),trUtf8("Failed to save graph hierarchy"));
    else
      addRecentDocument(savedFile);

    delete os;
  }
}

void GraphPerspective::importGraph(const std::string& module,
                                   DataSet& data) {
  Graph* g;

  if (!module.empty()) {
    PluginProgress* prg = progress(NoProgressOption);
    prg->setTitle(module);
    g = tlp::importGraph(module,data,prg);

    if (g == NULL) {
      QMessageBox::critical(_mainWindow,trUtf8("Import error"),
                            QString("<i>") + module.c_str() + trUtf8("</i> failed to import data.<br/><br/><b>") + tlp::tlpStringToQString(prg->getError()) + "</b>");
      delete prg;
      return;
    }

    delete prg;
    std::string name;

    if (!g->getAttribute<std::string>("name", name)) {
      QString n = tlp::tlpStringToQString(module) + " - " + tlp::tlpStringToQString(data.toString());
      n.replace(QRegExp("[\\w]*::"),""); // remove words before "::"
      g->setName(tlp::QStringToTlpString(n));
    }
  }
  else {
    g = tlp::newGraph();
  }

  _graphs->addGraph(g);
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
  if (path.isEmpty()) {
    QString path = QFileDialog::getSaveFileName(_mainWindow,trUtf8("Save project"),QString(),"Tulip Project (*.tlpx)");

    if (!path.isEmpty()) {
      if (!path.endsWith(".tlpx"))
        path+=".tlpx";

      return saveAs(path);
    }

    return false;
  }

  SimplePluginProgressDialog progress(_mainWindow);
  progress.showPreview(false);
  progress.show();
  QMap<Graph*,QString> rootIds = _graphs->writeProject(_project,&progress);
  _ui->workspace->writeProject(_project,rootIds,&progress);
  _project->write(path,&progress);
  TulipSettings::instance().addToRecentDocuments(path);

  return true;
}

void GraphPerspective::open(QString fileName) {
  QMap<std::string, std::string> modules;
  std::list<std::string> imports = PluginLister::instance()->availablePlugins<ImportModule>();

  std::string filters("Tulip project (*.tlpx);;");
  std::string filterAny("Any supported format (");

  for(std::list<std::string>::const_iterator it = imports.begin(); it != imports.end(); ++it) {
    ImportModule* m = PluginLister::instance()->getPluginObject<ImportModule>(*it, NULL);
    std::list<std::string> fileExtension(m->fileExtensions());

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

  if (fileName.isNull()) // If open() was called without a parameter, open the file dialog
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
        params.set("file::filename", std::string(fileName.toUtf8().data()));
        addRecentDocument(fileName);
        importGraph(modules[extension], params);
        QDir::setCurrent(QFileInfo(fileName.toUtf8().data()).absolutePath());
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
#ifdef BUILD_PYTHON_COMPONENTS
    _developFrame->setProject(_project);
#endif

    for (QMap<QString,tlp::Graph*>::iterator it = rootIds.begin(); it != rootIds.end() ; ++it) {
      it.value()->setAttribute("file", std::string(path.toUtf8().data()));
    }

    delete prg;
  }
  else {
    Perspective::openProjectFile(path);
  }
}

void GraphPerspective::deleteSelectedElements() {
  Observable::holdObservers();
  tlp::Graph* graph = _graphs->currentGraph();
  tlp::BooleanProperty* selection = graph->getProperty<BooleanProperty>("viewSelection");

  graph->push();
  tlp::Iterator<edge>* itEdges = selection->getEdgesEqualTo(true);
  graph->delEdges(itEdges, false);
  delete itEdges;

  tlp::Iterator<node>* itNodes = selection->getNodesEqualTo(true);
  graph->delNodes(itNodes, false);
  delete itNodes;

  Observable::unholdObservers();
}

void GraphPerspective::invertSelection() {
  Observable::holdObservers();
  tlp::Graph* graph = _graphs->currentGraph();
  tlp::BooleanProperty* selection = graph->getProperty<BooleanProperty>("viewSelection");
  graph->push();
  selection->reverse();
  Observable::unholdObservers();
}

void GraphPerspective::cancelSelection() {
  Observable::holdObservers();
  tlp::Graph* graph = _graphs->currentGraph();
  tlp::BooleanProperty* selection = graph->getProperty<BooleanProperty>("viewSelection");
  graph->push();
  selection->setAllEdgeValue(false);
  selection->setAllNodeValue(false);
  Observable::unholdObservers();
}

void GraphPerspective::selectAll() {
  Observable::holdObservers();
  tlp::Graph* graph = _graphs->currentGraph();
  tlp::BooleanProperty* selection = graph->getProperty<BooleanProperty>("viewSelection");
  graph->push();

  if (graph == graph->getRoot()) {
    selection->setAllEdgeValue(true);
    selection->setAllNodeValue(true);
  }
  else {
    node n;
    forEach(n, graph->getNodes()) {
      selection->setNodeValue(n,true);
    }
    edge e;
    forEach(e, graph->getEdges()) {
      selection->setEdgeValue(e,true);
    }
  }

  Observable::unholdObservers();
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
  std::set<node> groupedNodes;
  node n;
  forEach(n, selection->getNodesEqualTo(true)) {
    if(graph->isElement(n))
      groupedNodes.insert(n);
  }

  if (groupedNodes.empty()) {
    Observable::unholdObservers();
    qCritical() << trUtf8("[Group] Cannot create meta-nodes from empty selection").toUtf8().data();
    return;
  }

  graph->push();

  bool changeGraph=false;

  if (graph == graph->getRoot()) {
    qWarning() << trUtf8("[Group] Grouping can not be done on the root graph. A subgraph has automatically been created").toUtf8().data();
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

Graph *GraphPerspective::createSubGraph(Graph *graph) {
  if (graph == NULL)
    return NULL;

  graph->push();

  Observable::holdObservers();

  tlp::BooleanProperty* selection = graph->getProperty<BooleanProperty>("viewSelection");
  edge e;
  forEach(e,selection->getEdgesEqualTo(true)) {
    const pair<node, node> &ends = graph->ends(e);

    if (!selection->getNodeValue(ends.first)) {
#ifndef NDEBUG
      qDebug() << trUtf8("[Create subgraph] node #") << QString::number(ends.first.id) << trUtf8(" source of edge #") << QString::number(e.id) << trUtf8(" automatically added to selection.");
#endif
      selection->setNodeValue(ends.first,true);
    }

    if (!selection->getNodeValue(ends.second)) {
#ifndef NDEBUG
      qDebug() << trUtf8("[Create subgraph] node #") << QString::number(ends.second.id) << trUtf8(" target of edge #") << QString::number(e.id) << trUtf8(" automatically added to selection.");
#endif
      selection->setNodeValue(ends.second,true);
    }
  }
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
  _ui->actionInvert_selection->setEnabled(enabled);
  _ui->actionSelect_All->setEnabled(enabled);
  _ui->actionCancel_selection->setEnabled(enabled);
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
  _ui->previousPageButton->setVisible(enabled);
  _ui->pageCountLabel->setVisible(enabled);
  _ui->nextPageButton->setVisible(enabled);

  if (graph == NULL) {
    _ui->workspace->switchToStartupMode();
    _ui->exposeModeButton->setChecked(false);
    _ui->searchButton->setChecked(false);
    _ui->pythonButton->setChecked(false);
    setSearchOutput(false);
  }
  else {
    _ui->workspace->setGraphForFocusedPanel(graph);
  }
}

void GraphPerspective::CSVImport() {
  bool mustDeleteGraph = false;

  if (_graphs->size()==0) {
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
  foreach(QString panelName, QStringList() << "Spreadsheet view" << "Node Link Diagram view") {
    View* view = PluginLister::instance()->getPluginObject<View>(panelName.toStdString(),NULL);

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
  Iterator<node>* it = viewLayout->getNonDefaultValuatedNodes();

  if (!it->hasNext()) {
    std::string str;
    g->applyPropertyAlgorithm("Random layout", viewLayout, str);
  }

  delete it;

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
  open(action->text());
}

void GraphPerspective::treatEvent(const tlp::Event &ev) {
  if (dynamic_cast<const tlp::PluginEvent*>(&ev)) {
    pluginsListChanged();
  }
}

void GraphPerspective::setWorkspaceMode() {
  _ui->workspaceButton->setChecked(true);
  _ui->developButton->setChecked(false);
  _ui->centralWidget->setCurrentIndex(0);
}

void GraphPerspective::setDevelopMode() {
  _ui->workspaceButton->setChecked(false);
  _ui->developButton->setChecked(true);
  _ui->centralWidget->setCurrentIndex(1);
}

void GraphPerspective::showDocumentation() {
  DocumentationNavigator::showDocumentation();
}

PLUGIN(GraphPerspective)
