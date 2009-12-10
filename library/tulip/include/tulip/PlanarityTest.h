//-*-c++-*-
/*
 Author: Sophie BARDET & Alexandre BURNETT & David Auber
 Email : sophie.bardet@gmail.com, aburnett397@gmail.com
 Last modification : 19/06/2005
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/

#ifndef TULIP_PLANARITYTESTX_H
#define TULIP_PLANARITYTESTX_H

#include <list>
#include <tulip/tulipconf.h>
#include <tulip/tuliphash.h>
#include <tulip/ObservableGraph.h>
#include <tulip/Edge.h> 

namespace tlp {

class Graph;

/** \addtogroup graph_test */ 
/*@{*/
/// class for testing the planarity of a graph
class TLP_SCOPE PlanarityTest : public GraphObserver {
public:
  /*
    The set of edges of the graph is modified during the execution of
    the methods below. If you call one of them while iterating
    on the edges of the graph, be careful to use a StableIterator
    to avoid any possible invalidation of the iterator.
  */
  static bool isPlanar(Graph *graph);
  static bool planarEmbedding(Graph *graph);
  static std::list<edge> getObstructionsEdges(Graph *graph);
  
private:
  void addEdge(Graph *,const edge);
  void delEdge(Graph *,const edge);
  void reverseEdge(Graph *,const edge);
  void addNode(Graph *,const node);
  void delNode(Graph *,const node);
  void destroy(Graph *);
  bool compute(Graph *graph);
  PlanarityTest() : GraphObserver(false) {}
  static PlanarityTest * instance;
  TLP_HASH_MAP<unsigned long, bool> resultsBuffer;
};
/*@}*/

}
#endif
