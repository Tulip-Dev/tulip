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

%ModuleHeaderCode
#include <tulip/GraphTools.h>
%End

namespace tlp {

    std::vector<tlp::node> bfs(tlp::Graph *graph, tlp::node root = tlp::node());
%Docstring
tlp.bfs(graph[, root])

.. versionadded:: 4.2

Performs a breadth-first search on a graph and returns a list
containing the nodes of the graph in the order they have been visited.

:param graph: The graph to traverse with a BFS.
:type graph: :class:`tlp.Graph`
:param root: The node from whom to start the BFS. If not provided, the root node will be assigned to a source node in the graph (node with input degree equals to 0). If there is no source node in the graph, a random node will be picked.
:type root: :class:`tlp.node`
:rtype: list of :class:`tlp.node`
:throws: an exception if the provided root node does not belong to the graph.
%End

%MethodCode
    if (!a1->isValid() || a0->isElement(*a1)) {
        sipRes = new std::vector<tlp::node>(tlp::bfs(a0, *a1));
    } else {
        sipIsErr = throwInvalidNodeException(a0, *a1);
    }
%End

//========================================================================================

    std::vector<tlp::node> dfs(tlp::Graph *graph, tlp::node root = tlp::node());
%Docstring
tlp.dfs(graph[, root])

.. versionadded:: 4.2

Performs a depth-first search on a graph and returns a list
containing the nodes of the graph in the order they have been visited.

:param graph: The graph to traverse with a DFS.
:type graph: :class:`tlp.Graph`
:param root: The node from whom to start the DFS. If not provided, the root node will be assigned to a source node in the graph (node with input degree equals to 0). If there is no source node in the graph, a random node will be picked.
:type root: :class:`tlp.node`
:rtype: list of :class:`tlp.node`
:throws: an exception if the provided root node does not belong to the graph.
%End

%MethodCode
    if (!a1->isValid() || a0->isElement(*a1)) {
        sipRes = new std::vector<tlp::node>(tlp::dfs(a0, *a1));
    } else {
        sipIsErr = throwInvalidNodeException(a0, *a1);
    }
%End

};
