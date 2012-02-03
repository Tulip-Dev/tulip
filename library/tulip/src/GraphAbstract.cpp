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

#include <tulip/GraphAbstract.h>
#include <tulip/PropertyManager.h>
#include <tulip/GraphProperty.h>
#include <tulip/StlIterator.h>
#include <tulip/StableIterator.h>
#include <tulip/GraphView.h>
#include <tulip/GraphImpl.h>
#include <tulip/ConcatIterator.h>

using namespace std;
using namespace tlp;

const string metaGraphPropertyName = "viewMetaGraph";

//=========================================================================
GraphAbstract::GraphAbstract(Graph *supergraph, unsigned int sgId)
  :supergraph(supergraph ? supergraph : this),
   root((supergraph == this) ? this : supergraph->getRoot()),
   subGraphToKeep(NULL), metaGraphProperty(NULL) {
  // get id
  if (supergraph != this)
    id = ((GraphImpl *) getRoot())->getSubGraphId(sgId);

  propertyContainer = new PropertyManager(this);
}
//=========================================================================
GraphAbstract::~GraphAbstract() {
  StableIterator<Graph *> itS(getSubGraphs());

  while(itS.hasNext()) {
    Graph* sg = itS.next();

    // avoid double free
    // in a push context, a 'deleted' graph (see delSubGraph)
    // may still have a non empty list of subgraphs
    if (sg->getSuperGraph() == this) {
      if (id == 0)
        // indicates root destruction (see below)
        sg->id = 0;

      delete sg;
    }
  }

  delete propertyContainer;

  if (id != 0)
    ((GraphImpl *) getRoot())->freeSubGraphId(id);
}
//=========================================================================
void GraphAbstract::clear() {
  StableIterator<Graph *> itS(getSubGraphs());

  while(itS.hasNext())
    delAllSubGraphs(itS.next());

  StableIterator<node> itN(getNodes());

  while(itN.hasNext())
    delNode(itN.next());
}
//=========================================================================
void GraphAbstract::restoreSubGraph(Graph* sg) {
  subgraphs.push_back(sg);
  sg->setSuperGraph(this);
}
//=========================================================================
void GraphAbstract::setSubGraphToKeep(Graph* sg) {
  subGraphToKeep = sg;
}
//=========================================================================
Graph *GraphAbstract::addSubGraph(BooleanProperty *selection, unsigned int id,
                                  std::string name) {
  Graph *tmp = new GraphView(this, selection, id);
  tmp->setAttribute("name", name);
  subgraphs.push_back(tmp);
  notifyAddSubGraph(tmp);
  return tmp;
}
//=========================================================================
Graph *GraphAbstract::getNthSubGraph(unsigned int n) const {
  if (n >= subgraphs.size())
    return NULL;

  return subgraphs[n];
}
//=========================================================================
unsigned int GraphAbstract::numberOfSubGraphs() const {
  return subgraphs.size();
}
//=========================================================================
unsigned int GraphAbstract::numberOfDescendantGraphs() const {
  GRAPH_SEQ::const_iterator it = subgraphs.begin();
  int result = numberOfSubGraphs();

  while(it != subgraphs.end()) {
    result += (*it)->numberOfDescendantGraphs();
    it++;
  }

  return result;
}
//=========================================================================
void GraphAbstract::delSubGraph(Graph *toRemove) {
  // look for the graph we want to remove in the subgraphs
  GRAPH_SEQ::iterator it =
    std::find(subgraphs.begin(), subgraphs.end(), toRemove);

  assert(it != subgraphs.end());

  if (it != subgraphs.end()) {
    subGraphToKeep = NULL;
    notifyDelSubGraph(toRemove);

    // remove from subgraphs
    subgraphs.erase(it);

    Iterator<Graph *> *itS = toRemove->getSubGraphs();

    // add toRemove subgraphs
    while (itS->hasNext()) {
      restoreSubGraph(itS->next());
    }

    delete itS;

    // subGraphToKeep may have change on notifyDelSubGraph
    // see GraphUpdatesRecorder::delSubGraph
    // in GraphUpdatesRecorder.cpp
    if (toRemove != subGraphToKeep) {
      // avoid deletion of toRemove subgraphs
      toRemove->clearSubGraphs();
      delete toRemove;
    }
    else
      // toRemove is not deleted,
      // and its subgraphs list is not erased;
      // beacause it is registered into a GraphUpdatesRecorder
      // in order it can be restored on undo or redo
      toRemove->notifyDestroy();
  }
}
//=========================================================================
void GraphAbstract::removeSubGraph(Graph * toRemove) {
  GRAPH_SEQ::iterator it =
    std::find(subgraphs.begin(), subgraphs.end(), toRemove);

  if (it != subgraphs.end()) {
    subgraphs.erase(it);
  }
}
//=========================================================================
void GraphAbstract::delAllSubGraphs(Graph * toRemove) {
  if (this != toRemove->getSuperGraph() || this==toRemove)
    // this==toRemove : root graph
    return;

  StableIterator<Graph *> itS(toRemove->getSubGraphs());

  while (itS.hasNext())
    ((GraphAbstract*) toRemove)->delAllSubGraphs(itS.next());

  delSubGraph(toRemove);
}
//=========================================================================
void GraphAbstract::clearSubGraphs() {
  subgraphs.clear();
}
//=========================================================================
Graph* GraphAbstract::getSuperGraph()const {
  return supergraph;
}
//=========================================================================
Graph* GraphAbstract::getRoot() const {
  return root;
}
//=========================================================================
void GraphAbstract::setSuperGraph(Graph *sg) {
  supergraph=sg;
}
//=========================================================================
Iterator<Graph *> * GraphAbstract::getSubGraphs() const {
  return new StlIterator<Graph *, GRAPH_SEQ::const_iterator >(subgraphs.begin(),subgraphs.end());
}
//=========================================================================
bool GraphAbstract::isSubGraph(const Graph* sg) const {
  return (std::find(subgraphs.begin(), subgraphs.end(), sg) != subgraphs.end());
}
//=========================================================================
bool GraphAbstract::isDescendantGraph(const Graph* sg) const {
  if (isSubGraph(sg))
    return true;

  GRAPH_SEQ::const_iterator it = subgraphs.begin();

  while(it != subgraphs.end()) {
    if ((*it)->isDescendantGraph(sg))
      return true;

    it++;
  }

  return false;
}
//=========================================================================
Graph* GraphAbstract::getSubGraph(unsigned int sgId) const {
  GRAPH_SEQ::const_iterator it = subgraphs.begin();

  while(it != subgraphs.end()) {
    if ((*it)->getId() == sgId)
      return *it;

    it++;
  }

  return NULL;
}
//=========================================================================
Graph* GraphAbstract::getSubGraph(const std::string &name) const {
  GRAPH_SEQ::const_iterator it = subgraphs.begin();

  while(it != subgraphs.end()) {
    if ((*it)->getName() == name)
      return *it;

    it++;
  }

  return NULL;
}
//=========================================================================
Graph* GraphAbstract::getDescendantGraph(unsigned int sgId) const {
  Graph* sg = getSubGraph(sgId);

  if (sg)
    return sg;

  GRAPH_SEQ::const_iterator it = subgraphs.begin();

  while(it != subgraphs.end()) {
    if ((sg = (*it)->getDescendantGraph(sgId)))
      return sg;

    it++;
  }

  return NULL;
}
//=========================================================================
Graph* GraphAbstract::getDescendantGraph(const string &name) const {
  Graph* sg = getSubGraph(name);

  if (sg)
    return sg;

  GRAPH_SEQ::const_iterator it = subgraphs.begin();

  while(it != subgraphs.end()) {
    if ((sg = (*it)->getDescendantGraph(name)))
      return sg;

    it++;
  }

  return NULL;
}
//=========================================================================
node GraphAbstract::getOneNode() const {
  node result;
  Iterator<node> *it = getNodes();

  if (it->hasNext())
    result = it->next();

  delete it;
  return result;
}
//=========================================================================
edge GraphAbstract::getOneEdge() const {
  edge result;
  Iterator<edge> *it=getEdges();

  if (it->hasNext())
    result = it->next();

  delete it;
  return result;
}
//=========================================================================
node GraphAbstract::getInNode(const node n,unsigned int i)const {
  assert(i<=indeg(n) && i>0);
  Iterator<node>*itN=getInNodes(n);
  node result;

  for(unsigned int j=i+1; j>1; --j) {
    result=itN->next();
  }

  delete itN;
  return result;
}
//=========================================================================
node GraphAbstract::getOutNode(const node n,unsigned int i)const {
  assert(i<=outdeg(n) && i>0);
  Iterator<node>*itN=getOutNodes(n);
  node result;

  for(unsigned int j=i+1; j>1; --j) {
    result=itN->next();
  }

  delete itN;
  return result;
}
//=========================================================================
unsigned int GraphAbstract::deg(const node n) const {
  unsigned int deg=0;
  Iterator<edge> *it=getInOutEdges(n);

  while (it->hasNext()) {
    it->next();
    ++deg;
  }

  delete it;
  return deg;
}
//=========================================================================
unsigned int GraphAbstract::indeg(const node n) const {
  unsigned int deg=0;
  Iterator<edge> *it=getInEdges(n);

  while (it->hasNext()) {
    it->next();
    ++deg;
  }

  delete it;
  return deg;
}
//=========================================================================
unsigned int GraphAbstract::outdeg(const node n) const {
  unsigned int deg=0;
  Iterator<edge> *it=getOutEdges(n);

  while(it->hasNext()) {
    it->next();
    ++deg;
  }

  delete it;
  return deg;
}
//=========================================================================
void GraphAbstract::delNodes(Iterator<node>* itN, bool deleteInAllGraphs) {
  assert(itN != NULL);

  while(itN->hasNext()) {
    delNode(itN->next(), deleteInAllGraphs);
  }
}
//=========================================================================
node GraphAbstract::source(const edge e) const {
  return root->source(e);
}
//=========================================================================
void GraphAbstract::setSource(const edge e, const node newSrc) {
  assert(isElement(e));
  root->setEnds(e, newSrc, node());
}
//=========================================================================
node GraphAbstract::target(const edge e) const {
  return root->target(e);
}
//=========================================================================
void GraphAbstract::setTarget(const edge e, const node newTgt) {
  assert(isElement(e));
  root->setEnds(e, node(), newTgt);
}
//=========================================================================
const std::pair<node, node>& GraphAbstract::ends(const edge e) const {
  return root->ends(e);
}
//=========================================================================
void GraphAbstract::setEnds(const edge e, const node newSrc, const node newTgt) {
  assert(isElement(e));
  root->setEnds(e, newSrc, newTgt);
}
//=========================================================================
node GraphAbstract::opposite(const edge e, const node n) const {
  return root->opposite(e, n);
}
//=========================================================================
void GraphAbstract::reverse(const edge e) {
  assert(isElement(e));
  root->reverse(e);
}
//=========================================================================
edge GraphAbstract::existEdge(const node n1, const node n2,
                              bool directed) const {
  Iterator<edge> *it;

  if (directed)
    it = getOutEdges(n1);
  else
    it = getInOutEdges(n1);

  while (it->hasNext()) {
    edge e(it->next());

    if (opposite(e, n1) == n2) {
      delete it;
      return e;
    }
  }

  delete it;
  return edge();
}
//=========================================================================
void GraphAbstract::delEdges(Iterator<edge>* itE, bool deleteInAllGraphs) {
  assert(itE != NULL);

  while(itE->hasNext()) {
    delEdge(itE->next(), deleteInAllGraphs);
  }
}
//=========================================================================
bool GraphAbstract::existProperty(const std::string &name) const {
  return propertyContainer->existProperty(name);
}
//=========================================================================
bool GraphAbstract::existLocalProperty(const std::string &name) const {
  return propertyContainer->existLocalProperty(name);
}
//=========================================================================
PropertyInterface* GraphAbstract::getProperty(const string &str) const {
  return propertyContainer->getProperty(str);
}
//=========================================================================
void GraphAbstract::delLocalProperty(const std::string &name) {
  assert(existLocalProperty(name));
  notifyBeforeDelLocalProperty(name);
  propertyContainer->delLocalProperty(name);
  notifyAfterDelLocalProperty(name);
}
//=========================================================================
void GraphAbstract::addLocalProperty(const std::string &name, PropertyInterface *prop) {
  assert(!existLocalProperty(name));
  propertyContainer->setLocalProperty(name, prop);

  if (name == metaGraphPropertyName) {
    metaGraphProperty = (GraphProperty *) prop;
  }

  notifyAddLocalProperty(name);
}
//=========================================================================
void GraphAbstract::notifyAddInheritedProperty(const std::string& name) {
  if (hasOnlookers())
    sendEvent(GraphEvent(*this, GraphEvent::TLP_ADD_INHERITED_PROPERTY, name));
}
//=========================================================================
void GraphAbstract::notifyBeforeDelInheritedProperty(const std::string& name) {
  if (hasOnlookers())
    sendEvent(GraphEvent(*this, GraphEvent::TLP_BEFORE_DEL_INHERITED_PROPERTY, name,Event::TLP_INFORMATION));
}
//=========================================================================
void GraphAbstract::notifyAfterDelInheritedProperty(const std::string& name) {
  if (hasOnlookers())
    sendEvent(GraphEvent(*this, GraphEvent::TLP_AFTER_DEL_INHERITED_PROPERTY, name));
}
//=========================================================================
Iterator<std::string>* GraphAbstract::getLocalProperties() const {
  return propertyContainer->getLocalProperties();
}
//=========================================================================
Iterator<std::string>* GraphAbstract::getInheritedProperties() const {
  return propertyContainer->getInheritedProperties();
}
//=========================================================================
Iterator<std::string>* GraphAbstract::getProperties() const {
  return new ConcatIterator<std::string> (propertyContainer->getLocalProperties(),propertyContainer->getInheritedProperties());
}
//=========================================================================
Iterator<PropertyInterface*>* GraphAbstract::getLocalObjectProperties() const {
  return propertyContainer->getLocalObjectProperties();
}
//=========================================================================
Iterator<PropertyInterface*>* GraphAbstract::getInheritedObjectProperties() const {
  return propertyContainer->getInheritedObjectProperties();
}
//=========================================================================
Iterator<PropertyInterface*>* GraphAbstract::getObjectProperties() const {
  return new ConcatIterator<PropertyInterface*> (propertyContainer->getLocalObjectProperties(),propertyContainer->getInheritedObjectProperties());
}
//=========================================================================
unsigned int GraphAbstract::numberOfNodes() const {
  unsigned int result=0;
  Iterator<node> *it=getNodes();

  while(it->hasNext()) {
    it->next();
    result++;
  }

  delete it;
  return result;
}
//=========================================================================
unsigned int GraphAbstract::numberOfEdges() const {
  unsigned int result=0;
  Iterator<edge> *it=getEdges();

  while (it->hasNext()) {
    it->next();
    result++;
  }

  delete it;
  return result;
}
//=========================================================================
DataSet& GraphAbstract::getNonConstAttributes() {
  return attributes;
}
//----------------------------------------------------------------
bool GraphAbstract::isMetaNode(const node n) const {
  assert(isElement(n));
  return metaGraphProperty && metaGraphProperty->hasNonDefaultValue(n);
}
//----------------------------------------------------------------
bool GraphAbstract::isMetaEdge(const edge e) const {
  assert(isElement(e));
  return metaGraphProperty && metaGraphProperty->hasNonDefaultValue(e);
}
//=========================================================================
Graph* GraphAbstract::getNodeMetaInfo(const node n) const {
  if (metaGraphProperty)
    return metaGraphProperty->getNodeValue(n);

  return NULL;
}

