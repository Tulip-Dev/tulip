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

namespace tlp {
class Graph;

/** \addtogroup graph_test */ 
/*@{*/
/// class for testing if the sg is biconnected
class TLP_SCOPE BiconnectedTest : public GraphObserver {
public:
  /**
   * Return true if the sg is biconnected (ie. one must remove at least two nodes in order
   * to disconnect the sg) else false.
   */
  static bool isBiconnected(Graph *sg);
  /**
   * If the sg is not biconnected, add edges in order to make the sg
   * biconnected. The new edges are added in addedEdges.
   */
  static void makeBiconnected(Graph *sg, std::vector<edge>& addedEdges);

private:
  void connect(Graph *, std::vector<edge>& addedEdges);
  bool compute(Graph *);
  void addEdge(Graph *,const edge);
  void delEdge(Graph *,const edge);
  void reverseEdge(Graph *,const edge);
  void addNode(Graph *,const node);
  void delNode(Graph *,const node);
  void destroy(Graph *);
  BiconnectedTest(); //to ensure singleton
  static BiconnectedTest * instance;
  stdext::hash_map<unsigned int,bool> resultsBuffer;
};
}
/*@}*/
#endif
