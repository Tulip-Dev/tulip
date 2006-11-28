//-*-c++-*-
/*
 Author: David Auber
 Email : auber@labri.fr
 Last modification : 20/08/2001
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/

#ifndef TULIP_COLORSALGO_H
#define TULIP_COLORSALGO_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "Types.h"
#include "Property.h"

namespace tlp {

class PropertyContext;
class ColorProperty;

/** \addtogroup plugins */ 
/*@{*/
/// Interface for color plug-ins
class TLP_SCOPE ColorAlgorithm:public PropertyAlgorithm< ColorType , ColorType >
{ 
  ///
  friend class ColorProperty;
public:
  ColorProperty *colorResult;

  static const char* getClassName() {
    return "ColorAlgorithm";
  }

protected:
  ///
  ColorAlgorithm (const PropertyContext & context);
  ///
  virtual ~ColorAlgorithm();
};
/*@}*/

}
#endif









