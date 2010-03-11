//-*-c++-*-
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

#include "tulip/Graph.h"
#include "tulip/GraphImpl.h"
#include "tulip/PropertyManager.h"
#include "tulip/GraphView.h"
#include "tulip/LayoutProperty.h"
#include "tulip/GraphIterator.h"
#include "tulip/StableIterator.h"
#include "tulip/ForEach.h"
#include "tulip/GraphUpdatesRecorder.h"

using namespace std;
using namespace tlp;

/* 
 * function to test if an edge e exist in the adjacency of a node
 */
static bool existEdgeE(Graph *g, const node n1, const node n2, edge e) {
  Iterator<edge> *it = g->getOutEdges(n1);
  while (it->hasNext()) {
    edge e1(it->next());
    if (e == e1) {
      delete it;
      return true;
    }
  } delete it;
  return false;
}
/* 
 * function to test the integrity of the graph structure
 */
/* no needed for the moment
static bool integrityTest(Graph *graph) {
  Iterator<edge> *itE = graph->getEdges();
  set<edge> edgesTest;
  while(itE->hasNext()) {
    edge e = itE->next();
    edgesTest.insert(e);
    if (!existEdgeE(graph, graph->source(e), graph->target(e), e)) {
      cerr << "edge do not exist in neighbood" << endl;
      delete itE;
      return false;
    }
  }
  delete itE;
  Iterator<node> *itN = graph->getNodes();
  while(itN->hasNext()) {
    node n(itN->next());
    unsigned int degree = 0;
    Iterator<edge> *it = graph->getInOutEdges(n);
    while (it->hasNext()) {
      edge e = it->next();
      bool found = edgesTest.find(e)!=edgesTest.end();
      if (graph->isElement(e)!=found) {
	cerr << "isElment function not valid" << endl;
	delete it;
	delete itN;
	return false;
      }
      if (!graph->isElement(e)) {
	cerr << "Adjacency edges are not valid" << endl;
	delete it;
	delete itN;
	return false;
      }
      degree++;
    } delete it;
    if (graph->deg(n) != degree) {
      cerr << "degree failed" << endl;
      return false;
    }
  }delete itN;
  return true;
}
*/
//----------------------------------------------------------------
GraphImpl::GraphImpl():
  GraphAbstract(this),Observer(false),nbNodes(0), nbEdges(0) {
  // id 0 is for the root
  graphIds.get();
  outDegree.setAll(0);
}
//----------------------------------------------------------------
GraphImpl::~GraphImpl() {
  unobserveUpdates();
  // delete recorders
  if (!recorders.empty()) {
    recorders.front()->stopRecording(this);
    stdext::slist<GraphUpdatesRecorder*>::iterator it = recorders.begin();
    while(it != recorders.end()) {
      delete (*it);
      it++;
    }
    recorders.clear();
  }
  delPreviousRecorders();

  notifyDestroy();
  StableIterator<Graph *> itS(getSubGraphs());
  while(itS.hasNext())
    delAllSubGraphsInternal(itS.next(), true);
  delete propertyContainer; //must be done here because Property proxy needs to access to the graph structure
  removeGraphObservers();
  removeObservers();
  for (Nodes::iterator i=nodes.begin();i!=nodes.end();++i) {
    i->deallocateAll();
  }
}
//----------------------------------------------------------------
void GraphImpl::clear() {
  GraphAbstract::clear();
  nbNodes=0;
  nbEdges=0;
  outDegree.setAll(0);
}
//----------------------------------------------------------------
bool GraphImpl::isElement(const node n) const {
  return !nodeIds.is_free(n.id);
}
//----------------------------------------------------------------
bool GraphImpl::isElement(const edge e) const {
  return !edgeIds.is_free(e.id);
}
//----------------------------------------------------------------
unsigned int GraphImpl::getSubGraphId(unsigned int id) {
  if (id == 0)
    return graphIds.get();
  graphIds.getFreeId(id);
  return id;
}
//----------------------------------------------------------------
void GraphImpl::freeSubGraphId(unsigned int id) {
  graphIds.free(id);
}
//----------------------------------------------------------------
node GraphImpl::restoreNode(node newNode) {
  outDegree.set(newNode.id, 0);
  while (nodes.size() <= newNode.id){
    nodes.push_back(EdgeContainer());
  }
  assert(nodes[newNode.id].empty());
  nbNodes++;
  notifyAddNode(this, newNode);
  notifyObservers();
  return newNode;
}
//----------------------------------------------------------------
node GraphImpl::addNode() {
  return restoreNode(node(nodeIds.get()));
}
//----------------------------------------------------------------
void GraphImpl::addNode(const node n) {
  cerr << "Warning : "  << __PRETTY_FUNCTION__ << " ... Impossible operation on Root Graph" << endl;
}
//----------------------------------------------------------------
void GraphImpl::reserveNodes(unsigned int nbNodes) {
  if (nbNodes)
    nodes.reserve(nbNodes);
}
//----------------------------------------------------------------
bool GraphImpl::hasFragmentedNodeIds() {
  return nodeIds.hasFreeIds();
}
//----------------------------------------------------------------
edge GraphImpl::addEdgeInternal(edge newEdge, const node s,
				const node t, bool updateContainers) {
  assert(isElement(s) && isElement(t));
  pair< node , node > tmp(s,t);
  outDegree.set(s.id, outDegree.get(s.id) + 1);
  while (edges.size()<=newEdge.id){
    edges.push_back(tmp);
  }
  edges[newEdge.id] = tmp;
  if (updateContainers) {
    nodes[s.id].push_back(newEdge);
    nodes[t.id].push_back(newEdge);
  }
  nbEdges++;
  notifyAddEdge(this, newEdge);
  notifyObservers();
  return newEdge;
}
//----------------------------------------------------------------
void GraphImpl::restoreContainer(node n, vector<edge>& edges) {
  EdgeContainer& container = nodes[n.id];
  container.clear();
  for(unsigned int i = 0; i < edges.size(); ++i)
    container.push_back(edges[i]);
}
//----------------------------------------------------------------
edge GraphImpl::restoreEdge(edge newEdge, const node s,const node t) {
  return addEdgeInternal(newEdge, s, t, false);
}
//----------------------------------------------------------------
edge GraphImpl::addEdge(const node s,const node t) {
  return addEdgeInternal(edge(edgeIds.get()), s, t, true);
}
//----------------------------------------------------------------
void GraphImpl::addEdge(const edge e) {
  cerr << "{Warning ] : "  << __PRETTY_FUNCTION__ << " ... Impossible operation on Root Graph" << endl;
  cerr << "\t Trying to add edge " << e.id << " (" << source(e).id << "," << target(e).id << ")" << endl;
}
//----------------------------------------------------------------
void GraphImpl::reserveEdges(unsigned int nbEdges) {
  if (nbEdges)
    edges.reserve(nbEdges);
}
//----------------------------------------------------------------
bool GraphImpl::hasFragmentedEdgeIds() {
  return edgeIds.hasFreeIds();
}
//----------------------------------------------------------------
void GraphImpl::delNodeInternal(const node n) {
  propertyContainer->erase(n);
  nodes[n.id].clear();
  nodeIds.free(n.id);
  nbNodes--;
}
//----------------------------------------------------------------
void GraphImpl::removeNode(const node n) {
  assert (isElement(n));
  notifyDelNode(this, n);
  delNodeInternal(n);
  notifyObservers();
}
//----------------------------------------------------------------
void GraphImpl::delNode(const node n) {
  assert (isElement(n));
  notifyDelNode(this, n);
  // propagate to subgraphs
  Iterator<Graph *>*itS=getSubGraphs();
  while (itS->hasNext()) {
    Graph *subgraph = itS->next();
    assert(subgraph != this);
    if (subgraph->isElement(n))
      subgraph->delNode(n);
  } delete itS;
  set<edge> loops;
  bool haveLoops = false;
  for(EdgeContainer::iterator i=nodes[n.id].begin(); i!=nodes[n.id].end(); ++i) {
    node s = opposite(*i, n);
    if (s!=n) {
      if (source(*i) == s)
	outDegree.set(s.id, outDegree.get(s.id) - 1);
      removeEdge(*i, n);
    }
    else {
      loops.insert(*i);
      haveLoops = true;
    }
  }
  if (haveLoops) {
    set<edge>::const_iterator it;
    for ( it = loops.begin(); it!=loops.end(); ++it) {
      removeEdge(*it, n);
    }
  }
  delNodeInternal(n);
  notifyObservers();
}
//----------------------------------------------------------------
void GraphImpl::delEdge(const edge e) {
  assert(existEdgeE(this, source(e),target(e), e));
  if (!isElement(e)) {
    return;
  }
  //Warning, the current implementation doesn't manage the updating of 
  //properties for upper_subgraph in the case of Super Graph Implementation
  node s = source(e);
  node t = target(e);
  outDegree.set(s.id, outDegree.get(s.id)-1);
  Iterator<Graph *>*itS=getSubGraphs();
  while (itS->hasNext()) {
    Graph *subgraph = itS->next();
    assert(subgraph != this);
    if (subgraph->isElement(e))
      subgraph->delEdge(e);
  } delete itS;
  removeEdge(e);
}
//----------------------------------------------------------------
void GraphImpl::delAllNode(const node n){delNode(n);}
//----------------------------------------------------------------
void GraphImpl::delAllEdge(const edge e){delEdge(e);}
//----------------------------------------------------------------
void GraphImpl::setEdgeOrder(const node n,const vector<edge> &v ) {
  //  cerr << __PRETTY_FUNCTION__ << "not tested function" << endl;
  if (v.size()==0) return;
  MutableContainer<int> isEle;
  isEle.setAll(0); 
  for (vector<edge>::const_iterator it=v.begin();it!=v.end();++it) {
    isEle.set(it->id, isEle.get(it->id)+1);
  }
  vector<edge>::const_iterator it2=v.begin();
  EdgeContainer currentOrder = nodes[n.id];
  for (unsigned int i=0; i<currentOrder.size(); ++i) {
    if ( isEle.get(currentOrder[i].id)>0 ) {
      isEle.set(currentOrder[i].id, isEle.get(currentOrder[i].id) -1);
      currentOrder[i] = *it2;
      ++it2;
    }
  }
}
//----------------------------------------------------------------
void GraphImpl::swapEdgeOrder(const node n,const edge e1 , const edge e2) {
  //  cerr << __PRETTY_FUNCTION__ << " not tested function" << endl;
  if (e1==e2) return;
  EdgeContainer adjacency=nodes[n.id];
  unsigned int e1Pos=UINT_MAX,e2Pos=UINT_MAX;
  for (unsigned int i=0;i<deg(n);++i) {
    if (adjacency[i]==e1) e1Pos=i;
    if (adjacency[i]==e2) e2Pos=i;
    if (e1Pos!=UINT_MAX && e2Pos!=UINT_MAX) break;
  }
  assert(e1Pos!=UINT_MAX && e2Pos!=UINT_MAX);
  adjacency[e1Pos]=e2;
  adjacency[e2Pos]=e1;
}
//----------------------------------------------------------------
Iterator<node>* GraphImpl::getNodes()const
{return (new xSGraphNodeIterator(this));}
//----------------------------------------------------------------
Iterator<node>* GraphImpl::getInNodes(const node n)const
{return (new xInNodesIterator(this,n));}
//----------------------------------------------------------------
Iterator<node>* GraphImpl::getOutNodes(const node n)const
{return (new xOutNodesIterator(this,n));}
//----------------------------------------------------------------
Iterator<node>* GraphImpl::getInOutNodes(const node n)const
{return (new xInOutNodesIterator(this,n));}
//----------------------------------------------------------------
Iterator<edge>* GraphImpl::getEdges()const
{return (new xSGraphEdgeIterator(this));}
//----------------------------------------------------------------
Iterator<edge>* GraphImpl::getInEdges(const node n)const
{return (new xInEdgesIterator(this,n));}
//----------------------------------------------------------------
Iterator<edge>* GraphImpl::getOutEdges(const node n)const
{return (new xOutEdgesIterator(this,n));}
//----------------------------------------------------------------
Iterator<edge>* GraphImpl::getInOutEdges(const node n)const
{return (new xInOutEdgesIterator(this,n));}
//----------------------------------------------------------------
unsigned int GraphImpl::deg(const node n) const {
  assert(isElement(n));
  return nodes[n.id].size();
}
//----------------------------------------------------------------
unsigned int GraphImpl::indeg(const node n) const {
  assert(isElement(n));
  return nodes[n.id].size()-outDegree.get(n.id);
}
//----------------------------------------------------------------
unsigned int GraphImpl::outdeg(const node n) const {
  assert(isElement(n));
  return outDegree.get(n.id);
}
//----------------------------------------------------------------
node GraphImpl::source(const edge e)const {
  assert(isElement(e));
  return edges[e.id].first;
}
//----------------------------------------------------------------
node GraphImpl::target(const edge e)const {
  assert(isElement(e));
  return edges[e.id].second;
}
//=========================================================================
const std::pair<node, node>& GraphImpl::ends(const edge e) const {
  assert(isElement(e));
  return edges[e.id];
}
//=========================================================================
node GraphImpl::opposite(const edge e, const node n) const {
  assert(isElement(e));
  const std::pair<node, node>& eEnds = edges[e.id];
  assert((eEnds.first == n) || (eEnds.second == n));
  return (eEnds.first == n) ? eEnds.second : eEnds.first;
}
//----------------------------------------------------------------
void GraphImpl::reverse(const edge e) {
  assert(isElement(e));
  node src = edges[e.id].first;
  node tgt = edges[e.id].second;
  edges[e.id].first  = tgt;
  edges[e.id].second = src;
  outDegree.set(src.id, outDegree.get(src.id) - 1);
  outDegree.set(tgt.id, outDegree.get(tgt.id) + 1);
  // notification
  notifyReverseEdge(this,e);
  notifyObservers();

  // propagate edge reversal on subgraphs
  Graph* sg;
  forEach(sg, getSubGraphs()) {
    ((GraphView*) sg)->reverse(e, src, tgt);
  }
}
//----------------------------------------------------------------
unsigned int GraphImpl::numberOfEdges()const{return nbEdges;}
//----------------------------------------------------------------
unsigned int GraphImpl::numberOfNodes()const{return nbNodes;}
//----------------------------------------------------------------
void GraphImpl::removeEdge(const edge e, node dontUpdateNode) {
  assert(isElement(e));
  notifyDelEdge(this,e);
  propertyContainer->erase(e);
  edgeIds.free(e.id);
  nbEdges--;
  pair<node, node>edgeNodes = edges[e.id];
  // remove from source's edges
  node n = edgeNodes.first;
  if (n != dontUpdateNode)
    removeEdge(nodes[n.id], e);
  // remove from target's edges
  n = edgeNodes.second;
  if (n != dontUpdateNode)
    removeEdge(nodes[n.id], e);
  notifyObservers();
}
//----------------------------------------------------------------
void GraphImpl::removeEdge(EdgeContainer &c, const edge e) {
  bool copy = false;
  EdgeContainer::iterator previous;
  for (EdgeContainer::iterator i=c.begin(); i!=c.end(); ++i) {
    edge e1 = *i;
    if (copy)
      *previous = e1;
    previous = i;
    if (e1 == e)  
      copy = true;
  }
  if (copy)
    c.pop_back();
}
//----------------------------------------------------------------
bool GraphImpl::canPop() {
  return (!recorders.empty());
}
//----------------------------------------------------------------
bool GraphImpl::canUnpop() {
  return (!previousRecorders.empty());
}
//----------------------------------------------------------------
void GraphImpl::delPreviousRecorders() {
  stdext::slist<GraphUpdatesRecorder*>::iterator it =
    previousRecorders.begin();
  while(it != previousRecorders.end()) {
    delete (*it);
    it++;
  }
  previousRecorders.clear();
}
//----------------------------------------------------------------
void GraphImpl::update(std::set<Observable *>::iterator,
		       std::set<Observable *>::iterator) {
  // an update occurs in the graph hierarchy
  // so delete the previous recorders
  delPreviousRecorders();
  unobserveUpdates();
}
//----------------------------------------------------------------
void GraphImpl::observableDestroyed(Observable*) {
  // a sub graph has been removed
  // so delete the previous recorders
  delPreviousRecorders();
  unobserveUpdates();
}
//----------------------------------------------------------------
void GraphImpl::observeUpdates(Graph *g) {
  g->addObserver(this);
  observedGraphs.push_front(g);

  // loop on local properties
  PropertyInterface* prop;
  forEach(prop, g->getLocalObjectProperties()) {
    prop->addObserver(this);
    observedProps.push_front(prop);
  }

  // loop on subgraphs
  Graph* sg;
  forEach(sg, g->getSubGraphs()) {
    observeUpdates(sg);
  }
}  
//----------------------------------------------------------------
void GraphImpl::unobserveUpdates() {
  // loop on observed graphs
  while(!observedGraphs.empty()) {
    observedGraphs.front()->removeObserver(this);
    observedGraphs.pop_front();
  }

  // loop on observed properties
  while(!observedProps.empty()) {
    observedProps.front()->removeObserver(this);
    observedProps.pop_front();
  }
}  
//----------------------------------------------------------------
#define NB_MAX_RECORDERS 10
void GraphImpl::push(bool unpopAllowed) {
  // from now if previous recorders exist
  // they cannot be unpop
  // so delete them
  delPreviousRecorders();
  // end any previous updates observation
  unobserveUpdates();

  if (!recorders.empty())
    // stop recording for current recorder
    recorders.front()->stopRecording(this);
  GraphUpdatesRecorder* recorder = new GraphUpdatesRecorder(unpopAllowed);
  recorder->startRecording(this);
  recorders.push_front(recorder);

  // delete first pushed recorder if needed
  int nb = 0;
  stdext::slist<GraphUpdatesRecorder*>::iterator it = recorders.begin();
  while(it != recorders.end()) {
    if (nb == NB_MAX_RECORDERS) {
      GraphUpdatesRecorder* recorder = (*it);
      delete recorder;
      recorders.erase(it);
      break;
    }
    nb++;
    it++;
  }
}
//----------------------------------------------------------------
void GraphImpl::pop(bool unpopAllowed) {
  // save the front recorder
  // to allow unpop
  if (!recorders.empty()) {
    //if (!previousRecorders.empty())
      unobserveUpdates();
    GraphUpdatesRecorder* prevRecorder = recorders.front();
    if (unpopAllowed && prevRecorder->restartAllowed)
      prevRecorder->recordNewValues(this);
    prevRecorder->stopRecording(this);
    // undo all recorded updates
    prevRecorder->doUpdates(this, true);
    // push it
    if (unpopAllowed && prevRecorder->restartAllowed) {
      previousRecorders.push_front(prevRecorder);
      // observe any updates
      // in order to remove previous recorders if needed
      observeUpdates(this);
    } else
      delete prevRecorder;
    // must be done here (see canPop, canUnpop)
    recorders.pop_front();
    // restart the front recorder
    if (!recorders.empty())
      recorders.front()->restartRecording(this);
  }
}
//----------------------------------------------------------------
void GraphImpl::unpop() {
  int nbPrev = previousRecorders.size();
  if (nbPrev != 0) {
    unobserveUpdates();
    if (!recorders.empty())
      recorders.front()->stopRecording(this);
    GraphUpdatesRecorder* prevRecorder = previousRecorders.front();
    previousRecorders.pop_front();
    recorders.push_front(prevRecorder);
    // redo all recorded updates
    prevRecorder->doUpdates(this, false);
    prevRecorder->restartRecording(this);
    // if previous recorders can be unpop
    // ensure they will be removed
    // with the next update
    if (nbPrev > 1)
      observeUpdates(this);
  }
}
//----------------------------------------------------------------
bool GraphImpl::nextPopKeepPropertyUpdates(PropertyInterface* prop) {
  if (!recorders.empty()) {
    if (recorders.front()->dontObserveProperty(prop)) {
      stdext::slist<GraphUpdatesRecorder*>::iterator it = recorders.begin();
      if (++it != recorders.end())
	// allow the previous recorder to record
	// the next property updates
	prop->addPropertyObserver((*it));
      return true;
    } 
#ifndef NDEBUG
      else {
      cerr << __PRETTY_FUNCTION__ << " the observation of Property " << prop->getName()  << " cannot be stopped " << endl;
      return false;
    }
#endif
  }
  return false;
}
//----------------------------------------------------------------
bool GraphImpl::canDeleteProperty(Graph* g, PropertyInterface *prop) {
  return recorders.empty() ||
    !recorders.front()->isAddedOrDeletedProperty(g, prop);
}

