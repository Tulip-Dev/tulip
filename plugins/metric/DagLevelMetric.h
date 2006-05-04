//-*-c++-*-
#ifndef _DagLevelMETRIC_H
#define _DagLevelMETRIC_H

#include <tulip/TulipPlugin.h>
/** \addtogroup metric */
/*@{*/
/// DagLevelMeric.h - An implementation of DAG layer decomposition.
/** This plugin is an implementation of a DAG layer decomposition
 *
 *  <b>HISTORY</b>
 *
 *  - 01/12/1999 Verson 0.0.1: Initial release
 *
 *  - 06/11/2003 Verson 0.0.2: Documentation and code clean up and optimization
 *
 *
 *  \note This algorithm works on general DAG, the complexity is in O(|E|+|V|);
 *
 *
 *  \author David Auber University Bordeaux I France: Email:auber@tulip-software.com
 *
 *  <b>LICENCE</b>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by  
 *  the Free Software Foundation; either version 2 of the License, or     
 *  (at your option) any later version.
 *
 */
class DagLevelMetric:public DoubleAlgorithm {
public:
  DagLevelMetric(const PropertyContext &);
  ~DagLevelMetric();
  bool run();
  bool check(std::string &);
};
/*@}*/
#endif
