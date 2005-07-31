//-*-c++-*-
/**
 Author: David Auber
 Email : auber@labri.fr
 Last modification : 01/09/2003
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/

#ifndef TULIP_CONNECTEDTEST_H
#define TULIP_CONNECTEDTEST_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#if (__GNUC__ < 3)
#include <hash_set>
#else
#include <ext/hash_set>
#endif
#include "tulip/ObservableGraph.h"

class SuperGraph;
class SelectionProxy;

/** \addtogroup graph_test */ 
/*@{*/
class TLP_SCOPE ConnectedTest : private GraphObserver {
public:
  /**
   * Return true if the graph is connected (ie. it exists an undirected path 
   * between each pair of nodes) else false.
   */
  static bool isConnected(SuperGraph *graph);
  /**
   * If the graph is not connected, add edges in order to make the graph
   * connected. The new edges are added in addedEdges.
   */
  static void makeConnected(SuperGraph *graph, std::vector<edge>& addedEdges);
  /**
   * Return the number of connected componnents in the graph;
   */
  static unsigned int numberOfConnectedComponnents(SuperGraph *graph);

private:
  void connect(SuperGraph *, std::vector<node>& toLink);
  bool compute(SuperGraph *);
  void addEdge(SuperGraph *,const edge);
  void delEdge(SuperGraph *,const edge);
  void reverseEdge(SuperGraph *,const edge);
  void addNode(SuperGraph *,const node);
  void delNode(SuperGraph *,const node);
  void destroy(SuperGraph *);
  ConnectedTest();
  static ConnectedTest * instance;
  stdext::hash_map<unsigned int,bool> resultsBuffer;
};
/*@}*/
#endif
