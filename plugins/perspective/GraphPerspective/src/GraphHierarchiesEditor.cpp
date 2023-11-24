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
#include <vector>
#include "GraphHierarchiesEditor.h"

#include <QDebug>
#include <QContextMenuEvent>
#include <QMenu>
#include <QGraphicsEffect>
#include <QPainter>
#include <QTextDocument>
#include <QToolButton>
#include <QSortFilterProxyModel>
#include <QMessageBox>

#include <tulip/Perspective.h>
#include <tulip/BooleanProperty.h>
#include <tulip/TulipMetaTypes.h>

#include "GraphPerspective.h"
#include <tulip/GraphHierarchiesModel.h>
#include "ui_GraphHierarchiesEditor.h"

using namespace tlp;

CustomTreeView::CustomTreeView(QWidget *parent) : QTreeView(parent) {
  header()->setSectionResizeMode(QHeaderView::ResizeToContents);
  connect(this, SIGNAL(collapsed(const QModelIndex &)), this, SLOT(resizeFirstColumnToContent()));
  connect(this, SIGNAL(expanded(const QModelIndex &)), this, SLOT(resizeFirstColumnToContent()));
}

int CustomTreeView::sizeHintForColumn(int col) const {
  if (!model() || col > 0)
    return -1;

  ensurePolished();
  int hint = 0;
  QModelIndex index = model()->index(0, col);

  while (index.isValid()) {
    if (viewport()->rect().contains(visualRect(index))) {
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
      auto w = itemDelegate(index)->sizeHint(viewOptions(), index).width();
#else
      QStyleOptionViewItem option;
      initViewItemOption(&option);
      auto w = itemDelegateForIndex(index)->sizeHint(option, index).width();
#endif
      hint = qMax(hint, visualRect(index).x() + w);
    }

    index = indexBelow(index);
  }

  return qMin(hint, viewport()->rect().width());
}

void CustomTreeView::scrollContentsBy(int dx, int dy) {
  if (dy != 0 && dx == 0) {
    resizeFirstColumnToContent();
  }

  QTreeView::scrollContentsBy(dx, dy);
}

void CustomTreeView::setModel(QAbstractItemModel *m) {
  QTreeView::setModel(m);
  resizeFirstColumnToContent();
}

void CustomTreeView::setAllHierarchyVisible(const QModelIndex &index, bool visible) {
  auto iModel = index.model();
  int childCount = iModel->rowCount(index);
  for (int i = 0; i < childCount; i++) {
    auto child = iModel->index(i, 0, index);
    // Recursively call the function for each child node.
    setAllHierarchyVisible(child, visible);
  }

  if (visible) {
    if (!isExpanded(index))
      expand(index);
  } else {
    if (isExpanded(index))
      collapse(index);
  }
}

void CustomTreeView::resizeFirstColumnToContent() {
  resizeColumnToContents(0);
}

GraphHierarchiesEditor::GraphHierarchiesEditor(QWidget *parent)
    : QWidget(parent), _ui(new Ui::GraphHierarchiesEditorData), _contextGraph(nullptr),
      _model(nullptr), _currentSelection(nullptr) {
  _ui->setupUi(this);
  _ui->hierarchiesTree->addAction(_ui->actionDelete_All);
  _ui->actionDelete_All->setShortcutContext(Qt::WidgetWithChildrenShortcut);

  QToolButton *linkButton = new QToolButton();
  linkButton->setObjectName("linkButton");
  linkButton->setIcon(QIcon(":/tulip/gui/icons/16/link.png"));
  linkButton->setToolTip("Click here to disable the synchronization with workspace active "
                         "panel.\nWhen synchronization is enabled, the graph currently "
                         "displayed\nin the active panel, becomes the current one in the Graphs "
                         "panel.");
  linkButton->setIconSize(QSize(22, 22));
  linkButton->setMinimumSize(25, 25);
  linkButton->setMaximumSize(25, 25);
  linkButton->setCheckable(true);
  linkButton->setChecked(true);
  _ui->header->insertWidget(linkButton);
  _linkButton = linkButton;
  connect(linkButton, SIGNAL(toggled(bool)), this, SLOT(toggleSynchronization(bool)));
  _ui->hierarchiesTree->installEventFilter(this);

  connect(_ui->hierarchiesTree, SIGNAL(clicked(const QModelIndex &)), this,
          SLOT(clicked(const QModelIndex &)));
}

bool GraphHierarchiesEditor::synchronized() const {
  return _linkButton->isChecked();
}

