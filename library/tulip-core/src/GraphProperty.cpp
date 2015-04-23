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

#if defined(__GNUC__) && __GNUC__ >= 4 && ((__GNUC_MINOR__ == 2 && __GNUC_PATCHLEVEL__ >= 1) || (__GNUC_MINOR__ >= 3))
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif

#include <tulip/GraphProperty.h>

using namespace std;
using namespace tlp;

const string GraphProperty::propertyTypename="graph";

//==============================
GraphProperty::GraphProperty (Graph *sg, const std::string& n) :
  AbstractProperty<GraphType, EdgeSetType>(sg, n) {
  setAllNodeValue(0);
}
//==============================
GraphProperty::~GraphProperty() {
  if (graph) {
    Iterator<node> *it = graph->getNodes();

    while(it->hasNext()) {
      node n = it->next();

      if (getNodeValue(n) != NULL)
        getNodeValue(n)->removeListener(this);
    }

    delete it;

    if (getNodeDefaultValue() != NULL) {
      getNodeDefaultValue()->removeListener(this);
    }
  }
}
//==============================
void GraphProperty::setAllNodeValue(const GraphType::RealType& g) {
  //remove all observed graphs if any
  Iterator<node> *it = getNonDefaultValuatedNodes();

  while(it->hasNext()) {
    node n = it->next();
    getNodeValue(n)->removeListener(this);
  }

  delete it;
  set<node> emptySet;
  referencedGraph.setAll(emptySet);

  if (getNodeDefaultValue() != NULL) {
    getNodeDefaultValue()->removeListener(this);
  }

  AbstractGraphProperty::setAllNodeValue(g);

  if (g != NULL)
    g->addListener(this);
}
//==============================
void GraphProperty::setNodeValue(const node n, const GraphType::RealType& sg) {
  //gestion désabonnement
  Graph * oldGraph = getNodeValue(n);

  if (oldGraph != NULL && oldGraph != sg) {
    //gestion du désabonnement
    bool notDefault;
    set<node> &refs = referencedGraph.get(oldGraph->getId(), notDefault);

    if (notDefault) {
      refs.erase(n);

      if (refs.empty())  {
        if (oldGraph != getNodeDefaultValue())
          oldGraph->removeListener(this);

        referencedGraph.set(oldGraph->getId(), set<node>());
      }
    }
    else if (oldGraph != getNodeDefaultValue())
      oldGraph->removeListener(this);
  }

  AbstractGraphProperty::setNodeValue(n, sg);

  if (sg == NULL || oldGraph == sg)
    return;

  //Gestion de l'abonnement
  sg->addListener(this);

  if (sg != getNodeDefaultValue()) {
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
//============================================================
PropertyInterface* GraphProperty::clonePrototype(Graph * g, const std::string& n) {
  if( !g )
    return NULL;

  // allow to get an unregistered property (empty name)
  GraphProperty * p = n.empty()
                      ? new GraphProperty(g) : g->getLocalProperty<GraphProperty>( n );

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
bool GraphProperty::setEdgeStringValue(const edge, const std::string&) {
  return false;
}
//=============================================================
// disabled use setAllEdgeValue instead
bool GraphProperty::setAllEdgeStringValue(const std::string&) {
  return false;
}
//=============================================================
const set<edge>& GraphProperty::getReferencedEdges(const edge e) const {
  return ((GraphProperty *) this)->edgeProperties.get(e.id);
}
//=============================================================
void GraphProperty::treatEvent(const Event& evt) {
  if (evt.type() == Event::TLP_DELETE) {
    // From my point of view the use of dynamic_cast should be correct
    // but it fails, so I use reinterpret_cast (pm)
    Graph* sg = reinterpret_cast<Graph *>(evt.sender());

    if(sg) {
#ifndef NDEBUG
      tlp::warning() << "Tulip Warning : A graph pointed by metanode(s) has been deleted, the metanode(s) pointer has been set to zero in order to prevent segmentation fault" << std::endl;
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
        }

        delete it;
        setAllNodeValue(0);
        //restore values
        it = graph->getNodes();

        while(it->hasNext()) {
          node n = it->next();
          setNodeValue(n, backup.get(n.id));
        }

        delete it;
      }

      const set<node>& refs = referencedGraph.get(sg->getId());

      set<node>::const_iterator it = refs.begin();

      if (it != refs.end()) {
        // don't change values if this non longer exists (when undoing)
        if (graph->existProperty(name)) {
          for (; it!=refs.end(); ++it) {
            AbstractGraphProperty::setNodeValue((*it), 0);
          }
        }

        referencedGraph.set(sg->getId(), set<node>());
      }
    }
  }
}

bool GraphProperty::readNodeDefaultValue(std::istream& iss) {
  // must read 0 (see GraphType::writeb)
  unsigned int id = 0;

  if (!bool(iss.read((char *) &id, sizeof(id))))
    return false;

  assert(id == 0);
  return id == 0;
}

bool GraphProperty::readNodeValue(std::istream& iss, node n) {
  // must read the id of a subgraph
  unsigned int id = 0;

  if (!bool(iss.read((char *) &id, sizeof(id))))
    return false;

  Graph* sg = graph->getRoot()->getDescendantGraph(id);
  setNodeValue(n, sg);
  return true;
}
