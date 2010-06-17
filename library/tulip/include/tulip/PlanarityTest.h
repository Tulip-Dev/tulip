/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
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
