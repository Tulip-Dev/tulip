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
#include <tulip/GraphDecorator.h>

using namespace tlp;

GraphDecorator::GraphDecorator(Graph* s):graph_component(s) {
  assert(s);
}

GraphDecorator::~GraphDecorator() {
  notifyDestroy();
}


//============================================================
void GraphDecorator::clear() {
  graph_component->clear();
}

//============================================================
Graph *GraphDecorator::addSubGraph(BooleanProperty *selection,
                                   const std::string& name) {
  Graph* sg = graph_component->addSubGraph(selection, name);
  notifyBeforeAddSubGraph(sg);
  notifyAfterAddSubGraph(sg);
  return sg;
}

//============================================================
void GraphDecorator::delSubGraph(Graph *s) {
  notifyBeforeDelSubGraph(s);
  graph_component->delSubGraph(s);
  notifyAfterDelSubGraph(s);
}
//============================================================
unsigned int GraphDecorator::numberOfSubGraphs() const {
  return graph_component->numberOfSubGraphs();
}
//============================================================
unsigned int GraphDecorator::numberOfDescendantGraphs() const {
  return graph_component->numberOfDescendantGraphs();
}
//============================================================
void GraphDecorator::restoreSubGraph(Graph *) {
  tlp::warning() << "Warning : "  << __PRETTY_FUNCTION__ << " ... Impossible operation" << std::endl;
}

//============================================================
void GraphDecorator::setSubGraphToKeep(Graph *) {
  tlp::warning() << "Warning : "  << __PRETTY_FUNCTION__ << " ... Impossible operation" << std::endl;
}

//============================================================
void GraphDecorator::removeSubGraph(Graph*) {
  tlp::warning() << "Warning : "  << __PRETTY_FUNCTION__ << " ... Impossible operation" << std::endl;
}

//============================================================
void GraphDecorator::clearSubGraphs() {
  tlp::warning() << "Warning : "  << __PRETTY_FUNCTION__ << " ... Impossible operation" << std::endl;
}

//============================================================
void GraphDecorator::delAllSubGraphs(Graph *s) {
  notifyBeforeDelSubGraph(s);
  graph_component->delAllSubGraphs(s);
  notifyAfterDelSubGraph(s);
}

//============================================================
Graph* GraphDecorator::getSuperGraph()const {
  return graph_component->getSuperGraph();
}

//============================================================
node GraphDecorator::createMetaNode(const std::set<node> &nodeSet, bool multiEdges, bool delAllEdge) {
  return graph_component->createMetaNode(nodeSet, multiEdges, delAllEdge);
}

void GraphDecorator::createMetaNodes(Iterator<Graph *> *itS, Graph *quotientGraph, std::vector<node>& metaNodes) {
  graph_component->createMetaNodes(itS, quotientGraph, metaNodes);
}

node GraphDecorator::createMetaNode(Graph* subGraph, bool multiEdges, bool delAllEdge) {
  return graph_component->createMetaNode(subGraph, multiEdges, delAllEdge);
}

//============================================================
void GraphDecorator::setSuperGraph(Graph* s) {
  graph_component->setSuperGraph(s);
}

//============================================================
Graph* GraphDecorator::getRoot() const {
  return graph_component->getRoot();
}

//============================================================
Iterator<Graph *> * GraphDecorator::getSubGraphs() const {
  return graph_component->getSubGraphs();
}

//=========================================================================
bool GraphDecorator::isSubGraph(const Graph* sg) const {
  return graph_component->isSubGraph(sg);
}
//=========================================================================
bool GraphDecorator::isDescendantGraph(const Graph* sg) const {
  return graph_component->isDescendantGraph(sg);
}
//=========================================================================
Graph* GraphDecorator::getSubGraph(unsigned int sgId) const {
  return graph_component->getSubGraph(sgId);
}
//=========================================================================
Graph* GraphDecorator::getSubGraph(const std::string &name) const {
  return graph_component->getSubGraph(name);
}
//=========================================================================
Graph* GraphDecorator::getDescendantGraph(unsigned int sgId) const {
  return graph_component->getDescendantGraph(sgId);
}
//=========================================================================
Graph* GraphDecorator::getDescendantGraph(const std::string &name) const {
  return graph_component->getDescendantGraph(name);
}
//============================================================
node GraphDecorator::source(const edge e)const {
  return graph_component->source(e);
}

