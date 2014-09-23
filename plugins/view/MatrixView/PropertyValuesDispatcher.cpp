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


#include "PropertyValuesDispatcher.h"

#include <tulip/ForEach.h>
#include <tulip/Graph.h>
#include <tulip/IntegerProperty.h>
#include <tulip/BooleanProperty.h>

#include <cassert>
#include <typeinfo>

using namespace tlp;
using namespace std;

PropertyValuesDispatcher::PropertyValuesDispatcher(tlp::Graph *source, tlp::Graph *target,
    const std::set<std::string> &sourceToTargetProperties, const std::set<std::string> &targetToSourceProperties,
    tlp::IntegerVectorProperty *graphEntitiesToDisplayedNodes, tlp::BooleanProperty *displayedNodesAreNodes, tlp::IntegerProperty *displayedNodesToGraphEntities, tlp::IntegerProperty *displayedEdgesToGraphEdges, QHash<tlp::edge,tlp::edge>&  edgesMap):
  _source(source), _target(target),
  _graphEntitiesToDisplayedNodes(graphEntitiesToDisplayedNodes), _displayedNodesAreNodes(displayedNodesAreNodes),
  _displayedNodesToGraphEntities(displayedNodesToGraphEntities),
  _displayedEdgesToGraphEdges(displayedEdgesToGraphEdges),
  _edgesMap(edgesMap),
  _sourceToTargetProperties(sourceToTargetProperties),
  _targetToSourceProperties(targetToSourceProperties), _modifying(false) {
  assert(source);
  assert(target);
  assert(graphEntitiesToDisplayedNodes);
  assert(displayedNodesAreNodes);
  assert(displayedNodesToGraphEntities);

  Observable::holdObservers();
  string s;
  forEach (s, source->getProperties())
  addLocalProperty(source, s);

  forEach (s, target->getProperties())
  addLocalProperty(target, s);

  Observable::unholdObservers();

  source->addListener(this);
  target->addListener(this);
}

void PropertyValuesDispatcher::afterSetNodeValue(tlp::PropertyInterface *sourceProp, const tlp::node n) {
  if (_modifying)
    return;

  _modifying = true;

  if (sourceProp->getGraph()->getRoot() == _source->getRoot()) {
    PropertyInterface *targetProp = _target->getProperty(sourceProp->getName());
    vector<int> vect = _graphEntitiesToDisplayedNodes->getNodeValue(n);

    for(vector<int>::iterator it = vect.begin(); it != vect.end(); ++it)
      targetProp->setNodeStringValue(node(*it), sourceProp->getNodeStringValue(n));
  }
  else if (sourceProp->getGraph()->getRoot() == _target->getRoot()) {
    PropertyInterface *targetProp = _source->getProperty(sourceProp->getName());
    unsigned int id = _displayedNodesToGraphEntities->getNodeValue(n);

    if (_displayedNodesAreNodes->getNodeValue(n))
      targetProp->setNodeStringValue(node(id), sourceProp->getNodeStringValue(n));
    else {
      targetProp->setEdgeStringValue(edge(id), sourceProp->getNodeStringValue(n));
      sourceProp->setEdgeStringValue(_edgesMap[edge(id)], sourceProp->getNodeStringValue(n));

      vector<int> vect = _graphEntitiesToDisplayedNodes->getEdgeValue(edge(id));

      for(vector<int>::iterator it = vect.begin(); it != vect.end(); ++it)  {
        node n1(*it);

        if (n1 != n)
          sourceProp->setNodeStringValue(n1, sourceProp->getNodeStringValue(n));
      }
    }
  }

  _modifying = false;
}

void PropertyValuesDispatcher::afterSetEdgeValue(tlp::PropertyInterface *sourceProp, const tlp::edge e) {
  if (_modifying)
    return;

  _modifying = true;

  if (sourceProp->getGraph()->getRoot() == _source->getRoot()) {
    PropertyInterface *targetProp = _target->getProperty(sourceProp->getName());
    vector<int> vect = _graphEntitiesToDisplayedNodes->getEdgeValue(e);

    for(vector<int>::iterator it = vect.begin(); it != vect.end(); ++it)
      targetProp->setNodeStringValue(node(*it), sourceProp->getEdgeStringValue(e));

    targetProp->setEdgeStringValue(_edgesMap[e], sourceProp->getEdgeStringValue(e));
  }
  else if (sourceProp->getGraph()->getRoot() == _target->getRoot()) {
    PropertyInterface *targetProp = _source->getProperty(sourceProp->getName());
    unsigned int id = _displayedEdgesToGraphEdges->getEdgeValue(e);

    targetProp->setEdgeStringValue(edge(id), sourceProp->getEdgeStringValue(e));

    vector<int> vect = _graphEntitiesToDisplayedNodes->getEdgeValue(edge(id));

    for(vector<int>::iterator it = vect.begin(); it != vect.end(); ++it)
      sourceProp->setNodeStringValue(node(*it), sourceProp->getEdgeStringValue(e));
  }

  _modifying = false;
}

