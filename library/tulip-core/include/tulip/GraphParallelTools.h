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

#ifndef GRAPH_PARALLEL_TOOLS_H
#define GRAPH_PARALLEL_TOOLS_H

#include <tulip/Graph.h>
#include <tulip/ParallelTools.h>

namespace tlp {

// ===================================================================================

template <typename NodeIndexFunction>
void inline TLP_MAP_NODES_AND_INDICES(const tlp::Graph *graph,
                                      const NodeIndexFunction &nodeIndexFunction) {
  unsigned int i = 0;
  for (auto n : graph->nodes()) {
    nodeIndexFunction(n, i++);
  }
}

// ===================================================================================

/**
 * Template function to ease the creation of OpenMP parallel jobs taking
 * a graph node as parameter.
 *
 * @since Tulip 5.2
 *
 * @param graph the graph on which to run job on the nodes
 * @param nodeFunction callable object (e.g. lambda function) taking a tlp::node as parameter
 *
 * Example of use:
 *
 * @code
 * auto computationIntensiveTask = [&](const tlp::node &n) {
 *  double result = 0;
 *   ...
 *  return result;
 * };
 * tlp::NodeStaticProperty<double> result(graph);
 * TLP_PARALLEL_MAP_NODES(graph, [&](const tlp::node &n) {
 *   // run task in a thread
 *   result[n] = computationIntensiveTask(n);
 * });
 * @endcode
 */
template <typename NodeFunction>
void inline TLP_PARALLEL_MAP_NODES(const tlp::Graph *graph, const NodeFunction &nodeFunction) {
  TLP_PARALLEL_MAP_VECTOR<tlp::node, NodeFunction>(graph->nodes(), nodeFunction);
}

// ===================================================================================

/**
 * Template function to ease the creation of OpenMP parallel jobs taking
 * a graph node and its iteration index as parameter.
 *
 * @since Tulip 5.2
 *
 * @param graph the graph on which to run job on the nodes
 * @param nodeIndexFunction callable object (e.g. lambda function) taking a tlp::node and
 *    and unsigned integer as parameter
 *
 * Example of use:
 *
 * @code
 * auto computationIntensiveTask = [&](const tlp::node &n, unsigned int i) {
 *  double result = 0;
 *   ...
 *  return result;
 * };
 * tlp::NodeStaticProperty<double> result(graph);
 * TLP_PARALLEL_MAP_EDGES(graph, [&](const tlp::node &n, unsigned int i) {
 *   // run task in a thread
 *   result[n] = computationIntensiveTask(n, i);
 * });
 * @endcode
 */
template <typename NodeFunction>
void inline TLP_PARALLEL_MAP_NODES_AND_INDICES(const tlp::Graph *graph,
                                               const NodeFunction &nodeFunction) {
  TLP_PARALLEL_MAP_VECTOR_AND_INDICES<tlp::node, NodeFunction>(graph->nodes(), nodeFunction);
}

// ===================================================================================

template <typename EdgeIndexFunction>
void inline TLP_MAP_EDGES_AND_INDICES(const tlp::Graph *graph,
                                      const EdgeIndexFunction &edgeIndexFunction) {
  unsigned int i = 0;
  for (auto e : graph->edges()) {
    edgeIndexFunction(e, i++);
  }
}

// ===================================================================================

/**
 * Template function to ease the creation of OpenMP parallel jobs taking
 * a graph edge as parameter.
 *
 * @since Tulip 5.2
 *
 * @param graph the graph on which to run job on the edges
 * @param edgeFunction callable object (e.g. lambda function) taking a tlp::edge as parameter
 *
 * Example of use:
 *
 * @code
 * auto computationIntensiveTask = [&](const tlp::edge &e) {
 *  double result = 0;
 *   ...
 *  return result;
 * };
 * tlp::EdgeStaticProperty<double> result(graph);
 * TLP_PARALLEL_MAP_EDGES(graph, [&](const tlp::edge &e) {
 *   // run task in a thread
 *   result[e] = computationIntensiveTask(e);
 * });
 * @endcode
 */
template <typename EdgeFunction>
void inline TLP_PARALLEL_MAP_EDGES(const tlp::Graph *graph, const EdgeFunction &edgeFunction) {
  TLP_PARALLEL_MAP_VECTOR<tlp::edge, EdgeFunction>(graph->edges(), edgeFunction);
}

// ===================================================================================

/**
 * Template function to ease the creation of OpenMP parallel jobs taking
 * a graph edge and its iteration index as parameter.
 *
 * @since Tulip 5.2
 *
 * @param graph the graph on which to run job on the edges
 * @param edgeIndexFunction callable object (e.g. lambda function) taking a tlp::edge and
 *    and unsigned integer as parameter
 *
 * Example of use:
 *
 * @code
 * auto computationIntensiveTask = [&](const tlp::edge &e, unsigned int i) {
 *  double result = 0;
 *   ...
 *  return result;
 * };
 * tlp::EdgeStaticProperty<double> result(graph);
 * TLP_PARALLEL_MAP_EDGES(graph, [&](const tlp::edge &e, unsigned int i) {
 *   // run task in a thread
 *   result[e] = computationIntensiveTask(e, i);
 * });
 * @endcode
 */
template <typename EdgeFunction>
void inline TLP_PARALLEL_MAP_EDGES_AND_INDICES(const tlp::Graph *graph,
                                               const EdgeFunction &edgeFunction) {
  TLP_PARALLEL_MAP_VECTOR_AND_INDICES<tlp::edge, EdgeFunction>(graph->edges(), edgeFunction);
}
} // namespace tlp

#endif
