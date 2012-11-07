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

// +-------------------------------------------------------------------------+
// | Tulip Python Bindings                                                   |
// | inspired from bindings by the Booggie project development team          |
// | (http://booggie.org/)                                                   |
// +-------------------------------------------------------------------------+

namespace tlp {

class TreeTest {
%TypeHeaderCode
#include <tulip/TreeTest.h>
%End
public:
  
  static bool isTree(tlp::Graph *graph);
%Docstring
tlp.TreeTest.isTree(graph)

Returns :const:`True` if the graph is a rooted tree (i.e. a graph with one node designated as the root),
:const:`False` otherwise.

:param graph: the graph on which to perform the tree test
:type graph: :class:`tlp.Graph`
:rtype: boolean
%End  

//====================================================================================
  
  static bool isFreeTree(tlp::Graph *graph);
%Docstring
tlp.TreeTest.isFreeTree(graph)

Returns :const:`True` if the graph is a topological tree
(i.e. if the graph was undirected, there would be no cycle),
:const:`False` otherwise.

:param graph: the graph on which to perform the free tree test
:type graph: :class:`tlp.Graph`
:rtype: boolean
%End  
  
  
//====================================================================================  
  
  static void makeRootedTree(tlp::Graph *freeTree, tlp::node root);
%Docstring
tlp.TreeTest.makeRootedTree(graph, root)

Turns a free tree into a rooted tree.

:param graph: the tree to make rooted
:type graph: :class:`tlp.Graph`
:param root: the node to designate as the root of the tree
:type root: :class:`tlp.node`
%End  
  
  
//====================================================================================  
  
  static tlp::Graph *computeTree(tlp::Graph* graph);
%Docstring
tlp.TreeTest.computeTree(graph)

Computes a rooted tree from the graph. The algorithm is the following :

* if the graph is a rooted tree, returns the graph
* if the graph is a free tree, returns a rooted clone subgraph
* if the graph is connected, makes a clone subgraph and returns a rooted spanning tree of that clone
* if the graph is not connected, makes a clone subgraph, computes a tree for each of its connected components, adds a simple source and returns the clone.

:param graph: the graph on which to compute a rooted tree
:type graph: :class:`tlp.Graph`
:rtype: :class:`tlp.Graph`
%End  
  
//====================================================================================  
  
  static void cleanComputedTree(tlp::Graph *graph, tlp::Graph *tree);
%Docstring
tlp.TreeTest.cleanComputedTree(graph, tree)

Cleans the graph from a tree previously computed with the :meth:`tlp.TreeTest.computeTree` method.

:param graph: the graph to clean from a tree
:type graph: :class:`tlp.Graph`
:param tree: the tree to remove from the graph 
:type tree: :class:`tlp.Graph`
%End  
  
private :

	TreeTest();  

};

};

