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


#ifndef TULIP_CONNECTEDTEST_H
#define TULIP_CONNECTEDTEST_H

#include <set>
#include <vector>
#include <tulip/tuliphash.h>
#include <tulip/Observable.h>
#include <tulip/Graph.h>

namespace tlp {

/**
 * @ingroup Checks
 * @brief @brief Performs a test of connexity on the graph, and provides a function to make a graph connected.
 * From Wikipedia: "A graph is said to be connected if every pair of vertices in the graph are connected." (i.e. there is a path between every pair of vertices).
 **/
class TLP_SCOPE ConnectedTest : public Observable {
public:

  /**
   * @brief Checks if a graph is connected (i.e. there is a path between every pair of vertices).
   *
   * @param graph The graph to check.
   * @return bool True if the graph is connected, false otherwise.
   **/
  static bool isConnected(const Graph* const graph);

  /**
   * @brief If the graph is not connected, adds edges to make it connected.
   *
   * @param graph The graph to make connected.
   * @param addedEdges The edges that were added to make it connected.
   * @return void
   **/
  static void makeConnected(Graph *graph, std::vector<edge>& addedEdges);

  /**
   * @brief Gets the number of connected components in the graph.
   *
   * @param graph The graph in which to count the number of connected components.
   * @return unsigned int The number of connected componments.
   **/
  static unsigned int numberOfConnectedComponents(const Graph* const graph);

  /**
   * @brief Computes the sets of connected components and stores the result in the components vector.
   *
   * @param graph The graph on which to compute connected components.
   * @param components The components that were found. It is passed as a reference to avoid copying the data when returning.
   * @return void
   * @note The components parameter can be returned with c++11 thanks to move constructors without performance loss, chenge this function once c++11 compilers are used.
   **/
  static void computeConnectedComponents(const Graph *graph, std::vector< std::set<node> >& components);

private:
  ConnectedTest();


  /**
   * @brief Makes the graph connected.
   *
   * @param graph The graph to make connected.
   * @param toLink The nodes that need to be linked so the graph is connected.
   * @return void
   **/
  void connect(const Graph * const, std::vector<node>& toLink);

  /**
   * @brief check if the graph is biconnected.
   *
   * @param graph the graph to check.
   * @return bool true if the graph is connected, false otherwise.
   **/
  bool compute(const Graph * const);

  //override of Observable::treatEvent to remove the cached result for a graph if it is modified.
  virtual void treatEvent(const Event&);

  /**
   * @brief Singleton instance of this class.
   **/
  static ConnectedTest * instance;
  /**
   * @brief Stored results for graphs. When a graph is updated, its entry is removed from the hashmap.
   **/
  TLP_HASH_MAP<const Graph*,bool> resultsBuffer;
};

}

#endif
