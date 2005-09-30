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

#ifndef _IDMETRIC_H
#define _IDMETRIC_H

#include <tulip/TulipPlugin.h>

/** \addtogroup metric */
/*@{*/
/// IdMetric.h - This plugins assigns id to nodes and edges.
/** This plugins assigns id to nodes and edges. The id are those used by tulip.
 *
 *  <b>HISTORY</b>
 *
 *  - 26/02/2001 Verson 0.0.1: First implementation.
 *
 *  \note This algorithm works on general graphs. 
 *
 *  \author David Auber University Bordeaux I France: Email:auber@tulip-software.org
 *
 *  <b>LICENCE</b>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by  
 *  the Free Software Foundation; either version 2 of the License, or     
 *  (at your option) any later version.
 *
 */
class IdMetric:public Metric { 
public:
  IdMetric(const PropertyContext &);
  double getNodeValue(const node n);
  double getEdgeValue(const edge n);
};
/*@}*/


#endif











