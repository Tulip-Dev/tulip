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
#include "Kruskal.h"

PLUGIN(Kruskal)

using namespace std;
using namespace tlp;

namespace {
const char * paramHelp[] = {
  HTML_HELP_OPEN() \
  HTML_HELP_DEF( "type", "NumericProperty" ) \
  HTML_HELP_DEF( "default", "\"viewMetric\"" ) \
  HTML_HELP_BODY() \
  "Metric containing the edges weights." \
  HTML_HELP_CLOSE(),
};
}
//======================================================
Kruskal::Kruskal(const tlp::PluginContext* context):BooleanAlgorithm(context) {
  addInParameter<NumericProperty*> ("edge weight", paramHelp[0], "viewMetric");
}
//======================================================
Kruskal::~Kruskal() {
}
//======================================================
#include <tulip/ConnectedTest.h>
bool Kruskal::check(std::string &erreurMsg) {
  if (ConnectedTest::isConnected(graph)) {
    erreurMsg = "";
    return true;
  }
  else {
    erreurMsg = "The graph must be connected.";
    return false;
  }
}
//======================================================
/// Compute the Minimum Spanning Tree
bool Kruskal::run() {
  /* Initialisation */
  NumericProperty *edgeWeight = NULL;

  if ( dataSet!=NULL) {
    dataSet->get("edge weight", edgeWeight);
  }

  if (edgeWeight == NULL)
    edgeWeight = graph->getProperty<DoubleProperty>("viewMetric");

  selectMinimumSpanningTree(graph, result, edgeWeight, pluginProgress);

  return true;
}
//=======================================================================
