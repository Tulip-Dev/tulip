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

#ifndef _TLPGRAPHTOOLS_H
#define _TLPGRAPHTOOLS_H

#include <set>
#include <list>
#include <tulip/tuliphash.h>
#include <tulip/Node.h>
#include <tulip/Edge.h>

#include <tulip/PlanarConMap.h>

namespace tlp {

class BooleanProperty;
class DoubleProperty;
class IntegerProperty;
class NumericProperty;

/**
 *  This ordering was first introduced by C. Gutwenger and P. Mutzel in \n
 *  "Grid embeddings of biconnected planar graphs", \n
 *  "Extended Abstract, Max-Planck-Institut für Informatik," \n
 *  "Saarbrücken, Germany, 1997" \n
 *  Let n be the number of nodes, the original algorithm complexity is in O(n).\n
 *  But the implementation of the canonical ordering has not been made in O(n).\n
 */
TLP_SCOPE std::vector<std::vector<node> > computeCanonicalOrdering(PlanarConMap *,
    std::vector<edge>  *dummyEdges = NULL,
    PluginProgress *pluginProgress = NULL);
/**
 * Find all the graph centers, that version does not manage edge weight.
 * complexity O(n * m). Only works on connected graphs.
 */
TLP_SCOPE std::vector<node> computeGraphCenters(Graph * graph);
/**
 * return a node that can be considered as the graph center.
 * It is an heuristic, thus it is not absolutely sure that this
 * node is a graph center. Only works on connected graphs.
 */
TLP_SCOPE node graphCenterHeuristic(Graph * graph,
                                    PluginProgress *pluginProgress = NULL);
/**
 * return a new node connected to all previously
 * existing nodes which had a null indegree
 */
TLP_SCOPE node makeSimpleSource(Graph* graph);

TLP_SCOPE void makeProperDag(Graph* graph,std::list<node> &addedNodes,
                             TLP_HASH_MAP<edge,edge> &replacedEdges,
                             IntegerProperty* edgeLength = NULL);

/**
   * Select a spanning forest of the graph,
   * i.e for all graph elements (nodes or edges) belonging to that forest
   * the selectionProperty associated value is true. The value is false
   * for the other elements
   */
TLP_SCOPE void selectSpanningForest(Graph* graph, BooleanProperty *selectionProperty,
                                    PluginProgress *pluginProgress = NULL);

/**
 * Select a spanning tree of a graph assuming it is connected;
 * i.e for all graph elements (nodes or edges) belonging to that tree
 * the selectionProperty associated value is true. The value is false
 * for the other elements
 */
TLP_SCOPE void selectSpanningTree(Graph* graph, BooleanProperty *selection,
                                  PluginProgress *pluginProgress = NULL);

/**
 * Select the minimum spanning tree (Kruskal algorithm) of a weighted graph,
 * i.e for all graph elements (nodes or edges) belonging to that tree
 * the selectionProperty associated value is true. The value is false
 * for the other elements
 */
TLP_SCOPE void selectMinimumSpanningTree(Graph* graph, BooleanProperty *selectionProperty,
    NumericProperty *weight = NULL,
    PluginProgress *pluginProgress = NULL);


/**
 * @brief Performs a breadth-first search on a graph.
 * @param graph The graph to traverse with a BFS.
 * @param root The node from whom to start the BFS. If not provided, the root
 * node will be assigned to a source node in the graph (node with input degree equals to 0).
 * If there is no source node in the graph, a random node will be picked.
 * @return A vector containing the nodes of the graph in the order they have been visited by the BFS.
 */
TLP_SCOPE std::vector<node> bfs(const Graph *graph, node root = node());

/**
 * @brief Performs a depth-first search on a graph.
 * @param graph The graph to traverse with a DFS.
 * @param root The node from whom to start the DFS. If not provided, the root
 * node will be assigned to a source node in the graph (node with input degree equals to 0).
 * If there is no source node in the graph, a random node will be picked.
 * @return A vector containing the nodes of the graph in the order they have been visited by the DFS.
 */
TLP_SCOPE std::vector<node> dfs(const Graph *graph, node root = node());

/*
 * builds a uniform quantification with the NumericProperty associated values
 * of the nodes of a graph
 */
TLP_SCOPE void buildNodesUniformQuantification(const Graph* graph, const NumericProperty* prop, unsigned int k, std::map<double, int>& mapping);

/*
 * builds a uniform quantification with the NumericProperty associated values
 * of the edges of a graph
 */
TLP_SCOPE void buildEdgesUniformQuantification(const Graph* graph, const NumericProperty* prop, unsigned int k, std::map<double, int>& mapping);

}
#endif
///@endcond
