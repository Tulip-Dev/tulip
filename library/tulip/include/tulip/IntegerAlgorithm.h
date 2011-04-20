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


#ifndef TULIP_INTALGO_H
#define TULIP_INTALGO_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <tulip/PropertyAlgorithm.h>

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
  ///
  virtual ~IntegerAlgorithm(){}

protected:
  ///
  IntegerAlgorithm (const PropertyContext  &context);
};
/*@}*/

}
#endif






