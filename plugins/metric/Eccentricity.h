//-*-c++-*-
#ifndef _ECCENTRICITYMETRIC_H
#define _ECCENTRICITYMETRIC_H

#include <tulip/TulipPlugin.h>

/** \addtogroup metric */
/*@{*/
/// EccentricyMetric.h - Compute the eccentricity of each node.
/** This plugin compute the eccentricity of each node, 
 *
 * Eccentricity is the maximum distance to go from a node to all others.
 * In this version the value is normalized (1 means that a node is in the center 
 * of the network, 0 means that a node is the more eccentric in the network). 
 *
 * More information  about the use of eccentricity metric can be found in :
 *
 * Visone: Analysis and visualization of social networks. \n
 * "Book. Graph Drawing Software. (Ed. Michael Junger & Petra Mutzel", \n
 * Authors : Ulrik Brandes and Dorothea Wagner. \n
 * "2004", \n
 * pages 321-340.
 *  
 *  <b>HISTORY</b>
 *
 *  18/06/2004 Verson 0.0.1: Initial release
 *
 *  \note The complexity of the algorithm is O(|V| * |E|) time and O(1) space.  
 *
 *
 *  \author David Auber University Bordeaux I France: Email:auber@tulip-software.com
 *  Tamara Munzner, University of British Columbia Canada. Email: tmm@cs.ubc.ca
 *  
 * <b>LICENCE</b>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by  
 *  the Free Software Foundation; either version 2 of the License, or     
 *  (at your option) any later version.
 *
 */
class EccentricityMetric:public Metric { 
public:
  EccentricityMetric(const PropertyContext &);
  ~EccentricityMetric();
  bool run();
};
/*@}*/
#endif
