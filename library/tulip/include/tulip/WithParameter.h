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
struct TLP_SCOPE WithParameter {
  ///
  tlp::StructDef getParameters();
  ///
  template<typename Ty> void addParameter(const char* str,
					  const char* inHelp=0,
					  const char* inDefValue = 0,
					  bool isMandatory = true)
  { parameters.template add<Ty>(str,inHelp,inDefValue,isMandatory); }
protected:
  ///
  StructDef parameters;
};
/*@}*/

}
#endif


