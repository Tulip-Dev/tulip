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
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <tulip/StableIterator.h>
#include <tulip/ForEach.h>
#include <tulip/BooleanProperty.h>
#include <tulip/Graph.h>
#include <tulip/GraphIterator.h>
#include <tulip/GraphImpl.h>
#include <tulip/GraphView.h>
#include <tulip/PropertyManager.h>

using namespace std;
namespace tlp {
//----------------------------------------------------------------
GraphView::GraphView(Graph *supergraph, BooleanProperty *filter,
		     unsigned int sgId):
  GraphAbstract(supergraph),
  nNodes(0),
  nEdges(0) {
  // get id
  id = ((GraphImpl *) getRoot())->getSubGraphId(sgId);
  nodeAdaptativeFilter.setAll(false);
  edgeAdaptativeFilter.setAll(false);
  inDegree.setAll(0);
  outDegree.setAll(0);
  if (filter == 0) return;
  Iterator<unsigned int> *it=0;
  it = filter->nodeProperties.findAll(true);

  Iterator<node> *iteN;
  if (it==0)
    iteN =getSuperGraph()->getNodes();
  else
    iteN = new UINTIterator<node>(it);
  
  while (iteN->hasNext()) {
    node n=iteN->next();
    if (filter->getNodeValue(n)) addNode(n);
  } delete iteN;

  it = filter->edgeProperties.findAll(true);

  Iterator<edge> *iteE;
  if (it==0)
    iteE = getSuperGraph()->getEdges();
  else
    iteE = new UINTIterator<edge>(it);

  while (iteE->hasNext()) {
    edge e=iteE->next();
    if (filter->getEdgeValue(e)) addEdge(e);
  } delete iteE;
}
//----------------------------------------------------------------
GraphView::~GraphView() {
  notifyDestroy();
  StableIterator<Graph *> itS(getSubGraphs());
  while(itS.hasNext())
    delAllSubGraphsInternal(itS.next(), true);
  delete propertyContainer; //must be done here because Property proxy needs to access to the graph structure
  removeGraphObservers();
  //removeObservers();
  ((GraphImpl *) getRoot())->freeSubGraphId(id);
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
unsigned int GraphView::deg(const node n) const {
  return inDegree.get(n.id)+outDegree.get(n.id);
}
//----------------------------------------------------------------
unsigned int GraphView::indeg(const node n) const {
  return inDegree.get(n.id);
}
//----------------------------------------------------------------
unsigned int GraphView::outdeg(const node n) const {
  return outDegree.get(n.id);
}
//----------------------------------------------------------------
void GraphView::reverse(const edge e, const node src, const node tgt) {
  if (isElement(e)) {
    outDegree.set(src.id, outDegree.get(src.id)-1);
    inDegree.set(tgt.id, inDegree.get(tgt.id)-1);
    inDegree.set(src.id, inDegree.get(src.id)+1);
    outDegree.set(tgt.id, outDegree.get(tgt.id)+1);

    notifyReverseEdge(this,e);
    notifyObservers();

    // propagate edge reversal on subgraphs
    Graph* sg;
    forEach(sg, getSubGraphs()) {
      ((GraphView*) sg)->reverse(e, src, tgt);
    }
  }
}
//----------------------------------------------------------------
void GraphView::setEnds(const edge e, const node src, const node tgt,
			const node newSrc, const node newTgt) {
  if (isElement(e)) {
    if (isElement(newSrc) && isElement(newTgt)) {
      notifyBeforeSetEnds(this, e);
      if (src != newSrc) {
	outDegree.set(src.id, outDegree.get(src.id)-1);
	outDegree.set(newSrc.id, outDegree.get(newSrc.id) + 1);
      }
      if (tgt != newTgt) {
	inDegree.set(tgt.id, inDegree.get(tgt.id)-1);
	inDegree.set(newTgt.id, inDegree.get(newTgt.id)+1);
      }
      // notification
      notifyAfterSetEnds(this, e);
      notifyObservers();
      
      // propagate edge ends update on subgraphs
      Graph* sg;
      forEach(sg, getSubGraphs()) {
	((GraphView*) sg)->setEnds(e, src, tgt, newSrc, newTgt);
      }
    }
    else
      // delete edge if new edge ends do no belong to the graph
      delEdge(e);
  }
}
//----------------------------------------------------------------
node GraphView::addNode() {
  node tmp = getSuperGraph()->addNode();
  return restoreNode(tmp);
}
//----------------------------------------------------------------
node GraphView::restoreNode(node n) {
  nodeAdaptativeFilter.set(n.id, true);
  ++nNodes;
  notifyAddNode(this, n);
  notifyObservers();
  return n;
}
//----------------------------------------------------------------
void GraphView::addNode(const node n) {
  assert(getRoot()->isElement(n));
  if (!isElement(n)) {
    if (!getSuperGraph()->isElement(n)) getSuperGraph()->addNode(n);
    restoreNode(n);
  }
}
//----------------------------------------------------------------
edge GraphView::addEdgeInternal(edge e) {
  edgeAdaptativeFilter.set(e.id,true);
  ++nEdges;
  const std::pair<node, node>& eEnds = ends(e);
  node src = eEnds.first;
  node tgt = eEnds.second;
  outDegree.set(src.id, outDegree.get(src.id)+1);
  inDegree.set(tgt.id, inDegree.get(tgt.id)+1);
  notifyAddEdge(this, e);
  notifyObservers();
  return e;
}
//----------------------------------------------------------------
edge GraphView::restoreEdge(edge e, const node,const node) {
  return addEdgeInternal(e);
}
//----------------------------------------------------------------
edge GraphView::addEdge(const node n1,const node n2) {
  assert(isElement(n1));
  assert(isElement(n2));
  return addEdgeInternal(getSuperGraph()->addEdge(n1,n2));
}
//----------------------------------------------------------------
void GraphView::addEdge(const edge e) {
  assert(getRoot()->isElement(e));
  assert(isElement(source(e)));
  assert(isElement(target(e)));
  if (!edgeAdaptativeFilter.get(e.id)) {
    if (!getSuperGraph()->isElement(e))
      getSuperGraph()->addEdge(e);
    addEdgeInternal(e);
  }
}
//----------------------------------------------------------------
void GraphView::delNodeInternal(const node n) {
  nodeAdaptativeFilter.set(n.id, false);
  propertyContainer->erase(n);
  --nNodes;
}
//----------------------------------------------------------------
void GraphView::removeNode(const node n) {
  notifyDelNode(this, n);
  delNodeInternal(n);
  notifyObservers();
}
//----------------------------------------------------------------
void GraphView::delNode(const node n, bool deleteInAllGraphs) {
  if(deleteInAllGraphs) {
    getRoot()->delNode(n, true);
  }
  else {
    assert (isElement(n));
    notifyDelNode(this, n);
    // propagate to subgraphs
    Iterator<Graph *>*itS = getSubGraphs();
    while (itS->hasNext()) {
      Graph *subGraph = itS->next();
      if (subGraph->isElement(n))
        subGraph->delNode(n);
    } delete itS;
    // remove node's edges
    set<edge> loops;
    bool haveLoops = false;
    StableIterator<edge> itE(getInOutEdges(n));
    while(itE.hasNext()) {
      edge e = itE.next();
      node s = opposite(e, n);
      if (s!=n) {
        removeEdge(e);
      }
      else {
        loops.insert(e);
        haveLoops = true;
      }
    }
    if (haveLoops) {
      set<edge>::const_iterator it;
      for ( it = loops.begin(); it!=loops.end(); ++it) {
        removeEdge(*it);
      }
    }
    delNodeInternal(n);
    notifyObservers();
  }
}
//----------------------------------------------------------------
void GraphView::delEdgeInternal(const edge e) {
  edgeAdaptativeFilter.set(e.id,false);
  propertyContainer->erase(e);
  --nEdges;
  const std::pair<node, node>& eEnds = ends(e);
  node src = eEnds.first;
  node tgt = eEnds.second;
  outDegree.set(src.id, outDegree.get(src.id)-1);
  inDegree.set(tgt.id, inDegree.get(tgt.id)-1);
}
//----------------------------------------------------------------
void GraphView::removeEdge(const edge e) {
  assert(isElement(e));
  notifyDelEdge(this,e);
  delEdgeInternal(e);
  notifyObservers();
}
//----------------------------------------------------------------
void GraphView::delEdge(const edge e, bool deleteInAllGraphs) {
  if(deleteInAllGraphs) {
    getRoot()->delEdge(e, true);
  }
  else {
    assert(isElement(e));
    notifyDelEdge(this,e);
    // propagate to subgraphs
    Iterator<Graph *>*itS=getSubGraphs();
    while (itS->hasNext()) {
      Graph *subGraph = itS->next();
      if (subGraph->isElement(e))
        subGraph->delEdge(e);
    } delete itS;
    delEdgeInternal(e);
    notifyObservers();
  }
}
//----------------------------------------------------------------
void GraphView::delAllNode(const node n){
  delNode(n, true);
}
//----------------------------------------------------------------
void GraphView::delAllEdge(const edge e){
  delEdge(e, true);
}
//----------------------------------------------------------------
void GraphView::setEdgeOrder(const node n,const std::vector<edge> &v ) {
  getRoot()->setEdgeOrder(n,v);
}
//----------------------------------------------------------------
void GraphView::swapEdgeOrder(const node n,const edge e1 , const edge e2) {
  getRoot()->swapEdgeOrder(n,e1,e2);
}
//----------------------------------------------------------------
Iterator<node>* GraphView::getNodes() const {
  Iterator<unsigned int> *it=0;
  it = nodeAdaptativeFilter.findAll(true);
  if (it==0)
    return new SGraphNodeIterator(this, nodeAdaptativeFilter);
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
  it = edgeAdaptativeFilter.findAll(true);
  if (it==0)
    return new SGraphEdgeIterator(this, edgeAdaptativeFilter);
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
void GraphView::reserveNodes(unsigned int) {
  cerr << "{Warning ] : "  << __PRETTY_FUNCTION__ << " ... Impossible operation on a Sub Graph" << endl;
}
//----------------------------------------------------------------
unsigned int GraphView::numberOfEdges() const {
  return nEdges;
}
//----------------------------------------------------------------
void GraphView::reserveEdges(unsigned int) {
  cerr << "{Warning ] : "  << __PRETTY_FUNCTION__ << " ... Impossible operation on a Sub Graph" << endl;
}
//----------------------------------------------------------------
bool GraphView::canPop() {
  return getRoot()->canPop();
}
//----------------------------------------------------------------
bool GraphView::canUnpop() {
  return getRoot()->canUnpop();
}
//----------------------------------------------------------------
bool GraphView::canPopThenUnpop() {
  return getRoot()->canPopThenUnpop();
}
//----------------------------------------------------------------
void GraphView::push(bool unpopAllowed) {
  getRoot()->push(unpopAllowed);
}
//----------------------------------------------------------------
void GraphView::pop(bool unpopAllowed) {
  getRoot()->pop(unpopAllowed);
}
//----------------------------------------------------------------
bool GraphView::nextPopKeepPropertyUpdates(PropertyInterface* prop) {
  return getRoot()->nextPopKeepPropertyUpdates(prop);
}
//----------------------------------------------------------------
void GraphView::unpop() {
  getRoot()->unpop();
}
}
