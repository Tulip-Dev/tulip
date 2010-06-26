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

#ifndef TULIP_STRINGALGO_H
#define TULIP_STRINGALGO_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "tulip/PropertyAlgorithm.h"

namespace tlp {

class PropertyContext;
class StringProperty;

/** \addtogroup plugins */ 
/*@{*/

/// Interface for String plug-ins
class TLP_SCOPE StringAlgorithm:public PropertyAlgorithm { 
  ///
  friend class StringProperty;
public:
  /** This string is the one associated to the algorithm,
      The data should be store in it*/
  StringProperty *stringResult;
  ///
  virtual ~StringAlgorithm(){}

protected:
  ///
  StringAlgorithm (const PropertyContext &context);
};
/*@}*/

}
#endif









