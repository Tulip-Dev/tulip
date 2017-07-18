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

PLUGIN(ConnectedComponent)

using namespace tlp;

ConnectedComponent::ConnectedComponent(const tlp::PluginContext* context):DoubleAlgorithm(context) {}
//======================================================
bool ConnectedComponent::run() {
  std::vector<std::vector<node> > components;
  ConnectedTest::computeConnectedComponents(graph, components);

  // assign the index of each component as value for its nodes
  for (unsigned curComponent = 0; curComponent < components.size(); ++curComponent) {
    std::vector<node>& component = components[curComponent];

    for(std::vector<node>::const_iterator itNode = component.begin(); itNode!=component.end(); ++itNode) {
      result->setNodeValue(*itNode, curComponent);
    }
  }

  // propagate nodes computed value to edges
   const std::vector<edge>& edges = graph->edges();
   unsigned int nbEdges = edges.size();

   for (unsigned int i = 0; i < nbEdges; ++i) {
     edge e = edges[i];
     result->setEdgeValue(e, result->getNodeValue(graph->source(e)));
  }

  if (dataSet!=NULL)
    dataSet->set<unsigned>("#connected components", components.size());

  return true;
}
//======================================================

