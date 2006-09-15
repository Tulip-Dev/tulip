/**
 Author: David Auber
 Email : auber@labri.fr
 Last modification : 20/08/2001
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "tulip/GraphView.h"
#include "tulip/BooleanProperty.h"
#include "tulip/Graph.h"
#include "tulip/GraphIterator.h"
#include "tulip/StableIterator.h"

using namespace std;
using namespace tlp;
//----------------------------------------------------------------
GraphView::GraphView(Graph *father,BooleanProperty *filter):
  GraphAbstract(father),
  nNodes(0),
  nEdges(0) {
  nodeAdaptativeFilter.setAll(false);
  edgeAdaptativeFilter.setAll(false);
  //  inDegree.setAll(0);
  //  outDegree.setAll(0);
  if (filter == 0) return;
  Iterator<unsigned int> *it=0;
  try {
    it = filter->nodeProperties.findAll(true);
  } catch (ImpossibleOperation &e) {
    it=0;
  }
  Iterator<node> *iteN;
  if (it==0)
    iteN =getFather()->getNodes();
  else
    iteN = new UINTIterator<node>(it);
  
  while (iteN->hasNext()) {
    node n=iteN->next();
    if (filter->getNodeValue(n)) addNode(n);
  } delete iteN;

  try {
    it = filter->edgeProperties.findAll(true);
  } catch (ImpossibleOperation &e) {
    it=0;
  }
  Iterator<edge> *iteE;
  if (it==0)
    iteE = getFather()->getEdges();
  else
    iteE = new UINTIterator<edge>(it);

  while (iteE->hasNext()) {
    edge e=iteE->next();
    if (filter->getEdgeValue(e)) addEdge(e);
  } delete iteE;
}
//----------------------------------------------------------------
GraphView::~GraphView() {
  notifyDestroy(this);
  delete propertyContainer; //must be done here because Property proxy needs to access to the sg structure
  removeObservers();
}
//----------------------------------------------------------------
bool GraphView::isElement(const node n) const {
  return nodeAdaptativeFilter.get(n.id);
}
//----------------------------------------------------------------
bool GraphView::isElement(const edge e) const {
  return edgeAdaptativeFilter.get(e.id);
}
//----------------------------------------------------------------
//unsigned int GraphView::deg(const node n) const {
//  return inDegree.get(n.id)+outDegree.get(n.id);
//}
//----------------------------------------------------------------
//unsigned int GraphView::indeg(const node n) const {
//  return inDegree.get(n.id);
//}
//----------------------------------------------------------------
//unsigned int GraphView::outdeg(const node n) const {
//  return outDegree.get(n.id);
//}
//----------------------------------------------------------------
/*
  void GraphView::reverse(const edge e) {
  notifyReverseEdge(this,e);
  node src = source(e);
  node tgt = target(e);
  outDegree.set(src.id, outDegree.get(src.id)-1);
  inDegree.set(tgt.id, inDegree.get(tgt.id)-1);
  inDegree.set(src.id, inDegree.get(src.id)+1);
  outDegree.set(tgt.id, outDegree.get(tgt.id)+1);
  getFather()->reverse(e);
  }
*/
//----------------------------------------------------------------
node GraphView::addNode() {
  node tmp = getFather()->addNode();
  nodeAdaptativeFilter.set(tmp.id,true);
  ++nNodes;
  //  inDegree.set(tmp.id,0);
  //  outDegree.set(tmp.id,0);
  notifyAddNode(this, tmp);
  return tmp;
}
//----------------------------------------------------------------
void GraphView::addNode(const node n) {
  assert(getRoot()->isElement(n));
  if (!isElement(n)) {
    if (!getFather()->isElement(n)) getFather()->addNode(n);
    nodeAdaptativeFilter.set(n.id,true);
    ++nNodes;
    //    inDegree.set(n.id,0);
    //    outDegree.set(n.id,0);
    notifyAddNode(this, n);
  }
}
//----------------------------------------------------------------
edge GraphView::addEdge(const node n1,const node n2) {
  assert(isElement(n1));
  assert(isElement(n2));
  edge tmp = getFather()->addEdge(n1,n2);
  edgeAdaptativeFilter.set(tmp.id,true);
  ++nEdges;
  //  outDegree.set(n1.id, outDegree.get(n1.id)+1);
  //  inDegree.set(n2.id, inDegree.get(n2.id)+1);
  notifyAddEdge(this, tmp);
  return tmp;
}
//----------------------------------------------------------------
void GraphView::addEdge(const edge e) {
  assert(getRoot()->isElement(e));
  assert(isElement(source(e)));
  assert(isElement(target(e)));
  if (!edgeAdaptativeFilter.get(e.id)) {
    if (!getFather()->isElement(e)) getFather()->addEdge(e);
    edgeAdaptativeFilter.set(e.id,true);
    ++nEdges;
    //    outDegree.set(source(e).id, outDegree.get(source(e).id)+1);
    //    inDegree.set(target(e).id, inDegree.get(target(e).id)+1);
    notifyAddEdge(this, e);
  }
}
//----------------------------------------------------------------
void GraphView::delNode(const node n) {
  assert (isElement(n));
  notifyDelNode(this, n);
  Iterator<Graph *>*itS = getSubGraphs();
  while (itS->hasNext()) {
    Graph *subsg = itS->next();
    if (subsg->isElement(n)) subsg->delNode(n);
  } delete itS;
  set<edge> loops;
  unsigned int toRemove = 0;
  bool haveLoops = false;
  StableIterator<edge> itE(getInOutEdges(n));
  while(itE.hasNext()) {
    edge e = itE.next();
    node s = opposite(e, n);
    if (s!=n) {
      notifyDelEdge(this, e);
      edgeAdaptativeFilter.set(e.id, false);
      getPropertyManager()->erase(e);
      ++toRemove;
    }
    else {
      loops.insert(e);
      haveLoops = true;
    }
  }
  if (haveLoops) {
    set<edge>::const_iterator it;
    for ( it = loops.begin(); it!=loops.end(); ++it) {
      notifyDelEdge(this, *it);
      getPropertyManager()->erase(*it);
      edgeAdaptativeFilter.set(it->id, false);
      ++toRemove;
    }
  }
  nodeAdaptativeFilter.set(n.id, false);
  getPropertyManager()->erase(n);
  --nNodes;
  nEdges -= toRemove;
}
//----------------------------------------------------------------
void GraphView::delEdge(const edge e) {
  assert(isElement(e));
  //  if (isElement(e)) {
    notifyDelEdge(this,e);
    Iterator<Graph *>*itS=getSubGraphs();
    while (itS->hasNext()) {
      Graph *subsg = itS->next();
      if (subsg->isElement(e)) subsg->delEdge(e);
    } delete itS;
    edgeAdaptativeFilter.set(e.id,false);
    getPropertyManager()->erase(e);
    --nEdges;
    //    outDegree.set(source(e).id, outDegree.get(source(e).id)-1);
    //    inDegree.set(target(e).id, inDegree.get(target(e).id)-1);
    //  }
}
//----------------------------------------------------------------
void GraphView::delAllNode(const node n){
  getFather()->delAllNode(n);
}
//----------------------------------------------------------------
void GraphView::delAllEdge(const edge e){
  getFather()->delAllEdge(e);
}
//----------------------------------------------------------------
void GraphView::setEdgeOrder(const node n,const std::vector<edge> &v ) {
  getFather()->setEdgeOrder(n,v);
}
//----------------------------------------------------------------
void GraphView::swapEdgeOrder(const node n,const edge e1 , const edge e2) {
  getFather()->swapEdgeOrder(n,e1,e2);
}
//----------------------------------------------------------------
Iterator<node>* GraphView::getNodes() const {
  Iterator<unsigned int> *it=0;
  try {
    it = nodeAdaptativeFilter.findAll(true);
  } catch (ImpossibleOperation &e) {
    it=0;
    //    if (numberOfNodes()<getFather()->numberOfNodes() / 2)
    //      cerr << "GraphView : Optimization problem: " << numberOfNodes() << "/" << getFather()->numberOfNodes() << endl;
  }
  if (it==0)
    return (new SGraphNodeIterator(this, nodeAdaptativeFilter));
  else
    return (new UINTIterator<node>(it));
}
//----------------------------------------------------------------
Iterator<node>* GraphView::getInNodes(const node n) const 
{return (new InNodesIterator(this, edgeAdaptativeFilter,n));}
//----------------------------------------------------------------
Iterator<node>* GraphView::getOutNodes(const node n)const 
{return (new OutNodesIterator(this, edgeAdaptativeFilter,n));}
//----------------------------------------------------------------
Iterator<node>* GraphView::getInOutNodes(const node n)const 
{return (new InOutNodesIterator(this, edgeAdaptativeFilter,n));}
//----------------------------------------------------------------
Iterator<edge>* GraphView::getEdges()const {
  Iterator<unsigned int> *it=0;
  try {
    it = edgeAdaptativeFilter.findAll(true);
  } catch (ImpossibleOperation &e) {
    it=0;
  }
  if (it==0)
    return (new SGraphEdgeIterator(this, edgeAdaptativeFilter));
  else
    return (new UINTIterator<edge>(it));
}
//----------------------------------------------------------------
Iterator<edge>* GraphView::getInEdges(const node n)const 
{return (new InEdgesIterator(this, edgeAdaptativeFilter,n));}
//----------------------------------------------------------------
Iterator<edge>* GraphView::getOutEdges(const node n)const 
{return (new OutEdgesIterator(this, edgeAdaptativeFilter,n));}
//----------------------------------------------------------------
Iterator<edge>* GraphView::getInOutEdges(const node n)const
{return (new InOutEdgesIterator(this, edgeAdaptativeFilter,n));}
//----------------------------------------------------------------
unsigned int GraphView::numberOfNodes()const {
  return nNodes;
}
//----------------------------------------------------------------
unsigned int GraphView::numberOfEdges() const {
  return nEdges;
}
//----------------------------------------------------------------
