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
class TLP_SCOPE PlanarityTest : private GraphObserver {
public:
  /*
    The set of edges of the graph is modified during the execution of
    the methods below. If you call one of them while iterating
    on the edges of the graph, be careful to use a StableIterator
    to avoid any possible invalidation of the iterator.
  */

  /**
   * Returns true if the graph is planar (i.e. the graph can be drawn in such a way that no edges cross each other),
   * false otherwise.
   */
  static bool isPlanar(Graph *graph);

  /**
   * Returns true if the current embedding of the graph is planar, false otherwise.
   */
  static bool isPlanarEmbedding(Graph *graph);

  /**
   * Changes the order of edges around the nodes in order to make the
   * embedding planar (the graph must be planar for that).
   * Returns true if the graph is planar, false otherwise.
   */
  static bool planarEmbedding(Graph *graph);

  /**
   * Returns a list of edges that prevents to make the graph planar
   * (ie. part of the minor of K3,3 or K5).
   */
  static std::list<edge> getObstructionsEdges(Graph *graph);
  
private:
  // override some GraphObserver methods
  void addEdge(Graph *,const edge);
  void delEdge(Graph *,const edge);
  void addNode(Graph *,const node);
  void delNode(Graph *,const node);
  void destroy(Graph *);
  bool compute(Graph *graph);
  PlanarityTest() {}
  static PlanarityTest * instance;
  TLP_HASH_MAP<unsigned long, bool> resultsBuffer;
};
/*@}*/

}
#endif
