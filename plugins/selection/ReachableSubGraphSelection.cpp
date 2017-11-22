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

#include "ReachableSubGraphSelection.h"

#include <tulip/StringCollection.h>
#include <tulip/GraphMeasure.h>

using namespace tlp;

PLUGIN(ReachableSubGraphSelection)

static const char *paramHelp[] = {
    // edge direction
    "This parameter defines the navigation direction.",

    // starting nodes
    "This parameter defines the starting set of nodes used to walk in the graph.",

    // distance
    "This parameter defines the maximal distance of reachable nodes."};

static const char *directionValuesDescription =
    "output edges : <i>follow ouput edges (directed)</i><br>"
    "input edges : <i>follow input edges (reverse-directed)</i><br>"
    "all edges : <i>all edges (undirected)</i>";

static const char *edgesDirectionLabels[] = {"output edges", "input edges", "all edges"};

ReachableSubGraphSelection::ReachableSubGraphSelection(const tlp::PluginContext *context)
    : BooleanAlgorithm(context) {
  addInParameter<StringCollection>("edge direction", paramHelp[0],
                                   "output edges;input edges;all edges", true,
                                   directionValuesDescription);
  addInParameter<BooleanProperty>("starting nodes", paramHelp[1], "viewSelection");
  addInParameter<int>("distance", paramHelp[2], "5");
}

ReachableSubGraphSelection::~ReachableSubGraphSelection() {}

///===========================================================
bool ReachableSubGraphSelection::run() {
  unsigned int maxDistance = 5;
  StringCollection edgeDirectionCollecion;
  EDGE_TYPE edgeDirection = DIRECTED;
  BooleanProperty *startNodes = graph->getProperty<BooleanProperty>("viewSelection");

  if (dataSet != nullptr) {
    dataSet->get("distance", maxDistance);

    // Get the edge orientation
    bool found(false);

    if (dataSet->get("edge direction", edgeDirectionCollecion))
      found = true;
    else
      found = dataSet->get("edges direction", edgeDirectionCollecion); // former buggy parameter
                                                                       // name

    if (found) {
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
    if (!dataSet->get("starting nodes", startNodes))
      dataSet->get("startingnodes", startNodes);
  }

  unsigned num_nodes = 0, num_edges = 0;

  if (startNodes) {
    // as the input selection property and the result property can be the same one,
    // if needed, use a stable iterator to keep a copy of the input selected nodes as all values
    // of the result property are reseted to false below
    // delete done by the forEach macro
    Iterator<node> *itN = (result == startNodes)
                              ? new StableIterator<tlp::node>(startNodes->getNodesEqualTo(true))
                              : startNodes->getNodesEqualTo(true);

    TLP_HASH_MAP<node, bool> reachables;

    result->setAllEdgeValue(false);
    result->setAllNodeValue(false);

    // iterate on startNodes add them and their reachables
    node current;
    forEach(current, itN) {
      reachables[current] = true;
      markReachableNodes(graph, current, reachables, maxDistance, edgeDirection);
    }

    TLP_HASH_MAP<node, bool>::const_iterator itr = reachables.begin();
    TLP_HASH_MAP<node, bool>::const_iterator ite = reachables.end();

    // select nodes
    while (itr != ite) {
      result->setNodeValue(itr->first, true);
      ++itr;
      ++num_nodes;
    }

    // select corresponding edges
    edge e;
    forEach(e, graph->getEdges()) {
      const std::pair<node, node> &ends = graph->ends(e);

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
    dataSet->set("#Edges added to the selection", num_edges);
    dataSet->set("#Nodes added to the selection", num_nodes);
  }

  return true;
}
