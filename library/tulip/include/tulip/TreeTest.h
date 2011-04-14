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

  /**
   *  Returns true if the graph is a rooted tree (i.e. a graph with one node designated as the root),
   *  false otherwise.
   */
  static bool isTree(Graph *graph);

  /**
   *  Returns true if the graph is a topological tree
   *  (i.e. if the graph was undirected, there would be no cycle),
   *  false otherwise.
   */
  static bool isFreeTree(Graph *graph);

  /**
   * Turns a free tree into a rooted tree.
   */
  static void makeRootedTree(Graph *freeTree, node root);

  /**
   * Synonymous of the makeRootedTree method.
   */
  static void makeDirectedTree(Graph *freeTree, node root) {
    makeRootedTree(freeTree, root);
  };

  /**
   * Computes a rooted tree from the graph.
   * The algorithm is the following :
   * - if the graph is a rooted tree, returns the graph
   * - if the graph is a free tree, returns a rooted clone subgraph
   * - if the graph is connected, makes a clone subgraph
   *   and returns a rooted spanning tree of that clone
   * - if the graph is not connected, makes a clone subgraph,
   *   computes a tree for each of its connected components,
   *   adds a simple source and returns the clone.
   */  
  static Graph *computeTree(Graph* graph, PluginProgress *pluginProgress = 0);
  
  /**
   * Cleans the graph from a tree previously computed
   * with the computeTree method.
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
