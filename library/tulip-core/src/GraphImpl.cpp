/**
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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
#include <tulip/GraphIterators.h>
#include <tulip/GraphUpdatesRecorder.h>
#include <tulip/BooleanProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/IntegerProperty.h>
#include <tulip/GraphProperty.h>
#include <tulip/LayoutProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/StringProperty.h>
#include <tulip/TulipViewSettings.h>

using namespace std;
using namespace tlp;

#ifndef NDEBUG
/*
 * function to test if an edge e exist in the adjacency of a node
 */
static bool existEdgeE(Graph *g, const node n1, const node, edge e) {
  for (auto e1 : g->getOutEdges(n1)) {
    if (e == e1) {
      return true;
    }
  }
  return false;
}
#endif
//----------------------------------------------------------------
// we define a subclass of IntegerProperty to add check functions to the setting
// of the value of some view... properties which only admit enumerated values
class TLP_SCOPE IntegerEnumeratedProperty : public IntegerProperty {
  bool (*_checkNodeValue)(int);
  bool (*_checkEdgeValue)(int);
  void printValueError(tlp::StoredType<int>::ReturnedConstValue v, const std::string &eltType);

public:
  IntegerEnumeratedProperty(Graph *g, const std::string &n = "");
  void setNodeValue(const node n, tlp::StoredType<int>::ReturnedConstValue v) override;
  void setAllNodeValue(tlp::StoredType<int>::ReturnedConstValue v) override;
  void setValueToGraphNodes(tlp::StoredType<int>::ReturnedConstValue v,
                            const Graph *graph) override;
  void setEdgeValue(const edge e, tlp::StoredType<int>::ReturnedConstValue v) override;
  void setAllEdgeValue(tlp::StoredType<int>::ReturnedConstValue v) override;
  void setValueToGraphEdges(tlp::StoredType<int>::ReturnedConstValue v,
                            const Graph *graph) override;
};
//----------------------------------------------------------------
bool rejectNonNullValue(int v) {
  return !v;
}
//----------------------------------------------------------------
IntegerEnumeratedProperty::IntegerEnumeratedProperty(Graph *g, const std::string &n)
    : IntegerProperty(g, n), _checkNodeValue(nullptr), _checkEdgeValue(nullptr) {
  if (name == "viewShape") {
    _checkNodeValue = tlp::NodeShape::checkValue;
    _checkEdgeValue = tlp::EdgeShape::checkValue;
    return;
  } else if (name == "viewLabelPosition") {
    _checkNodeValue = tlp::LabelPosition::checkValue;
    _checkEdgeValue = tlp::LabelPosition::checkValue;
    return;
  } else if (name == "viewSrcAnchorShape" || name == "viewTgtAnchorShape") {
    _checkNodeValue = rejectNonNullValue;
    _checkEdgeValue = tlp::EdgeExtremityShape::checkValue;
  }
}
//----------------------------------------------------------------
void IntegerEnumeratedProperty::printValueError(tlp::StoredType<int>::ReturnedConstValue v,
                                                const std::string &eltType) {
  tlp::error() << "Error: '" << v << "' is not a valid " << eltType << " value for property \""
               << getName() << "\"\n";
}
//----------------------------------------------------------------
void IntegerEnumeratedProperty::setNodeValue(const node n,
                                             tlp::StoredType<int>::ReturnedConstValue v) {
  if (_checkNodeValue && !_checkNodeValue(v)) {
    printValueError(v, "node");
    return;
  }
  IntegerProperty::setNodeValue(n, v);
}
//----------------------------------------------------------------
void IntegerEnumeratedProperty::setEdgeValue(const edge e,
                                             tlp::StoredType<int>::ReturnedConstValue v) {
  if (_checkEdgeValue && !_checkEdgeValue(v)) {
    printValueError(v, "edge");
    return;
  }
  IntegerProperty::setEdgeValue(e, v);
}
//----------------------------------------------------------------
void IntegerEnumeratedProperty::setAllNodeValue(tlp::StoredType<int>::ReturnedConstValue v) {
  if (_checkNodeValue && !_checkNodeValue(v)) {
    printValueError(v, "node");
    return;
  }
  IntegerProperty::setAllNodeValue(v);
}
//----------------------------------------------------------------
void IntegerEnumeratedProperty::setValueToGraphNodes(tlp::StoredType<int>::ReturnedConstValue v,
                                                     const Graph *graph) {
  if (_checkNodeValue && !_checkNodeValue(v)) {
    printValueError(v, "node");
    return;
  }
  IntegerProperty::setValueToGraphNodes(v, graph);
}
//----------------------------------------------------------------
void IntegerEnumeratedProperty::setAllEdgeValue(tlp::StoredType<int>::ReturnedConstValue v) {
  if (_checkEdgeValue && !_checkEdgeValue(v)) {
    printValueError(v, "edge");
    return;
  }
  IntegerProperty::setAllEdgeValue(v);
}
//----------------------------------------------------------------
void IntegerEnumeratedProperty::setValueToGraphEdges(tlp::StoredType<int>::ReturnedConstValue v,
                                                     const Graph *graph) {
  if (_checkEdgeValue && !_checkEdgeValue(v)) {
    printValueError(v, "edge");
    return;
  }
  IntegerProperty::setValueToGraphEdges(v, graph);
}
//----------------------------------------------------------------
GraphImpl::GraphImpl() : GraphAbstract(this) {
  // id 0 is for the root
  graphIds.get();
}
//----------------------------------------------------------------
GraphImpl::~GraphImpl() {
  unobserveUpdates();

  // delete recorders
  if (!recorders.empty()) {
    recorders.front()->stopRecording(this);
    std::list<GraphUpdatesRecorder *>::iterator it = recorders.begin();

    while (it != recorders.end()) {
      delete (*it);
      ++it;
    }

    recorders.clear();
  }

  delPreviousRecorders();
}
//----------------------------------------------------------------
void GraphImpl::clear() {
  GraphAbstract::clear();
  storage.clear();
}
//----------------------------------------------------------------
Graph *GraphImpl::newGraph() {
  Graph *g = new GraphImpl();

  // set "view..'" properties defaults
  {
    auto prop = g->getProperty<ColorProperty>("viewColor");
    prop->setAllNodeValue(TulipViewSettings::defaultColor(NODE));
    prop->setAllEdgeValue(TulipViewSettings::defaultColor(EDGE));
  }
  {
    auto prop = g->getProperty<ColorProperty>("viewBorderColor");
    prop->setAllNodeValue(TulipViewSettings::defaultBorderColor(NODE));
    prop->setAllEdgeValue(TulipViewSettings::defaultBorderColor(EDGE));
  }
  {
    auto prop = g->getProperty<DoubleProperty>("viewBorderWidth");
    prop->setAllNodeValue(TulipViewSettings::defaultBorderWidth(NODE));
    prop->setAllEdgeValue(TulipViewSettings::defaultBorderWidth(EDGE));
  }
  {
    auto prop = g->getProperty<StringProperty>("viewFont");
    prop->setAllNodeValue(TulipViewSettings::defaultFontFile());
    prop->setAllEdgeValue(TulipViewSettings::defaultFontFile());
  }
  {
    auto prop = g->getProperty<IntegerProperty>("viewFontSize");
    prop->setAllNodeValue(TulipViewSettings::defaultFontSize());
    prop->setAllEdgeValue(TulipViewSettings::defaultFontSize());
  }
  {
    auto prop = g->getProperty<StringProperty>("viewIcon");
    prop->setAllNodeValue("fas-circle-question");
    prop->setAllEdgeValue("fas-circle-question");
  }
  {
    auto prop = g->getProperty<StringProperty>("viewLabel");
    prop->setAllNodeValue("");
    prop->setAllEdgeValue("");
  }
  {
    auto prop = g->getProperty<ColorProperty>("viewLabelColor");
    prop->setAllNodeValue(TulipViewSettings::defaultLabelColor());
    prop->setAllEdgeValue(TulipViewSettings::defaultLabelColor());
  }
  {
    auto prop = g->getProperty<ColorProperty>("viewLabelBorderColor");
    prop->setAllNodeValue(TulipViewSettings::defaultLabelBorderColor());
    prop->setAllEdgeValue(TulipViewSettings::defaultLabelBorderColor());
  }
  {
    auto prop = g->getProperty<DoubleProperty>("viewLabelBorderWidth");
    prop->setAllNodeValue(TulipViewSettings::defaultLabelBorderWidth());
    prop->setAllEdgeValue(TulipViewSettings::defaultLabelBorderWidth());
  }
  {
    auto prop = g->getProperty<IntegerEnumeratedProperty>("viewLabelPosition");
    prop->setAllNodeValue(TulipViewSettings::defaultLabelPosition());
    prop->setAllEdgeValue(TulipViewSettings::defaultLabelPosition());
  }
  {
    auto prop = g->getProperty<LayoutProperty>("viewLayout");
    prop->setAllNodeValue(Coord(0, 0, 0));
    prop->setAllEdgeValue(std::vector<Coord>());
  }
  {
    auto prop = g->getProperty<DoubleProperty>("viewMetric");
    prop->setAllNodeValue(0);
    prop->setAllEdgeValue(0);
  }
  {
    auto prop = g->getProperty<DoubleProperty>("viewRotation");
    prop->setAllNodeValue(0);
    prop->setAllEdgeValue(0);
  }
  {
    auto prop = g->getProperty<BooleanProperty>("viewSelection");
    prop->setAllNodeValue(false);
    prop->setAllEdgeValue(false);
  }
  {
    auto prop = g->getProperty<IntegerEnumeratedProperty>("viewShape");
    prop->setAllNodeValue(TulipViewSettings::defaultShape(NODE));
    prop->setAllEdgeValue(TulipViewSettings::defaultShape(EDGE));
  }
  {
    auto prop = g->getProperty<SizeProperty>("viewSize");
    prop->setAllNodeValue(TulipViewSettings::defaultSize(NODE));
    prop->setAllEdgeValue(TulipViewSettings::defaultSize(EDGE));
  }
  g->getProperty<IntegerEnumeratedProperty>("viewSrcAnchorShape")
      ->setAllEdgeValue(TulipViewSettings::defaultEdgeExtremitySrcShape());
  {
    auto prop = g->getProperty<SizeProperty>("viewSrcAnchorSize");
    prop->setAllEdgeValue(TulipViewSettings::defaultEdgeExtremitySrcSize());
    prop->setAllEdgeValue(TulipViewSettings::defaultEdgeExtremityTgtSize());
  }
  {
    auto prop = g->getProperty<StringProperty>("viewTexture");
    prop->setAllNodeValue("");
    prop->setAllEdgeValue("");
  }
  g->getProperty<IntegerEnumeratedProperty>("viewTgtAnchorShape")
      ->setAllEdgeValue(TulipViewSettings::defaultEdgeExtremityTgtShape());
  return g;
}
//----------------------------------------------------------------
edge GraphImpl::existEdge(const node src, const node tgt, bool directed) const {
  std::vector<edge> edges;
  return storage.getEdges(src, tgt, directed, edges, nullptr, true) ? edges[0] : edge();
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
void GraphImpl::addNodes(unsigned int nb, std::vector<node> &addedNodes) {
  if (nb) {
    storage.addNodes(nb, &addedNodes);

    if (hasOnlookers())
      sendEvent(GraphEvent(*this, GraphEvent::TLP_ADD_NODES, nb));
  }
}
//----------------------------------------------------------------
void GraphImpl::addNode(const node) {
  tlp::warning() << "Warning : " << __PRETTY_FUNCTION__ << " ... Impossible operation on Root Graph"
                 << std::endl;
}
//----------------------------------------------------------------
void GraphImpl::addNodes(Iterator<node> *) {
  tlp::warning() << "Warning : " << __PRETTY_FUNCTION__ << " ... Impossible operation on Root Graph"
                 << std::endl;
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
  assert(src.isValid() && tgt.isValid());
  edge newEdge = storage.addEdge(src, tgt);
  notifyAddEdge(newEdge);
  return newEdge;
}
//----------------------------------------------------------------
void GraphImpl::addEdges(const std::vector<std::pair<node, node>> &edges,
                         std::vector<edge> &addedEdges) {
  if (!edges.empty()) {
    storage.addEdges(edges, &addedEdges);

    if (hasOnlookers())
      sendEvent(GraphEvent(*this, GraphEvent::TLP_ADD_EDGES, edges.size()));
  }
}
//----------------------------------------------------------------
void GraphImpl::addEdges(const std::vector<std::pair<node, node>> &edges) {
  if (!edges.empty()) {
    storage.addEdges(edges);

    if (hasOnlookers())
      sendEvent(GraphEvent(*this, GraphEvent::TLP_ADD_EDGES, edges.size()));
  }
}
//----------------------------------------------------------------
void GraphImpl::addEdge(const edge e) {
  tlp::warning() << "Warning: " << __PRETTY_FUNCTION__ << " ... Impossible operation on Root Graph"
                 << std::endl;
  tlp::warning() << "\t Trying to add edge " << e.id << " (" << source(e).id << "," << target(e).id
                 << ")";
}
//----------------------------------------------------------------
void GraphImpl::addEdges(Iterator<edge> *) {
  tlp::warning() << "Warning: " << __PRETTY_FUNCTION__ << " ... Impossible operation on Root Graph"
                 << std::endl;
}
//----------------------------------------------------------------
void GraphImpl::reserveEdges(unsigned int nb) {
  storage.reserveEdges(nb);
}
//----------------------------------------------------------------
void GraphImpl::removeNode(const node n) {
  assert(isElement(n));
  notifyBeforeDelNode(n);

  // remove from storage and propertyContainer
  storage.removeFromNodes(n);
  propertyContainer->erase(n);

  notifyAfterDelNode(n);
}
//----------------------------------------------------------------
void GraphImpl::delNode(const node n, bool) {
  assert(isElement(n));
  std::vector<edge> edges(storage.adj(n));

  // use a stack for a dfs subgraphs propagation
  std::stack<Graph *> sgq;

  for (Graph *sg : subGraphs()) {
    if (sg->isElement(n))
      sgq.push(sg);
  }

  // subgraphs loop
  while (!sgq.empty()) {
    Graph *sg = sgq.top();

    for (Graph *ssg : sg->subGraphs()) {
      if (ssg->isElement(n))
        sgq.push(ssg);
    }

    if (sg == sgq.top()) {
      static_cast<GraphView *>(sg)->removeNode(n, edges);
      sgq.pop();
    }
  }

  // loop on inout edges of n
  // for notification and removal from propertyContainer
  for (auto e : edges) {
    // if e is a loop it may have been previously deleted
    if (isElement(e))
      removeEdge(e);
  }

  notifyBeforeDelNode(n);

  // delete n from storage
  storage.delNode(n);

  // remove from propertyContainer
  propertyContainer->erase(n);

  notifyAfterDelNode(n);
}
//----------------------------------------------------------------
void GraphImpl::delEdge(const edge e, bool) {
  assert(existEdgeE(this, source(e), target(e), e));

  if (!isElement(e)) {
    return;
  }

  // propagate to subgraphs
  for (Graph *subgraph : subGraphs()) {
    assert(subgraph != this);

    if (subgraph->isElement(e))
      subgraph->delEdge(e);
  }

  removeEdge(e);
}
//----------------------------------------------------------------
Iterator<node> *GraphImpl::getNodes() const {
  return new GraphNodeIterator(this, storage.getNodes());
}
//----------------------------------------------------------------
Iterator<node> *GraphImpl::getInNodes(const node n) const {
  return new GraphNodeIterator(this, storage.getInNodes(n));
}
//----------------------------------------------------------------
Iterator<node> *GraphImpl::getOutNodes(const node n) const {
  return new GraphNodeIterator(this, storage.getOutNodes(n));
}
//----------------------------------------------------------------
Iterator<node> *GraphImpl::getInOutNodes(const node n) const {
  return new GraphNodeIterator(this, storage.getInOutNodes(n));
}
//----------------------------------------------------------------
Iterator<edge> *GraphImpl::getEdges() const {
  return new GraphEdgeIterator(this, storage.getEdges());
}
//----------------------------------------------------------------
Iterator<edge> *GraphImpl::getInEdges(const node n) const {
  return new GraphEdgeIterator(this, storage.getInEdges(n));
}
//----------------------------------------------------------------
Iterator<edge> *GraphImpl::getOutEdges(const node n) const {
  return new GraphEdgeIterator(this, storage.getOutEdges(n));
}
//----------------------------------------------------------------
Iterator<edge> *GraphImpl::getInOutEdges(const node n) const {
  return new GraphEdgeIterator(this, storage.getInOutEdges(n));
}
//----------------------------------------------------------------
std::vector<edge> GraphImpl::getEdges(const node src, const node tgt, bool directed) const {
  std::vector<edge> edges;
  storage.getEdges(src, tgt, directed, edges);
  return edges;
}
//----------------------------------------------------------------
void GraphImpl::reverse(const edge e) {
  assert(isElement(e));
  auto eEnds = storage.ends(e);

  // notification
  notifyReverseEdge(e);

  storage.reverse(e);

  // propagate edge reversal on subgraphs
  for (Graph *sg : subGraphs()) {
    static_cast<GraphView *>(sg)->reverseInternal(e, eEnds.first, eEnds.second);
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
  eEnds = storage.ends(e);
  node nSrc = eEnds.first;
  node nTgt = eEnds.second;

  for (Graph *sg : subGraphs()) {
    static_cast<GraphView *>(sg)->setEndsInternal(e, src, tgt, nSrc, nTgt);
  }
}
//----------------------------------------------------------------
void GraphImpl::removeEdge(const edge e) {
  assert(isElement(e));
  notifyBeforeDelEdge(e);

  // remove from propertyContainer and storage
  storage.delEdge(e);
  propertyContainer->erase(e);

  notifyAfterDelEdge(e);
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
  std::list<GraphUpdatesRecorder *>::reverse_iterator it = previousRecorders.rbegin();

  // we delete previous recorders in reverse order
  // because they are pushed in front of previousRecorders
  // when they are popped from recorders,
  // so the lasts created are back in previousRecorders
  while (it != previousRecorders.rend()) {
    delete (*it);
    ++it;
  }

  previousRecorders.clear();
}
//----------------------------------------------------------------
void GraphImpl::treatEvents(const std::vector<Event> &) {
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
  for (PropertyInterface *prop : g->getLocalObjectProperties()) {
    prop->addObserver(this);
    observedProps.push_front(prop);
  }

  // loop on subgraphs
  for (Graph *sg : g->subGraphs()) {
    observeUpdates(sg);
  }
}
//----------------------------------------------------------------
void GraphImpl::unobserveUpdates() {
  // loop on observed graphs
  while (!observedGraphs.empty()) {
    observedGraphs.front()->removeObserver(this);
    observedGraphs.pop_front();
  }

  // loop on observed properties
  while (!observedProps.empty()) {
    observedProps.front()->removeObserver(this);
    observedProps.pop_front();
  }
}
//----------------------------------------------------------------
#define NB_MAX_RECORDERS 10
void GraphImpl::push(bool unpopAllowed, std::vector<PropertyInterface *> *propsToPreserve) {
  // from now if previous recorders exist
  // they cannot be unpop
  // so delete them
  delPreviousRecorders();

  bool hasRecorders = !recorders.empty();

  // if we have a current recorder with no updates
  // there is no need to push a new one, so go on with the same
  // (except if a temporary non redoable state is explicitly requested)
  if (unpopAllowed && hasRecorders && !recorders.front()->hasUpdates()) {
    return;
  }

  // end any previous updates observation
  unobserveUpdates();

  if (hasRecorders)
    // stop recording for current recorder
    recorders.front()->stopRecording(this);

  const GraphStorageIdsMemento *prevIdsMemento =
      hasRecorders ? recorders.front()->newIdsState : nullptr;

  GraphUpdatesRecorder *recorder = new GraphUpdatesRecorder(unpopAllowed, prevIdsMemento);
  recorder->startRecording(this);
  recorders.push_front(recorder);

  // if this is not a temporary state used for computation purpose
  // as in BubbleTree for example
  if (unpopAllowed) {
    // delete first pushed recorders (those at the end of the list) if needed
    unsigned int nb = recorders.size();

    if (nb > NB_MAX_RECORDERS) {
      std::list<GraphUpdatesRecorder *>::reverse_iterator it = recorders.rbegin();

      while (nb > NB_MAX_RECORDERS) {
        delete (*it);
        --nb;
        ++it;
      }
      recorders.resize(nb);
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
    // if (!previousRecorders.empty())
    unobserveUpdates();
    GraphUpdatesRecorder *prevRecorder = recorders.front();

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
void GraphImpl::popIfNoUpdates() {
  if (!recorders.empty() && !recorders.front()->hasUpdates())
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

    GraphUpdatesRecorder *prevRecorder = previousRecorders.front();
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
bool GraphImpl::canDeleteProperty(Graph *g, PropertyInterface *prop) {
  return recorders.empty() || !recorders.front()->isAddedOrDeletedProperty(g, prop);
}
