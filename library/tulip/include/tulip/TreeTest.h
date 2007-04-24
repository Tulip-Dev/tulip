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

#ifndef TULIP_TREETEST_H
#define TULIP_TREETEST_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#if (__GNUC__ < 3)
#include <hash_set>
#else
#include <ext/hash_set>
#endif
#include "tulip/ObservableGraph.h"
#include "tulip/MutableContainer.h"

namespace tlp {

class Graph;

/** \addtogroup graph_test */ 
/*@{*/
/// Class for testing if the graph is a tree
class TLP_SCOPE TreeTest : private GraphObserver {
public:
  static bool isTree(Graph *graph);
  static bool isFreeTree(Graph *graph);
  static void makeDirectedTree(Graph *graph, node root);
private:
  bool compute(Graph *);
  void addEdge(Graph *,const edge);
  void delEdge(Graph *,const edge);
  void reverseEdge(Graph *,const edge);
  void addNode(Graph *,const node);
  void delNode(Graph *,const node);
  void destroy(Graph *);
  TreeTest();
  static TreeTest * instance;
  bool isFreeTree (Graph *graph, node curRoot, node cameFrom,
		   MutableContainer<bool> &visited);
  void makeDirectedTree (Graph *graph, node curRoot, node cameFrom);
  stdext::hash_map<unsigned long,bool> resultsBuffer;
};
/*@}*/

}
#endif
