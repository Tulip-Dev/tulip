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

#ifndef _LAYOUTALGO_H
#define _LAYOUTALGO_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "tulip/PropertyAlgorithm.h"

namespace tlp {

class PropertyContext;
class LayoutProperty;

/**
 * \addtogroup plugins
 */ 
/*@{*/
///Interface for layout plug-ins
class TLP_SCOPE LayoutAlgorithm:public PropertyAlgorithm {
  ///
  friend class LayoutProperty;
public:
  /** 
   * This layout is the one associated to the plug-ins,
   * The data should be store in it
   */
  LayoutProperty *layoutResult;
  ///
  virtual ~LayoutAlgorithm();

protected: 
  ///
  LayoutAlgorithm (const PropertyContext &context);
};
/*@}*/

}
#endif








