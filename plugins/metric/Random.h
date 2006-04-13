//-*-c++-*-
#ifndef _RANDOMMETRIC_H
#define _RANDOMMETRIC_H

#include <tulip/TulipPlugin.h>

/** \addtogroup metric */
/*@{*/
/// RandomhMetric.h - This plugins assigns random values to nodes and edges.
/** This plugins assigns random values to nodes and edges. the values are between 0 and 1.
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
class RandomMetric:public MetricAlgorithm { 
public:
  RandomMetric(const PropertyContext &);
  bool run();
};
/*@}*/
#endif











