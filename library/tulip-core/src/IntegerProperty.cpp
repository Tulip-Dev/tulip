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
#include <climits>
#include <tulip/IntegerProperty.h>
#include <tulip/PropertyAlgorithm.h>
#include <tulip/GraphTools.h>

using namespace std;
using namespace tlp;

const string IntegerProperty::propertyTypename="int";
const string IntegerVectorProperty::propertyTypename="vector<int>";

//==============================
///Constructeur d'un IntegerProperty
IntegerProperty::IntegerProperty (Graph *g, const std::string& n):IntegerMinMaxProperty(g, n, -INT_MAX, INT_MAX, -INT_MAX, INT_MAX) {
}
//====================================================================
void IntegerProperty::clone_handler(AbstractProperty<tlp::IntegerType, tlp::IntegerType, tlp::NumericProperty> &proxyC) {
  if (typeid(this)==typeid(&proxyC)) {
    IntegerProperty *proxy=(IntegerProperty *)&proxyC;
    minMaxNode = proxy->minMaxNode;
    minMaxEdge = proxy->minMaxEdge;
  }
}
//=================================================================================
PropertyInterface* IntegerProperty::clonePrototype(Graph * g, const std::string& n) {
  if( !g )
    return 0;

  // allow to get an unregistered property (empty name)
  IntegerProperty * p = n.empty()
                        ? new IntegerProperty(g) : g->getLocalProperty<IntegerProperty>( n );
  p->setAllNodeValue( getNodeDefaultValue() );
  p->setAllEdgeValue( getEdgeDefaultValue() );
  return p;
}
//=================================================================================
void IntegerProperty::setNodeValue(const node n, const int &v) {
  IntegerMinMaxProperty::updateNodeValue(n, v);
  IntegerMinMaxProperty::setNodeValue(n, v);
}
//=================================================================================
void IntegerProperty::setEdgeValue(const edge e, const int &v) {
  IntegerMinMaxProperty::updateEdgeValue(e, v);
  IntegerMinMaxProperty::setEdgeValue(e, v);
}
//=================================================================================
void IntegerProperty::setAllNodeValue(const int &v) {
  IntegerMinMaxProperty::updateAllNodesValues(v);
  IntegerMinMaxProperty::setAllNodeValue(v);
}
//=================================================================================
void IntegerProperty::setAllEdgeValue(const int &v) {
  IntegerMinMaxProperty::updateAllEdgesValues(v);
  IntegerMinMaxProperty::setAllEdgeValue(v);
}
//=============================================================
void IntegerProperty::treatEvent(const Event& evt) {
  IntegerMinMaxProperty::treatEvent(evt);
}
//=================================================================================
int IntegerProperty::compare(const node n1, const node n2)const {
  return getNodeValue(n1) - getNodeValue(n2);
}
//=================================================================================
int IntegerProperty::compare(const edge e1, const edge e2)const {
  return getEdgeValue(e1) - getEdgeValue(e2);
}
//=================================================================================
PropertyInterface* IntegerVectorProperty::clonePrototype(Graph * g, const std::string& n) {
  if( !g )
    return 0;

  // allow to get an unregistered property (empty name)
  IntegerVectorProperty * p = n.empty()
                              ? new IntegerVectorProperty(g) : g->getLocalProperty<IntegerVectorProperty>( n );
  p->setAllNodeValue( getNodeDefaultValue() );
  p->setAllEdgeValue( getEdgeDefaultValue() );
  return p;
}

//===============================================================
void IntegerProperty::nodesUniformQuantification(unsigned int k) {
  std::map<double,int> nodeMapping;
  buildNodesUniformQuantification(graph, this, k, nodeMapping);

  Iterator<node> *itN=graph->getNodes();

  while(itN->hasNext()) {
    node itn=itN->next();
    setNodeValue(itn, nodeMapping[getNodeValue(itn)]);
  }

  delete itN;
}

//===============================================================
void IntegerProperty::edgesUniformQuantification(unsigned int k) {
  std::map<double,int> edgeMapping;
  buildEdgesUniformQuantification(graph, this, k, edgeMapping);

  Iterator<edge> *itE=graph->getEdges();

  while(itE->hasNext()) {
    edge ite=itE->next();
    setEdgeValue(ite, edgeMapping[getEdgeValue(ite)]);
  }

  delete itE;
}
