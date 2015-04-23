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

#ifndef TULIP_ACYCLICITY_TEST_H
#define TULIP_ACYCLICITY_TEST_H
#include <vector>
#include <tulip/Observable.h>
#include <tulip/MutableContainer.h>
#include <tulip/Graph.h>

namespace tlp {
class BooleanProperty;

/**
 * @ingroup Checks
 *
 * @brief Stores all the added information on self loops.
 *
 * Self loops are removed by adding two nodes and three edges.
 *
 * These are stores here, along with the old self looping edge.
 *
 * From Wikipedia: "A directed acyclic graph (DAG), is a directed graph with no directed cycles."
 **/
struct SelfLoops {
public:
  SelfLoops(node n1,node n2,edge e1, edge e2,edge e3,edge old):
    n1(n1),n2(n2),e1(e1),e2(e2),e3(e3),old(old) {
  }
  node n1,n2;
  edge e1,e2,e3,old;
};

/**
 * @ingroup Checks
 *
 * @brief This class provides tests for acyclicity on a graph.
 * Results are cached in a map of graphs and result.
 * This class observes the graphs that have been tested to remove the result from this graph if it is modified.
 * This forces the use of the singleton pattern instead of simply using static functions/members.
 **/
class TLP_SCOPE AcyclicTest : private Observable {
public:

  /**
   * @brief Checks whether the graph is acyclic or not.
   * The result is cached so subsequent calls are in O(1).
   *
   * @param graph The graph on which to perform the acyclicity test.
   * @return bool True if the graph is acyclic, false otherwise.
   **/
  static bool isAcyclic(const Graph *graph);

  /**
   * @brief Makes the graph acyclic by removing edges.
   *
   * @param graph The graph to make acyclic.
   * @param reversed The edges that were reversed during the process.
   * @param selfLoops Sets of two nodes and three edges that were added in stead of self loops.
   * @return void
   **/
  static void makeAcyclic(Graph* graph, std::vector<edge> &reversed, std::vector<tlp::SelfLoops> &selfLoops);

  /**
   * @brief Returns whether the graph is acyclic.
   * Collection of obstruction edges takes a bit of time, as iteration over the graph must continue even when it has been found cyclic.
   *
   * @param graph the graph to test for acyclicity
   * @param obstructionEdges If not null, will be filled with edges that cause the graph to be cyclic. Defaults to 0.
   * @return bool
   **/
  static bool acyclicTest(const Graph * graph, std::vector<edge> *obstructionEdges = NULL);

private:
  AcyclicTest();
  /**
   * @brief Singleton instance of this class.
   **/
  static AcyclicTest * instance;
  /**
   * @brief Stored results for graphs. When a graph is updated, its entry is removed from the hashmap.
   **/
  TLP_HASH_MAP<const Graph*, bool> resultsBuffer;

  //override of Observable::treatEvent to remove the cached result for a graph if it is modified.
  virtual void treatEvent(const Event&);
};
}

#endif //TULIP_ACYCLICITY_TEST_H