void GraphHierarchiesEditor::setModel(tlp::GraphHierarchiesModel *model) {
  _model = model;
  QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(_ui->hierarchiesTree);
  proxyModel->setSourceModel(model);
  proxyModel->setDynamicSortFilter(false);
  _ui->hierarchiesTree->setModel(proxyModel);
  _ui->hierarchiesTree->header()->resizeSection(0, 100);
  _ui->hierarchiesTree->header()->setSectionResizeMode(0, QHeaderView::Interactive);
  connect(_ui->hierarchiesTree->selectionModel(),
          SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)), this,
          SLOT(currentChanged(const QModelIndex &, const QModelIndex &)));
  connect(model, SIGNAL(currentGraphChanged(tlp::Graph *)), this,
          SLOT(currentGraphChanged(tlp::Graph *)));
}

Graph *GraphHierarchiesEditor::currentGraph() {
  return _model->currentGraph();
}

GraphHierarchiesEditor::~GraphHierarchiesEditor() {
  delete _ui;
}

void GraphHierarchiesEditor::contextMenuRequested(const QPoint &p) {
  _contextIndex = _ui->hierarchiesTree->indexAt(p);

  if (_contextIndex.isValid()) {
    _contextGraph = _contextIndex.data(tlp::GraphHierarchiesModel::GraphRole).value<tlp::Graph *>();
    if (_contextGraph == nullptr)
      return;
    QMenu menu;
    tlp::Perspective::redirectStatusTipOfMenu(&menu);
    menu.addAction(tlp::Perspective::typedInstance<GraphPerspective>()->createPanelAction());
    menu.addSeparator();
    menu.addAction(tlp::Perspective::typedInstance<GraphPerspective>()->exportAction());
    menu.addAction(_ui->actionSave_to_file);
    menu.addSeparator();
    menu.addAction(_ui->actionRename);
    menu.addSeparator();
    auto subMenu = menu.addMenu("Create");
    subMenu->setToolTipsVisible(true);
    subMenu->addAction(_ui->actionAdd_sub_graph);
    subMenu->addAction(_ui->actionCreate_induced_sub_graph);
    subMenu->addAction(_ui->actionClone_subgraph);

    if (_contextGraph->getRoot() != _contextGraph) {
      subMenu->addAction(_ui->actionClone_sibling);
      subMenu->addAction(_ui->actionClone_sibling_with_properties);
    }
    subMenu = menu.addMenu("Delete");
    subMenu->setToolTipsVisible(true);
    if (_contextGraph->getRoot() != _contextGraph) {
      subMenu->addAction(_ui->actionDelete_graph);
    }

    subMenu->addAction(_ui->actionDelete_All);
    subMenu->addAction(_ui->actionDelete_all_nodes);
    subMenu->addAction(_ui->actionDelete_all_edges);
    subMenu->addAction(_ui->actionDelete_selection);
    if (_contextGraph->getRoot() != _contextGraph)
      subMenu->addAction(_ui->actionDelete_selection_from_root_graph);
    if (!_contextGraph->subGraphs().empty()) {
      menu.addSeparator();
      if (!_ui->hierarchiesTree->isExpanded(_contextIndex))
        menu.addAction(_ui->actionExpand_hierarchy);
      else
        menu.addAction(_ui->actionCollapse_hierarchy);
    }
    menu.exec(_ui->hierarchiesTree->viewport()->mapToGlobal(p));
    _contextIndex = QModelIndex();
    _contextGraph = nullptr;
  }
}

void GraphHierarchiesEditor::clicked(const QModelIndex &index) {
  if (!index.isValid() || index.internalPointer() == nullptr)
    return;

  _contextGraph = index.data(tlp::TulipModel::GraphRole).value<tlp::Graph *>();
  _model->setCurrentGraph(_contextGraph);
  _contextGraph = nullptr;
}

void GraphHierarchiesEditor::doubleClicked(const QModelIndex &index) {
  if (!index.isValid() || index.internalPointer() == nullptr)
    return;

  _contextGraph = index.data(tlp::TulipModel::GraphRole).value<tlp::Graph *>();
  _model->setCurrentGraph(_contextGraph);
  tlp::Perspective::typedInstance<GraphPerspective>()->createPanelAction()->triggered();
  _contextGraph = nullptr;
}

void GraphHierarchiesEditor::currentChanged(const QModelIndex &index, const QModelIndex &previous) {
  if (synchronized() && index.isValid() && index.internalPointer()) {
    if (index == previous)
      return;

    _contextGraph = index.data(tlp::TulipModel::GraphRole).value<tlp::Graph *>();
    disconnect(_ui->hierarchiesTree->selectionModel(),
               SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)), this,
               SLOT(currentChanged(const QModelIndex &, const QModelIndex &)));
    _model->setCurrentGraph(_contextGraph);
    connect(_ui->hierarchiesTree->selectionModel(),
            SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)), this,
            SLOT(currentChanged(const QModelIndex &, const QModelIndex &)));
    _contextGraph = nullptr;
  }
}

