/**
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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

#include <stack>

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
GraphImpl::GraphImpl(): GraphAbstract(this) {
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
edge GraphImpl::existEdge(const node src, const node tgt,
                          bool directed) const {
  std::vector<edge> edges;
  return storage.getEdges(src, tgt, directed, edges, NULL, true) ? edges[0] : edge();
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
void GraphImpl::restoreNode(node newNode) {
  storage.restoreNode(newNode);
  notifyAddNode(newNode);
}
//----------------------------------------------------------------
node GraphImpl::addNode() {
  node newNode = storage.addNode();
  notifyAddNode(newNode);
  return newNode;
}
//----------------------------------------------------------------
void GraphImpl::addNodes(unsigned int nb) {
  if (nb) {
    storage.addNodes(nb);

    if (hasOnlookers())
      sendEvent(GraphEvent(*this, GraphEvent::TLP_ADD_NODES, nb));
  }
}
//----------------------------------------------------------------
void GraphImpl::addNodes(unsigned int nb, std::vector<node>& addedNodes) {
  if (nb) {
    storage.addNodes(nb, &addedNodes);

    if (hasOnlookers())
      sendEvent(GraphEvent(*this, GraphEvent::TLP_ADD_NODES, nb));
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
void GraphImpl::restoreEdge(edge newEdge, const node src, const node tgt) {
  storage.restoreEdge(src, tgt, newEdge);
  notifyAddEdge(newEdge);
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
    storage.addEdges(edges, &addedEdges);

    if (hasOnlookers())
      sendEvent(GraphEvent(*this, GraphEvent::TLP_ADD_EDGES, edges.size()));
  }
}
//----------------------------------------------------------------
void GraphImpl::addEdges(const std::vector<std::pair<node, node> >& edges) {
  if (!edges.empty()) {
    storage.addEdges(edges);

    if (hasOnlookers())
      sendEvent(GraphEvent(*this, GraphEvent::TLP_ADD_EDGES, edges.size()));
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
  std::vector<edge> edges(storage.adj(n));

  // use a stack for a dfs subgraphs propagation
  std::stack<Graph*> sgq;
  Iterator<Graph*>* sgs = getSubGraphs();

  while (sgs->hasNext()) {
    Graph* sg = sgs->next();

    if (sg->isElement(n))
      sgq.push(sg);
  }

  delete sgs;

  // subgraphs loop
  while(!sgq.empty()) {
    Graph* sg = sgq.top();

    sgs = sg->getSubGraphs();

    while (sgs->hasNext()) {
      Graph* ssg = sgs->next();

      if (ssg->isElement(n))
        sgq.push(ssg);
    }

    delete sgs;

    if (sg == sgq.top()) {
      static_cast<GraphView *>(sg)->removeNode(n, edges);
      sgq.pop();
    }
  }

  // loop on inout edges of n
  // for notification and removal from propertyContainer
  unsigned int nbEdges = edges.size();

  for (unsigned int i = 0; i < nbEdges; ++i) {
    edge e = edges[i];

    // if e is a loop it may have been previously deleted
    if (isElement(e))
      removeEdge(e);
  }

  notifyDelNode(n);
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
Iterator<node>* GraphImpl::getNodes() const {
  return new GraphNodeIterator(this, storage.getNodes());
}
//----------------------------------------------------------------
Iterator<node>* GraphImpl::getInNodes(const node n) const {
  return new GraphNodeIterator(this, storage.getInNodes(n));
}
//----------------------------------------------------------------
Iterator<node>* GraphImpl::getOutNodes(const node n) const {
  return new GraphNodeIterator(this, storage.getOutNodes(n));
}
//----------------------------------------------------------------
Iterator<node>* GraphImpl::getInOutNodes(const node n) const {
  return new GraphNodeIterator(this, storage.getInOutNodes(n));
}
//----------------------------------------------------------------
Iterator<edge>* GraphImpl::getEdges() const {
  return new GraphEdgeIterator(this, storage.getEdges());
}
//----------------------------------------------------------------
Iterator<edge>* GraphImpl::getInEdges(const node n) const {
  return new GraphEdgeIterator(this, storage.getInEdges(n));
}
//----------------------------------------------------------------
Iterator<edge>* GraphImpl::getOutEdges(const node n) const {
  return new GraphEdgeIterator(this, storage.getOutEdges(n));
}
//----------------------------------------------------------------
Iterator<edge>* GraphImpl::getInOutEdges(const node n) const {
  return new GraphEdgeIterator(this, storage.getInOutEdges(n));
}
//----------------------------------------------------------------
std::vector<edge> GraphImpl::getEdges(const node src, const node tgt,
                                      bool directed) const {
  std::vector<edge> edges;
  storage.getEdges(src, tgt, directed, edges);
  return edges;
}
//----------------------------------------------------------------
void GraphImpl::reverse(const edge e) {
  assert(isElement(e));
  std::pair<node, node> eEnds = storage.ends(e);

  // notification
  notifyReverseEdge(e);

  storage.reverse(e);

  // propagate edge reversal on subgraphs
  Graph* sg;
  forEach(sg, getSubGraphs()) {
    static_cast<GraphView*>(sg)->reverseInternal(e, eEnds.first, eEnds.second);
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
    static_cast<GraphView*>(sg)->setEndsInternal(e, src, tgt, nSrc, nTgt);
  }
}
//----------------------------------------------------------------
void GraphImpl::removeEdge(const edge e) {
  assert(isElement(e));
  notifyDelEdge(e);
  // remove from propertyContainer and storage
  storage.delEdge(e);
  propertyContainer->erase(e);
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

  bool hasRecorders = !recorders.empty();

  // if we have a current recorder with no updates
  // there is no need to push a new one, so go on with the same
  // (except if a temporary non redoable state is explicitely requested)
  if (unpopAllowed && hasRecorders && !recorders.front()->hasUpdates()) {
    return;
  }

  // end any previous updates observation
  unobserveUpdates();

  if (hasRecorders)
    // stop recording for current recorder
    recorders.front()->stopRecording(this);

  const GraphStorageIdsMemento* prevIdsMemento =
    hasRecorders ? recorders.front()->newIdsState : NULL;

  GraphUpdatesRecorder* recorder =
    new GraphUpdatesRecorder(unpopAllowed, prevIdsMemento);
  recorder->startRecording(this);
  recorders.push_front(recorder);

  // if this is not a temporary state used for computation purpose
  // as in BubbleTree for example
  if (unpopAllowed) {
    // delete first pushed recorders (those at the end of the list) if needed
    unsigned int nb = recorders.size();

    if (nb > NB_MAX_RECORDERS) {
      std::list<GraphUpdatesRecorder*>::reverse_iterator it =
        recorders.rbegin();

      while (nb > NB_MAX_RECORDERS) {
        delete (*it);
        --nb;
        ++it;
        recorders.pop_back();
      }
    }
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
void GraphImpl::popIfNoUpdates() {
  if(!recorders.empty() && !recorders.front()->hasUpdates())
    // no need of a "no updates" recorder
    this->pop(false);
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

