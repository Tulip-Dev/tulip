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
#include "LoopSelection.h"

PLUGIN(LoopSelection)

using namespace std;
using namespace tlp;

LoopSelection::LoopSelection(const tlp::PluginContext *context) : BooleanAlgorithm(context) {
  addOutParameter<unsigned int>("#edges selected", "The number of loops selected");
}
//============================================
bool LoopSelection::run() {
  result->setAllNodeValue(false);
  for (auto e : graph->edges()) {
    auto eEnds = graph->ends(e);

    if (eEnds.first == eEnds.second) {
      result->setEdgeValue(e, true);
    }
  }

  // output some useful information
  if (dataSet != nullptr) {
    dataSet->set("#edges selected", result->numberOfNonDefaultValuatedEdges(graph));
  }

  return true;
}
//============================================
