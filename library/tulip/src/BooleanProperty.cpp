/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
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

#include "tulip/BooleanProperty.h"
#include "tulip/PluginContext.h"
#include "tulip/Observable.h"
#include "tulip/BooleanAlgorithm.h"
#include "tulip/AbstractProperty.h"
#include "tulip/GraphIterator.h"

using namespace std;
using namespace tlp;

//=================================================================================
Iterator<node>* BooleanProperty::getNodesEqualTo(const bool val, Graph *sg) {
  if (sg == 0) sg = graph;
  Iterator<unsigned int> *it = 0;
  if (sg == graph)
    it = nodeProperties.findAll(val);
  if (it == 0)
    return new SGraphNodeIterator(sg, nodeProperties, val);
  return (new UINTIterator<node>(it));
}
//=================================================================================
Iterator<edge>* BooleanProperty::getEdgesEqualTo(const bool val, Graph *sg) {
  if (sg == 0) sg = graph;
  Iterator<unsigned int> *it=0;
  if (sg == graph)
    it = edgeProperties.findAll(val);
  if (it == 0)
    return new SGraphEdgeIterator(sg, edgeProperties, val);
  return (new UINTIterator<edge>(it));
}
//=================================================================================
//Fonctionnalit� suppl�mentaire ajout� au seletion 
void BooleanProperty::reverse() {
  Observable::holdObservers();
  Iterator<node> *itN=graph->getNodes();
  while (itN->hasNext()) {
    node itn=itN->next();
    if (getNodeValue(itn)==true)
      setNodeValue(itn,false);
    else
      setNodeValue(itn,true);
  } delete itN;
  Iterator<edge> *itE=graph->getEdges();
  while (itE->hasNext()) {
    edge ite=itE->next();
    if (getEdgeValue(ite)==true)
      setEdgeValue(ite,false);
    else
      setEdgeValue(ite,true);
  } delete itE;
  notifyObservers();
  Observable::unholdObservers();
}
//=================================================================================
void BooleanProperty::reverseEdgeDirection() {
  Iterator<edge> *itE=graph->getEdges();
  while (itE->hasNext()) {
    edge ite=itE->next();
    if (getEdgeValue(ite))
      graph->reverse(ite);
  } delete itE;
}
//=================================================================================
PropertyInterface* BooleanProperty::clonePrototype(Graph * g, const std::string& n)
{
	if( !g )
		return 0;
	BooleanProperty * p = g->getLocalProperty<BooleanProperty>( n );
	p->setAllNodeValue( getNodeDefaultValue() );
	p->setAllEdgeValue( getEdgeDefaultValue() );
	return p;
}
//=================================================================================
PropertyInterface* BooleanVectorProperty::clonePrototype(Graph * g, const std::string& n) {
  if( !g )
    return 0;
  BooleanVectorProperty * p = g->getLocalProperty<BooleanVectorProperty>( n );
  p->setAllNodeValue( getNodeDefaultValue() );
  p->setAllEdgeValue( getEdgeDefaultValue() );
  return p;
}



