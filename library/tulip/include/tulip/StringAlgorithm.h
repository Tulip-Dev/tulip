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









