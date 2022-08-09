/*
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

#ifndef TULIP_BICONNEX_H
#define TULIP_BICONNEX_H

#include <vector>

#include <tulip/tulipconf.h>

namespace tlp {

class Graph;
struct edge;

/**
 * @ingroup Checks
 * @brief Performs a test of biconnexity on the graph, and provides a function to make a graph
 *biconnected.
 * From Wikipedia: "A biconnected graph is connected and nonseparable, meaning that if any vertex
 *were to be removed, the graph will remain connected."
 **/
class TLP_SCOPE BiconnectedTest {

public:
  /**
   * @brief Checks whether the graph is biconnected (i.e. removing one edge does not disconnect the
   *graph, at least two must be removed).
   *
   * @param graph The graph to check for biconnectivity.
   * @return bool True if the graph is biconnected, false otherwise.
   **/
  static bool isBiconnected(const Graph *graph);

  /**
   * If the graph is not biconnected, adds edges in order to make the graph
   * biconnected. The new edges are added in addedEdges.
   */
  /**
   * @brief Adds edges to make the graph biconnected.
   *
   * @param graph The graph to make biconnected.
   * @param addedEdges The edges that were added in the process.
   * @return void
   **/
  static void makeBiconnected(Graph *graph, std::vector<edge> &addedEdges);
};
} // namespace tlp

#endif
