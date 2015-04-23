/*
 *
 * This file is part of Tulip (www.tulip-software.org)
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
///@cond DOXYGEN_HIDDEN

#ifndef _TLPGRAPHMEASEURE_H
#define _TLPGRAPHMEASEURE_H

#include <tulip/Node.h>
#include <tulip/MutableContainer.h>
#include <tulip/DoubleProperty.h>

namespace tlp {

class Graph;
class PluginProgress;
enum EDGE_TYPE {DIRECTED = 0, INV_DIRECTED = 1, UNDIRECTED = 2};
/**
 * returns the average path lengh of a graph, that is the sum
 * of the shortest distances for all pair of distinct nodes in that graph
 * divided by the number of those pairs. For a pair of non connected nodes,
 * the shorted distance is set to 0.
 * see http://en.wikipedia.org/wiki/Average_path_length for more details
 */
TLP_SCOPE double averagePathLength(const Graph* g, PluginProgress* = NULL);
// obsolete version for compatibility purpose
inline bool averagePathLength(Graph *g, double& result, PluginProgress *pp = NULL) {
  result = averagePathLength(g, pp);
  return true;
}
/*
 * returns the clustering coefficient of a graph
 * as the average of the local clustering coefficients
 * (see clusteringCoefficient function) of all the nodes.
 * see http://en.wikipedia.org/wiki/Clustering_coefficient for more details.
 */
TLP_SCOPE double averageClusteringCoefficient(const Graph *, PluginProgress * = NULL);
// obsolete version for compatibility purpose
inline bool averageCluster(Graph* g, double& result, PluginProgress* pp = NULL) {
  result = averageClusteringCoefficient(g, pp);
  return true;
}
/*
 * assigns to each node its local clustering coefficient
 * that is the proportion of edges between the nodes within its neighbourhood
 * divided by the number of edges that could possibly exist between them.
 * This quantifies how close its neighbors are to being a clique.
 * see http://en.wikipedia.org/wiki/Clustering_coefficient for more details
 */
TLP_SCOPE void clusteringCoefficient(const Graph *g, MutableContainer<double> &result, unsigned int maxDepth = 1, PluginProgress* = NULL);
/*
 * assigns to each node of a Directed Acyclic Graph a level such that
 * if the edge e(u,v) exists level(u) < level(v) the algorithm ensure that
 * the number of level used is minimal.
 *
 * Warning : the graph must be acyclic (no self loops).
 */
TLP_SCOPE void dagLevel(const Graph *graph, MutableContainer<unsigned int> &level, PluginProgress* = NULL);
// returns the maximum value of the degree of the graph's nodes
TLP_SCOPE unsigned int maxDegree(const Graph *);

// returns the minimum value of the degree of the graph's nodes
TLP_SCOPE unsigned int minDegree(const Graph *);
/*
 * computes the maximum distance from n to all the other nodes of graph
 * and store it into distance, (stored value is UINT_MAX for non connected nodes),
 * if direction is set to UNDIRECTED use undirected graph, DIRECTED use directed graph
 * and INV_DIRECTED use reverse directed graph (ie. all edges are reversed)
 * all the edge's weight is set to 1. (it uses a bfs thus the complexity is o(m), m = |E|).
 */
TLP_SCOPE unsigned int maxDistance(const Graph *graph, const node n, MutableContainer<unsigned int> &distance, EDGE_TYPE direction = UNDIRECTED);
/*
 * adds to a result set, all the nodes, according to direction,
 * at distance less or equal to maxDistance of startNode.
 * If direction is set to UNDIRECTED use undirected graph, DIRECTED use directed graph
 * and INV_DIRECTED use reverse directed graph (ie. all edges are reversed)
 */
TLP_SCOPE void reachableNodes(const Graph *graph, const node startNode, std::set<node> &result, unsigned int maxDistance, EDGE_TYPE direction = UNDIRECTED);
}
#endif
///@endcond
