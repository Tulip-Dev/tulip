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

#ifndef MINMAXPROPERTY_H
#define MINMAXPROPERTY_H

#include <tulip/tuliphash.h>
#include <tulip/Observable.h>
#include <tulip/AbstractProperty.h>

#define MINMAX_PAIR(TYPE) std::pair<typename TYPE::RealType, typename TYPE::RealType>
#define MINMAX_MAP(TYPE) typename TLP_HASH_MAP<unsigned int, MINMAX_PAIR(TYPE) >

namespace tlp {

/**
 * @brief Abstracts the computation of minimal and maximal values on node and edge values of properties.
 *
 * The value is lazily computed on first request.
 * The value is cached, and the cache is invalidated whenever it cannot be simply updated.
 **/
template<typename nodeType, typename edgeType, typename propType=PropertyInterface>
class MinMaxProperty : public tlp::AbstractProperty<nodeType, edgeType, propType> {
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
  MinMaxProperty(tlp::Graph* graph, const std::string& name, typename nodeType::RealType NodeMin,
                 typename nodeType::RealType NodeMax, typename edgeType::RealType EdgeMin, typename edgeType::RealType EdgeMax);

  virtual void treatEvent(const tlp::Event& ev);

  /**
   * @brief Gets the minimum value on the nodes.
   * It is only computed if it has never been retrieved before, or if the cached value could not be updated.
   *
   * @param graph The graph on which to compute.
   * @return The minimal value on this graph for this property.
   **/
  typename nodeType::RealType getNodeMin(Graph* graph = NULL);

  /**
   * @brief Computes the maximum value on the nodes.
   * It is only computed if it has never been retrieved before, or if the cached value could not be updated.
   *
   * @param graph The graph on which to compute.
   * @return The maximal value on this graph for this property.
   **/
  typename nodeType::RealType getNodeMax(Graph* graph = NULL);

  /**
   * @brief Computes the minimum value on the edges.
   * It is only computed if it has never been retrieved before, or if the cached value could not be updated.
   *
   * @param graph The graph on which to compute.
   * @return The minimal value on this graph for this property.
   **/
  typename edgeType::RealType getEdgeMin(Graph* graph = NULL);

  /**
   * @brief Computes the maximum value on the edges.
   * It is only computed if it has never been retrieved before, or if the cached value could not be updated.
   *
   * @param graph The graph on which to compute.
   * @return The maximal value on this graph for this property.
   **/
  typename edgeType::RealType getEdgeMax(Graph* graph = NULL);

  /**
   * @brief Updates the value on a node, and updates the minimal/maximal cached values if necessary.
   * Should be called by subclasses in order to keep the cache up to date.
   *
   * @param n The node for which the value is updated.
   * @param newValue The new value on this node.
   **/
  void updateNodeValue(tlp::node n, typename nodeType::RealType newValue);

  /**
   * @brief Updates the value on an edge, and updates the minimal/maximal cached values if necessary.
   * Should be called by subclasses in order to keep the cache up to date.
   *
   * @param e The edge for which the value is updated.
   * @param newValue The new value on this edge.
   **/
  void updateEdgeValue(tlp::edge e, typename edgeType::RealType newValue);

  /**
   * @brief Updates the value of all nodes, setting the maximum and minimum values to this.
   * Should be called by subclasses in order to keep the cache up to date.
   *
   * @param newValue The new maximal and minimal value.
   **/
  void updateAllNodesValues(typename nodeType::RealType newValue);

  /**
   * @brief Updates the value of all edges, setting the maximum and minimum values to this.
   * Should be called by subclasses in order to keep the cache up to date.
   *
   * @param newValue The new maximal and minimal value.
   **/
  void updateAllEdgesValues(typename edgeType::RealType newValue);

protected:
  MINMAX_MAP(nodeType) minMaxNode;
  MINMAX_MAP(edgeType) minMaxEdge;

  typename nodeType::RealType _nodeMin;
  typename nodeType::RealType _nodeMax;
  typename edgeType::RealType _edgeMin;
  typename edgeType::RealType _edgeMax;

  // this will indicate if we can stop propType::graph observation
  bool needGraphListener; // default is false

  MINMAX_PAIR(nodeType) computeMinMaxNode(Graph* graph);
  MINMAX_PAIR(edgeType) computeMinMaxEdge(Graph* graph);
  void removeListenersAndClearNodeMap();
  void removeListenersAndClearEdgeMap();
};

}

#include "cxx/minmaxproperty.cxx"

#endif //MINMAXPROPERTY_H
