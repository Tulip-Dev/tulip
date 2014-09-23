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

#ifndef NUMERICPROPERTY_H
#define NUMERICPROPERTY_H

#include <tulip/PropertyInterface.h>

namespace tlp {

/**
 * @brief Interface all numerical properties.
 * Property values are always returned as double
 **/
class NumericProperty : public PropertyInterface {
public:
  /**
   * @brief Returns the value associated with the node n in this property.
   * @param n The node for which we want to get the value of the property.
   **/
  virtual double getNodeDoubleValue(const node n) const=0;

  /**
   * @brief Gets the default node value of the property.
   * @return The default value of nodes.
   */
  virtual double getNodeDoubleDefaultValue() const=0;

  /**
   * @brief Gets the minimum value on the nodes.
   * @param graph The graph on which to compute.
   * @return The minimal value on this graph for this property.
   **/
  virtual double getNodeDoubleMin(Graph* graph = NULL)=0;

  /**
   * @brief Gets the maximum value on the nodes.
   * @param graph The graph on which to compute.
   * @return The maximal value on this graph for this property.
   **/
  virtual double getNodeDoubleMax(Graph* graph = NULL)=0;

  /**
   * @brief Returns the value associated with the edge e in this property.
   * @param e The edge for which we want to get the value of the property.
   **/
  virtual double getEdgeDoubleValue(const edge e) const=0;

  /**
   * @brief Gets the default edge value of the property.
   * @return The default value of edges.
   */
  virtual double getEdgeDoubleDefaultValue() const=0;

  /**
   * @brief Gets the minimum value on the edges.
   * @param graph The graph on which to compute.
   * @return The minimal value on this graph for this property.
   **/
  virtual double getEdgeDoubleMin(Graph* graph = NULL)=0;

  /**
   * @brief Gets the maximum value on the edges.
   * @param graph The graph on which to compute.
   * @return The maximal value on this graph for this property.
   **/
  virtual double getEdgeDoubleMax(Graph* graph = NULL)=0;

  /**
   * @brief computes a uniform quantification for the nodes
   * associated values
   */
  virtual void nodesUniformQuantification(unsigned int)=0;

  /**
   * @brief computes a uniform quantification for the edges
   * associated values
   */
  virtual void edgesUniformQuantification(unsigned int)=0;

  /**
   * @brief computes a uniform quantification for the nodes/edges
   * associated values
   */
  void uniformQuantification(unsigned int k) {
    nodesUniformQuantification(k);
    edgesUniformQuantification(k);
  }

  /**
   * @brief Creates a property of the same type (e.g. tlp::DoubleProperty)
   * The new property will be a copy of this property's values for all
   * the elements of the graph
   * @param graph The Graph in which to create the new property.
   * @return The newly created property.
   */
  virtual NumericProperty* copyProperty(Graph *graph) = 0;
};

}

#endif //NUMERICPROPERTY_H
