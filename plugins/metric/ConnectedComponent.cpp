/**
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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

ConnectedComponent::ConnectedComponent(const tlp::PluginContext *context)
    : DoubleAlgorithm(context) {

  addOutParameter<unsigned>("#connected components", "Number of connected components found");
}
//======================================================
bool ConnectedComponent::run() {
  std::vector<std::vector<node>> components;
  ConnectedTest::computeConnectedComponents(graph, components);

  // assign the index of each component as value for its nodes
  unsigned int i = 0;
  for (auto &component : components) {
    for (auto n : component) {
      result->setNodeValue(n, i);
    }
    ++i;
  }
  if (dataSet != nullptr)
    dataSet->set<unsigned>("#connected components", i);

  if (i > 0) {
    // propagate nodes computed value to edges
    for (auto e : graph->edges()) {
      result->setEdgeValue(e, result->getNodeValue(graph->source(e)));
    }
  }

  return true;
}
//======================================================
