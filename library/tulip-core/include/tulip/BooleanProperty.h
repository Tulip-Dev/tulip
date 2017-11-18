/*
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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

#ifndef TULIP_SELECTION_H
#define TULIP_SELECTION_H

#include <tulip/PropertyTypes.h>
#include <tulip/AbstractProperty.h>
#include <tulip/PropertyAlgorithm.h>

namespace tlp {

class PropertyContext;

/**
 * @ingroup Graph
 * @brief A graph property that maps a boolean value to graph elements.
 */
class TLP_SCOPE BooleanProperty : public AbstractProperty<tlp::BooleanType, tlp::BooleanType> {
public:
  BooleanProperty(Graph *g, const std::string &n = "")
      : AbstractProperty<BooleanType, BooleanType>(g, n) {}
  // PropertyInterface inherited methods
  PropertyInterface *clonePrototype(Graph *, const std::string &) const;
  static const std::string propertyTypename;
  const std::string &getTypename() const {
    return propertyTypename;
  }

  /**
   * Reverses all values associated to graph elements,
   * i.e true => false, false => true.
   * If sg is NULL, the graph given when creating the property is considered.
   */
  void reverse(const Graph *sg = NULL);

  /**
   * Reverses all the direction of edges of the visible graph
   * which are true in this BooleanProperty.
   * * If sg is NULL, the graph given when creating the property is considered.
   */
  void reverseEdgeDirection(Graph *sg = NULL);
};

/**
 * @ingroup Graph
 * @brief A graph property that maps a std::vector<bool> value to graph elements.
 */
class TLP_SCOPE BooleanVectorProperty
    : public AbstractVectorProperty<tlp::BooleanVectorType, tlp::BooleanType> {
public:
  BooleanVectorProperty(Graph *g, const std::string &n = "")
      : AbstractVectorProperty<BooleanVectorType, tlp::BooleanType>(g, n) {}
  // PropertyInterface inherited methods
  PropertyInterface *clonePrototype(Graph *, const std::string &) const;
  static const std::string propertyTypename;
  const std::string &getTypename() const {
    return propertyTypename;
  }
};
}

#endif
