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

#ifndef NDEBUG
/* 
 * function to test if an edge e exist in the adjacency of a node
 */
static bool existEdgeE(Graph *g, const node n1, const node, edge e) {
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
#endif
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
  GraphAbstract(this),Observer(false) {
  // id 0 is for the root
  graphIds.get();
}
//----------------------------------------------------------------
GraphImpl::~GraphImpl() {
  unobserveUpdates();
  // delete recorders
  if (!recorders.empty()) {
    recorders.front()->stopRecording(this);
    std::list<GraphUpdatesRecorder*>::iterator it = recorders.begin();
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
  removeGraphObservers();
//  removeObservers();
  //must be done here because Property proxy needs to access to the graph structure  delete propertyContainer;
}
//----------------------------------------------------------------
void GraphImpl::clear() {
  GraphAbstract::clear();
  storage.clear();
}
//----------------------------------------------------------------
bool GraphImpl::isElement(const node n) const {
  return storage.isElement(n);
}
//----------------------------------------------------------------
bool GraphImpl::isElement(const edge e) const {
  return storage.isElement(e);
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
  storage.addNode(newNode);
  notifyAddNode(this, newNode);
  notifyObservers();
  return newNode;
}
//----------------------------------------------------------------
node GraphImpl::addNode() {
  node newNode = storage.addNode();
  notifyAddNode(this, newNode);
  notifyObservers();
  return newNode;
}
//----------------------------------------------------------------
void GraphImpl::addNode(const node) {
  cerr << "Warning : "  << __PRETTY_FUNCTION__ << " ... Impossible operation on Root Graph" << endl;
}
//----------------------------------------------------------------
void GraphImpl::reserveNodes(unsigned int nb) {
  storage.reserveNodes(nb);
}
//----------------------------------------------------------------
void GraphImpl::restoreAdj(node n, vector<edge>& edges) {
  storage.restoreAdj(n, edges);
}
//----------------------------------------------------------------
edge GraphImpl::restoreEdge(edge newEdge, const node src, const node tgt) {
  storage.addEdge(src, tgt, newEdge, false);
  notifyAddEdge(this, newEdge);
  notifyObservers();
  return newEdge;
}
//----------------------------------------------------------------
edge GraphImpl::addEdge(const node src, const node tgt) {
  edge newEdge = storage.addEdge(src, tgt);
  notifyAddEdge(this, newEdge);
  notifyObservers();
  return newEdge;
}
//----------------------------------------------------------------
void GraphImpl::addEdge(const edge e) {
  cerr << "{Warning ] : "  << __PRETTY_FUNCTION__ << " ... Impossible operation on Root Graph" << endl;
  cerr << "\t Trying to add edge " << e.id << " (" << source(e).id << "," << target(e).id << ")" << endl;
}
//----------------------------------------------------------------
void GraphImpl::reserveEdges(unsigned int nb) {
  storage.reserveEdges(nb);
}
//----------------------------------------------------------------
void GraphImpl::removeNode(const node n) {
  assert(isElement(n));
  notifyDelNode(this, n);
  // remove from storage and propertyContainer
  storage.removeFromNodes(n);
  propertyContainer->erase(n);
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

  // loop on inout edges of n
  // for notification and removal from propertyContainer
  Iterator<edge>* edges = storage.getInOutEdges(n);
  set<edge> loops;
  while(edges->hasNext()) {
    edge e = edges->next();
    node s = opposite(e, n);
    if (s != n) {
      notifyDelEdge(this, e);
      propertyContainer->erase(e);
    } else
      loops.insert(e);
  } delete edges;
  if (!loops.empty()) {
    set<edge>::const_iterator it;
    for (it = loops.begin(); it!=loops.end(); ++it) {
      edge e = *it;
      notifyDelEdge(this, e);
      propertyContainer->erase(e);
   }
  }

  // delete n from storage
  storage.delNode(n);

  // remove from propertyContainer
  propertyContainer->erase(n);

  // notification
  notifyObservers();
}
//----------------------------------------------------------------
void GraphImpl::delEdge(const edge e) {
  assert(existEdgeE(this, source(e),target(e), e));
  if (!isElement(e)) {
    return;
  }
  // propagate to subgraphs
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
void GraphImpl::setEdgeOrder(const node n, const vector<edge> &v) {
  storage.setEdgeOrder(n, v);
}
//----------------------------------------------------------------
void GraphImpl::swapEdgeOrder(const node n, const edge e1 , const edge e2) {
  storage.swapEdgeOrder(n, e1, e2);
}
//----------------------------------------------------------------
Iterator<node>* GraphImpl::getNodes() const {
  return new GraphImplNodeIterator(this, storage.getNodes());
}
//----------------------------------------------------------------
Iterator<node>* GraphImpl::getInNodes(const node n) const {
  return new GraphImplNodeIterator(this, storage.getInNodes(n));
}
//----------------------------------------------------------------
Iterator<node>* GraphImpl::getOutNodes(const node n) const {
  return new GraphImplNodeIterator(this, storage.getOutNodes(n));
}
//----------------------------------------------------------------
Iterator<node>* GraphImpl::getInOutNodes(const node n) const {
  return new GraphImplNodeIterator(this, storage.getInOutNodes(n));
}
//----------------------------------------------------------------
Iterator<edge>* GraphImpl::getEdges() const {
  return new GraphImplEdgeIterator(this, storage.getEdges());
}
//----------------------------------------------------------------
Iterator<edge>* GraphImpl::getInEdges(const node n) const {
  return new GraphImplEdgeIterator(this, storage.getInEdges(n));
}
//----------------------------------------------------------------
Iterator<edge>* GraphImpl::getOutEdges(const node n) const {
  return new GraphImplEdgeIterator(this, storage.getOutEdges(n));
}
//----------------------------------------------------------------
Iterator<edge>* GraphImpl::getInOutEdges(const node n) const {
  return new GraphImplEdgeIterator(this, storage.getInOutEdges(n));
}
//----------------------------------------------------------------
unsigned int GraphImpl::deg(const node n) const {
  return storage.deg(n);
}
//----------------------------------------------------------------
unsigned int GraphImpl::indeg(const node n) const {
  assert(isElement(n));
  return storage.indeg(n);
}
//----------------------------------------------------------------
unsigned int GraphImpl::outdeg(const node n) const {
  assert(isElement(n));
  return storage.outdeg(n);
}
//----------------------------------------------------------------
node GraphImpl::source(const edge e)const {
  assert(isElement(e));
  return storage.source(e);
}
//----------------------------------------------------------------
node GraphImpl::target(const edge e) const {
  assert(isElement(e));
  return storage.target(e);
}
//----------------------------------------------------------------
const std::pair<node, node>& GraphImpl::ends(const edge e) const {
  return storage.ends(e);
}
//----------------------------------------------------------------
node GraphImpl::opposite(const edge e, const node n) const {
  assert(isElement(e));
  return storage.opposite(e, n);
}
//----------------------------------------------------------------
void GraphImpl::reverse(const edge e) {
  assert(isElement(e));
  std::pair<node, node> eEnds = storage.ends(e);

  storage.reverse(e);

  // notification
  notifyReverseEdge(this, e);
  notifyObservers();

  // propagate edge reversal on subgraphs
  Graph* sg;
  forEach(sg, getSubGraphs()) {
    ((GraphView*) sg)->reverse(e, eEnds.first, eEnds.second);
  }
}
//----------------------------------------------------------------
void GraphImpl::setEnds(const edge e, const node newSrc, const node newTgt) {
  assert(isElement(e));
  // not allowed on meta edge
  if (isMetaEdge(e)) {
    cerr << "Warning: invoking Graph::setEnds on meta edge " << e.id << endl;
    return;
  }

  // be aware that newSrc or newTgt may not be valid
  // to indicate that only one of the ends has to be changed
  std::pair<node, node> eEnds = storage.ends(e);
  node src = eEnds.first;
  node tgt = eEnds.second;
  // nothing to do if same ends
  if (src == newSrc && tgt == newTgt)
    return;

  // notification
  notifyBeforeSetEnds(this, e);

  storage.setEnds(e, newSrc, newTgt);

  // notification
  notifyAfterSetEnds(this, e);
  notifyObservers();

  // propagate edge reversal on subgraphs
  Graph* sg;
  eEnds = storage.ends(e);
  node nSrc = eEnds.first;
  node nTgt = eEnds.second;
  
  forEach(sg, getSubGraphs()) {
    ((GraphView*) sg)->setEnds(e, src, tgt, nSrc, nTgt);
  }
}
//----------------------------------------------------------------
unsigned int GraphImpl::numberOfEdges() const {
  return storage.numberOfEdges();
}
//----------------------------------------------------------------
unsigned int GraphImpl::numberOfNodes()const {
  return storage.numberOfNodes();
}
//----------------------------------------------------------------
void GraphImpl::removeEdge(const edge e) {
  assert(isElement(e));
  notifyDelEdge(this,e);
  // remove from propertyContainer and storage
  propertyContainer->erase(e);
  storage.delEdge(e);

  notifyObservers();
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
  std::list<GraphUpdatesRecorder*>::iterator it =
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
  std::list<GraphUpdatesRecorder*>::iterator it = recorders.begin();
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
    bool isAddedProp =
      recorders.front()->isAddedOrDeletedProperty(prop->getGraph(), prop);
    if (recorders.front()->dontObserveProperty(prop)) {
      std::list<GraphUpdatesRecorder*>::iterator it = recorders.begin();
      if (++it != recorders.end()) {
	// allow the previous recorder to record
	// the next property updates
	if (isAddedProp)
	  // register prop as a newly added
	  (*it)->addLocalProperty(prop->getGraph(), prop->getName());
	else 
	  prop->addPropertyObserver((*it));
      }
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

