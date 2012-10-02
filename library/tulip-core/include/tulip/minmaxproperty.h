/*
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
///@cond DOXYGEN_HIDDEN

#ifndef MINMAXPROPERTY_H
#define MINMAXPROPERTY_H

#include <tulip/tuliphash.h>
#include <tulip/Observable.h>

namespace tlp {

template<class Tnode, class Tedge>
class AbstractProperty;

template<typename nodeType, typename edgeType>
/**
 * @brief Abstracts the computation of minimal and maximal values on node and edge values of properties.
 *
 * The value is lazily computed on first request..
 * The value is cached, and the cache is invalidated whenever it cannot be simply updated.
 **/
class MinMaxProperty : public tlp::AbstractProperty<nodeType, edgeType> {
public:
  /**
   * @brief Constructs a MinMaxProperty.
   *
   * @param property The property on which to compute minimum and maximum values.
   * @param NodeMin The minimal value the property can take for nodes (e.g. INT_MIN)
   * @param NodeMax The maximal value the property can take for nodes (e.g. INT_MIN)
   * @param EdgeMin The minimal value the property can take for edges (e.g. INT_MIN)
   * @param EdgeMax The maximal value the property can take for edges (e.g. INT_MIN)
   **/
  MinMaxProperty(tlp::Graph* graph, std::string name, typename nodeType::RealType NodeMin,
                 typename nodeType::RealType NodeMax, typename edgeType::RealType EdgeMin, typename edgeType::RealType EdgeMax);

  /**
   * @brief A bridge to Observable's treatEvent() so this class can easily be delegated to.
   *
   * @param ev The event to treat.
   * @return void
   **/
  virtual void treatEvent(const tlp::Event& ev);

  /**
   * @brief Computes the minimum value on the nodes.
   *
   * @param graph The graph on which to compute.
   * @return :RealType The minimal value on this graph for this property.
   **/
  typename nodeType::RealType getNodeMin(Graph* graph = NULL);
  /**
   * @brief Computes the maximum value on the nodes.
   *
   * @param graph The graph on which to compute.
   * @return :RealType The maximal value on this graph for this property.
   **/
  typename nodeType::RealType getNodeMax(Graph* graph = NULL);
  /**
   * @brief Computes the minimum value on the edges.
   *
   * @param graph The graph on which to compute.
   * @return :RealType The minimal value on this graph for this property.
   **/
  typename edgeType::RealType getEdgeMin(Graph* graph = NULL);
  /**
   * @brief Computes the maximum value on the edges.
   *
   * @param graph The graph on which to compute.
   * @return :RealType The maximal value on this graph for this property.
   **/
  typename edgeType::RealType getEdgeMax(Graph* graph = NULL);

  /**
   * @brief Updates the value on a node, and updates the minimal/maximal cached values if necessary.
   *
   * @param n The node for which the value is updated.
   * @param newValue The new value on this node.
   * @return void
   **/
  void updateNodeValue(tlp::node n, typename nodeType::RealType newValue);
  /**
   * @brief Updates the value on an edge, and updates the minimal/maximal cached values if necessary.
   *
   * @param e The edge for which the value is updated.
   * @param newValue The new value on this edge.
   * @return void
   **/
  void updateEdgeValue(tlp::edge e, typename edgeType::RealType newValue);

  /**
   * @brief Updates the value of all nodes, setting the maximum and minimum values to this.
   *
   * @param newValue The new maximal and minimal value.
   * @return void
   **/
  void updateAllNodesValues(typename nodeType::RealType newValue);
  /**
   * @brief Updates the value of all edges, setting the maximum and minimum values to this.
   *
   * @param newValue The new maximal and minimal value.
   * @return void
   **/
  void updateAllEdgesValues(typename edgeType::RealType newValue);

protected:
  TLP_HASH_MAP<unsigned int, typename nodeType::RealType> maxNode, minNode;
  TLP_HASH_MAP<unsigned int, typename edgeType::RealType> maxEdge, minEdge;
  TLP_HASH_MAP<unsigned int, bool> nodeValueUptodate;
  TLP_HASH_MAP<unsigned int, bool> edgeValueUptodate;

  typename nodeType::RealType _nodeMin;
  typename nodeType::RealType _nodeMax;
  typename edgeType::RealType _edgeMin;
  typename edgeType::RealType _edgeMax;

  void computeMinMaxNode(Graph* graph);
  void computeMinMaxEdge(Graph* graph);
};

}

#include "cxx/minmaxproperty.cxx"

#endif //MINMAXPROPERTY_H
///@endcond
