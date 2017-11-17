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

#include <tulip/AcyclicTest.h>
#include <tulip/StaticProperty.h>

#include "SpanningDagSelection.h"

PLUGIN(SpanningDagSelection)

using namespace std;
using namespace tlp;

//=================================================================
SpanningDagSelection::SpanningDagSelection(const tlp::PluginContext* context):BooleanAlgorithm(context) {}
//=================================================================
bool SpanningDagSelection::run() {
  const vector<node> &nodes = graph->nodes();

  for(unsigned i=0; i<nodes.size(); ++i)
    result->setNodeValue(nodes[i],true);

  EdgeStaticProperty<bool> edgeprop(graph);
  edgeprop.setAll(true);

  vector<edge> obstructions;
  AcyclicTest::acyclicTest(graph, &obstructions);

  for (vector<edge>::const_iterator it = obstructions.begin(); it != obstructions.end(); ++it) {
    edgeprop[graph->edgePos(*it)]=false;
  }

  edgeprop.copyToProperty(result);

  //output some useful information
  if (dataSet!=NULL) {
    dataSet->set("#Edges selected", uint(graph->numberOfEdges()-obstructions.size()));
  }

  return true;
}
//=================================================================
