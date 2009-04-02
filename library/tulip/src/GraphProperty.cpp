#include <float.h>
#include "tulip/GraphProperty.h"
#include "tulip/PluginContext.h"
#include "tulip/Observable.h"
#include "tulip/AbstractProperty.h"

using namespace std;
using namespace tlp;

//==============================
GraphProperty::GraphProperty (Graph *sg) : 
  AbstractProperty<GraphType, EdgeSetType>(sg) {
  setAllNodeValue(0);
  // the property observes itself; see beforeSet... methods
  addPropertyObserver(this);
}
//==============================
GraphProperty::~GraphProperty() {
  Iterator<node> *it = graph->getNodes();
  while(it->hasNext()) {
    node n = it->next();
    if (getNodeValue(n) != 0)
      getNodeValue(n)->removeGraphObserver(this);
  } delete it;
  if (getNodeDefaultValue() != 0) {
    getNodeDefaultValue()->removeGraphObserver(this);
  }
  notifyDestroy(this);
}
//==============================
void GraphProperty::beforeSetAllNodeValue(PropertyInterface*) {
  //remove all observed graphs
  Iterator<node> *it = graph->getNodes();
  while(it->hasNext()) {
    node n = it->next();
    if (getNodeValue(n) != 0)
      getNodeValue(n)->removeGraphObserver(this);
  } delete it;
  set<node> emptySet;
  referencedGraph.setAll(emptySet);
  if (getNodeDefaultValue() != 0) {
    getNodeDefaultValue()->removeGraphObserver(this);
  }
}
void GraphProperty::afterSetAllNodeValue(PropertyInterface*) {
  if (getNodeDefaultValue() != 0) {
    getNodeDefaultValue()->addGraphObserver(this);
  }
}
//==============================
void GraphProperty::beforeSetNodeValue(PropertyInterface* prop, const node n) {
  //  cerr << __PRETTY_FUNCTION__ << endl;
  //gestion désabonnement
  Graph * oldGraph = getNodeValue(n); 
  if (oldGraph != NULL) {
    //gestion du désabonnement
    bool notDefault;
    set<node> &refs = referencedGraph.get(oldGraph->getId(), notDefault);
    if (notDefault) {
      refs.erase(n);
      if (refs.empty())  {
	if (oldGraph != getNodeDefaultValue())
	  oldGraph->removeGraphObserver(this);
	referencedGraph.set(oldGraph->getId(), set<node>());
      }
    } else if (oldGraph != getNodeDefaultValue())
      oldGraph->removeGraphObserver(this);
  }
}
void GraphProperty::afterSetNodeValue(PropertyInterface* prop, const node n) {
  Graph* sg = getNodeValue(n);
  if (sg == NULL)
    return;
  //Gestion de l'abonnement
  sg->addGraphObserver(this);
  if ( sg != getNodeDefaultValue() ) {
    bool notDefault;
    set<node> &refs = referencedGraph.get(sg->getId(), notDefault);
    if (notDefault)
      refs.insert(n);
    else {
      set<node> newSet;
      newSet.insert(n);
      referencedGraph.set(sg->getId(), newSet);
    }
  }
}
//==========================================================
void GraphProperty::destroy(Graph *sg) {
  //test si c'est la valeur par défaut;
  //  cerr << __PRETTY_FUNCTION__ << endl;
  //sinon
#ifndef NDEBUG
  cerr << "Tulip Warning : A graph pointed by metanode(s) has been deleted, the metanode(s) pointer has been set to zero in order to prevent segmentation fault" << endl;
#endif
  if (getNodeDefaultValue() == sg) {
    //we must backup old value
    MutableContainer<Graph *> backup;
    backup.setAll(0);
    Iterator<node> *it = graph->getNodes();
    while(it->hasNext()) {
      node n = it->next();
      if (getNodeValue(n) != sg)
	backup.set(n.id, getNodeValue(n));
    } delete it;
    setAllNodeValue(0);
    //restore values
    it = graph->getNodes();
    while(it->hasNext()) {
      node n = it->next();
      setNodeValue(n, backup.get(n.id));
    } delete it;
  }
  set<node> refs = referencedGraph.get(sg->getId());
  set<node>::const_iterator it = refs.begin();
  for (; it!=refs.end(); ++it) {
    setNodeValue(*it, 0);
  }
}
//============================================================
PropertyInterface* GraphProperty::clonePrototype(Graph * g, std::string n) {
  if( !g )
    return 0;
  GraphProperty * p = g->getLocalProperty<GraphProperty>( n );
  p->setAllNodeValue( getNodeDefaultValue() );
  p->setAllEdgeValue( getEdgeDefaultValue() );
  return p;
}
//=============================================================
void GraphProperty::copy( const node n0, const node n1, PropertyInterface * p ) {
  if( !p )
    return;
  GraphProperty * tp = dynamic_cast<GraphProperty*>(p);
  assert( tp );
  setNodeValue( n0, tp->getNodeValue(n1) );
}
//=============================================================
void GraphProperty::copy( const edge e0, const edge e1, PropertyInterface * p ) {
  if( !p ) return;
  GraphProperty * tp = dynamic_cast<GraphProperty*>(p);
  assert( tp );
  setEdgeValue( e0, tp->getEdgeValue(e1) );
}
//=============================================================
// disabled, use setNodeValue instead
bool GraphProperty::setNodeStringValue(const node, const std::string &) {
  return false;
}
//=============================================================
// disabled use setAllNodeValue instead
bool GraphProperty::setAllNodeStringValue(const std::string &) {
  return false;
}
//=============================================================
// disabled, use setEdgeValue instead
bool GraphProperty::setEdgeStringValue(const edge e, const std::string & v) {
  return false;
}
//=============================================================
// disabled use setAllEdgeValue instead
bool GraphProperty::setAllEdgeStringValue(const std::string & v) {
  return false;
}
//=============================================================
const set<edge>& GraphProperty::getReferencedEdges(const edge e) const{
  return ((GraphProperty *) this)->edgeProperties.get(e.id);
}
