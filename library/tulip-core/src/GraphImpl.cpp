/**
 *
 * This file is part of Tulip (www.tulip-software.org)
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

#include <queue>

#include <tulip/Graph.h>
#include <tulip/GraphImpl.h>
#include <tulip/PropertyManager.h>
#include <tulip/GraphView.h>
#include <tulip/GraphIterator.h>
#include <tulip/ForEach.h>
#include <tulip/GraphUpdatesRecorder.h>

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
  }

  delete it;
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
      tlp::warning() << "edge do not exist in neighbood";
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
  tlp::warning() << "isElment function not valid";
  delete it;
  delete itN;
  return false;
      }
      if (!graph->isElement(e)) {
  tlp::warning() << "Adjacency edges are not valid";
  delete it;
  delete itN;
  return false;
      }
      degree++;
    } delete it;
    if (graph->deg(n) != degree) {
      tlp::warning() << "degree failed";
      return false;
    }
  }delete itN;
  return true;
}
*/
//----------------------------------------------------------------
GraphImpl::GraphImpl():
  GraphAbstract(this, 0) {
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
      ++it;
    }

    recorders.clear();
  }

  delPreviousRecorders();

  // notify destruction
  observableDeleted();
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
edge GraphImpl::existEdge(const node src, const node tgt,
                          bool directed) const {
  std::vector<edge> edges;
  return storage.getEdges(src, tgt, directed, edges, true) ? edges[0] : edge();
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
  notifyAddNode(newNode);
  return newNode;
}
//----------------------------------------------------------------
void GraphImpl::restoreNodes(const std::vector<node>& nodes) {
  if (!nodes.empty()) {
    storage.restoreNodes(nodes);

    if (hasOnlookers())
      sendEvent(GraphEvent(*this, GraphEvent::TLP_ADD_NODES, nodes));
  }
}
//----------------------------------------------------------------
node GraphImpl::addNode() {
  node newNode = storage.addNode();
  notifyAddNode(newNode);
  return newNode;
}
//----------------------------------------------------------------
void GraphImpl::addNodes(unsigned int nb, std::vector<node>& addedNodes) {
  if (nb) {
    storage.addNodes(nb, addedNodes);

    if (hasOnlookers())
      sendEvent(GraphEvent(*this, GraphEvent::TLP_ADD_NODES, addedNodes));
  }
}
//----------------------------------------------------------------
void GraphImpl::addNode(const node) {
  tlp::warning() << "Warning : "  << __PRETTY_FUNCTION__ << " ... Impossible operation on Root Graph" << std::endl;
}
//----------------------------------------------------------------
void GraphImpl::addNodes(Iterator<node>*) {
  tlp::warning() << "Warning : "  << __PRETTY_FUNCTION__ << " ... Impossible operation on Root Graph" << std::endl;
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
  notifyAddEdge(newEdge);
  return newEdge;
}
//----------------------------------------------------------------
edge GraphImpl::addEdge(const node src, const node tgt) {
  assert(src.isValid()&&tgt.isValid());
  edge newEdge = storage.addEdge(src, tgt);
  notifyAddEdge(newEdge);
  return newEdge;
}
//----------------------------------------------------------------
void GraphImpl::addEdges(const std::vector<std::pair<node, node> >& edges,
                         std::vector<edge>& addedEdges) {
  if (!edges.empty()) {
    storage.addEdges(edges, addedEdges);

    if (hasOnlookers())
      sendEvent(GraphEvent(*this, GraphEvent::TLP_ADD_EDGES, addedEdges));
  }
}
//----------------------------------------------------------------
void GraphImpl::restoreEdges(const std::vector<edge>& edges,
                             const std::vector<std::pair<node, node> >& ends) {
  if (!edges.empty()) {
    storage.restoreEdges(edges, ends);

    if (hasOnlookers())
      sendEvent(GraphEvent(*this, GraphEvent::TLP_ADD_EDGES, edges));
  }
}
//----------------------------------------------------------------
void GraphImpl::addEdge(const edge e) {
  tlp::warning() << "Warning: "  << __PRETTY_FUNCTION__ << " ... Impossible operation on Root Graph" << std::endl;
  tlp::warning() << "\t Trying to add edge " << e.id << " (" << source(e).id << "," << target(e).id << ")";
}
//----------------------------------------------------------------
void GraphImpl::addEdges(Iterator<edge>*) {
  tlp::warning() << "Warning: "  << __PRETTY_FUNCTION__ << " ... Impossible operation on Root Graph" << std::endl;
}
//----------------------------------------------------------------
void GraphImpl::reserveEdges(unsigned int nb) {
  storage.reserveEdges(nb);
}
//----------------------------------------------------------------
void GraphImpl::removeNode(const node n) {
  assert(isElement(n));
  notifyDelNode(n);
  // remove from storage and propertyContainer
  storage.removeFromNodes(n);
  propertyContainer->erase(n);
}
//----------------------------------------------------------------
void GraphImpl::delNode(const node n, bool) {
  assert (isElement(n));
  notifyDelNode(n);
  // get edges vector with loops appearing only once
  std::vector<edge> edges;
  storage.getInOutEdges(n, edges, true);

  // use a queue for a dfs subgraphs propagation
  std::queue<Graph*> sgq;
  Iterator<Graph*>* sgs = getSubGraphs();

  while (sgs->hasNext()) {
    Graph* sg = sgs->next();

    if (sg->isElement(n))
      sgq.push(sg);
  }

  delete sgs;

  // subgraphs loop
  while(!sgq.empty()) {
    Graph* sg = sgq.front();

    sgs = sg->getSubGraphs();

    while (sgs->hasNext()) {
      Graph* ssg = sgs->next();

      if (ssg->isElement(n))
        sgq.push(ssg);
    }

    delete sgs;

    if (sg == sgq.front()) {
      ((GraphView *) sg)->removeNode(n, edges);
      sgq.pop();
    }
  }

  // loop on inout edges of n
  // for notification and removal from propertyContainer
  std::vector<edge>::const_iterator ite = edges.begin();

  while (ite != edges.end()) {
    edge e = (*ite);
    notifyDelEdge(e);
    propertyContainer->erase(e);
    ++ite;
  }

  // delete n from storage
  storage.delNode(n);

  // remove from propertyContainer
  propertyContainer->erase(n);
}
//----------------------------------------------------------------
void GraphImpl::delEdge(const edge e, bool) {
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
  }

  delete itS;

  removeEdge(e);
}
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
void GraphImpl::getInOutEdges(const node n, std::vector<edge>& edges,
                              bool loopsOnlyOnce) const {
  storage.getInOutEdges(n, edges, loopsOnlyOnce);
}
//----------------------------------------------------------------
std::vector<edge> GraphImpl::getEdges(const node src, const node tgt,
                                      bool directed) const {
  std::vector<edge> edges;
  storage.getEdges(src, tgt, directed, edges);
  return edges;
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
  notifyReverseEdge(e);

  // propagate edge reversal on subgraphs
  Graph* sg;
  forEach(sg, getSubGraphs()) {
    ((GraphView*) sg)->reverseInternal(e, eEnds.first, eEnds.second);
  }
}
//----------------------------------------------------------------
void GraphImpl::setEnds(const edge e, const node newSrc, const node newTgt) {
  assert(isElement(e));

  // not allowed on meta edge
  if (isMetaEdge(e)) {
    tlp::warning() << "Warning: invoking Graph::setEnds on meta edge " << e.id << std::endl;
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
  notifyBeforeSetEnds(e);

  storage.setEnds(e, newSrc, newTgt);

  // notification
  notifyAfterSetEnds(e);

  // propagate edge reversal on subgraphs
  Graph* sg;
  eEnds = storage.ends(e);
  node nSrc = eEnds.first;
  node nTgt = eEnds.second;

  forEach(sg, getSubGraphs()) {
    ((GraphView*) sg)->setEndsInternal(e, src, tgt, nSrc, nTgt);
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
  notifyDelEdge(e);
  // remove from propertyContainer and storage
  propertyContainer->erase(e);
  storage.delEdge(e);
}
//----------------------------------------------------------------
bool GraphImpl::canPop() {
  return (!recorders.empty());
}
//----------------------------------------------------------------
bool GraphImpl::canPopThenUnpop() {
  return (!recorders.empty() && recorders.front()->restartAllowed);
}
//----------------------------------------------------------------
bool GraphImpl::canUnpop() {
  return (!previousRecorders.empty());
}
//----------------------------------------------------------------
void GraphImpl::delPreviousRecorders() {
  std::list<GraphUpdatesRecorder*>::reverse_iterator it =
    previousRecorders.rbegin();

  // we delete previous recorders in reverse order
  // because they are pushed in front of previousRecorders
  // when they are popped from recorders,
  // so the lasts created are back in previousRecorders
  while(it != previousRecorders.rend()) {
    delete (*it);
    ++it;
  }

  previousRecorders.clear();
}
//----------------------------------------------------------------
void GraphImpl::treatEvents(const  std::vector<Event> &) {
  // an update occurs in the graph hierarchy
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
void GraphImpl::push(bool unpopAllowed,
                     std::vector<PropertyInterface*>* propsToPreserve) {
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
      delete (*it);
      recorders.erase(it);
      break;
    }

    ++nb;
    ++it;
  }

  if (propsToPreserve) {
    // the properties to preserve do not have to be observed
    for (unsigned int i = 0; i < propsToPreserve->size(); ++i)
      recorder->dontObserveProperty((*propsToPreserve)[i]);
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
    }
    else
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
bool GraphImpl::canDeleteProperty(Graph* g, PropertyInterface *prop) {
  return recorders.empty() ||
         !recorders.front()->isAddedOrDeletedProperty(g, prop);
}

