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
#include "ConnectedComponent.h"
#include "tulip/ConnectedTest.h"

using namespace tlp;

DOUBLEPLUGINOFGROUP(ConnectedComponent,"Connected Component","David Auber","01/07/2002","Alpha","1.0","Component");

ConnectedComponent::ConnectedComponent(const tlp::PropertyContext &context):DoubleAlgorithm(context) {}
//======================================================
ConnectedComponent::~ConnectedComponent(){}
//======================================================
bool ConnectedComponent::run() {
  std::vector<std::set<node> > components;
  ConnectedTest::computeConnectedComponents(graph, components);
  // assign the index of each component as value for its nodes
  unsigned int curComponent = 0;
  for (; curComponent < components.size(); curComponent++) {
    std::set<node>& component = components[curComponent];
    std::set<node>::const_iterator itNode = component.begin();
    double value = curComponent;
    for(; itNode!=component.end(); ++itNode) {
      doubleResult->setNodeValue(*itNode, value);
    }
  }
  // propagate nodes computed value to edges
  Iterator<edge> *itE=graph->getEdges();
    while (itE->hasNext()) {
      edge ite=itE->next();
      node source= graph->source(ite);
      node target= graph->target(ite);
      if (doubleResult->getNodeValue(source) == doubleResult->getNodeValue(target))
	doubleResult->setEdgeValue(ite, doubleResult->getNodeValue(source));
      else
	doubleResult->setEdgeValue(ite,curComponent);
    } delete itE;

  return true;
}
//======================================================







