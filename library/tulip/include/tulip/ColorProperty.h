/**
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
#ifndef TULIP_COLORS_H
#define TULIP_COLORS_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <tulip/PropertyTypes.h>
#include <tulip/AbstractProperty.h>
#include <tulip/ColorAlgorithm.h>

namespace tlp {

class PropertyContext;

typedef AbstractProperty<tlp::ColorType, tlp::ColorType, tlp::ColorAlgorithm> AbstractColorProperty;

/** \addtogroup properties */ 
/*@{*/
class TLP_SCOPE ColorProperty:public AbstractColorProperty { 
  friend class ColorAlgorithm;
public :
  ColorProperty (Graph *g, std::string n="") :AbstractColorProperty(g, n) {}
  // PropertyInterface inherited methods
  PropertyInterface* clonePrototype(Graph *, const std::string&);
  static const std::string propertyTypename;
  std::string getTypename() const {
    return propertyTypename;
  }  
};

class TLP_SCOPE ColorVectorProperty:public AbstractVectorProperty<tlp::ColorVectorType, tlp::ColorType> { 
public :
 ColorVectorProperty(Graph *g, std::string n="") :AbstractVectorProperty<ColorVectorType, tlp::ColorType>(g, n) {}
  // PropertyInterface inherited methods
  PropertyInterface* clonePrototype(Graph *, const std::string&);
  static const std::string propertyTypename;
  std::string getTypename() const {
    return propertyTypename;
  }
};
/*@}*/

}
#endif

