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
#include "SpanningTreeSelection.h"

#include <tulip/GraphTools.h>

PLUGIN(SpanningTreeSelection)

using namespace std;
using namespace tlp;

SpanningTreeSelection::SpanningTreeSelection(const tlp::PluginContext *context)
    : BooleanAlgorithm(context) {
  addOutParameter<unsigned int>("#edges selected", "The number of 'tree' selected edges");
}

///===========================================================
bool SpanningTreeSelection::run() {
  // first initialize result
  result->setAllNodeValue(false);
  result->setAllEdgeValue(false);

  if (graph->existProperty("viewSelection")) {
    BooleanProperty *viewSelection = graph->getProperty<BooleanProperty>("viewSelection");

    for (auto n : graph->nodes()) {
      if (viewSelection->getNodeValue(n) == true) {
        result->setNodeValue(n, true);
      }
    }
  }

  selectSpanningForest(graph, result, pluginProgress);

  // output some useful information
  if (dataSet != nullptr)
    dataSet->set("#edges selected", result->numberOfNonDefaultValuatedEdges());

  return true;
}
