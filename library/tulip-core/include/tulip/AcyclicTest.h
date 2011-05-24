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

#ifndef TULIP_TEST_H
#define TULIP_TEST_H

#include <vector>
#include <tulip/tuliphash.h>
#include <tulip/Observable.h>
#include <tulip/ObservableGraph.h>
#include <tulip/MutableContainer.h>


namespace tlp {
class Graph;
class BooleanProperty;

  class SelfLoops {
  public:
    SelfLoops(node n1,node n2,edge e1, edge e2,edge e3,edge old):
      n1(n1),n2(n2),e1(e1),e2(e2),e3(e3),old(old){
    }
    node n1,n2;
    edge e1,e2,e3,old;  
  };

/** \addtogroup graph_test */ 
/*@{*/
/// Class for testing if the graph is acyclic
  class TLP_SCOPE AcyclicTest : private GraphObserver, private Observable {
public:

  /**
   * Returns true if the graph is acyclic, false otherwise.
   * The result is cached (ie. the next call with the same graph is done in O(1) time)
   */
  static bool isAcyclic(const Graph *graph);

  /**
    * Makes the graph acyclic, by reversing edge direction (feedback arc set problem).
    * If there is self loops, a new node is added with two edges that points to it.
    *
    */
  static void makeAcyclic(Graph* graph, std::vector<edge> &reversed, std::vector<tlp::SelfLoops> &selfLoops);

  /**
    * Returns true if the graph is acyclic, false otherwise.
    * If the graph is not acyclic, uses obstructionEdges variable to store all edges
    * that create cycle.
    *
    */
  static bool acyclicTest(const Graph *, std::vector<edge> *obstructionEdges = 0);

private:

  //  override GraphObserver methods
  void addEdge(Graph *,const edge);
  void delEdge(Graph *,const edge);
  void reverseEdge(Graph *,const edge);
  void destroy(Graph *);
  AcyclicTest();
  static AcyclicTest * instance;
  TLP_HASH_MAP<unsigned long, bool> resultsBuffer;
  // override Observable::treatEvent
  virtual void treatEvent(const Event&);
};
}
/*@}*/
#endif
