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
#ifndef TLPEXTENDEDCLUSTEROPERATIONS
#define TLPEXTENDEDCLUSTEROPERATIONS

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <set>
#include <vector>
#include <string>
#include <tulip/Node.h>
#include <tulip/Graph.h>
#include <tulip/GraphProperty.h>

namespace tlp {
  /**
   * Function to open a metanode and replace all edges between that
   * meta node and other nodes in the graph.
   */
  inline void openMetaNode(Graph *graph, node n) {
    graph->openMetaNode(n);
  }

  /**
   * Function to close a subgraph into a metanode.  Edges from nodes
   * in the subgraph to nodes outside the subgraph are replaced with
   * edges from the metanode to the nodes outside the subgraph.
   */
  inline node createMetaNode(Graph *graph, const std::set<node> &nodeSet) {
    return graph->createMetaNode(nodeSet);
  }

  /**
   *  Return the subgraph induced by a set of nodes
   */
  inline Graph *inducedSubGraph(Graph* graph, const std::set<node> &nodeSet) {
    return graph->inducedSubGraph(nodeSet);
  }
}

#endif
