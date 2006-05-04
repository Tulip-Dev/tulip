//-*-c++-*-
#ifndef _DEPTHMETRIC_H
#define _DEPTHMETRIC_H

#include <tulip/TulipPlugin.h>


/** \addtogroup metric */
/*@{*/
/// DepthDoubleProperty.h - This plugins compute for each node n, the maximum path-length between n and the other node.
/** This plugins compute for each node n, the maximum path-length between n and the other node.
 *  The graph must be acyclic.
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
class DepthMetric:public DoubleAlgorithm { 
public:
  DepthMetric(const PropertyContext &);
  bool run();
  bool check(std::string &);
private:
  double getNodeValue(const node n);
};
/*@}*/
#endif
