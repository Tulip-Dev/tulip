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