void GraphHierarchiesEditor::updateSelectionInfos() {
  if (_currentSelection) {
    Graph *graph = _model->currentGraph();
    auto numNodes = _currentSelection->numberOfNonDefaultValuatedNodes(graph);
    auto numEdges = _currentSelection->numberOfNonDefaultValuatedEdges(graph);
    if (numNodes || numEdges) {
      QString text(" current graph selection: ");
      if (numNodes) {
        if (numNodes == 1)
          text += "one node";
        else
          text += QString::number(numNodes) + " nodes";
      }
      if (numEdges) {
        if (numNodes)
          text += ", ";
        if (numEdges == 1)
          text += "one edge";
        else
          text += QString::number(numEdges) + " edges";
      }
      _ui->selectionLabel->setText(text);
      return;
    }
  }
  _ui->selectionLabel->setText("");
}

void GraphHierarchiesEditor::currentGraphChanged(Graph *graph) {
  tlp::BooleanProperty *selection = nullptr;
  if (graph)
    selection = graph->getProperty<BooleanProperty>("viewSelection");
  if (selection != _currentSelection) {
    if (_currentSelection)
      _currentSelection->removeObserver(this);
    if (selection)
      selection->addObserver(this);
    _currentSelection = selection;
  }
  GraphPerspective::typedInstance<GraphPerspective>()->resetTitle();
  updateSelectionInfos();
}

void GraphHierarchiesEditor::treatEvents(const std::vector<tlp::Event> &evts) {
  assert(_currentSelection != nullptr);
  for (auto &ev : evts) {
    if (ev.type() == Event::TLP_DELETE) {
      _currentSelection = nullptr;
      break;
    }
    updateSelectionInfos();
  }
}

void GraphHierarchiesEditor::addSubGraph() {
  if (_contextGraph == nullptr)
    return;

  _contextGraph->push();
  _contextGraph->addSubGraph("empty subgraph");
}

void GraphHierarchiesEditor::cloneSubGraph() {
  if (_contextGraph == nullptr)
    return;

  _contextGraph->push();
  std::string sgName("clone subgraph of ");
  _contextGraph->addCloneSubGraph(sgName + _contextGraph->getName());
}

void GraphHierarchiesEditor::cloneSibling() {
  if (_contextGraph == nullptr)
    return;

  _contextGraph->push();
  std::string sgName("clone sibling of ");
  _contextGraph->addCloneSubGraph(sgName + _contextGraph->getName(), true);
}

void GraphHierarchiesEditor::cloneSiblingWithProperties() {
  if (_contextGraph == nullptr)
    return;

  _contextGraph->push();
  std::string sgName("clone sibling of ");
  _contextGraph->addCloneSubGraph(sgName + _contextGraph->getName(), true, true);
}

void GraphHierarchiesEditor::addInducedSubGraph() {
  if (_contextGraph == nullptr)
    return;

  GraphPerspective *persp = GraphPerspective::typedInstance<GraphPerspective>();
  persp->createSubGraph(_contextGraph);
}

void GraphHierarchiesEditor::delGraph() {
  if (_contextGraph == nullptr &&
      !_ui->hierarchiesTree->selectionModel()->selectedRows(0).empty()) {
    _contextGraph = _ui->hierarchiesTree->selectionModel()
                        ->selectedRows(0)[0]
                        .data(tlp::TulipModel::GraphRole)
                        .value<tlp::Graph *>();
  }

  if (_contextGraph == nullptr)
    return;

  GraphPerspective::typedInstance<GraphPerspective>()->closePanelsForGraph(_contextGraph);
  _contextGraph->push();

  if (_contextGraph->getRoot() == _contextGraph) {
    delete _contextGraph;
    _model->setCurrentGraph(nullptr);
  } else {
    tlp::Graph *sg = _contextGraph->getSuperGraph();
    sg->delSubGraph(_contextGraph);
    _model->setCurrentGraph(sg);
  }

  _ui->hierarchiesTree->selectionModel()->reset();
  _contextGraph = nullptr;
}

