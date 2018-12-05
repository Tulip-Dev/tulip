/*
 *
 * This file is part of Tulip (http://tulip.labri.fr)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
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
// | (https://sourceforge.net/projects/booggie/)                             |
// +-------------------------------------------------------------------------+

%ModuleHeaderCode
#include <tulip/GraphTools.h>
%End

namespace tlp {

void bfs(tlp::Graph *graph, std::vector<tlp::node> &nodes /Out/);
%Docstring
tlp.bfs(graph)

.. versionadded:: 5.0

Performs a breadth-first search on a graph and returns a list
containing the nodes of the graph in the order they have been visited.

:param graph: The graph to traverse with a BFS.
:type graph: :class:`tlp.Graph`
:rtype: list of :class:`tlp.node`
%End

 //========================================================================================

void bfs(tlp::Graph *graph, tlp::node root, std::vector<tlp::node> &nodes /Out/);
%Docstring
tlp.bfs(graph, root)

.. versionadded:: 4.2

Performs a breadth-first search on a graph and returns a list
containing the nodes of the graph in the order they have been visited.

:param graph: The graph to traverse with a BFS.
:type graph: :class:`tlp.Graph`
:param root: The node from whom to start the BFS.
:type root: :class:`tlp.node`
:rtype: list of :class:`tlp.node`
:throws: an exception if the provided root node does not belong to the graph.
%End

%MethodCode
  if (a0->isElement(*a1)) {
    tlp::bfs(a0, *a1, *a2);
  } else {
    sipIsErr = throwInvalidNodeException(a0, *a1);
  }
%End

//========================================================================================

  void dfs(tlp::Graph *graph, std::vector<tlp::node> &nodes /Out/);
%Docstring
tlp.dfs(graph)

.. versionadded:: 5.0

Performs a depth-first search on a graph and returns a list
containing the nodes of the graph in the order they have been visited.

:param graph: The graph to traverse with a DFS.
:type graph: :class:`tlp.Graph`
:rtype: list of :class:`tlp.node`
%End

//========================================================================================

  void dfs(tlp::Graph *graph, tlp::node root, std::vector<tlp::node> &nodes /Out/);
%Docstring
tlp.dfs(graph, root)

.. versionadded:: 4.2

Performs a depth-first search on a graph and returns a list
containing the nodes of the graph in the order they have been visited.

:param graph: The graph to traverse with a DFS.
:type graph: :class:`tlp.Graph`
:param root: The node from whom to start the DFS.
:type root: :class:`tlp.node`
:rtype: list of :class:`tlp.node`
:throws: an exception if the provided root node does not belong to the graph.
%End

%MethodCode
  if (a0->isElement(*a1)) {
    tlp::dfs(a0, *a1, *a2);
  } else {
    sipIsErr = throwInvalidNodeException(a0, *a1);
  }
%End

//========================================================================================

enum ShortestPathType { OnePath = 0, OneDirectedPath = 1 , OneReversedPath = 2,
			AllPaths = 3, AllDirectedPaths = 4, AllReversedPaths = 5 };

bool selectShortestPaths(const tlp::Graph *graph, tlp::node src, tlp::node tgt, tlp::ShortestPathType pathType, const tlp::DoubleProperty *weights, tlp::BooleanProperty* selection);
%Docstring
tlp.selectShortestPaths(graph, src, tgt, pathType, weights, selection)

.. versionadded::5.2

select the shortest paths between two nodes. Returns false if no path exist between the src and tgt nodes; true if not.

:param graph: The graph to compute on.
:type graph: :class:`tlp.Graph`
:param src: The source node of the paths
:type node: :class:`tlp.node`
:param tgt: The target node of the paths
:type node: :class:`tlp.node`
:param pathType: The type of path to consider
:type pathType: tlp.OnePath, tlp.OneDirectedPath, tlp.OneReversedPath, tlp.AllPaths, tlp.AllDirectedPaths, tlp.AllReversedPaths
:param weights: A graph property giving the edges weight if weighted paths have to be considered. Can be set to null to select unweighted paths.
:type weights:class:`tlp.DoubleProperty`
:param selection: The graph property to consider as selection.
:type selection:class:`tlp.BooleanProperty`
:rtype: boolean
:throws: an exception if one of the source or target nodes does not belong to the graph.
%End

%MethodCode
  if (a0->isElement(*a1)) {
    if (a0->isElement(*a2)) {
     sipRes = tlp::selectShortestPaths(a0, *a1, *a2, a3, a4, a5);
    } else {
      sipIsErr = throwInvalidNodeException(a0, *a2);
    }
  } else {
      sipIsErr = throwInvalidNodeException(a0, *a1);
  }
%End
};
