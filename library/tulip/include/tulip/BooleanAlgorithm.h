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

#ifndef _SELECTIONALGO_H
#define _SELECTIONALGO_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "PropertyAlgorithm.h"

namespace tlp {

class PropertyContext;
class BooleanProperty;

/**
 * \addtogroup plugins
 */ 
/*@{*/

/// Interface for selection plug-ins
class TLP_SCOPE BooleanAlgorithm:public PropertyAlgorithm { 
  ///
  friend class BooleanProperty;
public:
  /** This Boolean is the one associated to the algorithm,
      The data should be store in it*/
  BooleanProperty *booleanResult;

protected:
  ///
  BooleanAlgorithm (const PropertyContext &);
  ///
  virtual ~BooleanAlgorithm(){}
};
/*@*/

}
#endif









