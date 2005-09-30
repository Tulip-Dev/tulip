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

#if (__GNUC__ < 3)
#include <hash_set>
#else
#include <ext/hash_set>
#endif
#include "tulip/ObservableGraph.h"
#include <list>


class SuperGraph;
class SelectionProxy;

namespace tlp {
  struct SelfLoops {
    SelfLoops(node n1,node n2,edge e1, edge e2,edge e3,edge old):
      n1(n1),n2(n2),e1(e1),e2(e2),e3(e3),old(old){
    }
    node n1,n2;
    edge e1,e2,e3,old;  
  };
}

/** \addtogroup graph_test */ 
/*@{*/
/// Class for testing if the graph is acyclic
class TLP_SCOPE AcyclicTest : public GraphObserver {
public:
  static bool isAcyclic(SuperGraph *graph);
  static void makeAcyclic(SuperGraph* graph, std::set<edge> &reversed, std::list<tlp::SelfLoops> &selfLoops);

private:
  bool compute(SuperGraph *);
  bool acyclicTest(SuperGraph *,node ,SelectionProxy *,SelectionProxy *);
  void addEdge(SuperGraph *,const edge);
  void delEdge(SuperGraph *,const edge);
  void reverseEdge(SuperGraph *,const edge);
  void destroy(SuperGraph *);
  AcyclicTest();
  static AcyclicTest * instance;
  stdext::hash_map<unsigned int,bool> resultsBuffer;
};
/*@}*/
#endif
