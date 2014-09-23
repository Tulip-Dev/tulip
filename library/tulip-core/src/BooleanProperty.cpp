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
Iterator<node>* BooleanProperty::getNodesEqualTo(const bool val, Graph *sg) {
  if (sg == NULL) sg = graph;

  Iterator<unsigned int> *it = NULL;

  if (sg == graph)
    it = nodeProperties.findAll(val);

  if (it == NULL)
    return new SGraphNodeIterator(sg, nodeProperties, val);

  return (new UINTIterator<node>(it));
}
//=================================================================================
Iterator<edge>* BooleanProperty::getEdgesEqualTo(const bool val, Graph *sg) {
  if (sg == NULL) sg = graph;

  Iterator<unsigned int> *it=0;

  if (sg == graph)
    it = edgeProperties.findAll(val);

  if (it == NULL)
    return new SGraphEdgeIterator(sg, edgeProperties, val);

  return (new UINTIterator<edge>(it));
}
//=================================================================================
//Fonctionnalite supplementaire ajoute au selection
void BooleanProperty::reverse() {
  Observable::holdObservers();
  Iterator<node> *itN=graph->getNodes();

  while (itN->hasNext()) {
    node itn=itN->next();

    if (getNodeValue(itn)==true)
      setNodeValue(itn,false);
    else
      setNodeValue(itn,true);
  }

  delete itN;
  Iterator<edge> *itE=graph->getEdges();

  while (itE->hasNext()) {
    edge ite=itE->next();

    if (getEdgeValue(ite)==true)
      setEdgeValue(ite,false);
    else
      setEdgeValue(ite,true);
  }

  delete itE;
  Observable::unholdObservers();
}
//=================================================================================
void BooleanProperty::reverseEdgeDirection() {
  Iterator<edge> *itE=graph->getEdges();

  while (itE->hasNext()) {
    edge ite=itE->next();

    if (getEdgeValue(ite))
      graph->reverse(ite);
  }

  delete itE;
}
//=================================================================================
PropertyInterface* BooleanProperty::clonePrototype(Graph * g, const std::string& n) {
  if( !g )
    return 0;

  // allow to get an unregistered property (empty name)
  BooleanProperty * p = n.empty()
                        ? new BooleanProperty(g) : g->getLocalProperty<BooleanProperty>( n );
  p->setAllNodeValue( getNodeDefaultValue() );
  p->setAllEdgeValue( getEdgeDefaultValue() );
  return p;
}
//=================================================================================
PropertyInterface* BooleanVectorProperty::clonePrototype(Graph * g, const std::string& n) {
  if( !g )
    return 0;

  // allow to get an unregistered property (empty name)
  BooleanVectorProperty * p = n.empty()
                              ? new BooleanVectorProperty(g) : g->getLocalProperty<BooleanVectorProperty>( n );
  p->setAllNodeValue( getNodeDefaultValue() );
  p->setAllEdgeValue( getEdgeDefaultValue() );
  return p;
}



