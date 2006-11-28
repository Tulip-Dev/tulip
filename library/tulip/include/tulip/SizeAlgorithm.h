//-*-c++-*-
/*
 Author: David Auber
 Email : auber@labri.fr
 Last modification : 20/08/2001
 This program is free software; you can redistribute it and/or modify  *
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/

#ifndef TULIP_SIZESALGO_H
#define TULIP_SIZESALGO_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "Types.h"
#include "Property.h"

namespace tlp {

class PropertyContext;
class SizeProperty;

/**
 * \addtogroup plugins
 */ 
/*@{*/
/// Interface for sizes plug-ins
class TLP_SCOPE SizeAlgorithm:public PropertyAlgorithm< SizeType , SizeType >
{ 
  ///
  friend class SizeProperty;
public:
  SizeProperty *sizeResult;

  static const char* getClassName() {
    return "SizeAlgorithm";
  }

protected:
  ///
  SizeAlgorithm (const PropertyContext  &context);
  ///
  virtual ~SizeAlgorithm(){}
};
/*@}*/

}
#endif









