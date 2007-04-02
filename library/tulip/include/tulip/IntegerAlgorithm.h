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


#ifndef TULIP_INTALGO_H
#define TULIP_INTALGO_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "PropertyAlgorithm.h"

namespace tlp {

class PropertyContext;
class IntegerProperty;

/** \addtogroup plugins */ 
/*@{*/

/// Interface for int plug-ins
class TLP_SCOPE IntegerAlgorithm:public PropertyAlgorithm { 
  ///
  friend class IntegerProperty;
public:
  /** This int obj is the one associated to the algorithm,
      The data should be store in it*/
  IntegerProperty *integerResult;

protected:
  ///
  IntegerAlgorithm (const PropertyContext  &context);
  ///
  virtual ~IntegerAlgorithm(){}
};
/*@}*/

}
#endif






