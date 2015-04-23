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

#ifndef TULIP_BICONNEX_H
#define TULIP_BICONNEX_H

#include <tulip/MutableContainer.h>
#include <tulip/Observable.h>
#include <tulip/Graph.h>

namespace tlp {

/**
 * @ingroup Checks
 * @brief Performs a test of biconnexity on the graph, and provides a function to make a graph biconnected.
 * From Wikipedia: "A biconnected graph is connected and nonseparable, meaning that if any vertex were to be removed, the graph will remain connected."
 **/
class TLP_SCOPE BiconnectedTest : private Observable {

public:
  /**
   * @brief Checks whether the graph is biconnected (i.e. removing one edge does not disconnect the graph, at least two must be removed).
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
  static void makeBiconnected(Graph *graph, std::vector<edge>& addedEdges);

private:
  BiconnectedTest();

  /**
   * @brief Makes the graph biconnected.
   *
   * @param graph The graph to make biconnected.
   * @param addedEdges The edges that were added to make it biconnected.
   * @return void
   **/
  void connect(Graph * graph, std::vector<edge>& addedEdges);

  /**
   * @brief check if the graph is biconnected.
   *
   * @param graph the graph to check.
   * @return bool true if the graph is biconnected, false otherwise.
   **/
  bool compute(const Graph * graph);

  //override of Observable::treatEvent to remove the cached result for a graph if it is modified.
  virtual void treatEvent(const Event&);

  /**
   * @brief Singleton instance of this class.
   **/
  static BiconnectedTest * instance;
  /**
   * @brief Stored results for graphs. When a graph is updated, its entry is removed from the hashmap.
   **/
  TLP_HASH_MAP<const Graph*,bool> resultsBuffer;
};
}

#endif
