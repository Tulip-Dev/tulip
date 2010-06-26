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
#include <float.h>
#include "tulip/GraphProperty.h"
#include "tulip/PluginContext.h"
#include "tulip/Observable.h"
#include "tulip/AbstractProperty.h"

using namespace std;
using namespace tlp;

//==============================
GraphProperty::GraphProperty (Graph *sg, std::string n) : 
  AbstractProperty<GraphType, EdgeSetType>(sg, n) {
  setAllNodeValue(0);
  // the property observes itself; see beforeSet... methods
  addPropertyObserver(this);
  // but do not need to be in observables
  PropertyObserver::removeObservable(this);
}
//==============================
GraphProperty::~GraphProperty() {
  if (graph) {
    Iterator<node> *it = graph->getNodes();
    while(it->hasNext()) {
      node n = it->next();
      if (getNodeValue(n) != 0)
	getNodeValue(n)->removeGraphObserver(this);
    } delete it;
    if (getNodeDefaultValue() != 0) {
      getNodeDefaultValue()->removeGraphObserver(this);
    }
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
  // nothing to do if n points to a graph being removed
  // see destroy
  if (n == currentNode)
    return;
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
  // nothing to do if n points to a graph being removed
  // see destroy
  if (n == currentNode)
    return;
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
  //  cerr << __PRETTY_FUNCTION__ << endl;
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
  const set<node>& refs = referencedGraph.get(sg->getId());
  set<node>::const_iterator it = refs.begin();
  if (it != refs.end()) {
    // don't change values if this non longer exists (when undoing)
    if (graph->existProperty(name)) {
      for (; it!=refs.end(); ++it) {
	// set current node to allow
	// a call to setNodeValue (needed by undo/redo mechanism)
	// and do nothing in (before/after)SetNodeValue
	currentNode = (*it);
	setNodeValue(currentNode, 0);
      }
      currentNode = node();
    }
    referencedGraph.set(sg->getId(), set<node>());
    sg->removeGraphObserver(this);
  }
}
//============================================================
PropertyInterface* GraphProperty::clonePrototype(Graph * g, const std::string& n) {
  if( !g )
    return 0;
  GraphProperty * p = g->getLocalProperty<GraphProperty>( n );
  p->setAllNodeValue( getNodeDefaultValue() );
  p->setAllEdgeValue( getEdgeDefaultValue() );
  return p;
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
