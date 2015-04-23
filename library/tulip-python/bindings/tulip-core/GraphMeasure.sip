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
#include <tulip/GraphMeasure.h>
%End

namespace tlp {

  	enum EDGE_TYPE {DIRECTED = 0, INV_DIRECTED = 1, UNDIRECTED = 2};

	double averagePathLength(const tlp::Graph* g, tlp::PluginProgress *pluginProgress= 0);
%Docstring
tlp.averagePathLength(graph)

Returns the average path length of a graph, that is the sum
of the shortest distances for all pair of distinct nodes in that graph
divided by the number of those pairs. For a pair of non connected nodes,
the shorted distance is set to 0.

:param graph: the graph on which to compute the average path length
:type graph: :class:`tlp.Graph`
:rtype: float
%End
	
  	double averageClusteringCoefficient(const tlp::Graph *graph, tlp::PluginProgress *pluginProgress= 0);
%Docstring
tlp.averageClusteringCoefficient(graph)
  	
Returns the clustering coefficient of a graph
as the average of the local clustering coefficients
(see :func:`tlp.clusteringCoefficient`) of all the nodes.

:param graph: the graph on wich to compute the average clustering coefficient
:type graph: :class:`tlp.Graph`
:rtype: float
%End

  
  	void clusteringCoefficient(const tlp::Graph *g, tlp::DoubleProperty *result, unsigned int maxDepth = 1, tlp::PluginProgress* = NULL);
%Docstring 
tlp.clusteringCoefficient(graph, result[, maxDepth = 1])

Assigns to each node its local clustering coefficient
that is the proportion of edges between the nodes within its neighbourhood
divided by the number of edges that could possibly exist between them.
This quantifies how close its neighbors are to being a clique.

:param graph: the graph on which to compute the clustering coefficient for each node
:type graph: :class:`tlp.Graph`
:param result: a graph property in which the results will be stored
:type result: :class:`tlp.DoubleProperty`
:param maxDepth: the maximum distance between each node and its neighbours.
:type maxDepth: integer  	
%End 
  	
%MethodCode
	tlp::MutableContainer<double> result;
	tlp::clusteringCoefficient(a0, result, a2, a3);
	tlp::node n;
	forEach(n, a0->getNodes()) {
		a1->setNodeValue(n, result.get(n));
	}
%End  	
 
 	

  void dagLevel(const tlp::Graph *graph, tlp::IntegerProperty* result, tlp::PluginProgress* = 0);
%Docstring
tlp.dagLevel(graph, result)

Assigns to each node of a Directed Acyclic Graph a level such that
if the edge e(u,v) exists level(u) < level(v). 
The algorithm ensures that the number of level used is minimal.

:param graph: the graph on which to compute the dag level
:type graph: :class:`tlp.Graph`
:param result: a graph property in which the results will be stored
:type result: :class:`tlp.IntegerProperty`

.. warning:: The graph must be acyclic (no self loops).

%End  
  
%MethodCode
	tlp::MutableContainer<unsigned int> result;
	tlp::dagLevel(a0, result, a2);
	tlp::node n;
	forEach(n, a0->getNodes()) {
		a1->setNodeValue(n, static_cast<int>(result.get(n)));
	}
%End  
  
	unsigned int maxDegree(const tlp::Graph *graph);
%Docstring
tlp.maxDegree(graph)

Returns the maximum degree of the graph's nodes.

:param graph: the graph on which to compute the maximum degree
:type graph: :class:`tlp.Graph`
:rtype: integer
%End  
  
	unsigned int minDegree(const tlp::Graph *graph);
%Docstring
tlp.minDegree(graph)

Returns the minimum degree of the graph's nodes.

:param graph: the graph on which to compute the minimum degree
:type graph: :class:`tlp.Graph`
:rtype: integer
%End
  
  	unsigned int maxDistance(const tlp::Graph *graph, const tlp::node node, tlp::IntegerProperty *result, tlp::EDGE_TYPE direction = tlp::UNDIRECTED);
%Docstring
tlp.maxDistance(graph, node, result, direction)
  
Computes the distances from a node to all the other nodes of a graph and return the maximum one.

:param graph: the graph on which to compute the maximum distance
:type graph: :class:`tlp.Graph`
:param node: an existing graph node
:type node: :class:`tlp.node`
:param result: a graph property in which the results will be stored
:type result: :class:`tlp.IntegerProperty`
:param direction: specify if the graph must be directed or not
:type direction: tlp.DIRECTED, tlp.INV_DIRECTED, tlp.UNDIRECTED
:rtype: integer
:throws: an exception if the node does not belong to the graph
%End  	
  	
%MethodCode
	if (a0->isElement(*a1)) {
		tlp::MutableContainer<unsigned int> result;
		tlp::maxDistance(a0, *a1, result, a3);
		tlp::node n;
		forEach(n, a0->getNodes()) {
			a2->setNodeValue(n, static_cast<int>(result.get(n)));
		}
	} else {
		sipIsErr = throwInvalidNodeException(a0, *a1);
	}
%End  	

  	void reachableNodes(const tlp::Graph *graph, const tlp::node startNode, std::set<tlp::node> &result /Out/, unsigned int maxDistance, tlp::EDGE_TYPE direction = tlp::UNDIRECTED);
%Docstring
tlp.reachableNodes(graph, startNode, maxDistance, direction)

Returns the nodes that can be reached from a node, according to a direction,
at distance less or equal to a maximum one.

:param graph: the graph to work on
:type graph: :class:`tlp.Graph`
:param startNode: an existing graph node
:type startNode: :class:`tlp.node`
:param maxDistance: the maximum distance to reach nodes
:type maxDistance: integer
:param direction: specify if the graph must be directed or not
:type direction: tlp.DIRECTED, tlp.INV_DIRECTED, tlp.UNDIRECTED  
:rtype: set of :class:`tlp.node`
:throws: an exception if the node does not belong to the graph
%End 

%MethodCode
	if (a0->isElement(*a1)) {
		tlp::reachableNodes(a0, *a1, *a2, a3);
	} else {
		sipIsErr = throwInvalidNodeException(a0, *a1);
	}
%End  	 	
};
