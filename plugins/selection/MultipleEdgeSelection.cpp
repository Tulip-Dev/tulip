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
#include "MultipleEdgeSelection.h"

#include <tulip/SimpleTest.h>

PLUGIN(MultipleEdgeSelection)

using namespace std;
using namespace tlp;

MultipleEdgeSelection::MultipleEdgeSelection(const tlp::PluginContext *context)
    : BooleanAlgorithm(context) {
  addInParameter<bool>("directed", "Indicates if the graph should be considered as directed.",
                       "false");
  addOutParameter<unsigned int>("#edges selected", "The number of multiple edges selected");
}

bool MultipleEdgeSelection::run() {
  bool directed = false;
  if (dataSet)
    dataSet->get("directed", directed);

  vector<edge> multipleEdges;
  SimpleTest::simpleTest(graph, &multipleEdges, nullptr, directed);
  result->setAllNodeValue(false);
  result->setAllEdgeValue(false);

  for (auto e : multipleEdges) {
    result->setEdgeValue(e, true);
  }

  // output some useful information
  if (dataSet != nullptr)
    dataSet->set("#edges selected", uint(multipleEdges.size()));

  return true;
}
