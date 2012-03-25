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

/*
 Author: David Auber
 Email : auber@labri.fr
 Last modification : 20/08/2001
 This program is free software; you can redistribute it and/or modify  *
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.
*/

#ifndef TULIP_PROPERTY_H
#define TULIP_PROPERTY_H

#include <tulip/PluginContext.h>
#include <tulip/Graph.h>
#include <tulip/Algorithm.h>

namespace tlp {

/**
 * \addtogroup plugins
 */
/*@{*/
///
/**
 * @brief This base class describes plug-ins who only modify one property, e.g. selection.
 **/
class TLP_SCOPE PropertyAlgorithm: public tlp::Algorithm {
public :
  /**
   * @brief Builds a new plug-in that modifies a single property.
   *
   * @param context The context containing the graph and PropertyInterface this plug-in has access to, as well as a PluginProgress.
   **/
  PropertyAlgorithm(const tlp::PluginContext* context):Algorithm(context) {}

  virtual std::string category() const {
    return "PropertyAlgorithm";
  }
};

template<class Property>
class TLP_SCOPE TemplateAlgorithm : public PropertyAlgorithm {
public:
  Property* result;
  TemplateAlgorithm (const tlp::PluginContext* context) : tlp::PropertyAlgorithm(context), result(NULL) {}
};
/*@}*/

}
#endif