//============================================================
void GraphDecorator::setSource(const edge e, const node n) {
  graph_component->setSource(e, n);
}

//============================================================
node GraphDecorator::target(const edge e)const {
  return graph_component->target(e);
}

//============================================================
void GraphDecorator::setTarget(const edge e, const node n) {
  graph_component->setTarget(e, n);
}

//============================================================
node GraphDecorator::opposite(const edge e, const node n)const {
  return graph_component->opposite(e,n);
}

//============================================================
bool GraphDecorator::hasEdge(const node n, const node m,
                             bool directed) const {
  return graph_component->hasEdge(n, m, directed);
}

//============================================================
std::vector<edge> GraphDecorator::getEdges(const node n, const node m,
    bool directed) const {
  return graph_component->getEdges(n, m, directed);
}

//============================================================
edge GraphDecorator::existEdge(const node n, const node m,
                               bool directed) const {
  return graph_component->existEdge(n, m, directed);
}

//============================================================
const std::pair<node, node>& GraphDecorator::ends(const edge e) const {
  return graph_component->ends(e);
}

//============================================================
void GraphDecorator::setEnds(const edge e, const node src, const node tgt) {
  graph_component->setEnds(e, src, tgt);
}

//============================================================
void GraphDecorator::reverse(const edge e) {
  notifyReverseEdge(e);
  graph_component->reverse(e);
}

//============================================================
unsigned int GraphDecorator::deg(const node n)const {
  return graph_component->deg(n);
}

//============================================================
unsigned int GraphDecorator::indeg(const node n)const {
  return graph_component->indeg(n);
}

//============================================================
unsigned int GraphDecorator::outdeg(const node n)const {
  return graph_component->outdeg(n);
}

//============================================================
Graph* GraphDecorator::getNodeMetaInfo(const node n)const {
  return graph_component->getNodeMetaInfo(n);
}

//============================================================
node GraphDecorator::getOneNode() const {
  return graph_component->getOneNode();
}

//============================================================
node GraphDecorator::getInNode(const node n,unsigned int i)const {
  return graph_component->getInNode(n,i);
}

//============================================================
node GraphDecorator::getOutNode(const node n,unsigned int i) const {
  return graph_component->getOutNode(n,i);
}

edge GraphDecorator::getOneEdge() const {
  return graph_component->getOneEdge();
}

//============================================================
unsigned int GraphDecorator::numberOfNodes()const {
  return graph_component->numberOfNodes();
}

//============================================================
void GraphDecorator::reserveNodes(unsigned int nbNodes) {
  return graph_component->reserveNodes(nbNodes);
}

//============================================================
unsigned int GraphDecorator::numberOfEdges()const {
  return graph_component->numberOfEdges();
}

//============================================================
void GraphDecorator::reserveEdges(unsigned int nbEdges) {
  return graph_component->reserveEdges(nbEdges);
}

//============================================================
bool GraphDecorator::isElement(const node n) const {
  return graph_component->isElement(n);
}

//============================================================
bool GraphDecorator::isMetaNode(const node n) const {
  return graph_component->isMetaNode(n);
}

//============================================================
bool GraphDecorator::isElement(const edge e) const {
  return graph_component->isElement(e);
}

//============================================================
bool GraphDecorator::isMetaEdge(const edge e) const {
  return graph_component->isMetaEdge(e);
}

//============================================================
node GraphDecorator::addNode() {
  node newNode = graph_component->addNode();
  notifyAddNode(newNode);
  return newNode;
}

//============================================================
void GraphDecorator::addNodes(unsigned int nb, std::vector<node>& addedNodes) {
  graph_component->addNodes(nb, addedNodes);

  if (hasOnlookers())
    sendEvent(GraphEvent(*this, GraphEvent::TLP_ADD_NODES, addedNodes));
}

//============================================================
void GraphDecorator::addNode(const node) {
  tlp::warning() << "Warning : "  << __PRETTY_FUNCTION__ << " ... Impossible operation" << std::endl;
}

