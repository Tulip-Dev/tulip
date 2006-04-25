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

class SuperGraph;
class SelectionProxy;

/** \addtogroup graph_test */ 
/*@{*/
/// Class for testing if the graph is a tree
class TLP_SCOPE TreeTest : private GraphObserver {
public:
  static bool isTree(SuperGraph *graph);

private:
  bool compute(SuperGraph *);
  void addEdge(SuperGraph *,const edge);
  void delEdge(SuperGraph *,const edge);
  void reverseEdge(SuperGraph *,const edge);
  void addNode(SuperGraph *,const node);
  void delNode(SuperGraph *,const node);
  void destroy(SuperGraph *);
  TreeTest();
  static TreeTest * instance;
  stdext::hash_map<unsigned long,bool> resultsBuffer;
};
/*@}*/
#endif
