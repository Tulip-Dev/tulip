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

#ifndef TULIP_SIMPLETEST_H
#define TULIP_SIMPLETEST_H

#include <vector>

#include <tulip/tulipconf.h>

namespace tlp {

class Graph;
struct edge;

/**
 * @ingroup Checks
 * @brief Performs test to check if a graph is Simple.
 * An undirected graph is simple if it has no loops and no more than one
 * edge between any unordered pair of vertices.
 * A directed graph is simple if has no loops and no more than one
 * edge between any ordered pair of vertices.
 **/
class TLP_SCOPE SimpleTest {
public:
  /**
   * @brief Checks if the graph is simple (i.e. it contains no self loops or parallel edges).
   *
   * @param graph The graph to check.
   * @param directed Whether the graph shall be considered directed or not.
   * @return bool True if the graph is simple, false otherwise.
   **/
  static bool isSimple(const Graph *graph, const bool directed = false);
  /**
   * @brief Checks if the graph has self loop edges.
   *
   * @param graph The graph to check.
   * @return bool True if the graph has loops, false otherwise.
   **/
  static bool hasLoops(const Graph *graph);
  /**
   * @brief Checks if the graph has parallel edges.
   *
   * @param graph The graph to check.
   * @param directed Whether the graph shall be considered directed or not.
   * @return bool True if the graph has parallel edges, false otherwise.
   **/
  static bool hasParallelEdges(const Graph *graph, const bool directed = false);

  /**
   * Makes the graph  simple by removing self loops and parallel edges if any.
   * The removed edges are stored in the removed vector.
   */
  /**
   * @brief Makes the graph simple, by removing self loops and parallel edges if any.
   *
   * @param graph The graph to make simple.
   * @param removed The edges that were removed to make the graph simple.
   * @param directed Whether the graph shall be considered directed or not.
   * @return void
   **/
  static void makeSimple(Graph *graph, std::vector<edge> &removed, const bool directed = false);

  /**
   * Performs simple test and stores found parallel edges in the multipleEdges vector
   * and found self loops in the loops vector.
   * Returns true if the graph is simple, false otherwise.
   */
  /**
   * @brief Checks if the graph is simple, and stores parallel edges and self loops in different
   *vectors.
   *
   * @param graph The graph to check for simplicity.
   * @param multipleEdges The parallel edges that need to be removed to make the graph simple.
   *Defaults to 0.
   * @param loops The self loops that need to be removed to make the graph simple. Defaults to 0.
   * @param directed Whether the graph shall be considered directed or not.
   * @return bool True if the graph is simple, false otherwise.
   **/
  static bool simpleTest(const Graph *graph, std::vector<edge> *multipleEdges = nullptr,
                         std::vector<edge> *loops = nullptr, const bool directed = false);
};
} // namespace tlp
#endif
