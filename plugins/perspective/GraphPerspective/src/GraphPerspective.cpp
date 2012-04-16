/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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
#include "GraphPerspective.h"

#include <QtGui/QMessageBox>
#include <QtGui/QFileDialog>
#include <QtGui/QCloseEvent>
#include <QtGui/QMessageBox>

#include <tulip/ImportModule.h>
#include <tulip/Graph.h>
#include <tulip/View.h>
#include <tulip/SimplePluginProgressWidget.h>
#include <tulip/GraphHierarchiesModel.h>
#include <tulip/CSVImportWizard.h>
#include <tulip/GraphModel.h>
#include <tulip/GraphTableItemDelegate.h>
#include <tulip/GraphPropertiesModel.h>

#include "ui_GraphPerspectiveMainWindow.h"

#include "GraphPerspectiveLogger.h"
#include "ImportWizard.h"
#include "PanelSelectionWizard.h"
#include "GraphHierarchiesEditor.h"
#include "ShadowFilter.h"

#ifndef NDEBUG
#include <modeltest.h>
#endif

#include <QtCore/QDebug>

using namespace tlp;

QSet<QString> GraphPerspective::RESERVED_PROPERTIES = QSet<QString>();

GraphPerspective::GraphPerspective(const tlp::PluginContext* c): Perspective(c), _ui(0), _graphs(new GraphHierarchiesModel(this)), _logger(NULL) {
#ifndef NDEBUG
  new ModelTest(_graphs,this);
#endif /* NDEBUG */
  Q_INIT_RESOURCE(GraphPerspective);
}

