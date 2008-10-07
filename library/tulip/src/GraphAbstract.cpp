#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "tulip/GraphAbstract.h"
#include "tulip/BooleanProperty.h"
#include "tulip/GraphProperty.h"
#include "tulip/StlIterator.h"
#include "tulip/StableIterator.h"
#include "tulip/GraphView.h"
#include "tulip/ConcatIterator.h"

using namespace std;
using namespace tlp;

const string metaGraphProperty = "viewMetaGraph";

//=========================================================================
GraphAbstract::GraphAbstract(Graph *supergraph):supergraph(supergraph){
  if (supergraph==0) supergraph=this;
  propertyContainer=new PropertyManagerImpl(this);
}
//=========================================================================
GraphAbstract::~GraphAbstract() {

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
Graph *GraphAbstract::addSubGraph(BooleanProperty *selection){
  Graph *tmp = new GraphView(this, selection);
  subgraphs.push_back(tmp);
  notifyAddSubGraph(this, tmp);
  return tmp;
}
//=========================================================================
void GraphAbstract::delSubGraph(Graph *toRemove) {
  //check if the graph we want to remove is a subgraph of the graph
#ifndef NDEBUG
  bool find = false;
  for (GRAPH_SEQ::iterator it = subgraphs.begin(); it != subgraphs.end(); it++) {
    if (*it == toRemove) {
      find = true;
      break;
    }
  }
  assert(find);
#endif

  notifyDelSubGraph(this, toRemove);

  Iterator<Graph *> *itS = toRemove->getSubGraphs();
  while (itS->hasNext()) {
    Graph *tmp = itS->next();
    subgraphs.push_back(tmp);
    tmp->setSuperGraph(this);
  } delete itS;
  for (GRAPH_SEQ::iterator it = subgraphs.begin(); it != subgraphs.end(); it++) {
    if (*it == toRemove) {
      subgraphs.erase(it);
      break;
    }
  }
  delete toRemove;
}
//=========================================================================
void GraphAbstract::delAllSubGraphs(Graph * toRemove) {
  notifyDelSubGraph(this, toRemove);
  StableIterator<Graph *> itS(toRemove->getSubGraphs());
  while (itS.hasNext())
    toRemove->delAllSubGraphs(itS.next());
  for (GRAPH_SEQ::iterator it = subgraphs.begin(); it != subgraphs.end(); it++) {
    if (*it == toRemove) {
      subgraphs.erase(it);
      break;
    }
  }
  delete toRemove;
}
//=========================================================================
Graph* GraphAbstract::getSuperGraph()const {
  return supergraph;
}
//=========================================================================
Graph* GraphAbstract::getRoot()const {
  Graph *result=getSuperGraph();
  while (result!=result->getSuperGraph()) 
    result=result->getSuperGraph();
  return result;
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
  for(unsigned int j=i+1;j>1;--j){
    result=itN->next();
  }delete itN;
  return result;
}
//=========================================================================
node GraphAbstract::getOutNode(const node n,unsigned int i)const {
  assert(i<=outdeg(n) && i>0);
  Iterator<node>*itN=getOutNodes(n);
  node result;
  for(unsigned int j=i+1; j>1; --j) {
    result=itN->next();
  } delete itN;
  return result;
}
//=========================================================================
unsigned int GraphAbstract::deg(const node n) const {
  unsigned int deg=0;
  Iterator<edge> *it=getInOutEdges(n);
  while (it->hasNext()) {
    it->next();
    ++deg;
  } delete it;
  return deg;
}
//=========================================================================
unsigned int GraphAbstract::indeg(const node n) const {
  unsigned int deg=0;
  Iterator<edge> *it=getInEdges(n);
  while (it->hasNext()) {
    it->next();
    ++deg;
  } delete it;
  return deg;
}
//=========================================================================
unsigned int GraphAbstract::outdeg(const node n) const {
  unsigned int deg=0;
  Iterator<edge> *it=getOutEdges(n);
  while(it->hasNext()) {
    it->next();
    ++deg;
  } delete it;
  return deg;
}
//=========================================================================
node GraphAbstract::source(const edge e) const {
  return getSuperGraph()->source(e);
}
//=========================================================================
node GraphAbstract::target(const edge e) const {
  return getSuperGraph()->target(e);
}
//=========================================================================
node GraphAbstract::opposite(const edge e, const node n)const {
  return ( (source(e)==n) ? target(e):source(e) );
}
//=========================================================================
void GraphAbstract::reverse(const edge e) {
  notifyReverseEdge(this, e);
  getSuperGraph()->reverse(e);
}
//=========================================================================
edge GraphAbstract::existEdge(const node n1, const node n2)const {
  Iterator<edge> *it = getOutEdges(n1);
  while (it->hasNext()) {
    edge e(it->next());
    if (opposite(e, n1) == n2) {
      delete it;
      return e;
    }
  } delete it;
  return edge();
}
//=========================================================================
bool GraphAbstract::existProperty(const std::string &name) {
  return propertyContainer->existProperty(name);
}
//=========================================================================
bool GraphAbstract::existLocalProperty(const std::string &name) {
  return propertyContainer->existLocalProperty(name);
}
//=========================================================================
PropertyInterface* GraphAbstract::getProperty(const string &str) {
  return propertyContainer->getProperty(str);
}
//=========================================================================
void GraphAbstract::delLocalProperty(const std::string &name) {
  notifyDelLocalProperty(this, name);
  propertyContainer->delLocalProperty(name);
}
//=========================================================================
void GraphAbstract::addLocalProperty(const std::string &name, PropertyInterface *prop) {
  assert(!existLocalProperty(name));
  propertyContainer->setLocalProxy(name, prop);
  notifyAddLocalProperty(this, name);
}
//=========================================================================
Iterator<std::string>* GraphAbstract::getLocalProperties() {
  return propertyContainer->getLocalProperties();
}
//=========================================================================
Iterator<std::string>* GraphAbstract::getInheritedProperties() {
  return propertyContainer->getInheritedProperties();
}
//=========================================================================
Iterator<std::string>* GraphAbstract::getProperties() {
  return new ConcatIterator<std::string> (propertyContainer->getLocalProperties(),propertyContainer->getInheritedProperties());
}
//=========================================================================
unsigned int GraphAbstract::numberOfNodes() const {
  unsigned int result=0;
  Iterator<node> *it=getNodes();
  while(it->hasNext()) {
    it->next();
    result++;
  } delete it;
  return result;
}
//=========================================================================
unsigned int GraphAbstract::numberOfEdges() const {
  unsigned int result=0;
  Iterator<edge> *it=getEdges();
  while (it->hasNext()) {
    it->next();
    result++;
  } delete it;
  return result;
}
//----------------------------------------------------------------
bool GraphAbstract::isMetaNode(const node n) const {
  assert(isElement(n));
  return getNodeMetaInfo(n) != NULL;
}
//----------------------------------------------------------------
bool GraphAbstract::isMetaEdge(const edge e) const {
  assert(isElement(e));
  return getReferencedEdges(e).size() != 0;
}
//=========================================================================
Graph* GraphAbstract::getNodeMetaInfo(const node n) const {
  if (((GraphAbstract *) this)->existProperty(metaGraphProperty)) {
    tlp::PropertyInterface* prop =
      ((GraphAbstract *) this)->getProperty(metaGraphProperty);
    assert(typeid((*prop)) == typeid(GraphProperty));
    return ((GraphProperty *) prop)->getNodeValue(n);
  }
  return NULL;
}

// only used to return a reference on an empty vector of edges
static set<edge> noReferencedEdges;
//=========================================================================
const set<edge>& GraphAbstract::getReferencedEdges(const edge e) const {
  if (((GraphAbstract *) this)->existProperty(metaGraphProperty)) {
    tlp::PropertyInterface* prop =
      ((GraphAbstract *) this)->getProperty(metaGraphProperty);
    assert(typeid((*prop)) == typeid(GraphProperty));
    return ((GraphProperty *) prop)->getReferencedEdges(e);
  }
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
