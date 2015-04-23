/*
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
///@cond DOXYGEN_HIDDEN


#ifndef TULIP_SIMPLETEST_H
#define TULIP_SIMPLETEST_H

#include <tulip/tuliphash.h>
#include <tulip/Observable.h>
#include <tulip/Graph.h>

namespace tlp {

/**
 * @ingroup Checks
 * @brief Performs test to check if a graph is Simple.
 * From Wikipedia: "A simple graph is an undirected graph that has no loops and no more than one edge between any two different vertices."
 **/
class TLP_SCOPE SimpleTest : private Observable {
public:

  /**
   * @brief Checks if the graph is simple (i.e. it contains no self loops or parallel edges).
   *
   * @param graph The graph to check.
   * @return bool True if the graph is simple, false otherwise.
   **/
  static bool isSimple(const Graph *graph);

  /**
   * Makes the graph  simple by removing self loops and parallel edges if any.
   * The removed edges are stored in the removed vector.
   */
  /**
   * @brief Makes the graph simple, by removing self loops and parallel edges if any.
   *
   * @param graph The graph to make simple.
   * @param removed The edges that were removed to make the graph simple.
   * @return void
   **/
  static void makeSimple(Graph* graph, std::vector<edge> &removed);

  /**
   * Performs simple test and stores found parallel edges in the multipleEdges vector
   * and found self loops in the loops vector.
   * Returns true if the graph is simple, false otherwise.
   */
  /**
   * @brief Checks if the graph is simple, and stores parallel edges and self loops in different vectors.
   *
   * @param graph The graph to check for simplicity.
   * @param multipleEdges The parallel edges that need to be removed to make the graph simple. Defaults to 0.
   * @param loops The self loops that need to be removed to make the graph simple. Defaults to 0.
   * @return bool True if the graph is simple, false otherwise.
   **/
  static bool simpleTest(const Graph *graph, std::vector<edge> *multipleEdges=NULL, std::vector<edge> *loops=NULL);

private:
  SimpleTest();
  // override Observable::treatEvent
  void treatEvent(const Event&);
  void deleteResult(Graph *graph);

  /**
   * @brief Singleton instance of this class.
   **/
  static SimpleTest *instance;
  /**
   * @brief Stored results for graphs. When a graph is updated, its entry is removed from the hashmap.
   **/
  TLP_HASH_MAP<const Graph*, bool> resultsBuffer;
};


}
#endif
///@endcond
