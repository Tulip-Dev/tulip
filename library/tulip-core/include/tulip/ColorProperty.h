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

#ifndef TULIP_COLORS_H
#define TULIP_COLORS_H

#include <tulip/PropertyTypes.h>
#include <tulip/AbstractProperty.h>

namespace tlp {

class PropertyContext;

typedef AbstractProperty<tlp::ColorType, tlp::ColorType> AbstractColorProperty;

/**
 * @ingroup Graph
 * @brief A graph property that maps a tlp::Color value to graph elements.
 */
class TLP_SCOPE ColorProperty:public AbstractColorProperty {
public :
  ColorProperty (Graph *g, const std::string& n="") :AbstractColorProperty(g, n) {}
  // PropertyInterface inherited methods
  PropertyInterface* clonePrototype(Graph *, const std::string&);
  static const std::string propertyTypename;
  const std::string& getTypename() const {
    return propertyTypename;
  }

  int compare(const node n1, const node n2) const;
  int compare(const edge e1, const edge e2) const;

};

/**
 * @ingroup Graph
 * @brief A graph property that maps a std::vector<tlp::Color> value to graph elements.
 */
class TLP_SCOPE ColorVectorProperty:public AbstractVectorProperty<tlp::ColorVectorType, tlp::ColorType> {
public :
  ColorVectorProperty(Graph *g, const std::string& n="") :AbstractVectorProperty<ColorVectorType, tlp::ColorType>(g, n) {}
  // PropertyInterface inherited methods
  PropertyInterface* clonePrototype(Graph *, const std::string&);
  static const std::string propertyTypename;
  const std::string& getTypename() const {
    return propertyTypename;
  }
};


}
#endif

