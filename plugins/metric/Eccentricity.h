//-*-c++-*-
#ifndef _ECCENTRICITYMETRIC_H
#define _ECCENTRICITYMETRIC_H

#include <tulip/TulipPlugin.h>

/** \addtogroup metric */
/*@{*/
/// EccentricyMetric.h - Compute the eccentricity of each node.
/** This plugin compute the eccentricity of each node, more information 
 * about the use of eccentricity metric can be found in :
 *
 * Visone: Analysis and visualization of social networks.
 * "Book. Graph Drawing Software. (Ed. Michael Junger & Petra Mutzel",
 * Authors : Ulrik Brandes and Dorothea Wagner.
 * "2004",
 * pages 321-340.
 *  
 *  HISTORY:
 *
 *  18/06/2004 Verson 0.0.1: Initial release
 *
 *  NOTE :
 *  The complexity of the algorithm is O(|V|+|E|) time and O(|V|) space.  
 *
 *  AUTHOR:
 *
 *  David Auber University Bordeaux I France: Email:auber@tulip-software.com
 *  Tamara Munzner, University of British Columbia Canada. Email: tmm@cs.ubc.ca
 *  
 *  LICENCE:
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
  /*
    private :
    double computeValue(const node n);
  */
};
/*@}*/
#endif
