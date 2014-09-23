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


#ifndef TULIP_TRICONNECTEDTEST_H
#define TULIP_TRICONNECTEDTEST_H

#include <tulip/tuliphash.h>
#include <tulip/Observable.h>
#include <tulip/Graph.h>

namespace tlp {

/**
 * @ingroup Checks
 * @brief Provides functions to test if a graph is Triconnected.
 **/
class TLP_SCOPE TriconnectedTest : private Observable {
public:

  /**
   * Returns true if the graph is triconnected (i.e. a connected graph such that deleting any two nodes (and incident edges)
   * results in a graph that is still connected), false otherwise.
   */
  /**
   * @brief Checks if the graph is triconnected.
   * Creates a clone sugraph in which to operate, then iterates over the nodes, and deletes them.
   * Once the node is deleted, checks if the graph is biconnected.
   * If it is not, then the graph is not triconnected.
   * If it is, adds back the node and its edges.
   *
   * @param graph The graph to check is triconnected.
   * @return bool True if the graph is triconnected, false otherwise.
   **/
  static bool isTriconnected(Graph *graph);

private:
  TriconnectedTest();

  bool compute(tlp::Graph* graph);
  // override Observable::treatEvent
  void treatEvent(const Event&);

  /**
   * @brief Singleton instance of this class.
   **/
  static TriconnectedTest * instance;
  /**
   * @brief Stored results for graphs. When a graph is updated, its entry is removed from the hashmap.
   **/
  TLP_HASH_MAP<const Graph*, bool> resultsBuffer;
};


}
#endif
///@endcond
