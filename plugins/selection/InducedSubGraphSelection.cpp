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

#include <tulip/StableIterator.h>

#include "InducedSubGraphSelection.h"

using namespace tlp;

PLUGIN(InducedSubGraphSelection)

//=================================================================================
static const char *paramHelp[] = {
    // selection
    "This property indicates the selected nodes from which the induced subgraph is computed.",
    // Use edges
    "If true, source and target nodes of selected edges will also be added in the input set of "
    "nodes."};
//=================================================================================
InducedSubGraphSelection::InducedSubGraphSelection(const tlp::PluginContext *context)
    : BooleanAlgorithm(context) {
  addInParameter<BooleanProperty>("selection", paramHelp[0], "viewSelection");
  addInParameter<bool>("use edges", paramHelp[1], "false");
  addOutParameter<unsigned int>("#edges selected", "The number of newly selected edges");
}
//=================================================================================
bool InducedSubGraphSelection::run() {
  BooleanProperty *entrySelection = nullptr;
  bool useEdges = false;

  if (dataSet != nullptr) {
    dataSet->get("selection", entrySelection);
    dataSet->get("use edges", useEdges);
  }

  if (entrySelection == nullptr)
    entrySelection = graph->getProperty<BooleanProperty>("viewSelection");

  // as the input selection property and the result property can be the same one,
  // if needed, use a stable iterator to keep a copy of the input selected nodes as all values
  // of the result property are reset to false below
  // deletion is done by the for loop
  Iterator<node> *itN = (result == entrySelection)
                            ? stableIterator(entrySelection->getNodesEqualTo(true))
                            : entrySelection->getNodesEqualTo(true);

  Iterator<edge> *itE = (result == entrySelection)
                            ? stableIterator(entrySelection->getEdgesEqualTo(true))
                            : entrySelection->getEdgesEqualTo(true);

  result->setAllNodeValue(false);
  result->setAllEdgeValue(false);

  // add input selected nodes to result selection
  for (const node &current : itN) {
    result->setNodeValue(current, true);
  }

  if (useEdges) {
    for (const edge &e : itE) {
      result->setNodeValue(graph->source(e), true);
      result->setNodeValue(graph->target(e), true);
    }
  } else {
    delete itE;
  }

  // now add edges whose extremities are selected to result selection
  unsigned sel = 0;
  for (const node &current : result->getNodesEqualTo(true)) {
    for (const edge &e : graph->getOutEdges(current)) {
      if (result->getNodeValue(graph->target(e))) {
        result->setEdgeValue(e, true);
        ++sel;
      }
    }
  }

  // output some useful information
  if (dataSet != nullptr) {
    dataSet->set("#edges selected", sel);
  }

  return true;
}
//=================================================================================
