#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "tulip/GraphAbstract.h"
#include "tulip/BooleanProperty.h"
#include "tulip/StlIterator.h"
#include "tulip/StableIterator.h"
#include "tulip/GraphView.h"
#include "tulip/ConcatIterator.h"

using namespace std;
using namespace tlp;

//=========================================================================
GraphAbstract::GraphAbstract(Graph *father):father(father){
  if (father==0) father=this;
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

  Iterator<Graph *> *itS = toRemove->getSubGraphs();
  while (itS->hasNext()) {
    Graph *tmp = itS->next();
    subgraphs.push_back(tmp);
    tmp->setFather(this);
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
Graph* GraphAbstract::getFather()const {
  return father;
}
//=========================================================================
Graph* GraphAbstract::getRoot()const {
  Graph *result=getFather();
  while (result!=result->getFather()) 
    result=result->getFather();
  return result;
}
//=========================================================================
void GraphAbstract::setFather(Graph *sg) {
  father=sg;
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
  return getFather()->source(e);
}
//=========================================================================
node GraphAbstract::target(const edge e) const {
  return getFather()->target(e);
}
//=========================================================================
node GraphAbstract::opposite(const edge e, const node n)const {
  return ( (source(e)==n) ? target(e):source(e) );
}
//=========================================================================
void GraphAbstract::reverse(const edge e) {
  notifyReverseEdge(this,e);
  getFather()->reverse(e);
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
bool GraphAbstract::existProperty(const std::string&name) {
  return propertyContainer->existProperty(name);
}
//=========================================================================
bool GraphAbstract::existLocalProperty(const std::string&name) {
  return propertyContainer->existLocalProperty(name);
}
//=========================================================================
PropertyInterface* GraphAbstract::getProperty(const string &str) {
  return propertyContainer->getProperty(str);
}
//=========================================================================
void GraphAbstract::delLocalProperty(const std::string&name) {
  propertyContainer->delLocalProperty(name);
}
//=========================================================================
void GraphAbstract::addLocalProperty(const std::string &name, PropertyInterface *prop) {
  assert(!existLocalProperty(name));
  propertyContainer->setLocalProxy(name, prop);
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
//=========================================================================
