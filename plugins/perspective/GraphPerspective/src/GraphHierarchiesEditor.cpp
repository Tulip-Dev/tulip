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
#include "GraphHierarchiesEditor.h"

#include <QtCore/QDebug>
#include <QtGui/QContextMenuEvent>
#include <QtGui/QMenu>
#include <QtGui/QGraphicsEffect>
#include <QtGui/QPainter>
#include <QtGui/QTextDocument>
#include <QtGui/QToolButton>
#include <QtGui/QSortFilterProxyModel>

#include <tulip/Perspective.h>
#include <tulip/BooleanProperty.h>
#include <tulip/TulipMetaTypes.h>

#include "GraphPerspective.h"
#include <tulip/GraphHierarchiesModel.h>
#include "ui_GraphHierarchiesEditor.h"

GraphHierarchiesEditor::GraphHierarchiesEditor(QWidget *parent): QWidget(parent), _ui(new Ui::GraphHierarchiesEditorData), _contextGraph(NULL), _model(NULL) {
  _ui->setupUi(this);
  _ui->hierarchiesTree->addAction(_ui->actionDelete_All);
  _ui->actionDelete_All->setShortcutContext(Qt::WidgetWithChildrenShortcut);

  QToolButton* linkButton = new QToolButton();
  linkButton->setObjectName("linkButton");
  linkButton->setIcon(QIcon(":/tulip/graphperspective/icons/16/link.png"));
  linkButton->setToolTip(trUtf8("Synchronize with workspace active panel"));
  linkButton->setIconSize(QSize(22,22));
  linkButton->setMinimumSize(25,25);
  linkButton->setMaximumSize(25,25);
  linkButton->setCheckable(true);
  linkButton->setChecked(true);
  _ui->header->insertWidget(linkButton);
  _linkButton = linkButton;
  _ui->hierarchiesTree->installEventFilter(this);

  connect(_ui->hierarchiesTree, SIGNAL(collapsed(const QModelIndex &)),
          this, SLOT(resizeFirstColumnToContent()));
  connect(_ui->hierarchiesTree, SIGNAL(expanded(const QModelIndex &)),
          this, SLOT(resizeFirstColumnToContent()));
}

bool GraphHierarchiesEditor::synchronized() const {
  return _linkButton->isChecked();
}

void GraphHierarchiesEditor::setModel(tlp::GraphHierarchiesModel *model) {
  _model = model;
  QSortFilterProxyModel* proxyModel = new QSortFilterProxyModel(_ui->hierarchiesTree);
  proxyModel->setSourceModel(model);
  _ui->hierarchiesTree->setModel(proxyModel);
  _ui->hierarchiesTree->header()->resizeSection(0,100);
  resizeFirstColumnToContent();
  connect(model, SIGNAL(rowsInserted(const QModelIndex &, int, int)), this, SLOT(resizeFirstColumnToContent()));
  connect(model, SIGNAL(rowsRemoved(const QModelIndex &, int, int)), this, SLOT(resizeFirstColumnToContent()));
}

GraphHierarchiesEditor::~GraphHierarchiesEditor() {
  delete _ui;
}

void GraphHierarchiesEditor::contextMenuRequested(const QPoint& p) {
  _contextIndex = _ui->hierarchiesTree->indexAt(p);

  if (_contextIndex.isValid()) {
    _contextGraph = _contextIndex.data(tlp::GraphHierarchiesModel::GraphRole).value<tlp::Graph*>();
    QMenu menu;
    menu.addAction(_ui->actionCreate_panel);
    menu.addSeparator();
    menu.addAction(_ui->actionExport);
    menu.addAction(_ui->actionSave_to_file);
    menu.addSeparator();
    menu.addAction(_ui->actionRename);
    menu.addSeparator();
    menu.addAction(_ui->actionAdd_sub_graph);
    menu.addAction(_ui->actionCreate_induced_sub_graph);
    menu.addAction(_ui->actionClone_subgraph);
    menu.addSeparator();

    if(_contextGraph->getRoot() != _contextGraph) {
      menu.addAction(_ui->actionDelete_graph);
    }

    menu.addAction(_ui->actionDelete_All);
    menu.exec(_ui->hierarchiesTree->viewport()->mapToGlobal(p));
    _contextGraph = NULL;
  }
}

void GraphHierarchiesEditor::doubleClicked(const QModelIndex& index) {
  if (!index.isValid() || index.internalPointer() == NULL)
    return;

  _contextGraph = (tlp::Graph*)index.internalPointer();
  _model->setCurrentGraph(_contextGraph);
  createPanel();
  _contextGraph = NULL;
}

