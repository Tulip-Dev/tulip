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

#include "ReachableSubGraphSelection.h"

#include <tulip/StringCollection.h>

using namespace tlp;

PLUGIN(ReachableSubGraphSelection)

static const char *paramHelp[] = {
    // edges direction
    "This parameter defines the navigation direction.",

    // starting nodes
    "This parameter defines the starting set of nodes used to walk in the graph.",

    // distance
    "This parameter defines the maximal distance of reachable nodes."};

static const char *directionValuesDescription = "output edges : <i>follow ouput edges (directed)</i><br>"
                                                "input edges : <i>follow input edges (reverse-directed)</i><br>"
                                                "all edges : <i>all edges (undirected)</i>";

static const char *edgesDirectionLabels[] = {"output edges", "input edges", "all edges"};

ReachableSubGraphSelection::ReachableSubGraphSelection(const tlp::PluginContext *context) : BooleanAlgorithm(context) {
  addInParameter<StringCollection>("edges direction", paramHelp[0], "output edges;input edges;all edges", true, directionValuesDescription);
  addInParameter<BooleanProperty>("starting nodes", paramHelp[1], "viewSelection");
  addInParameter<int>("distance", paramHelp[2], "5");
}

ReachableSubGraphSelection::~ReachableSubGraphSelection() {
}

///===========================================================
bool ReachableSubGraphSelection::run() {
  unsigned int maxDistance = 5;
  StringCollection edgeDirectionCollecion;
  EDGE_TYPE edgeDirection = DIRECTED;
  BooleanProperty *startNodes = graph->getProperty<BooleanProperty>("viewSelection");

  if (dataSet != nullptr) {
    dataSet->get("distance", maxDistance);

    // Get the edge orientation
    if (dataSet->get("edges direction", edgeDirectionCollecion)) {
      if (edgeDirectionCollecion.getCurrentString() == edgesDirectionLabels[0]) {
        edgeDirection = DIRECTED;
      } else if (edgeDirectionCollecion.getCurrentString() == edgesDirectionLabels[1]) {
        edgeDirection = INV_DIRECTED;
      } else if (edgeDirectionCollecion.getCurrentString() == edgesDirectionLabels[2]) {
        edgeDirection = UNDIRECTED;
      }
    } else {
      // If the new parameter is not defined search for the old one.
      int direction = 0;

      if (dataSet->get("direction", direction)) {
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
      }
    }

    // keep startingnodes for compatibility
    if (dataSet->exist("startingnodes"))
      dataSet->get("startingnodes", startNodes);
    else
      dataSet->get("starting nodes", startNodes);
  }

  if (startNodes) {
    // as the input selection property and the result property can be the same one,
    // if needed, use a stable iterator to keep a copy of the input selected nodes as all values
    // of the result property are reseted to false below
    Iterator<node> *itN =
        (result == startNodes) ? new StableIterator<tlp::node>(startNodes->getNodesEqualTo(true)) : startNodes->getNodesEqualTo(true);

    std::set<node> reachables;

    result->setAllEdgeValue(false);
    result->setAllNodeValue(false);

    // iterate on startNodes add them and their reachables
    for (node current : itN) {
      reachables.insert(current);
      reachableNodes(graph, current, reachables, maxDistance, edgeDirection);
    }

    std::set<node>::const_iterator itr = reachables.begin();
    std::set<node>::const_iterator ite = reachables.end();

    // select nodes
    while (itr != ite) {
      result->setNodeValue((*itr), true);
      ++itr;
    }

    // select corresponding edges
    for (edge e : graph->getEdges()) {
      const std::pair<node, node> &ends = graph->ends(e);

      if (result->getNodeValue(ends.first) && result->getNodeValue(ends.second))
        result->setEdgeValue(e, true);
    }

  } else {
    result->setAllEdgeValue(false);
    result->setAllNodeValue(false);
  }

  return true;
}
