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

#ifndef TULIP_TREETEST_H
#define TULIP_TREETEST_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "tulip/tuliphash.h"
#include "tulip/ObservableGraph.h"
#include "tulip/MutableContainer.h"
#include "tulip/PluginProgress.h"

namespace tlp {

class Graph;

/** \addtogroup graph_test */ 
/*@{*/
/// Class for testing if the graph is a tree
class TLP_SCOPE TreeTest : private GraphObserver {
public:
  // check if the graph is a rooted tree
  static bool isTree(Graph *graph);
  // check if the graph is a free tree
  static bool isFreeTree(Graph *graph);
  // turns a free tree into a rooted tree
  static void makeRootedTree(Graph *freeTree, node root);
  // synonymous of the makeRootedTree
  static void makeDirectedTree(Graph *freeTree, node root) {
    makeRootedTree(freeTree, root);
  };

  /**
   * Compute a rooted tree from the graph.
   * The algorithm is the following
   * - if the graph is a rooted tree, return the graph
   * - if the graph is a free tree, return a rooted clone subgraph
   * - if the graph is connected, make a clone subgraph
   *   return a rooted spanning tree of that clone
   * - if the graph is not connected, make a clone subgraph,
   *   compute a tree for each of its connected components,
   *   add a simple source and return the clone.
   */  
  static Graph *computeTree(Graph* graph, PluginProgress *pluginProgress = 0);
  
  /**
   * Clean the graph from a tree previously computed
   * with the computeTree function
   */
  static void cleanComputedTree(Graph *graph, Graph *tree);

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
  bool isFreeTree (Graph *graph, node curRoot);
  TLP_HASH_MAP<unsigned long,bool> resultsBuffer;
};
/*@}*/

}
#endif
