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

#include <tulip/BooleanProperty.h>
#include <tulip/Observable.h>
#include <tulip/GraphIterator.h>

using namespace std;
using namespace tlp;

const string BooleanProperty::propertyTypename = "bool";
const string BooleanVectorProperty::propertyTypename = "vector<bool>";

//=================================================================================
Iterator<node>* BooleanProperty::getNodesEqualTo(const bool val,
    const Graph *sg) {
  if (sg == NULL) sg = graph;

  Iterator<unsigned int> *it = NULL;

  if (sg == graph)
    it = nodeProperties.findAll(val);

  if (it == NULL)
    return new SGraphNodeIterator(sg, nodeProperties, val);

  return (new UINTIterator<node>(it));
}
//=================================================================================
Iterator<edge>* BooleanProperty::getEdgesEqualTo(const bool val,
    const Graph *sg) {
  if (sg == NULL) sg = graph;

  Iterator<unsigned int> *it=0;

  if (sg == graph)
    it = edgeProperties.findAll(val);

  if (it == NULL)
    return new SGraphEdgeIterator(sg, edgeProperties, val);

  return (new UINTIterator<edge>(it));
}
//=================================================================================
void BooleanProperty::reverse(const Graph *sg) {
  if (sg == NULL) sg = graph;

  const vector<node>& nodes = sg->nodes();
  const vector<edge>& edges = sg->edges();
  unsigned nbNodes = nodes.size();
  unsigned nbEdges = edges.size();

  for(unsigned i=0; i<nbNodes; ++i) {
    node n(nodes[i]);
    setNodeValue(n,!getNodeValue(n));
  }

  for(unsigned i=0; i<nbEdges; ++i) {
    edge e(edges[i]);
    setEdgeValue(e,!getEdgeValue(e));
  }
}

//=================================================================================
void BooleanProperty::reverseEdgeDirection(Graph *sg) {
  if (sg == NULL) sg = graph;

  const vector<edge>&edges = sg->edges();
  unsigned nbEdges = edges.size();

  for(unsigned i=0; i<nbEdges; ++i) {
    edge e(edges[i]);

    if(getEdgeValue(e))
      sg->reverse(e);
  }
}

//=================================================================================
PropertyInterface* BooleanProperty::clonePrototype(Graph * g, const std::string& n) const {
  if( g==NULL )
    return NULL;

  // allow to get an unregistered property (empty name)
  BooleanProperty * p = n.empty()
                        ? new BooleanProperty(g) : g->getLocalProperty<BooleanProperty>( n );
  p->setAllNodeValue( getNodeDefaultValue() );
  p->setAllEdgeValue( getEdgeDefaultValue() );
  return p;
}
//=================================================================================
PropertyInterface* BooleanVectorProperty::clonePrototype(Graph * g, const std::string& n) const {
  if( !g )
    return 0;

  // allow to get an unregistered property (empty name)
  BooleanVectorProperty * p = n.empty()
                              ? new BooleanVectorProperty(g) : g->getLocalProperty<BooleanVectorProperty>( n );
  p->setAllNodeValue( getNodeDefaultValue() );
  p->setAllEdgeValue( getEdgeDefaultValue() );
  return p;
}



