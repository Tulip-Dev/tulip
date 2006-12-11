#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#if (QT_REL == 3)
#include <qlistview.h>
#include <qmessagebox.h>
#include <qinputdialog.h>
#include <qpopupmenu.h>
#include "../include/tulip/SGHierarchyWidget.h"
#else
#include <Qt3Support/q3listview.h>
#include <QtGui/qmessagebox.h>
#include <QtGui/qinputdialog.h>
#include <Qt3Support/q3popupmenu.h>
#include "tulip/Qt3ForTulip.h"
#include "../include/tulip/SGHierarchyWidgetQt3.h"
#endif

#include <tulip/Graph.h>
#include <tulip/BooleanProperty.h>

using namespace std;
using namespace tlp;

//=======================================================
struct ClusterListViewItem: public QListViewItem {
  Graph *_graph;
  ClusterListViewItem(Graph * graph, QListViewItem *item) : QListViewItem(item), _graph(graph) {}
  ClusterListViewItem(Graph * graph, QListView *item) : QListViewItem(item), _graph(graph) {}
  Graph * getGraph() const {
    return _graph;
  }
};
//=======================================================
/* 
 *  Constructs a SGHierarchyWidget which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 */
SGHierarchyWidget::SGHierarchyWidget(Graph *graph , QWidget* parent,  const char* name, Qt::WFlags fl) : 
  SGHierarchyWidgetData(parent, name, fl),
  _currentGraph(graph) {
  setCaption(trUtf8("Cluster Tree"));
  treeView->setColumnText(0,trUtf8("Subgraph Hierachy"));
#if (QT_REL == 3)
  connect(treeView, SIGNAL(currentChanged(QListViewItem *)), SLOT(changeGraph(QListViewItem *)));
  connect(treeView, SIGNAL(contextMenuRequested ( QListViewItem *, const QPoint &, int )),
	  SLOT(rightButtonSGHierarchyWidget( QListViewItem *, const QPoint &, int )));
#else
  // because of moc control
  connect(treeView, SIGNAL(currentChanged(Q3ListViewItem *)), SLOT(changeGraph(Q3ListViewItem *)));
  connect(treeView, SIGNAL(contextMenuRequested ( Q3ListViewItem *, const QPoint &, int )),
	  SLOT(rightButtonSGHierarchyWidget( Q3ListViewItem *, const QPoint &, int )));
#endif
  update();
}
//=======================================================
SGHierarchyWidget::SGHierarchyWidget(QWidget* parent, const char* name, Qt::WFlags fl) :
  SGHierarchyWidgetData(parent, name,fl),
  _currentGraph(0) {
  setCaption(trUtf8("Cluster Tree"));
  treeView->setColumnText(0,trUtf8("Subgraph Hierachy"));
#if (QT_REL == 3)
  connect(treeView, SIGNAL(currentChanged(QListViewItem *)), SLOT(changeGraph(QListViewItem *)));
  connect(treeView, SIGNAL(contextMenuRequested ( QListViewItem *, const QPoint &, int )),
	  SLOT(rightButtonSGHierarchyWidget( QListViewItem *, const QPoint &, int )));
#else
  // because of moc control
  connect(treeView, SIGNAL(currentChanged(Q3ListViewItem *)), SLOT(changeGraph(Q3ListViewItem *)));
  connect(treeView, SIGNAL(contextMenuRequested ( Q3ListViewItem *, const QPoint &, int )),
	  SLOT(rightButtonSGHierarchyWidget( Q3ListViewItem *, const QPoint &, int )));
#endif
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
    treeView->clear();
    //    QListViewItem *item = new QListViewItem( treeView, 0 );
    //    item->setText( 0, trUtf8( "No hierachy" ) );
    return;
  }
  if (_currentGraph==0 || (graphItems.get(graph->getId())==0)) {
    _currentGraph = graph;
    update();
  }
  currentGraphChanged(graph);
  emit graphChanged(_currentGraph);
}
//=======================================================
void SGHierarchyWidget::currentGraphChanged(const Graph *graph) {
  //  cerr << __PRETTY_FUNCTION__ << endl;
  QListViewItem  *item = graphItems.get(graph->getId());
  if (item != 0) {
#if (QT_REL == 3)
    disconnect(treeView, SIGNAL(currentChanged(QListViewItem *)), this, SLOT(changeGraph(QListViewItem *)));
#else
    disconnect(treeView, SIGNAL(currentChanged(Q3ListViewItem *)), this, SLOT(changeGraph(Q3ListViewItem *)));
#endif
    treeView->setCurrentItem(item);
    treeView->ensureItemVisible(item);
#if (QT_REL == 3)
    connect(treeView, SIGNAL(currentChanged(QListViewItem *)), this, SLOT(changeGraph(QListViewItem *)));
#else
    connect(treeView, SIGNAL(currentChanged(Q3ListViewItem *)), this, SLOT(changeGraph(Q3ListViewItem *)));
#endif   
    _currentGraph = ((ClusterListViewItem *)item)->getGraph();
  }
}
//=======================================================
//Building of the cluster tree view
void SGHierarchyWidget::buildTreeView(QListView *item, Graph *p) {
  //  cerr << __PRETTY_FUNCTION__ << endl;
  QListViewItem *tmpItem = new ClusterListViewItem(p, item);
  tmpItem->setText(0, QString(p->getAttribute<string>("name").c_str()));
  graphItems.set(p->getId(), tmpItem);
  Iterator<Graph *> *itS = p->getSubGraphs();
  while (itS->hasNext())
    buildTreeView(tmpItem, itS->next());
  delete itS;
  treeView->setOpen(tmpItem, true);
}
//=======================================================
void SGHierarchyWidget::buildTreeView(QListViewItem *item, Graph *p) {
  QListViewItem *tmpItem = new ClusterListViewItem(p, item);
  tmpItem->setText(0, QString(p->getAttribute<string>("name").c_str()));
  graphItems.set(p->getId(), tmpItem);
  Iterator<Graph *> *itS=p->getSubGraphs();
  while (itS->hasNext())
    buildTreeView(tmpItem, itS->next());
  delete itS;
}
//=======================================================
void SGHierarchyWidget::update() {  
  //cerr << __PRETTY_FUNCTION__ << endl;
  treeView->clear();
  graphItems.setAll(0);
  if (_currentGraph != 0) {
    buildTreeView(treeView, _currentGraph->getRoot());
    currentGraphChanged(_currentGraph);
  }
  show();
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
  if (!recursive)
    _currentGraph->delSubGraph(graph);
  else
    _currentGraph->delAllSubGraphs(graph);
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
  QString text = QInputDialog::getText( "Cluster name" , "Please enter the cluster name" ,
                                        QLineEdit::Normal,QString::null, &ok, this);
  if (ok) {
    Graph *tmp=_currentGraph->getSuperGraph()->addSubGraph();
    tmp->setAttribute("name",string(text.latin1()));
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
  QString text = QInputDialog::getText( "Cluster name" , "Please enter the cluster name" ,
                                        QLineEdit::Normal,QString::null, &ok, this);
  if (ok) {
    BooleanProperty sel1(_currentGraph);
    sel1.setAllNodeValue(true);
    sel1.setAllEdgeValue(true);
    _currentGraph = _currentGraph->addSubGraph(&sel1);
    _currentGraph->setAttribute("name",string(text.latin1()));
    update();
    emit graphChanged(_currentGraph);
  }
}
//=======================================================
void SGHierarchyWidget::contextMoveUpCluster() {
  std::cerr << "Not Implemented" << __PRETTY_FUNCTION__ << std::endl;
  //  _clusterTree->moveUp(_currentSubGraph);
  //  update();
}
//=======================================================
void SGHierarchyWidget::contextRenameCluster() {
  bool ok;
  QString text = QInputDialog::getText( trUtf8("Cluster Name") ,  trUtf8("Please enter the cluster name"),
					QLineEdit::Normal, _currentGraph->getAttribute<string>("name").c_str(), &ok, this);
  if (ok) {
    _currentGraph->setAttribute("name",string(text.latin1()));
    graphItems.get(_currentGraph->getId())->setText(0, text);
  }
}
//=======================================================
void SGHierarchyWidget::rightButtonSGHierarchyWidget(QListViewItem *item, const QPoint &p, int c) {
  if (item == 0) return;
  QPopupMenu menu(this, "cluster_tree_context_menu");
  menu.insertItem(trUtf8("Remove"), this, SLOT(contextRemoveCluster()));
  menu.insertItem(trUtf8("Remove all"), this, SLOT(contextRemoveAllCluster()));
  menu.insertItem(trUtf8("Clone"), this, SLOT(contextCloneCluster()));
  menu.insertItem(trUtf8("SubGraph Clone"), this, SLOT(contextCloneSubgraphCluster()));
  menu.insertItem(trUtf8("Rename"), this, SLOT(contextRenameCluster()));
  menu.exec(p);
}
//=======================================================
void SGHierarchyWidget::changeGraph(QListViewItem *item, const QPoint &p, int i) {
  changeGraph(item);
}
//**********************************************************************
void SGHierarchyWidget::changeGraph(QListViewItem *item) {
  //  cerr << __PRETTY_FUNCTION__ << endl;
  _currentGraph = ((ClusterListViewItem *)item)->getGraph();
  emit graphChanged(_currentGraph);
}
//=======================================================
