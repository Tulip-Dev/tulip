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
#include "ConnectedComponent.h"
#include <tulip/ConnectedTest.h>

using namespace tlp;

PLUGIN(ConnectedComponent)

ConnectedComponent::ConnectedComponent(const tlp::PluginContext* context):DoubleAlgorithm(context) {}
//======================================================
ConnectedComponent::~ConnectedComponent() {}
//======================================================
bool ConnectedComponent::run() {
  std::vector<std::set<node> > components;
  ConnectedTest::computeConnectedComponents(graph, components);

  // assign the index of each component as value for its nodes
  //unsigned int curComponent = 0;
  for (unsigned int curComponent=0; curComponent < components.size(); ++curComponent) {
    std::set<node>& component = components[curComponent];

    for(std::set<node>::const_iterator itNode = component.begin(); itNode!=component.end(); ++itNode) {
      result->setNodeValue(*itNode, curComponent);
    }
  }

  // propagate nodes computed value to edges
  Iterator<edge> *itE=graph->getEdges();

  while (itE->hasNext()) {
    edge ite=itE->next();
    node source= graph->source(ite);
    //    node target= graph->target(ite);
    //    if (result->getNodeValue(source) == result->getNodeValue(target))
    result->setEdgeValue(ite, result->getNodeValue(source));
    //    else
    //  result->setEdgeValue(ite,curComponent);
  }

  delete itE;

  return true;
}
//======================================================

