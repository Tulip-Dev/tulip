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

#ifndef _METAGRAPHALGO_H
#define _METAGRAPHALGO_H

#include "Types.h"
#include "Property.h"

class PropertyContext;
class GraphProperty;

/** \addtogroup plugins */ 
/*@{*/

/// Interface for metric plug-ins
class GraphAlgorithm:public PropertyAlgorithm<GraphType , GraphType>
{ 
  ///
  friend class GraphProperty;
public:
  /** This sg property is the one associated to the algorithm,
    *  The data should be store in it
    */
  GraphProperty *sgResult;
protected:
  ///
  GraphAlgorithm (const PropertyContext & context);
  ///
  virtual ~GraphAlgorithm(){}
};

/*@}*/
#endif

