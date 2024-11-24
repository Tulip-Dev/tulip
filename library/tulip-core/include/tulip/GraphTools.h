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

#ifndef _TLPGRAPHTOOLS_H
#define _TLPGRAPHTOOLS_H

#include <map>
#include <list>
#include <set>
#include <stack>
#include <vector>
#include <tulip/tuliphash.h>
#include <tulip/tulipconf.h>
#include <tulip/Node.h>
#include <tulip/Edge.h>
#include <tulip/MutableContainer.h>
#include <tulip/StaticProperty.h>
#include <tulip/Iterator.h>

namespace tlp {
class BooleanProperty;
class DoubleProperty;
class Graph;
class IntegerProperty;
class NumericProperty;
class PlanarConMap;
class PluginProgress;

enum EDGE_TYPE { UNDIRECTED = 0, INV_DIRECTED = 1, DIRECTED = 2 };
#define IN_EDGE INV_DIRECTED
#define OUT_EDGE DIRECTED
#define INOUT_EDGE UNDIRECTED

typedef Iterator<node> *(*NodesIteratorFn)(const tlp::Graph *, const tlp::node);
typedef Iterator<edge> *(*EdgesIteratorFn)(const tlp::Graph *, const tlp::node);

/**
 * return a function to get an Iterator on the adjacent nodes of a graph node
 * according the given direction
 */
TLP_SCOPE NodesIteratorFn getNodesIterator(EDGE_TYPE direction);

/**
 * return a function to get an Iterator on the adjacent edges of a graph node
 * according the given direction
 */
TLP_SCOPE EdgesIteratorFn getEdgesIterator(EDGE_TYPE direction);

/**
 *  This ordering was first introduced by C. Gutwenger and P. Mutzel in \n
 *  "Grid embeddings of biconnected planar graphs", \n
 *  "Extended Abstract, Max-Planck-Institut für Informatik," \n
 *  "Saarbrücken, Germany, 1997" \n
 *  Let n be the number of nodes, the original algorithm complexity is in O(n).\n
 *  But the implementation of the canonical ordering has not been made in O(n).\n
 */
TLP_SCOPE std::vector<std::vector<node>>
computeCanonicalOrdering(PlanarConMap *, std::vector<edge> *dummyEdges = nullptr,
                         PluginProgress *pluginProgress = nullptr);
/**
 * Find all the graph centers, that version does not manage edge weight.
 * complexity O(n * m). Only works on connected graphs.
 */
TLP_SCOPE std::vector<node> computeGraphCenters(Graph *graph);
/**
 * return a node that can be considered as the graph center.
 * It is an heuristic, thus it is not absolutely sure that this
 * node is a graph center. Only works on connected graphs.
 */
TLP_SCOPE node graphCenterHeuristic(Graph *graph, PluginProgress *pluginProgress = nullptr);
/**
 * return a new node connected to all previously
 * existing nodes which had a null indegree
 */
TLP_SCOPE node makeSimpleSource(Graph *graph);

TLP_SCOPE void makeProperDag(Graph *graph, std::list<node> &addedNodes,
                             tlp_hash_map<edge, edge> &replacedEdges,
                             IntegerProperty *edgeLength = nullptr);

/**
 * Select a spanning forest of the graph,
 * i.e for all graph elements (nodes or edges) belonging to that forest
 * the selectionProperty associated value is true. The value is false
 * for the other elements
 */
TLP_SCOPE void selectSpanningForest(Graph *graph, BooleanProperty *selectionProperty,
                                    PluginProgress *pluginProgress = nullptr);

/**
 * Select a spanning tree of a graph assuming it is connected;
 * i.e for all graph elements (nodes or edges) belonging to that tree
 * the selectionProperty associated value is true. The value is false
 * for the other elements
 */
TLP_SCOPE void selectSpanningTree(Graph *graph, BooleanProperty *selection,
                                  PluginProgress *pluginProgress = nullptr);

/**
 * Select the minimum spanning tree (Kruskal algorithm) of a weighted graph,
 * i.e for all graph elements (nodes or edges) belonging to that tree
 * the selectionProperty associated value is true. The value is false
 * for the other elements
 */
TLP_SCOPE void selectMinimumSpanningTree(Graph *graph, BooleanProperty *selectionProperty,
                                         NumericProperty *weight = nullptr,
                                         PluginProgress *pluginProgress = nullptr);

/**
 * @brief Performs a breadth-first search on a graph.
 * @param graph The graph to traverse with a BFS.
 * @param nodes a vector to fill with the nodes of the graph in the order they have been visited by
 * the BFS.
 * @param root The node from whom to start the BFS. If not provided, the root
 * node will be assigned to a source node in the graph (node with input degree equals to 0).
 * If there is no source node in the graph, a random node will be picked.
 */
TLP_SCOPE void bfs(const Graph *graph, node root, std::vector<node> &nodes);

/**
 * @brief Performs a cumulative breadth-first search on every node of a graph.
 * @param graph The graph to traverse with a BFS.
 * @param nodes a vector to fill with the nodes of the graph in the order they have been visited by
 * the BFS.
 */
TLP_SCOPE void bfs(const Graph *graph, std::vector<node> &nodes);

/**
 * @brief Performs a depth-first search on a graph.
 * @param graph The graph to traverse with a DFS.
 * @param nodes a vector to fill with the nodes of the graph in the order they have been visited by
 * the DFS.
 * @param root The node from whom to start the DFS. If not provided, the root
 * node will be assigned to a source node in the graph (node with input degree equals to 0).
 * If there is no source node in the graph, a random node will be picked.
 * @return A vector containing the nodes of the graph in the order they have been visited by the
 * DFS.
 */
TLP_SCOPE void dfs(const Graph *graph, node root, std::vector<node> &nodes);

/**
 * @brief Performs a cumulative depth-first search on every node of a graph.
 * @param graph The graph to traverse with a DFS.
 * @param nodes a vector to fill with the nodes of the graph in the order they have been visited by
 * the DFS.
 */
TLP_SCOPE void dfs(const Graph *graph, std::vector<node> &nodes);

/*
 * builds a uniform quantification with the NumericProperty associated values
 * of the nodes of a graph
 */
TLP_SCOPE void buildNodesUniformQuantification(const Graph *graph, const NumericProperty *prop,
                                               unsigned int k, std::map<double, int> &mapping);

/*
 * builds a uniform quantification with the NumericProperty associated values
 * of the edges of a graph
 */
TLP_SCOPE void buildEdgesUniformQuantification(const Graph *graph, const NumericProperty *prop,
                                               unsigned int k, std::map<double, int> &mapping);

/**
 * @brief Extends selection to have a graph (no dangling edge)
 * @param graph The graph to compute on.
 * @param selection The Boolean property to consider. The selection will be extend using this
 * property.
 * @return The number of element added to the selection property.
 */
TLP_SCOPE unsigned makeSelectionGraph(const Graph *graph, BooleanProperty *selection,
                                      bool *test = nullptr);

/**
 * @enum This Enum describes the possible types of path to select between a source and target nodes
 * It is used in tlp::selectShortestPaths. Reversed means the same than Directed from target node to
 *source node.
 **/
enum ShortestPathType {
  OnePath = 0,
  OneDirectedPath = 1,
  OneReversedPath = 2,
  AllPaths = 3,
  AllDirectedPaths = 4,
  AllReversedPaths = 5
};

/**
 * @brief select the shortest paths between two nodes
 * @param graph The graph to compute on.
 * @param src The source node of the paths
 * @param tgt The target node of the paths
 * @param pathType The type of path to consider (chosen among tlp::ShortestPathType enumeration
 * values)
 * @param weights A Double property giving the edges weight if weighted paths have to be considered.
 * Can be set to null to select unweighted paths.
 * @param selection The Boolean property to consider as selection for which the values corresponding
 * to the nodes/edges owning to the shortests path(s) will be set to True.
 * @return true if a path exists between the src and tgt nodes; false if not.
 */
TLP_SCOPE bool selectShortestPaths(const Graph *const graph, node src, node tgt,
                                   ShortestPathType pathType, const DoubleProperty *const weights,
                                   BooleanProperty *selection);

/*
 * mark as reachable (set the corresponding value in "reachables" map to true),
 * all the nodes, according to direction,
 * at distance less or equal to maxDistance of startNode.
 * If direction is set to UNDIRECTED use undirected graph,
 * DIRECTED use directed graph
 * and INV_DIRECTED use reverse directed graph (ie. all edges are reversed)
 */
TLP_SCOPE void markReachableNodes(const Graph *graph, const node startNode,
                                  tlp_hash_map<node, bool> &reachables, unsigned int maxDistance,
                                  EDGE_TYPE direction = UNDIRECTED);

TLP_SCOPE void computeDijkstra(const Graph *const graph, node src,
                               const EdgeStaticProperty<double> &weights,
                               NodeStaticProperty<double> &nodeDistance, EDGE_TYPE direction,
                               tlp_hash_map<node, std::list<node>> &ancestors,
                               std::stack<node> *queueNodes = nullptr,
                               MutableContainer<int> *numberOfPaths = nullptr);
} // namespace tlp
#endif
///@endcond
