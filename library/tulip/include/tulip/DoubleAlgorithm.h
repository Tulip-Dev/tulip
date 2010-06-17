/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
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

#ifndef _METRICALGO_H
#define _METRICALGO_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "tulip/Types.h"
#include "tulip/PropertyAlgorithm.h"

namespace tlp {

class PropertyContext;
class DoubleProperty;

/** \addtogroup plugins */ 
/*@{*/
/// Interface for metric plug-ins
class TLP_SCOPE DoubleAlgorithm:public PropertyAlgorithm { 
  ///
  friend class DoubleProperty;
public:
  /** This metric is the one associated to the algorithm,
      The data should be store in it*/
  DoubleProperty *doubleResult;
  ///
  virtual ~DoubleAlgorithm();

protected:
  ///
  DoubleAlgorithm (const PropertyContext  &context);
};
/*@}*/

}
#endif
