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

/**
 * @brief This class describes parameters on a plug-in.
 * These parameters can be of any type, and are used to generate a GUI that will be shown when the plug-in in invoked by the user.
 * It is mainly used by algorithms to display options to the user, e.g. a clustering algorithm can let the user choose which measure to use.
 **/
struct TLP_SCOPE WithParameter {
  
  /**
   * @brief Retrieves the parameters.
   *
   * @return :StructDef the parameters of the plug-in.
   **/
  tlp::StructDef getParameters();
  
  /**
   * @brief Adds a parameter to the plug-in.
   *
   * @param str The name of the parameter to add.
   * @param inHelp A description of the parameter, that will de displayed to the user. Defaults to 0.
   * @param inDefValue The default value the parameter should take, to be the initial value in the GUI. Defaults to 0.
   * @param isMandatory Whether this parameter requires a value or not. Defaults to true.
   * @return void
   **/
  template<typename T> void addParameter(const char* str,
					 const char* inHelp=0,
					 const char* inDefValue = 0,
					 bool isMandatory = true)
  { parameters.template add<T>(str,inHelp,inDefValue,isMandatory); }
protected:
  
  /**
   * @brief The internal structure storing the parameters.
   **/
  StructDef parameters;
};
/*@}*/

}
#endif