void GraphHierarchiesEditor::delAllGraph() {
  if (_contextGraph == nullptr &&
      !_ui->hierarchiesTree->selectionModel()->selectedRows(0).empty()) {
    _contextGraph = _ui->hierarchiesTree->selectionModel()
                        ->selectedRows(0)[0]
                        .data(tlp::TulipModel::GraphRole)
                        .value<tlp::Graph *>();
  }

  if (_contextGraph == nullptr)
    return;

  if (_contextGraph->getRoot() == _contextGraph) {
    GraphPerspective *perspective = GraphPerspective::typedInstance<GraphPerspective>();

    if (QMessageBox::question(parentWidget(), "Delete a whole hierarchy",
                              "You are going to delete a complete graph hierarchy. This operation "
                              "cannot be undone. Do you really want to continue?",
                              QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Ok) {
      perspective->closePanelsForGraph(_contextGraph);
      delete _contextGraph;
      _model->setCurrentGraph(nullptr);

      if (_model->empty()) {
        perspective->mainWindow()->setWindowModified(false);
        perspective->resetTitle();
      }
    }
  } else {
    _contextGraph->push();
    GraphPerspective::typedInstance<GraphPerspective>()->closePanelsForGraph(_contextGraph);
    tlp::Graph *sg = _contextGraph->getSuperGraph();
    sg->delAllSubGraphs(_contextGraph);
    _model->setCurrentGraph(sg);
  }

  _ui->hierarchiesTree->selectionModel()->reset();
  _contextGraph = nullptr;
}

void GraphHierarchiesEditor::delAllNodes() {
  if (_contextGraph == nullptr)
    return;

  _contextGraph->push();
  Observable::holdObservers();
  _contextGraph->clear();
  Observable::unholdObservers();
}

void GraphHierarchiesEditor::delAllEdges() {
  if (_contextGraph == nullptr)
    return;

  _contextGraph->push();
  Observable::holdObservers();
  std::vector<edge> edges = _contextGraph->edges();
  _contextGraph->delEdges(edges);
  Observable::unholdObservers();
}

void GraphHierarchiesEditor::delSelection(bool fromRoot) {
  Observable::holdObservers();
  tlp::BooleanProperty *selection = _contextGraph->getProperty<BooleanProperty>("viewSelection");

  std::vector<tlp::edge> edgesToDelete =
      iteratorVector(selection->getEdgesEqualTo(true, _contextGraph));
  bool hasPush = !edgesToDelete.empty();

  if (hasPush) {
    _contextGraph->push();
    _contextGraph->delEdges(edgesToDelete, fromRoot);
  }

  std::vector<tlp::node> nodesToDelete =
      iteratorVector(selection->getNodesEqualTo(true, _contextGraph));

  if (!hasPush && !nodesToDelete.empty())
    _contextGraph->push();

  _contextGraph->delNodes(nodesToDelete, fromRoot);

  Observable::unholdObservers();
}

void GraphHierarchiesEditor::delSelectionFromRoot() {
  delSelection(true);
}

void GraphHierarchiesEditor::renameGraph() {
  if (_contextIndex.isValid() &&
      _ui->hierarchiesTree->selectionModel()->selectedRows(0).size() == 1) {
    // Qt bug workaround
    // there is a potential pb when calling
    // _ui->hierarchiesTree->model()->index(_contextIndex.row(), 0, _contextIndex.parent())
    // to get the index of the name to edit
    // because the call to _contextIndex.parent() may cause a crash
    // same pb if using _contextIndex.sibling(__contextIndex.row(), 0) instead
    // It seems safer to use an index build from the selectionModel()
    _ui->hierarchiesTree->edit(_ui->hierarchiesTree->selectionModel()->selectedRows(0)[0]);
  }
}

void GraphHierarchiesEditor::saveGraphHierarchyInTlpFile() {
  tlp::Perspective::typedInstance<GraphPerspective>()->saveGraphHierarchyInTlpFile(_contextGraph);
}

void GraphHierarchiesEditor::toggleSynchronization(bool f) {
  if (f) {
    _linkButton->setIcon(QIcon(":/tulip/gui/icons/16/link.png"));
    _linkButton->setToolTip("Click here to disable the synchronization with workspace active "
                            "panel.\nWhen synchronization is enabled, the graph currently "
                            "displayed\nin the active panel, becomes the current one in the Graphs "
                            "panel.");
  } else {
    _linkButton->setIcon(QIcon(":/tulip/gui/icons/16/unlink.png"));
    _linkButton->setToolTip("Click here to enable the synchronization with workspace active "
                            "panel.\nWhen synchronization is enabled, the graph currently "
                            "displayed\nin the active panel, becomes the current one in the Graphs "
                            "panel.");
  }

  emit changeSynchronization(f);
}

void GraphHierarchiesEditor::setSynchronizeButtonVisible(bool f) {
  _linkButton->setVisible(f);
}

void GraphHierarchiesEditor::collapseGraphHierarchy() {
  _ui->hierarchiesTree->setAllHierarchyVisible(_contextIndex, false);
}

void GraphHierarchiesEditor::expandGraphHierarchy() {
  _ui->hierarchiesTree->setAllHierarchyVisible(_contextIndex, true);
}