//============================================================
void GraphDecorator::addNodes(Iterator<node>*) {
  tlp::warning() << "Warning : "  << __PRETTY_FUNCTION__ << " ... Impossible operation" << std::endl;
}

//============================================================
node GraphDecorator::restoreNode(node n) {
  tlp::warning() << "Warning : "  << __PRETTY_FUNCTION__ << " ... Impossible operation" << std::endl;
  return n;
}

//============================================================
void GraphDecorator::restoreNodes(const std::vector<node>&) {
  tlp::warning() << "Warning : "  << __PRETTY_FUNCTION__ << " ... Impossible operation" << std::endl;
}

//============================================================
void GraphDecorator::removeNode(const node) {
  tlp::warning() << "Warning : "  << __PRETTY_FUNCTION__ << " ... Impossible operation" << std::endl;
}

//============================================================
edge GraphDecorator::addEdge(const node n, const node n2) {
  edge newEdge = graph_component->addEdge(n, n2);
  notifyAddEdge(newEdge);
  return newEdge;
}

//============================================================
void GraphDecorator::addEdges(const std::vector<std::pair<node, node> >& edges,
                              std::vector<edge>& addedEdges) {
  graph_component->addEdges(edges, addedEdges);

  if (hasOnlookers())
    sendEvent(GraphEvent(*this, GraphEvent::TLP_ADD_EDGES, addedEdges));
}

//============================================================
void GraphDecorator::addEdge(const edge) {
  tlp::warning() << "Warning : "  << __PRETTY_FUNCTION__ << " ... Impossible operation" << std::endl;
}

//============================================================
void GraphDecorator::addEdges(Iterator<edge>*) {
  tlp::warning() << "Warning : "  << __PRETTY_FUNCTION__ << " ... Impossible operation" << std::endl;
}

//============================================================
edge GraphDecorator::restoreEdge(edge e, node, node) {
  tlp::warning() << "Warning : "  << __PRETTY_FUNCTION__ << " ... Impossible operation" << std::endl;
  return e;
}

//============================================================
void GraphDecorator::restoreEdges(const std::vector<edge>&,
                                  const std::vector<std::pair<node, node> >&) {
  tlp::warning() << "Warning : "  << __PRETTY_FUNCTION__ << " ... Impossible operation" << std::endl;
}
//============================================================
void GraphDecorator::removeEdge(const edge) {
  tlp::warning() << "Warning : "  << __PRETTY_FUNCTION__ << " ... Impossible operation" << std::endl;
}

//============================================================
void GraphDecorator::delNode(const node n, bool deleteInAllGraphs) {
  notifyDelNode(n);
  graph_component->delNode(n, deleteInAllGraphs);
}

//============================================================
void GraphDecorator::delNodes(Iterator<node>* itN, bool deleteInAllGraphs) {
  assert(itN != NULL);

  while(itN->hasNext()) {
    delNode(itN->next(), deleteInAllGraphs);
  }
}

//============================================================
void GraphDecorator::delEdge(const edge e, bool deleteInAllGraphs) {
  notifyDelEdge(e);
  graph_component->delEdge(e, deleteInAllGraphs);
}

//=========================================================================
void GraphDecorator::delEdges(Iterator<edge>* itE, bool deleteInAllGraphs) {
  assert(itE != NULL);

  while(itE->hasNext()) {
    delEdge(itE->next(), deleteInAllGraphs);
  }
}

//============================================================
void GraphDecorator::setEdgeOrder(const node n,const std::vector<edge> & s) {
  graph_component->setEdgeOrder(n,s);
}

void GraphDecorator::swapEdgeOrder(const node n ,const edge e1, const edge e2) {
  graph_component->swapEdgeOrder(n, e1, e2);
}

//============================================================
Iterator<node>* GraphDecorator::getNodes() const {
  return graph_component->getNodes();
}

//============================================================
Iterator<node>* GraphDecorator::getInNodes(const node n) const {
  return graph_component->getInNodes(n);
}

//============================================================
Iterator<node>* GraphDecorator::getOutNodes(const node n) const {
  return graph_component->getOutNodes(n);
}