void PropertyValuesDispatcher::afterSetAllNodeValue(tlp::PropertyInterface *sourceProp) {
  if (sourceProp->getGraph()->getRoot() == _source->getRoot()) {
    PropertyInterface *targetProp = _target->getProperty(sourceProp->getName());
    string val = sourceProp->getNodeDefaultStringValue();
    node n;
    forEach(n, _displayedNodesAreNodes->getNodesEqualTo(true))
    targetProp->setNodeStringValue(n, val);
  }
  else if (sourceProp->getGraph()->getRoot() == _target->getRoot()) {
    PropertyInterface *targetProp = _source->getProperty(sourceProp->getName());
    targetProp->setAllNodeStringValue(sourceProp->getNodeDefaultStringValue());
    targetProp->setAllEdgeStringValue(sourceProp->getNodeDefaultStringValue());
  }
}

void PropertyValuesDispatcher::afterSetAllEdgeValue(tlp::PropertyInterface *sourceProp) {
  if (sourceProp->getGraph()->getRoot() == _source->getRoot()) {
    PropertyInterface *targetProp = _target->getProperty(sourceProp->getName());
    string val = sourceProp->getEdgeDefaultStringValue();
    node n;
    forEach(n, _displayedNodesAreNodes->getNodesEqualTo(false))
    targetProp->setNodeStringValue(n, val);
  }
  else if (sourceProp->getGraph()->getRoot() == _target->getRoot()) {
    PropertyInterface *targetProp = _source->getProperty(sourceProp->getName());
    targetProp->setAllEdgeStringValue(sourceProp->getNodeDefaultStringValue());
  }
}

void PropertyValuesDispatcher::addLocalProperty(tlp::Graph *g, const std::string &name) {
  if ((g == _source && _sourceToTargetProperties.find(name) != _sourceToTargetProperties.end()) ||
      (g == _target && _targetToSourceProperties.find(name) != _targetToSourceProperties.end())) {
    Observable::holdObservers();
    PropertyInterface *sourceProp = g->getProperty(name);
    afterSetAllNodeValue(sourceProp);
    afterSetAllEdgeValue(sourceProp);
    node n;
    forEach(n, sourceProp->getNonDefaultValuatedNodes())
    afterSetNodeValue(sourceProp,n);

    edge e;
    forEach(e, sourceProp->getNonDefaultValuatedEdges())
    afterSetEdgeValue(sourceProp, e);
    Observable::unholdObservers();

    sourceProp->addListener(this);
  }
}

void PropertyValuesDispatcher::treatEvent(const tlp::Event& evt) {
  if (typeid(evt) == typeid(GraphEvent)) {
    const GraphEvent* gEvt = dynamic_cast<const GraphEvent*>(&evt);
    Graph* graph = gEvt->getGraph();

    if (gEvt->getType() == GraphEvent::TLP_ADD_LOCAL_PROPERTY)
      addLocalProperty(graph, gEvt->getPropertyName());
  }
  else {
    const PropertyEvent* propEvt = dynamic_cast<const PropertyEvent*>(&evt);

    if (propEvt) {
      PropertyInterface* prop = propEvt->getProperty();

      switch(propEvt->getType()) {
      case PropertyEvent::TLP_AFTER_SET_NODE_VALUE:
        afterSetNodeValue(prop, propEvt->getNode());
        return;

      case PropertyEvent::TLP_AFTER_SET_ALL_NODE_VALUE:
        afterSetAllNodeValue(prop);
        return;

      case PropertyEvent::TLP_AFTER_SET_ALL_EDGE_VALUE:
        afterSetAllEdgeValue(prop);
        return;

      case PropertyEvent::TLP_AFTER_SET_EDGE_VALUE:
        afterSetEdgeValue(prop, propEvt->getEdge());
        return;

      default:
        return;
      }
    }
  }
}
