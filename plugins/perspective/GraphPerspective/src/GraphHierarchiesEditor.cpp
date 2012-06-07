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

#include <tulip/Perspective.h>
#include <tulip/BooleanProperty.h>
#include <tulip/TulipMetaTypes.h>

#include "GraphPerspective.h"
#include <tulip/GraphHierarchiesModel.h>
#include "ui_GraphHierarchiesEditor.h"

GraphHierarchiesEditor::GraphHierarchiesEditor(QWidget *parent): QWidget(parent), _ui(new Ui::GraphHierarchiesEditorData), _contextGraph(NULL) {
  _ui->setupUi(this);
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
}

bool GraphHierarchiesEditor::synchronized() const {
  return _linkButton->isChecked();
}

void GraphHierarchiesEditor::setModel(tlp::GraphHierarchiesModel *model) {
  _ui->hierarchiesTree->setModel(model);
  _ui->hierarchiesTree->header()->resizeSections(QHeaderView::ResizeToContents);
}

GraphHierarchiesEditor::~GraphHierarchiesEditor() {
  delete _ui;
}

void GraphHierarchiesEditor::contextMenuRequested(const QPoint& p) {
  _contextIndex = _ui->hierarchiesTree->indexAt(p);

  if (_contextIndex.isValid()) {
    _contextGraph = (tlp::Graph*)_contextIndex.internalPointer();
    QMenu menu;
    menu.addAction(_ui->actionExport);
    menu.addAction(_ui->actionCreate_panel);
    menu.addSeparator();
    menu.addAction(_ui->actionRename);
    menu.addAction(_ui->actionAdd_sub_graph);
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
  tlp::Graph* g = _contextGraph;

  if (sender() == _ui->addPanelButton) {
    QModelIndexList selectedGraphs = _ui->hierarchiesTree->selectionModel()->selectedRows();

    if (selectedGraphs.size()>0)
      g = _ui->hierarchiesTree->model()->data(selectedGraphs[0],tlp::TulipModel::GraphRole).value<tlp::Graph*>();
  }

  if (g == NULL) {
    g = static_cast<tlp::GraphHierarchiesModel*>(_ui->hierarchiesTree->model())->currentGraph();

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

void GraphHierarchiesEditor::repackHeaders() {
  for (int i=0; i<_ui->hierarchiesTree->model()->columnCount(); ++i) {
    _ui->hierarchiesTree->resizeColumnToContents(i);
  }
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
