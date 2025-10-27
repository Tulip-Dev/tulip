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

#include "ReachableSubGraphSelection.h"

#include <tulip/StringCollection.h>
#include <tulip/GraphTools.h>
#include <tulip/StableIterator.h>

using namespace tlp;

PLUGIN(ReachableSubGraphSelection)

static const char *paramHelp[] = {
    // edge direction
    "This parameter defines the navigation direction.",

    // selection
    "This property indicates the selected nodes used to walk in the graph.",

    // max distance
    "This parameter defines the maximum path length (number of edges) between a node already selected and a node that can be reached."};

static const char *directionValuesDescription =
    "output edges: <i>follow output edges (directed)</i><br>"
    "input edges : <i>follow input edges (reverse-directed)</i><br>"
    "all edges   : <i>all edges (undirected)</i>";

#define DIRECTION "output edges; input edges;all edges"
#define DIRECTION_OUTPUT 0
#define DIRECTION_INPUT 1
#define DIRECTION_ALL 2

ReachableSubGraphSelection::ReachableSubGraphSelection(const tlp::PluginContext *context)
    : BooleanAlgorithm(context) {
  addInParameter<StringCollection>("edge direction", paramHelp[0], DIRECTION, true,
                                   directionValuesDescription);
  addInParameter<BooleanProperty>("selection", paramHelp[1], "viewSelection");
  addInParameter<int>("max distance", paramHelp[2], "5");
  addOutParameter<unsigned int>("#edges selected", "The number of newly selected edges");
  addOutParameter<unsigned int>("#nodes selected", "The number of newly selected nodes");
}

///===========================================================
bool ReachableSubGraphSelection::run() {
  unsigned int maxDistance = 5;
  StringCollection edgeDirectionCollection(DIRECTION);
  edgeDirectionCollection.setCurrent(0);
  EDGE_TYPE edgeDirection = DIRECTED;
  BooleanProperty *startNodes = graph->getProperty<BooleanProperty>("viewSelection");

  if (dataSet != nullptr) {
    dataSet->get("max distance", maxDistance);

    // Get the edge orientation
    int direction = 0;
    if (dataSet->get("edge direction", edgeDirectionCollection))
      direction = edgeDirectionCollection.getCurrent();
    else
      // If the new parameter is not defined search for the very former one.
      dataSet->get("direction", direction);

    switch (direction) {
    case 0:
      edgeDirection = DIRECTED;
      break;

    case 1:
      edgeDirection = INV_DIRECTED;
      break;

    case 2:
      edgeDirection = UNDIRECTED;
    }

    dataSet->get("selection", startNodes);
  }

  unsigned num_nodes = 0, num_edges = 0;

  if (startNodes) {
    // as the input selection property and the result property can be the same one,
    // if needed, use a stable iterator to keep a copy of the input selected nodes as all values
    // of the result property are reset to false below
    // deletion is done by the for loop
    Iterator<node> *itN = (result == startNodes) ? stableIterator(startNodes->getNodesEqualTo(true))
                                                 : startNodes->getNodesEqualTo(true);

    tlp_hash_map<node, bool> reachables;

    result->setAllEdgeValue(false);
    result->setAllNodeValue(false);

    // iterate on startNodes add them and their reachables
    for (const node &current : itN) {
      reachables[current] = true;
      markReachableNodes(graph, current, reachables, maxDistance, edgeDirection);
    }

    // select nodes
    for (auto it : reachables) {
      result->setNodeValue(it.first, true);
      ++num_nodes;
    }

    // select corresponding edges
    auto ite = reachables.cend();
    for (const edge e : graph->edges()) {
      auto ends = graph->ends(e);

      if ((reachables.find(ends.first) != ite) && (reachables.find(ends.second) != ite)) {
        result->setEdgeValue(e, true);
        ++num_edges;
      }
    }

  } else {
    result->setAllEdgeValue(false);
    result->setAllNodeValue(false);
  }

  // output some useful information
  if (dataSet != nullptr) {
    dataSet->set("#edges selected", num_edges);
    dataSet->set("#nodes selected", num_nodes);
  }

  return true;
}