//============================================================
Iterator<node>* GraphDecorator::getInOutNodes(const node n) const {
  return graph_component->getInOutNodes(n);
}
//============================================================
Iterator<node>* GraphDecorator::bfs(const node root) const {
  return graph_component->bfs(root);
}
//============================================================
Iterator<node>* GraphDecorator::dfs(const node root) const {
  return graph_component->dfs(root);
}
//============================================================
Iterator<edge>* GraphDecorator::getEdges() const {
  return graph_component->getEdges();
}
//============================================================
Iterator<edge>* GraphDecorator::getOutEdges(const node n) const {
  return graph_component->getOutEdges(n);
}
//============================================================
Iterator<edge>* GraphDecorator::getInOutEdges(const node n) const {
  return graph_component->getInOutEdges(n);
}
//============================================================
Iterator<edge>* GraphDecorator::getInEdges(const node n) const {
  return graph_component->getInEdges(n);
}
//============================================================
Iterator<edge>* GraphDecorator::getEdgeMetaInfo(const edge e) const {
  return graph_component->getEdgeMetaInfo(e);
}
//============================================================
DataSet& GraphDecorator::getNonConstAttributes() {
  return graph_component->getNonConstAttributes();
}
//============================================================
PropertyInterface* GraphDecorator::getProperty(const std::string &name) const {
  return graph_component->getProperty(name);
}

//============================================================
bool GraphDecorator::existProperty(const std::string&name) const {
  return graph_component->existProperty(name);
}

//============================================================
bool GraphDecorator::existLocalProperty(const std::string&name) const {
  return graph_component->existLocalProperty(name) ;
}

//============================================================
void GraphDecorator::delLocalProperty(const std::string&name) {
  notifyBeforeDelLocalProperty(name);
  graph_component->delLocalProperty(name);
  notifyAfterDelLocalProperty(name);
}

//============================================================
void GraphDecorator::addLocalProperty(const std::string &name, PropertyInterface *prop) {
  notifyBeforeAddLocalProperty(name);
  graph_component->addLocalProperty(name, prop);
  notifyAddLocalProperty(name);
}

//============================================================
bool GraphDecorator::renameLocalProperty(PropertyInterface* prop,
    const std::string& newName) {
  return graph_component->renameLocalProperty(prop, newName);
}

//============================================================
Iterator<std::string>* GraphDecorator::getLocalProperties() const {
  return graph_component->getLocalProperties();
}

//============================================================
Iterator<std::string>* GraphDecorator::getInheritedProperties() const {
  return graph_component->getInheritedProperties();
}

//============================================================
Iterator<std::string>* GraphDecorator::getProperties() const {
  return graph_component->getProperties();
}
//============================================================
Iterator<PropertyInterface*>* GraphDecorator::getLocalObjectProperties() const {
  return graph_component->getLocalObjectProperties();
}

//============================================================
Iterator<PropertyInterface*>* GraphDecorator::getInheritedObjectProperties() const {
  return graph_component->getInheritedObjectProperties();
}

//============================================================
Iterator<PropertyInterface*>* GraphDecorator::getObjectProperties() const {
  return graph_component->getObjectProperties();
}
//----------------------------------------------------------------
bool GraphDecorator::canPop() {
  return graph_component->canPop();
}
bool GraphDecorator::canUnpop() {
  return graph_component->canUnpop();
}
bool GraphDecorator::canPopThenUnpop() {
  return graph_component->canPopThenUnpop();
}
//----------------------------------------------------------------
void GraphDecorator::push(bool unpopAllowed,
                          std::vector<PropertyInterface*>* propertiesToPreserveOnPop) {
  return graph_component->push(unpopAllowed, propertiesToPreserveOnPop);
}
//----------------------------------------------------------------
void GraphDecorator::pop(bool unpopAllowed) {
  return graph_component->pop(unpopAllowed);
}
//----------------------------------------------------------------
void GraphDecorator::unpop() {
  return graph_component->unpop();
}
//----------------------------------------------------------------
void GraphDecorator::setName(const std::string &name) {
  graph_component->setName(name);
}
//----------------------------------------------------------------
std::string GraphDecorator::getName() const {
  return graph_component->getName();
}