void GraphHierarchiesEditor::addSubGraph() {
  if (_contextGraph == NULL)
    return;

  _contextGraph->addSubGraph();
}

void GraphHierarchiesEditor::cloneSubGraph() {
  if (_contextGraph == NULL)
    return;

  tlp::BooleanProperty* prop = new tlp::BooleanProperty(_contextGraph);
  prop->setAllNodeValue(true);
  prop->setAllEdgeValue(true);
  _contextGraph->addSubGraph(prop);
  delete prop;
}

void GraphHierarchiesEditor::addInducedSubGraph() {
  if (_contextGraph == NULL)
    return;
  GraphPerspective* persp = GraphPerspective::typedInstance<GraphPerspective>();
  persp->createSubGraph(_contextGraph);
}

void GraphHierarchiesEditor::delGraph() {
  if (_contextGraph == NULL && _ui->hierarchiesTree->selectionModel()->selectedRows(0).size()>0) {
    _contextGraph = _ui->hierarchiesTree->selectionModel()->selectedRows(0)[0].data(tlp::TulipModel::GraphRole).value<tlp::Graph*>();
  }

  if (_contextGraph == NULL)
    return;

  GraphPerspective::typedInstance<GraphPerspective>()->closePanelsForGraph(_contextGraph);
  _contextGraph->push();

  if (_contextGraph->getRoot() == _contextGraph) {
    delete _contextGraph;
    _model->setCurrentGraph(NULL);
  }
  else {
    tlp::Graph* sg = _contextGraph->getSuperGraph();
    _contextGraph->getSuperGraph()->delSubGraph(_contextGraph);
    _model->setCurrentGraph(sg);
  }

  _contextGraph=NULL;
}

void GraphHierarchiesEditor::delAllGraph() {
  if (_contextGraph == NULL && _ui->hierarchiesTree->selectionModel()->selectedRows(0).size()>0) {
    _contextGraph = _ui->hierarchiesTree->selectionModel()->selectedRows(0)[0].data(tlp::TulipModel::GraphRole).value<tlp::Graph*>();
  }

  if (_contextGraph == NULL)
    return;

  GraphPerspective::typedInstance<GraphPerspective>()->closePanelsForGraph(_contextGraph);
  _contextGraph->push();

  if (_contextGraph->getRoot() == _contextGraph) {
    delete _contextGraph;
    _model->setCurrentGraph(NULL);
  }
  else {
    tlp::Graph* sg = _contextGraph->getSuperGraph();
    _contextGraph->getSuperGraph()->delAllSubGraphs(_contextGraph);
    _model->setCurrentGraph(sg);
  }

  _contextGraph=NULL;
}

void GraphHierarchiesEditor::createPanel() {
  tlp::Graph* g = _contextGraph;

  if (sender() == _ui->addPanelButton) {
    QModelIndexList selectedGraphs = _ui->hierarchiesTree->selectionModel()->selectedRows();

    if (selectedGraphs.size()>0)
      g = _ui->hierarchiesTree->model()->data(selectedGraphs[0],tlp::TulipModel::GraphRole).value<tlp::Graph*>();
  }

  if (g == NULL) {
    g = _model->currentGraph();

    if (g == NULL)
      return;
  }

  tlp::Perspective::typedInstance<GraphPerspective>()->createPanel(g);
}

void GraphHierarchiesEditor::exportGraph() {
  tlp::Perspective::typedInstance<GraphPerspective>()->exportGraph(_contextGraph);
}

void GraphHierarchiesEditor::renameGraph() {
  _ui->hierarchiesTree->edit(_contextIndex);
}

void GraphHierarchiesEditor::saveGraphToFile() {
  tlp::Perspective::typedInstance<GraphPerspective>()->saveGraphToFile(_contextGraph);
}

void GraphHierarchiesEditor::setSynchronized(bool f) {
  _linkButton->setChecked(f);
}

void GraphHierarchiesEditor::setSynchronizeButtonVisible(bool f) {
  _linkButton->setVisible(f);
}

void GraphHierarchiesEditor::setAddPanelButtonVisible(bool f) {
  _ui->addPanelButton->setVisible(f);
}

void GraphHierarchiesEditor::resizeFirstColumnToContent() {
  _ui->hierarchiesTree->resizeColumnToContents(0);
}
