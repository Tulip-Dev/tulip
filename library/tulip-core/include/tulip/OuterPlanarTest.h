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

#ifndef OUTERPLANARTEST_H
#define OUTERPLANARTEST_H

#include <tulip/tuliphash.h>
#include <tulip/Observable.h>
#include <tulip/Graph.h>

namespace tlp {
struct edge;
struct node;

/**
 * @ingroup Checks
 * @brief Provides functions to test if a graph is Outer Planar.
 *
 * From Wikipedia: "An OuterPlanar Graph is a graph that can be drawn in the plane without crossings in such a way that all of the vertices belong to the unbounded face of the drawing.
 * Alternatively, a graph G is outerplanar if the graph formed from G by adding a new vertex, with edges connecting it to all the other vertices, is a planar graph."
 **/
class TLP_SCOPE OuterPlanarTest : private Observable {
public:

  /**
   * Returns true if the graph is outerplanar (i.e. a graph with an embedding
   * in the plane such that all vertices belong to the unbounded face of the embedding),
   * false otherwise.
   */
  /**
   * @brief Checks if a graph is outer planar (i.e. a graph with an embedding in the plane such that all vertices belong to the unbounded face of the embedding).
   *
   * @param graph The graph to check.
   * @return bool True if the graph is outer planar, false otherwise.
   * @note this cannot be const as it uses the planarity test, which is not const and would be complex (impossible?) to make const.
   **/
  static bool isOuterPlanar(Graph *graph);

private:
  OuterPlanarTest() {}
  // override Observable::treatEvent
  void treatEvent(const Event&);

  bool compute(Graph *graph);

  /**
   * @brief Singleton instance of this class.
   **/
  static OuterPlanarTest* instance;
  /**
   * @brief Stored results for graphs. When a graph is updated, its entry is removed from the hashmap.
   **/
  TLP_HASH_MAP<const Graph*, bool> resultsBuffer;
};

}

#endif
///@endcond
