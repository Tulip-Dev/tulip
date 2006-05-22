#include <float.h>
#include "tulip/GraphProperty.h"
#include "tulip/PluginContext.h"
#include "tulip/Observable.h"
#include "tulip/MetaGraphAlgorithm.h"
#include "tulip/AbstractProperty.h"

using namespace std;
using namespace tlp;

//==============================
GraphProperty::GraphProperty (Graph *sg) : 
  AbstractProperty<GraphType,GraphType>(sg) {
  setAllNodeValue(0);
  setAllEdgeValue(0);
}
//==============================
GraphProperty::~GraphProperty() {
  Iterator<node> *it = graph->getNodes();
  while(it->hasNext()) {
    node n = it->next();
    if (getNodeValue(n) != 0)
      getNodeValue(n)->removeObserver(this);
  } delete it;
  if (getNodeDefaultValue() != 0) {
    getNodeDefaultValue()->removeObserver(this);
  }
  notifyDestroy();
}
//==============================
void GraphProperty::setAllNodeValue_handler(const GraphType::RealType &sg) {
  //remove all observed sgs
  Iterator<node> *it = graph->getNodes();
  while(it->hasNext()) {
    node n = it->next();
    if (getNodeValue(n) != 0)
      getNodeValue(n)->removeObserver(this);
  } delete it;
  set<node> emptySet;
  referencedGraph.setAll(emptySet);
  if (getNodeDefaultValue() != 0) {
    getNodeDefaultValue()->removeObserver(this);
  }
  if (sg == 0) return;
  sg->addObserver(this);
}
//==============================
void GraphProperty::setNodeValue_handler(const node n, const GraphType::RealType &sg) {
  //  cerr << __PRETTY_FUNCTION__ << endl;
  //gestion désabonnement
  Graph * oldGraph = getNodeValue(n); 
  if (oldGraph != 0) {
    //gestion du désabonnement
    set<node> &refs = referencedGraph.getReference(oldGraph->getId()); //use of reference in order to prevent cloninf of the set (Dangerous)
    refs.erase(n);
    if (refs.empty() && oldGraph != getNodeDefaultValue())
      oldGraph->removeObserver(this);
    if (refs.empty())
      referencedGraph.set(oldGraph->getId(), set<node>());
  }
  if (sg == 0) return;
  //Gestion de l'abonnement
  sg->addObserver(this);
  if ( sg != getNodeDefaultValue() ) {
    set<node> &refs = referencedGraph.getReference(sg->getId()); //use of reference in order to prevent cloninf of the set (Dangerous)
    if (refs.empty()) { //Man
      set<node> newSet;
      newSet.insert(n);
      referencedGraph.set(sg->getId(), newSet);
    }
    else 
      refs.insert(n);
  }
}
//==========================================================
void GraphProperty::destroy(Graph *sg) {
  //test si c'est la valeur par défaut;
  //  cerr << __PRETTY_FUNCTION__ << endl;
  //sinon
  cerr << "Tulip Warning : A graph pointed by metanode(s) has been deleted, the metanode(s) pointer has been set to zero in order to prevent segmentation fault" << endl;
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
