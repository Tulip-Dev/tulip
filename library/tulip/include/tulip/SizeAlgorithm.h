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

#ifndef TULIP_SIZESALGO_H
#define TULIP_SIZESALGO_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "tulip/PropertyAlgorithm.h"

namespace tlp {

class PropertyContext;
class SizeProperty;

/**
 * \addtogroup plugins
 */ 
/*@{*/
/// Interface for sizes plug-ins
class TLP_SCOPE SizeAlgorithm:public PropertyAlgorithm { 
  ///
  friend class SizeProperty;
public:
  SizeProperty *sizeResult;
  ///
  virtual ~SizeAlgorithm(){}

protected:
  ///
  SizeAlgorithm (const PropertyContext  &context);
};
/*@}*/

}
#endif









