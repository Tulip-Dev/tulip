//-*-c++-*-
/*
 Author: David Auber
 Email : auber@labri.fr
 Last modification : 01/09/2003
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/

#ifndef TULIP_TEST_H
#define TULIP_TEST_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <vector>
#include "tulip/tuliphash.h"
#include "tulip/ObservableGraph.h"
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
class TLP_SCOPE AcyclicTest : public GraphObserver {
public:
  /** returns true if the graph is acyclic, false if not.
   *  The result is cached (ie. the next call with the same graph is done in O(1) time)
   */
  static bool isAcyclic(const Graph *graph);
  static void makeAcyclic(Graph* graph, std::vector<edge> &reversed, std::vector<tlp::SelfLoops> &selfLoops);
  static bool acyclicTest(const Graph *, std::vector<edge> *obstructionEdges = 0);

private:

  static bool dfsAcyclicTest(const Graph *graph, const node n, 
			     MutableContainer<bool> &visited, 
			     MutableContainer<bool> &finished,
			     std::vector<edge> *obstructionEdges = 0);
  void addEdge(Graph *,const edge);
  void delEdge(Graph *,const edge);
  void reverseEdge(Graph *,const edge);
  void destroy(Graph *);
  AcyclicTest();
  static AcyclicTest * instance;
  TLP_HASH_MAP<unsigned long, bool> resultsBuffer;
};
}
/*@}*/
#endif
