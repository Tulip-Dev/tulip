/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
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
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <iostream>

#include <QtGui/qinputdialog.h>
#include <QtGui/qmenu.h>
#include <QtGui/qmessagebox.h>

#include "tulip/SGHierarchyWidget.h"

#include <tulip/Graph.h>
#include <tulip/BooleanProperty.h>

using namespace std;

namespace tlp {

  //=======================================================
  class SGHierarchyWidgetItem: public QTreeWidgetItem {
    Graph *_graph;
  public:
    SGHierarchyWidgetItem(Graph* graph, QTreeWidgetItem* parent) : QTreeWidgetItem(parent), _graph(graph) { }
    Graph * getGraph() const {
      return _graph;
    }
  };
  //=======================================================
  /*
   *  Constructs a SGHierarchyWidget which is a child of 'parent'
   */
  SGHierarchyWidget::SGHierarchyWidget(QWidget* parent, Graph *graph) :
    QTreeWidget(parent),
    _currentGraph(graph) {
    setColumnCount(4);
    QStringList columnNames;
    columnNames << QString("Subgraph Hierarchy") << QString("Nb nodes")
		<< QString("Nb edges") << QString("Graph id");
    setHeaderLabels(columnNames);
    setUniformRowHeights(true);
    setSortingEnabled(true);
    setRootIsDecorated(false);
    setItemsExpandable(true);
    // because of moc control
    connect(this, SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)),
	    this, SLOT(changeGraph(QTreeWidgetItem*, QTreeWidgetItem*)));
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested (const QPoint &)),
	    SLOT(displayContextMenu(const QPoint &)));
    update();
  }
  //=======================================================
  Graph* SGHierarchyWidget::getGraph() const {
    return _currentGraph;
  }
  //=======================================================
  void SGHierarchyWidget::setGraph(Graph *graph) {
    //  cerr << __PRETTY_FUNCTION__ << endl;
    if (graph == 0) {
      _currentGraph = 0;
      clear();
      return;
    }
    if (_currentGraph==0 || (graphItems.get(graph->getId())==0)) {
      _currentGraph = graph;
      update();
    }
    currentGraphChanged(graph);
    //emit graphChanged(_currentGraph);
  }
  //=======================================================
  void SGHierarchyWidget::currentGraphChanged(const Graph *graph) {
    //  cerr << __PRETTY_FUNCTION__ << endl;
    QTreeWidgetItem* item = graphItems.get(graph->getId());
    if (item != 0) {
      disconnect(this, SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)),
		 this, SLOT(changeGraph(QTreeWidgetItem*, QTreeWidgetItem*)));
      setCurrentItem(item);
      scrollToItem(item);
      connect(this, SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)),
	      this, SLOT(changeGraph(QTreeWidgetItem*, QTreeWidgetItem*)));
      _currentGraph = ((SGHierarchyWidgetItem *)item)->getGraph();
    }
  }
  //=======================================================
  void SGHierarchyWidget::buildTreeView(Graph *graph, QTreeWidgetItem *parentItem) {
    QTreeWidgetItem *item = new SGHierarchyWidgetItem(graph, parentItem);
    setItemInfos(item, graph, graph->numberOfNodes(), graph->numberOfEdges());
    graphItems.set(graph->getId(), item);
    Iterator<Graph *> *itS= graph->getSubGraphs();
    while (itS->hasNext())
      buildTreeView(itS->next(), item);
    delete itS;
    if (!parentItem) {
      setCurrentItem(item);
      addTopLevelItem(item);
      expandItem(item);
    }
  }
  //=======================================================
  void SGHierarchyWidget::setItemInfos(QTreeWidgetItem *item, Graph *graph,
				       unsigned int nbNodes, unsigned int nbEdges) {
    item->setText(0, QString::fromUtf8(graph->getAttribute<string>("name").c_str()));
    char tmpstr[9];
    sprintf(tmpstr, " %.7d", nbNodes);
    item->setText(1, QString(tmpstr));
    sprintf(tmpstr, " %.7d", nbEdges);
    item->setText(2, QString(tmpstr));
    sprintf(tmpstr, " %.5d", graph->getId());
    item->setText(3, QString(tmpstr));
  }
  //=======================================================
  void SGHierarchyWidget::update() {
    //cerr << __PRETTY_FUNCTION__ << endl;
    clear();
    graphItems.setAll(0);
    if (_currentGraph != 0) {
      buildTreeView(_currentGraph->getRoot());
      currentGraphChanged(_currentGraph);
    }
  }
  //=======================================================
  void SGHierarchyWidget::updateCurrentGraphInfos(Graph *graph) {
    if (graph == 0 || _currentGraph==0) return;
    QTreeWidgetItem* item = graphItems.get(graph->getId());
    if (item != 0)
      setItemInfos(item, graph, graph->numberOfNodes(), graph->numberOfEdges());
    Iterator<Graph *> *subGraphsIt=graph->getSubGraphs();
    while(subGraphsIt->hasNext()){
      Graph *subGraph=subGraphsIt->next();
      updateCurrentGraphInfos(subGraph);
    }
  }
  //=======================================================
  //Cluster Tree Structure modification
  void SGHierarchyWidget::removeSubgraph(Graph *graph, bool recursive) {
    //cerr << __PRETTY_FUNCTION__ << endl;
    if (graph == 0) return;
    if (graph == graph->getRoot()) {
      QMessageBox::critical( 0, "Tulip Hierarchy Editor Remove Failed", QString("You cannot remove the root graph"));
      return;
    }
    emit aboutToRemoveView(graph);
    _currentGraph = graph->getSuperGraph();
  // allow to undo
  _currentGraph->push();
  Observable::holdObservers();
    if (!recursive)
      _currentGraph->delSubGraph(graph);
    else
      _currentGraph->delAllSubGraphs(graph);
   Observable::unholdObservers();
    update();
    emit graphChanged(_currentGraph);
  }
  //=======================================================
  //Cluster Tree Structure modification
  void SGHierarchyWidget::contextRemoveCluster() {
    removeSubgraph(_currentGraph, false);
  }
  //=======================================================
  void SGHierarchyWidget::contextRemoveAllCluster() {
    removeSubgraph(_currentGraph, true);
  }
  //=======================================================
  void SGHierarchyWidget::contextCloneCluster() {
    if (_currentGraph == _currentGraph->getRoot()) {
      QMessageBox::critical( 0, "Tulip Cluster Tree Editor Clone Failed",QString("You cannot clone the root cluster"));
      return;
    }
    bool ok;
    QString text = QInputDialog::getText(this, "Cluster name" , "Please enter the cluster name" ,
					 QLineEdit::Normal,QString::null, &ok);
    if (ok) {
    // allow to undo
    _currentGraph->push();
      Graph *tmp=_currentGraph->getSuperGraph()->addSubGraph();
      tmp->setAttribute("name", string(text.toUtf8().data()));
      Iterator<node> *itN=_currentGraph->getNodes();
      while (itN->hasNext())
	tmp->addNode(itN->next());
      delete itN;
      Iterator<edge> *itE=_currentGraph->getEdges();
      while (itE->hasNext())
	tmp->addEdge(itE->next());
      delete itE;
      update();
    }
  }
  //=======================================================
  void SGHierarchyWidget::contextCloneSubgraphCluster() {
    bool ok;
    QString text = QInputDialog::getText(this, "Cluster name" , "Please enter the cluster name" ,
					 QLineEdit::Normal,QString::null, &ok);
    if (ok) {
    // allow to undo
    _currentGraph->push();
      BooleanProperty sel1(_currentGraph);
      sel1.setAllNodeValue(true);
      sel1.setAllEdgeValue(true);
      _currentGraph = _currentGraph->addSubGraph(&sel1);
      _currentGraph->setAttribute("name", string(text.toUtf8().data()));
      update();
      emit graphChanged(_currentGraph);
    }
  }
  //=======================================================
  void SGHierarchyWidget::contextRenameCluster() {
    bool ok;
    QString text =
      QInputDialog::getText(this, "Cluster Name",
			    "Please enter the cluster name",
			    QLineEdit::Normal,
			    _currentGraph->getAttribute<string>("name").c_str(),
			    &ok);
    if (ok) {
      _currentGraph->setAttribute("name", string(text.toUtf8().data()));
      graphItems.get(_currentGraph->getId())->setText(0, text);
    }
  }
  //=======================================================
  void SGHierarchyWidget::displayContextMenu(const QPoint &p) {
    if (itemAt(p) == 0) return;
    QMenu menu(this);
    menu.addAction("Remove", this, SLOT(contextRemoveCluster()));
    menu.addAction("Remove all", this, SLOT(contextRemoveAllCluster()));
    menu.addAction("Clone", this, SLOT(contextCloneCluster()));
    menu.addAction("SubGraph Clone", this, SLOT(contextCloneSubgraphCluster()));
    menu.addAction("Rename", this, SLOT(contextRenameCluster()));
    menu.exec(mapToGlobal(p));
  }
  //=======================================================
  void SGHierarchyWidget::changeGraph(QTreeWidgetItem *item, const QPoint &p, int i) {
    changeGraph(item, NULL);
  }
  //**********************************************************************
  void SGHierarchyWidget::changeGraph(QTreeWidgetItem* current, QTreeWidgetItem*) {
    //  cerr << __PRETTY_FUNCTION__ << endl;
    if (current) {
      _currentGraph = ((SGHierarchyWidgetItem*)current)->getGraph();
      emit graphChanged(_currentGraph);
    }
  }
  //=======================================================

}
