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

#include "MakeSelectionGraph.h"

#include <tulip/GraphTools.h>
using namespace tlp;

static const char *paramHelp[] = {
    // selection
    "The property indicating the selected elements"};

MakeSelectionGraph::MakeSelectionGraph(const tlp::PluginContext *context)
    : BooleanAlgorithm(context) {
  addInParameter<BooleanProperty>("selection", paramHelp[0], "viewSelection");
}

bool MakeSelectionGraph::run() {
  BooleanProperty *sel = graph->getProperty<BooleanProperty>("viewSelection");

  if (dataSet != NULL)
    dataSet->get("selection", sel);

  result->copy(sel);
  unsigned added = makeSelectionGraph(graph, result);

  // output some useful information
  if (dataSet != NULL)
    dataSet->set("#elements added to the selection", added);

  return true;
}

isGraphTest::isGraphTest(const tlp::PluginContext *context) : tlp::GraphTest(context) {
  addInParameter<BooleanProperty>("selection", paramHelp[0], "viewSelection");
}

bool isGraphTest::test() {
  BooleanProperty *sel = graph->getProperty<BooleanProperty>("viewSelection");

  if (dataSet != NULL)
    dataSet->get("selection", sel);

  bool test;
  makeSelectionGraph(graph, sel, &test);
  return test;
}

PLUGIN(MakeSelectionGraph)
PLUGIN(isGraphTest)
