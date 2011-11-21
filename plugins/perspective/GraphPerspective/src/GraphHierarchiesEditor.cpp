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

#include "GraphPerspective.h"

#include <QtCore/QDebug>
#include <QtGui/QContextMenuEvent>
#include <QtGui/QMenu>
#include <tulip/BooleanProperty.h>

#include <tulip/TulipMetaTypes.h>
#include "GraphHierarchiesModel.h"
#include "ui_GraphHierarchiesEditor.h"

GraphHierarchiesEditor::GraphHierarchiesEditor(QWidget *parent): QWidget(parent), _ui(new Ui::GraphHierarchiesEditorData) {
  _ui->setupUi(this);
  _ui->hierarchiesTree->installEventFilter(this);
}

void GraphHierarchiesEditor::setModel(GraphHierarchiesModel *model) {
  _ui->hierarchiesTree->setModel(model);
  _ui->hierarchiesTree->header()->resizeSections(QHeaderView::ResizeToContents);
}

GraphHierarchiesEditor::~GraphHierarchiesEditor() {
  delete _ui;
}

void GraphHierarchiesEditor::activeGraphSelected(const QModelIndex& index) {
  tlp::Graph* graph = (tlp::Graph*)(_ui->hierarchiesTree->model()->index(index.row(),0,index.parent()).internalPointer());
  static_cast<GraphHierarchiesModel*>(_ui->hierarchiesTree->model())->setCurrentGraph(graph);
}

void GraphHierarchiesEditor::contextMenuRequested(const QPoint& p) {
  _contextIndex = _ui->hierarchiesTree->indexAt(p);

  if (_contextIndex.isValid()) {
    _contextGraph = (tlp::Graph*)_contextIndex.internalPointer();
    QMenu menu;
    menu.addAction(_ui->actionCreate_panel);
    menu.addSeparator();
    menu.addAction(_ui->actionAdd_sub_graph);
    menu.addAction(_ui->actionClone_subgraph);
    menu.addSeparator();
    menu.addAction(_ui->actionDelete_graph);
    menu.addAction(_ui->actionDelete_All);
    menu.exec(_ui->hierarchiesTree->viewport()->mapToGlobal(p));
    _contextGraph = NULL;
  }
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

void GraphHierarchiesEditor::delGraph() {
  if (_contextGraph == NULL)
    return;

  if (_contextGraph->getRoot() == _contextGraph)
    delete _contextGraph;
  else
    _contextGraph->getSuperGraph()->delSubGraph(_contextGraph);
}

void GraphHierarchiesEditor::delAllGraph() {
  if (_contextGraph == NULL)
    return;

  if (_contextGraph->getRoot() == _contextGraph)
    delete _contextGraph;
  else
    _contextGraph->getSuperGraph()->delAllSubGraphs(_contextGraph);
}

void GraphHierarchiesEditor::createPanel() {
  static_cast<GraphPerspective*>(GraphPerspective::instance())->createPanel(_contextGraph,_contextIndex);
}
