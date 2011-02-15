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
#ifndef _TULIPWITHPARAMETER
#define _TULIPWITHPARAMETER

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "tulip/Reflect.h"

namespace tlp {

/** \addtogroup plugins */ 
/*@{*/
/// This class enables to embed self described parameters of any type
/// parameters are stored in a protected instance of the StrucDef class
struct TLP_SCOPE WithParameter {
  /// Return the parameters
  tlp::StructDef getParameters();
  /// Add a parameter of type T with name str
  /// inHelp is the description of the parameter
  /// inDefValue is a string representation of the parameter default value
  /// isMandatory indicates if a value is needed for the parameter
  template<typename T> void addParameter(const char* str,
					 const char* inHelp=0,
					 const char* inDefValue = 0,
					 bool isMandatory = true)
  { parameters.template add<T>(str,inHelp,inDefValue,isMandatory); }
protected:
  /// the structure holding the parameters
  StructDef parameters;
};
/*@}*/

}
#endif


