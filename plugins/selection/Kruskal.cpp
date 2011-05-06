/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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

BOOLEANPLUGIN(Kruskal,"Kruskal","Anthony DON","14/04/03","Alpha","1.0");

using namespace std;
using namespace tlp;

namespace {
  const char * paramHelp[] = {
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "DoubleProperty" ) \
    HTML_HELP_DEF( "default", "\"viewMetric\"" ) \
    HTML_HELP_BODY() \
    "This parameter defines the metric used for edges weight." \
    HTML_HELP_CLOSE(),
  };
}
//======================================================
Kruskal::Kruskal(const tlp::PropertyContext &context):BooleanAlgorithm(context) {
  addParameter<DoubleProperty> ("edge weight", paramHelp[0], "viewMetric");
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
    erreurMsg = "The Graph must be connected";
    return false;
  }
}
//======================================================
/// Compute the Minimum Spanning Tree
bool Kruskal::run(){
  /* Initialisation */
  DoubleProperty *edgeWeight = 0;
  if ( dataSet!=0) {
    dataSet->get("edge weight", edgeWeight);
  }
  if (edgeWeight == 0)
    edgeWeight = graph->getProperty<DoubleProperty>("viewMetric");

  selectMinimumSpanningTree(graph, _result, edgeWeight, pluginProgress);

  return true;
}
//=======================================================================
