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

#ifndef MINMAXPROPERTY_H
#define MINMAXPROPERTY_H

#include <tulip/tuliphash.h>

#include <tulip/Observable.h>
#include <tulip/AbstractProperty.h>

#define MINMAX_PAIR(TYPE) std::pair<typename TYPE::RealType, typename TYPE::RealType>
#define MINMAX_MAP(TYPE) typename tlp_hash_map<unsigned int, MINMAX_PAIR(TYPE)>

#define NODE_VALUE typename nodeType::RealType
#define CONST_NODE_VALUE typename tlp::StoredType<typename nodeType::RealType>::ReturnedConstValue
#define EDGE_VALUE typename edgeType::RealType
#define CONST_EDGE_VALUE typename tlp::StoredType<typename edgeType::RealType>::ReturnedConstValue

namespace tlp {

/**
 * @brief Abstracts the computation of minimal and maximal values on node and edge values of
 *properties.
 *
 * The value is lazily computed on first request.
 * The value is cached, and the cache is invalidated whenever it cannot be simply updated.
 **/
template <typename nodeType, typename edgeType, typename propType = PropertyInterface>
class MinMaxProperty : public tlp::AbstractProperty<nodeType, edgeType, propType> {

  const MINMAX_PAIR(nodeType) & getNodeMinMax(const Graph *graph = nullptr);
  const MINMAX_PAIR(edgeType) & getEdgeMinMax(const Graph *graph = nullptr);

public:
  /**
   * @brief Constructs a MinMaxProperty.
   *
   * @param graph The graph to attach the property to.
   * @param name The name of the property.
   * @param NodeMin The minimal value the property can take for nodes (e.g. INT_MIN)
   * @param NodeMax The maximal value the property can take for nodes (e.g. INT_MIN)
   * @param EdgeMin The minimal value the property can take for edges (e.g. INT_MIN)
   * @param EdgeMax The maximal value the property can take for edges (e.g. INT_MIN)
   **/
  MinMaxProperty(tlp::Graph *graph, const std::string &name, NODE_VALUE NodeMin, NODE_VALUE NodeMax,
                 EDGE_VALUE EdgeMin, EDGE_VALUE EdgeMax);

  using tlp::AbstractProperty<nodeType, edgeType, propType>::operator=;
  void treatEvent(const tlp::Event &ev) override;

  /**
   * @brief Gets the minimum value on the nodes.
   * It is only computed if it has never been retrieved before, or if the cached value could not be
   *updated.
   *
   * @param graph The graph on which to compute.
   * @return The minimal value on this graph for this property.
   **/
  CONST_NODE_VALUE getNodeMin(const Graph *graph = nullptr) {
    return getNodeMinMax(graph).first;
  }

  /**
   * @brief Computes the maximum value on the nodes.
   * It is only computed if it has never been retrieved before, or if the cached value could not be
   *updated.
   *
   * @param graph The graph on which to compute.
   * @return The maximal value on this graph for this property.
   **/
  CONST_NODE_VALUE getNodeMax(const Graph *graph = nullptr) {
    return getNodeMinMax(graph).second;
  }

  /**
   * @brief Computes the minimum value on the edges.
   * It is only computed if it has never been retrieved before, or if the cached value could not be
   *updated.
   *
   * @param graph The graph on which to compute.
   * @return The minimal value on this graph for this property.
   **/
  CONST_EDGE_VALUE getEdgeMin(const Graph *graph = nullptr) {
    return getEdgeMinMax(graph).first;
  }

  /**
   * @brief Computes the maximum value on the edges.
   * It is only computed if it has never been retrieved before, or if the cached value could not be
   *updated.
   *
   * @param graph The graph on which to compute.
   * @return The maximal value on this graph for this property.
   **/
  CONST_EDGE_VALUE getEdgeMax(const Graph *graph = nullptr) {
    return getEdgeMinMax(graph).second;
  }

  /**
   * @brief Updates the value on a node, and updates the minimal/maximal cached values if necessary.
   * Should be called by subclasses in order to keep the cache up to date.
   *
   * @param n The node for which the value is updated.
   * @param newValue The new value on this node.
   **/
  void updateNodeValue(tlp::node n, CONST_NODE_VALUE newValue);

  /**
   * @brief Updates the value on an edge, and updates the minimal/maximal cached values if
   *necessary.
   * Should be called by subclasses in order to keep the cache up to date.
   *
   * @param e The edge for which the value is updated.
   * @param newValue The new value on this edge.
   **/
  void updateEdgeValue(tlp::edge e, CONST_EDGE_VALUE newValue);

  /**
   * @brief Updates the value of all nodes, setting the maximum and minimum values to this.
   * Should be called by subclasses in order to keep the cache up to date.
   *
   * @param newValue The new maximal and minimal value.
   **/
  void updateAllNodesValues(CONST_NODE_VALUE newValue);

  /**
   * @brief Updates the value of all edges, setting the maximum and minimum values to this.
   * Should be called by subclasses in order to keep the cache up to date.
   *
   * @param newValue The new maximal and minimal value.
   **/
  void updateAllEdgesValues(CONST_EDGE_VALUE newValue);

protected:
  MINMAX_MAP(nodeType) minMaxNode;
  MINMAX_MAP(edgeType) minMaxEdge;

  NODE_VALUE _nodeMin;
  NODE_VALUE _nodeMax;
  EDGE_VALUE _edgeMin;
  EDGE_VALUE _edgeMax;

  // this will indicate if we can stop propType::graph observation
  bool needGraphListener; // default is false

  const MINMAX_PAIR(nodeType) & computeMinMaxNode(const Graph *graph);
  const MINMAX_PAIR(edgeType) & computeMinMaxEdge(const Graph *graph);
  void removeListenersAndClearNodeMap();
  void removeListenersAndClearEdgeMap();
};
} // namespace tlp

#include "cxx/minmaxproperty.cxx"

#endif // MINMAXPROPERTY_H
