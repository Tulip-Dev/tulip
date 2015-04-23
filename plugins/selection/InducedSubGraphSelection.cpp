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

#include "InducedSubGraphSelection.h"

using namespace tlp;

PLUGIN(InducedSubGraphSelection)

//=================================================================================
namespace {
  const char * paramHelp[] = {
    // selectedNodes
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "Selection" ) \
    HTML_HELP_BODY() \
    "Set of nodes for which the induced sub-graph is computed." \
    HTML_HELP_CLOSE(),
  };
}
//=================================================================================
InducedSubGraphSelection::InducedSubGraphSelection(const tlp::PluginContext* context):
  BooleanAlgorithm(context) {
  addInParameter<BooleanProperty>("Nodes", paramHelp[0], "viewSelection");
}
//=================================================================================
bool InducedSubGraphSelection::run() {
  BooleanProperty *entrySelection = NULL;

  if (dataSet!=NULL)
    dataSet->get("Nodes", entrySelection);

  if (entrySelection == NULL)
    entrySelection = graph->getProperty<BooleanProperty>("viewSelection");

  // as the input selection property and the result property can be the same one,
  // if needed, use a stable iterator to keep a copy of the input selected nodes as all values
  // of the result property are reseted to false below
  Iterator<node>* itN = (result == entrySelection) ?
                          new StableIterator<tlp::node>(entrySelection->getNodesEqualTo(true)) :
                          entrySelection->getNodesEqualTo(true);

  result->setAllNodeValue(false);
  result->setAllEdgeValue(false);

  // add input selected nodes to result selection
  node current;
  forEach(current, itN) {
    result->setNodeValue(current, true);
  }

  // now add edges whose extremities are selected to result selection
  edge e;
  forEach(e, graph->getEdges()) {
    if (result->getNodeValue(graph->source(e)) && result->getNodeValue(graph->target(e))) {
      result->setEdgeValue(e, true);
    }
  }

  return true;
}
//=================================================================================