void GraphPerspective::registerReservedProperty(QString s) {
  RESERVED_PROPERTIES.insert(s);
}
bool GraphPerspective::isReservedPropertyName(QString s) {
  return RESERVED_PROPERTIES.contains(s);
}
void GraphPerspective::reserveDefaultProperties() {
  registerReservedProperty("viewColor");
  registerReservedProperty("viewLabelColor");
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

void graphPerspectiveLogger(QtMsgType type, const char* msg) {
  static_cast<GraphPerspective*>(Perspective::instance())->log(type,msg);
}
void GraphPerspective::log(QtMsgType type, const char* msg) {
  _ui->loggerFrame->setVisible(true);
  _logger->log(type,msg);
  _ui->loggerIcon->setPixmap(_logger->icon());
  _ui->loggerMessage->setText(QString::number(_logger->count()));
}

GraphPerspective::~GraphPerspective() {
  qInstallMsgHandler(0);
}

void GraphPerspective::logCleared() {
  _ui->loggerFrame->setVisible(false);
}

bool GraphPerspective::eventFilter(QObject* obj, QEvent* ev) {
  if (obj == _ui->loggerFrame && ev->type() == QEvent::MouseButtonPress)
    showLogger();

  if(obj == _mainWindow && ev->type() == QEvent::Close) {
    if(_graphs->needsSaving()) {
      QMessageBox::StandardButton answer = QMessageBox::question(_mainWindow, trUtf8("Save"), trUtf8("The project has been modified, do you want to save your changes ?"),
                                           QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel | QMessageBox::Escape);

      if(answer == QMessageBox::Yes) {
        save();
      }
      else if(answer == QMessageBox::Cancel) {
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

void GraphPerspective::start(tlp::PluginProgress *progress) {
  reserveDefaultProperties();
  _ui = new Ui::GraphPerspectiveMainWindowData;
  _ui->setupUi(_mainWindow);
  _ui->loggerFrame->setVisible(false);
  _logger = new GraphPerspectiveLogger(_mainWindow);
  _ui->loggerFrame->installEventFilter(this);
  _mainWindow->installEventFilter(this);
  connect(_logger,SIGNAL(cleared()),this,SLOT(logCleared()));

  qInstallMsgHandler(graphPerspectiveLogger);
  _mainWindow->installEventFilter(new ShadowFilter(this));
  connect(_ui->workspace,SIGNAL(addPanelRequest(tlp::Graph*)),this,SLOT(createPanel(tlp::Graph*)));
  connect(_graphs,SIGNAL(currentGraphChanged(tlp::Graph*)),this,SLOT(currentGraphChanged(tlp::Graph*)));

  // Dataset mode delegates and models
  QSortFilterProxyModel* nodesModel = new GraphSortFilterProxyModel;
  nodesModel->setSourceModel(new NodesGraphModel);
  nodesModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
  nodesModel->setFilterKeyColumn(-1);
  QSortFilterProxyModel* edgesModel = new GraphSortFilterProxyModel;
  edgesModel->setSourceModel(new EdgesGraphModel);
  edgesModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
  edgesModel->setFilterKeyColumn(-1);
  _ui->nodesTable->setModel(nodesModel);
  _ui->edgesTable->setModel(edgesModel);
  _ui->nodesTable->setItemDelegate(new GraphTableItemDelegate);
  _ui->edgesTable->setItemDelegate(new GraphTableItemDelegate);
  connect(_ui->nodesTable, SIGNAL(destroyed()), _ui->nodesTable->itemDelegate(), SLOT(deleteLater()));
  connect(_ui->edgesTable, SIGNAL(destroyed()), _ui->edgesTable->itemDelegate(), SLOT(deleteLater()));

  _ui->nodesTable->horizontalHeader()->setMovable(true);
  _ui->edgesTable->horizontalHeader()->setMovable(true);

  // Connect actions
  connect(_ui->actionMessages_log,SIGNAL(triggered()),this,SLOT(showLogger()));
  connect(_ui->actionFull_screen,SIGNAL(triggered(bool)),this,SLOT(showFullScreen(bool)));
  connect(_ui->actionImport,SIGNAL(triggered()),this,SLOT(importGraph()));
  connect(_ui->workspace,SIGNAL(panelFocused(tlp::View*)),this,SLOT(panelFocused(tlp::View*)));
  connect(_ui->actionSave_Project,SIGNAL(triggered()),this,SLOT(save()));
  connect(_ui->actionSave_Project_as,SIGNAL(triggered()),this,SLOT(saveAs()));
  connect(_ui->actionOpen_Project,SIGNAL(triggered()),this,SLOT(open()));
  connect(_ui->actionAnalyze,SIGNAL(triggered()),this,SLOT(modeSwitch()));
  connect(_ui->actionCharts,SIGNAL(triggered()),this,SLOT(modeSwitch()));
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
  connect(_ui->actionImport_CSV,SIGNAL(triggered()),this,SLOT(CSVImport()));
  connect(_ui->datasetFilterEdit,SIGNAL(editingFinished()),this,SLOT(setDatasetFilter()));
  connect(_ui->datasetPropertiesFilter,SIGNAL(currentIndexChanged(int)),this,SLOT(setDatasetFilterProperty()));
  connect(_ui->nodesTable->horizontalHeader(),SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(datasetHeaderContextMenuRequested(QPoint)));
  connect(_ui->edgesTable->horizontalHeader(),SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(datasetHeaderContextMenuRequested(QPoint)));
  _ui->nodesTable->horizontalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
  _ui->edgesTable->horizontalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(_ui->datasetSelectedToggle,SIGNAL(clicked(bool)),this,SLOT(setDatasetSelectedOnly(bool)));

  // D-BUS actions
  connect(_ui->actionPlugins_Center,SIGNAL(triggered()),this,SIGNAL(showTulipPluginsCenter()));
  connect(_ui->actionAbout_us,SIGNAL(triggered()),this,SIGNAL(showTulipAboutPage()));

  // Setting initial sizes for splitters
  _ui->mainSplitter->setSizes(QList<int>() << 200 << 1000);
  _ui->edgesTable->setVisible(false);

  _mainWindow->show();
  // Open project with model
  QMap<QString,tlp::Graph*> rootIds = _graphs->readProject(_project,progress);

  if (!_externalFile.isEmpty()) {
    QFileInfo externalFileInfo(_externalFile);

    if (externalFileInfo.exists()) {
      progress->setComment((trUtf8("Loading ") + externalFileInfo.fileName()).toStdString());
      DataSet dataSet;
      dataSet.set("file::filename", externalFileInfo.absoluteFilePath().toStdString());
      Graph *externalGraph = tlp::importGraph("TLP Import", dataSet, progress);
      _graphs->addGraph(externalGraph);
    }
  }

  _ui->filtersManager->findChild<HeaderFrame *>()->expand(false);
  _ui->graphHierarchiesEditor->setModel(_graphs);
  _ui->algorithmRunner->setModel(_graphs);
  _ui->workspace->setModel(_graphs);
  _ui->workspace->readProject(_project,rootIds,progress);
  _ui->propertiesEditor->setModel(_graphs);

  foreach(HeaderFrame *h, _ui->docksSplitter->findChildren<HeaderFrame *>()) {
    connect(h,SIGNAL(expanded(bool)),this,SLOT(refreshDockExpandControls()));
  }

  delete progress;
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

void GraphPerspective::showFullScreen(bool f) {
  if (f) {
    _maximised = _mainWindow->isMaximized();
    _mainWindow->showFullScreen();
  }
  else {
    _mainWindow->showNormal();

    if (_maximised)
      _mainWindow->showMaximized();
  }
}

void GraphPerspective::importGraph() {
  ImportWizard wizard(_mainWindow);

  if (wizard.exec() == QDialog::Accepted) {
    Graph* g = NULL;

    if (!wizard.algorithm().isNull()) {
      DataSet data = wizard.parameters();
      PluginProgress* prg = progress();
      g = tlp::importGraph(wizard.algorithm().toStdString(),data,prg);
      delete prg;

      if (g == NULL) {
        QMessageBox::critical(_mainWindow,trUtf8("Import error"),wizard.algorithm() + trUtf8(" failed to import data"));
        return;
      }

      std::string name;

      if (!g->getAttribute<std::string>("name",name)) {
        name = (wizard.algorithm() + " - " + wizard.parameters().toString().c_str()).toStdString();
        g->setAttribute<std::string>("name",name);
      }
    }
    else {
      g = newGraph();
    }

    _graphs->addGraph(g);
    _ui->graphHierarchiesEditor->repackHeaders();
  }
}

void GraphPerspective::createPanel(tlp::Graph* g) {
  if (_graphs->size() == 0)
    return;

  PanelSelectionWizard wizard(_graphs,_mainWindow);

  if (g != NULL)
    wizard.setSelectedGraph(g);
  else
    wizard.setSelectedGraph(_graphs->currentGraph());


  int result = wizard.exec();

  if (result == QDialog::Accepted && wizard.panel() != NULL) {
    _ui->workspace->addPanel(wizard.panel());
    _ui->workspace->setActivePanel(wizard.panel());
  }
}

void GraphPerspective::panelFocused(tlp::View* view) {
  disconnect(this,SLOT(focusedPanelGraphSet(tlp::Graph*)));

  if (!_ui->graphHierarchiesEditor->synchronized())
    return;

  connect(view,SIGNAL(graphSet(tlp::Graph*)),this,SLOT(focusedPanelGraphSet(tlp::Graph*)));
  focusedPanelGraphSet(view->graph());
}
void GraphPerspective::focusedPanelGraphSet(Graph* g) {
  _graphs->setCurrentGraph(g);
}

void GraphPerspective::save() {
  saveAs(_project->projectFile());
}

void GraphPerspective::saveAs(const QString& path) {
  if (path.isEmpty()) {
    QString path = QFileDialog::getSaveFileName(_mainWindow,trUtf8("Save file"),QString(),"Tulip Project (*.tlpx)");

    if (!path.isEmpty()) {
      saveAs(path);
    }

    return;
  }

  SimplePluginProgressDialog progress(_mainWindow);
  progress.show();
  QMap<Graph*,QString> rootIds = _graphs->writeProject(_project,&progress);
  _ui->workspace->writeProject(_project,rootIds,&progress);
  _project->write(path,&progress);
}

void GraphPerspective::open(const QString &/*path*/) {
}

void GraphPerspective::centerPanels(tlp::PropertyInterface* pi) {
  foreach(tlp::View* view, _ui->workspace->panels()) {
    if (view->isLayoutProperty(pi))
      view->centerView();
  }
}

void GraphPerspective::modeSwitch() {
  QWidget* mode = NULL;

  if (sender() == _ui->actionCharts) {
    setDatasetGraph(_graphs->currentGraph());
    mode = _ui->datasetModePage;
  }
  else if (sender() == _ui->actionAnalyze) {
    setDatasetGraph(NULL);
    mode = _ui->workspace;
  }

  if (mode != NULL)
    _ui->centralWidget->setCurrentWidget(mode);
}

void GraphPerspective::deleteSelectedElements() {
  Observable::holdObservers();
  tlp::Graph* graph = _graphs->currentGraph();
  tlp::BooleanProperty* selection = graph->getProperty<BooleanProperty>("viewSelection");

  tlp::Iterator<node>* itNodes = selection->getNodesEqualTo(true);
  graph->delNodes(itNodes, false);
  delete itNodes;

  tlp::Iterator<edge>* itEdges = selection->getEdgesEqualTo(true);
  graph->delEdges(itEdges, false);
  delete itEdges;

  Observable::unholdObservers();
}

void GraphPerspective::invertSelection() {
  Observable::holdObservers();
  tlp::Graph* graph = _graphs->currentGraph();
  tlp::BooleanProperty* selection = graph->getProperty<BooleanProperty>("viewSelection");
  selection->reverse();
  Observable::unholdObservers();
}

void GraphPerspective::cancelSelection() {
  Observable::holdObservers();
  tlp::Graph* graph = _graphs->currentGraph();
  tlp::BooleanProperty* selection = graph->getProperty<BooleanProperty>("viewSelection");
  selection->setAllEdgeValue(false);
  selection->setAllNodeValue(false);
  Observable::unholdObservers();
}

void GraphPerspective::selectAll() {
  Observable::holdObservers();
  tlp::Graph* graph = _graphs->currentGraph();
  tlp::BooleanProperty* selection = graph->getProperty<BooleanProperty>("viewSelection");
  selection->setAllEdgeValue(true);
  selection->setAllNodeValue(true);
  Observable::unholdObservers();
}

void GraphPerspective::undo() {
  Observable::holdObservers();
  tlp::Graph* graph = _graphs->currentGraph();
  graph->pop();
  Observable::unholdObservers();
}

void GraphPerspective::redo() {
  Observable::holdObservers();
  tlp::Graph* graph = _graphs->currentGraph();
  graph->unpop();
  Observable::unholdObservers();
}

void GraphPerspective::cut() {
  //TODO implement me
}

void GraphPerspective::paste() {
  //TODO implement me
}

void GraphPerspective::copy() {
  //TODO implement me
}

void GraphPerspective::group() {
  Observable::holdObservers();
  tlp::Graph* graph = _graphs->currentGraph();
  tlp::BooleanProperty* selection = graph->getProperty<BooleanProperty>("viewSelection");
  std::set<node> groupedNodes;
  node n;
  forEach(n, selection->getNodesEqualTo(true))
  groupedNodes.insert(n);

  if (groupedNodes.empty()) {
    qCritical() << trUtf8("[Group] Cannot create meta-nodes from empty selection");
    return;
  }

  bool changeGraph=false;

  if (graph == graph->getRoot()) {
    qWarning() << trUtf8("[Group] Grouping can not be done on the root graph. A subgraph has automatically been created");
    graph = graph->addCloneSubGraph("groups");
    changeGraph = true;
  }

  graph->createMetaNode(groupedNodes);

  if (!changeGraph)
    return;

  foreach(View* v, _ui->workspace->panels()) {
    if (v->graph() == graph->getRoot())
      v->setGraph(graph);
  }
  Observable::unholdObservers();
}

void GraphPerspective::createSubGraph() {
  Observable::holdObservers();

  tlp::Graph* graph = _graphs->currentGraph();
  tlp::BooleanProperty* selection = graph->getProperty<BooleanProperty>("viewSelection");
  edge e;
  forEach(e,selection->getEdgesEqualTo(true)) {
    node src = graph->source(e);
    node tgt = graph->target(e);

    if (!selection->getNodeValue(src))
      qDebug() << trUtf8("[Create subgraph] Source n") << src.id << trUtf8(" of e") << e.id << trUtf8(" automatically added to selection.");

    if (!selection->getNodeValue(tgt))
      qDebug() << trUtf8("[Create subgraph] Target n") << tgt.id << trUtf8(" of e") << e.id << trUtf8(" automatically added to selection.");

    selection->setNodeValue(src,true);
    selection->setNodeValue(tgt,true);
  }
  graph->addSubGraph(selection);
  Observable::unholdObservers();
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

  if (_ui->centralWidget->currentWidget() == _ui->datasetModePage)
    setDatasetGraph(graph);

  if (graph != NULL) {
    std::string name;
    graph->getAttribute<std::string>("name",name);
    _ui->currentGraphLabel->setText(name.c_str());
  }

  _ui->currentGraphFrame->setVisible(enabled);
}

void GraphPerspective::setDatasetGraph(Graph* graph) {
  NodesGraphModel* nodesModel = static_cast<NodesGraphModel*>(static_cast<QSortFilterProxyModel*>(_ui->nodesTable->model())->sourceModel());
  EdgesGraphModel* edgesModel = static_cast<EdgesGraphModel*>(static_cast<QSortFilterProxyModel*>(_ui->edgesTable->model())->sourceModel());
  nodesModel->setGraph(graph);
  edgesModel->setGraph(graph);
  _ui->datasetPropertiesFilter->setModel(new GraphPropertiesModel<PropertyInterface>(trUtf8("Select property"),graph));
}

void GraphPerspective::setDatasetFilter() {
  if (_ui->centralWidget->currentWidget() != _ui->datasetModePage)
    return;

  QString filter = _ui->datasetFilterEdit->text();
  static_cast<QSortFilterProxyModel*>(_ui->nodesTable->model())->setFilterFixedString(filter);
  static_cast<QSortFilterProxyModel*>(_ui->edgesTable->model())->setFilterFixedString(filter);
}

void GraphPerspective::setDatasetFilterProperty() {
  if (_ui->centralWidget->currentWidget() != _ui->datasetModePage || _ui->datasetPropertiesFilter->currentIndex() < 0)
    return;

  GraphSortFilterProxyModel* nodesModel = static_cast<GraphSortFilterProxyModel*>(_ui->nodesTable->model());
  GraphSortFilterProxyModel* edgesModel = static_cast<GraphSortFilterProxyModel*>(_ui->edgesTable->model());

  QVector<PropertyInterface*> properties;

  if (_ui->datasetPropertiesFilter->currentIndex() != 0)
    properties.push_back(_graphs->currentGraph()->getProperty(_ui->datasetPropertiesFilter->currentText().toStdString()));

  nodesModel->setProperties(properties);
  edgesModel->setProperties(properties);
  setDatasetFilter();
}

void GraphPerspective::CSVImport() {
  if (_graphs->currentGraph() == NULL)
    return;

  CSVImportWizard wizard(_mainWindow);
  wizard.setGraph(_graphs->currentGraph());
  _graphs->currentGraph()->push();
  Observable::holdObservers();
  int result = wizard.exec();

  if (result == QDialog::Rejected)
    _graphs->currentGraph()->pop();

  Observable::unholdObservers();
}

struct HeaderActionData {
  QTableView* _tableView;
  int _sectionId;

  HeaderActionData() {}
  HeaderActionData(QTableView* view, int section): _tableView(view), _sectionId(section) {}
};
Q_DECLARE_METATYPE(HeaderActionData)

void GraphPerspective::datasetHeaderContextMenuRequested(QPoint p) {
  QTableView* tableView = NULL;

  if (sender() == _ui->nodesTable->horizontalHeader())
    tableView = _ui->nodesTable;
  else if (sender() == _ui->edgesTable->horizontalHeader())
    tableView = _ui->edgesTable;

  QHeaderView* headerView = tableView->horizontalHeader();

  QMenu menu;
  QAction* showAllAction = menu.addAction(trUtf8("Show all"));
  showAllAction->setData(QVariant::fromValue<HeaderActionData>(HeaderActionData(tableView,0)));
  connect(showAllAction,SIGNAL(triggered()),this,SLOT(datasetHeaderShowAll()));
  QAction* hideAllAction = menu.addAction(trUtf8("Hide all except ") + tableView->model()->headerData(headerView->logicalIndexAt(p),Qt::Horizontal).toString());
  hideAllAction->setData(QVariant::fromValue<HeaderActionData>(HeaderActionData(tableView,headerView->logicalIndexAt(p))));

  connect(hideAllAction,SIGNAL(triggered()),this,SLOT(datasetHeaderHideAll()));
  menu.addSeparator();

  for (int i=0; i<tableView->model()->columnCount(); ++i) {
    QAction* a = menu.addAction(tableView->model()->headerData(i,Qt::Horizontal).toString());
    a->setCheckable(true);
    a->setChecked(!headerView->isSectionHidden(i));
    a->setData(QVariant::fromValue<HeaderActionData>(HeaderActionData(tableView,i)));
    connect(a,SIGNAL(triggered(bool)),this,SLOT(datasetHeaderSectionToggled(bool)));
  }

  menu.exec(headerView->viewport()->mapToGlobal(p));
}

void GraphPerspective::datasetHeaderSectionToggled(bool f) {
  QAction* action = static_cast<QAction*>(sender());
  HeaderActionData d = action->data().value<HeaderActionData>();
  d._tableView->horizontalHeader()->setSectionHidden(d._sectionId,!f);
}
void GraphPerspective::datasetHeaderHideAll() {
  QAction* action = static_cast<QAction*>(sender());
  QHeaderView* headerView = action->data().value<HeaderActionData>()._tableView->horizontalHeader();
  datasetHeaderSetAllVisible(headerView,false);
  headerView->setSectionHidden(action->data().value<HeaderActionData>()._sectionId,false);
}
void GraphPerspective::datasetHeaderShowAll() {
  datasetHeaderSetAllVisible(static_cast<QAction*>(sender())->data().value<HeaderActionData>()._tableView->horizontalHeader(),true);
}
void GraphPerspective::datasetHeaderSetAllVisible(QHeaderView* headerView, bool f) {
  for (int i=0; i<headerView->count(); ++i) {
    headerView->setSectionHidden(i,!f);
  }
}
void GraphPerspective::setDatasetSelectedOnly(bool f) {
  GraphSortFilterProxyModel* nodesModel = static_cast<GraphSortFilterProxyModel*>(_ui->nodesTable->model());
  GraphSortFilterProxyModel* edgesModel = static_cast<GraphSortFilterProxyModel*>(_ui->edgesTable->model());
  nodesModel->setSelectedOnly(f);
  edgesModel->setSelectedOnly(f);
}

PLUGIN(GraphPerspective)
