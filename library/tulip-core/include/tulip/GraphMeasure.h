/*
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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
#include <tulip/StaticProperty.h>
#include <tulip/GraphTools.h>

namespace tlp {

class Graph;
class PluginProgress;
/**
 * returns the average path length of a graph, that is the sum
 * of the shortest distances for all pair of distinct nodes in that graph
 * divided by the number of those pairs. For a pair of non connected nodes,
 * the shorted distance is set to 0.
 * see http://en.wikipedia.org/wiki/Average_path_length for more details
 */
TLP_SCOPE double averagePathLength(const Graph *g);
/*
 * return the clustering coefficient of a graph
 * as the average of the local clustering coefficients
 * (see clusteringCoefficient function) of all the nodes.
 * see http://en.wikipedia.org/wiki/Clustering_coefficient for more details.
 */
TLP_SCOPE double averageClusteringCoefficient(const Graph *);
/*
 * assign to each node its local clustering coefficient
 * that is the proportion of edges between the nodes within its neighbourhood
 * divided by the number of edges that could possibly exist between them.
 * This quantifies how close the node and its neighbors are close to a clique.
 * see http://en.wikipedia.org/wiki/Clustering_coefficient for more details or
 * the paper: D. J. Watts and S. H. Strogatz.
 * Collective dynamics of "small-world" networks.
 * Nature 393, 440 (1998) doi: https://doi.org/10.1038/30918\">10.1038/30918.
 */
TLP_SCOPE void clusteringCoefficient(const Graph *g, tlp::NodeStaticProperty<double> &result,
                                     _DEPRECATED unsigned int maxDepth = 1);
/*
 * assign to each node of a graph its (in/ou/inout) degree.
 * The weighted degree of a node is the sum of weights of
 * all its in/out/inout edges."
 * If no metric is specified, using a uniform metric value of 1 for all edges
 * it assigns the usual degree of nodes (number of neighbors).",
 * If norm is true, the measure is normalized in the following way:
 * unweighted case => m(n) = deg(n) / (#V - 1)
 * weighted case => m(n) = deg_w(n) / [(sum(e_w)/#E)(#V - 1)]
 */
TLP_SCOPE void degree(const Graph *graph, tlp::NodeStaticProperty<double> &deg,
                      EDGE_TYPE direction = UNDIRECTED, NumericProperty *weights = nullptr,
                      bool norm = false);
/*
 * assign to each node of a Directed Acyclic Graph a level such that
 * if the edge e(u,v) exists level(u) < level(v) the algorithm ensure that
 * the number of level used is minimal.
 *
 * Warning: the graph must be acyclic (no self loops).
 */
TLP_SCOPE void dagLevel(const Graph *graph, tlp::NodeStaticProperty<unsigned int> &level);
// return the maximum value of the degree of the graph's nodes
TLP_SCOPE unsigned int maxDegree(const Graph *);
// return the minimum value of the degree of the graph's nodes
TLP_SCOPE unsigned int minDegree(const Graph *);
/*
 * compute the maximum distance from the n (graph->nodes[nPos]) to all the other nodes of graph
 * and store it into distance, (stored value is UINT_MAX for non connected nodes),
 * if direction is set to UNDIRECTED use undirected graph, DIRECTED use directed graph
 * and INV_DIRECTED use reverse directed graph (ie. all edges are reversed)
 * all the edge's weight is set to 1. (it uses a bfs thus the complexity is o(m), m = |E|).
 */
TLP_SCOPE unsigned int maxDistance(const Graph *graph, const unsigned int nPos,
                                   tlp::NodeStaticProperty<unsigned int> &distance,
                                   EDGE_TYPE direction = UNDIRECTED);

/*
 * compute the maximum distance from the n (graph->nodes[nPos]) to all the other nodes of graph
 * and store it into distance, (stored value is DBL_MAX for non connected nodes),
 * if direction is set to UNDIRECTED use undirected graph, DIRECTED use directed graph
 * and INV_DIRECTED use reverse directed graph (ie. all edges are reversed)
 * Edge weights can be given, Dijkstra's algorithm is then used
 * (the complexity is then o((m + n)log n)) otherwise
 * all the edge's weight is set to 1. (it uses a bfs thus the complexity is o(m), m = |E|).
 */
TLP_SCOPE double maxDistance(const Graph *graph, const unsigned int nPos,
                             tlp::NodeStaticProperty<double> &distance,
                             const NumericProperty *const weights,
                             EDGE_TYPE direction = UNDIRECTED);
} // namespace tlp
#endif
///@endcond
