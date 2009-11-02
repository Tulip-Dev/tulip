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