// only used to return a reference on an empty vector of edges
static set<edge> noReferencedEdges;
//=========================================================================
const set<edge>& GraphAbstract::getReferencedEdges(const edge e) const {
  if (metaGraphProperty)
    return metaGraphProperty->getReferencedEdges(e);
  else
    return noReferencedEdges;
}
//=========================================================================
// Iterator on a vector of edges
// used for the edge associated value of a GraphProperty
class EdgeSetIterator:public Iterator<edge> {
  set<edge>::const_iterator it, itEnd;
public:
  EdgeSetIterator(const set<edge>& edges):
    it(edges.begin()), itEnd(edges.end()) {}
  ~EdgeSetIterator() {}
  edge next() {
    edge tmp=(*it);
    ++it;
    return tmp;
  }

  bool hasNext() {
    return (it!=itEnd);
  }
};

Iterator<edge>* GraphAbstract::getEdgeMetaInfo(const edge e) const {
  return new EdgeSetIterator(getReferencedEdges(e));
}

GraphProperty* GraphAbstract::getMetaGraphProperty() {
  if (metaGraphProperty)
    return metaGraphProperty;

  return metaGraphProperty =
           getRoot()->getProperty<GraphProperty>(metaGraphPropertyName);
}

void GraphAbstract::setName(const std::string &name) {
  setAttribute("name", name);
}

std::string GraphAbstract::getName() const {
  std::string name;
  getAttribute("name", name);
  return name;
}
