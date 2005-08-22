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

#ifndef TULIP_BICONNEX_H
#define TULIP_BICONNEX_H

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

/** \addtogroup graph_test */ 
/*@{*/
/// class for testing if the graph is biconnected
class TLP_SCOPE BiconnectedTest : public GraphObserver {
public:
  /**
   * Return true if the graph is biconnected (ie. one must remove at least two nodes in order
   * to disconnect the graph) else false.
   */
  static bool isBiconnected(SuperGraph *graph);
  /**
   * If the graph is not biconnected, add edges in order to make the graph
   * biconnected. The new edges are added in addedEdges.
   */
  static void makeBiconnected(SuperGraph *graph, std::vector<edge>& addedEdges);

private:
  void connect(SuperGraph *, std::vector<edge>& addedEdges);
  bool compute(SuperGraph *);
  void addEdge(SuperGraph *,const edge);
  void delEdge(SuperGraph *,const edge);
  void reverseEdge(SuperGraph *,const edge);
  void addNode(SuperGraph *,const node);
  void delNode(SuperGraph *,const node);
  void destroy(SuperGraph *);
  BiconnectedTest(); //to ensure singleton
  static BiconnectedTest * instance;
  stdext::hash_map<unsigned int,bool> resultsBuffer;
};
/*@}*/
#endif
