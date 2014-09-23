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
#include <tulip/BooleanProperty.h>
#include <tulip/AcyclicTest.h>

#include "SpanningDagSelection.h"

PLUGIN(SpanningDagSelection)

using namespace std;
using namespace tlp;

//=================================================================
SpanningDagSelection::SpanningDagSelection(const tlp::PluginContext* context):BooleanAlgorithm(context) {}
//=================================================================
bool SpanningDagSelection::run() {
  result->setAllNodeValue(true);
  result->setAllEdgeValue(true);
  vector<edge> obstructions;
  AcyclicTest::acyclicTest(graph, &obstructions);

  for (vector<edge>::const_iterator it = obstructions.begin(); it != obstructions.end(); ++it) {
    result->setEdgeValue(*it, false);
  }

  return true;
}
//=================================================================
