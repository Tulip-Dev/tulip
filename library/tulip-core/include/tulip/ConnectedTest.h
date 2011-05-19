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

#ifndef TULIP_CONNECTEDTEST_H
#define TULIP_CONNECTEDTEST_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <set>
#include <vector>
#include <tulip/tuliphash.h>
#include <tulip/Observable.h>
#include <tulip/ObservableGraph.h>

namespace tlp {

class Graph;

/** \addtogroup graph_test */ 
/*@{*/
/// class for testing if the graph is connected
 class TLP_SCOPE ConnectedTest : private GraphObserver, public Observable {
public:
  /**
   * Returns true if the graph is connected (ie. it exists an undirected path 
   * between each pair of nodes), false otherwise.
   */
  static bool isConnected(const Graph* const graph);
  /**
   * If the graph is not connected, adds edges in order to make the graph
   * connected. The new edges are added in addedEdges.
   */
  static void makeConnected(Graph *graph, std::vector<edge>& addedEdges);
  /**
   * Returns the number of connected components in the graph.
   */
  static unsigned int numberOfConnectedComponents(const Graph* const graph);
  /**
   * Computes the sets of connected components and stores the result in the components vector.
   */
  static void computeConnectedComponents(Graph *graph, std::vector< std::set<node> >& components);
    
private:
  void connect(const Graph * const, std::vector<node>& toLink);
  bool compute(const Graph * const);
  // override GraphObserver methods
  void addEdge(Graph *,const edge);
  void delEdge(Graph *,const edge);
  void reverseEdge(Graph *,const edge);
  void addNode(Graph *,const node);
  void delNode(Graph *,const node);
  void destroy(Graph *);
  ConnectedTest();
  static ConnectedTest * instance;
  TLP_HASH_MAP<unsigned long,bool> resultsBuffer;
  // override Observable::treatEvent
  virtual void treatEvent(const Event&);
};

}
/*@}*/
#endif
