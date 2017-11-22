/**
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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
    : BooleanAlgorithm(context) {}

SpanningTreeSelection::~SpanningTreeSelection() {}

///===========================================================
bool SpanningTreeSelection::run() {
  // first initialize result
  result->setAllNodeValue(false);
  result->setAllEdgeValue(false);

  if (graph->existProperty("viewSelection")) {
    BooleanProperty *viewSelection = graph->getProperty<BooleanProperty>("viewSelection");
    Iterator<node> *itN = graph->getNodes();

    while (itN->hasNext()) {
      node itn = itN->next();

      if (viewSelection->getNodeValue(itn) == true) {
        result->setNodeValue(itn, true);
      }
    }

    delete itN;
  }

  selectSpanningForest(graph, result, pluginProgress);

  // output some useful information
  if (dataSet != nullptr)
    dataSet->set("#Edges selected", result->numberOfNonDefaultValuatedEdges());

  return true;
}
