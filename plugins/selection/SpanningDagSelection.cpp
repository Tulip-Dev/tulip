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

#include <tulip/AcyclicTest.h>
#include <tulip/StaticProperty.h>

#include "SpanningDagSelection.h"

PLUGIN(SpanningDagSelection)

using namespace std;
using namespace tlp;

//=================================================================
SpanningDagSelection::SpanningDagSelection(const tlp::PluginContext *context)
    : BooleanAlgorithm(context) {
  addOutParameter<unsigned int>("#edges selected", "The number of 'dag' selected edges");
}
//=================================================================
bool SpanningDagSelection::run() {
  for (auto n : graph->nodes())
    result->setNodeValue(n, true);

  EdgeStaticProperty<bool> edgeprop(graph, true);

  vector<edge> obstructions;
  AcyclicTest::acyclicTest(graph, &obstructions);

  for (auto e : obstructions) {
    edgeprop[e] = false;
  }

  edgeprop.copyToProperty(result);

  // output some useful information
  if (dataSet != nullptr) {
    dataSet->set("#edges selected", uint(graph->numberOfEdges() - obstructions.size()));
  }

  return true;
}
//=================================================================
