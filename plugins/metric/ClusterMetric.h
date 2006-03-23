//-*-c++-*-
#ifndef _CLUSTERMETRIC_H
#define _CLUSTERMETRIC_H

#include <tulip/TulipPlugin.h>

/** \addtogroup metric */
/*@{*/
/// ClusterMetric.h - An implementation of the clustering parameter.
/** This plugin is an implementation of the Clustering Parameter.
 *  algorithm published in :
 *
 *  Y. Chiricota. F.Jourdan, an G.Melançon \n
 *  "Software component capture using graph clustering",\n
 *  "IWPC", \n
 *   2002.
 *
 *  Extended to unbounded depth (for the neigbors) :
 *
 *  <b>HISTORY</b>
 *
 *  - 01/12/2001 Verson 0.0.1: Initial release
 *
 *  - 25/02/2003 Verson 0.0.2: Documentation, code clean up and checking
 *
 *
 *  \note This algorithm works on general graphs. The algorithm use the parameter 
 *  depth in order to determine the depth of the neighbors. If no depth 
 *  is given, the plugins automatically popup a qt windows to ask the user for this
 *  parameter.
 *
 *
 *  \author David Auber University Bordeaux I France, Email : auber@tulip-software.org
 *
 *  <b>LICENCE</b>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by  
 *  the Free Software Foundation; either version 2 of the License, or     
 *  (at your option) any later version.
 *
 */
class ClusterMetric:public Metric { 
public:
  ClusterMetric(const PropertyContext &);
  bool run();

private:
  double getNodeValue(const node n);
  double getEdgeValue(const edge n);
  void buildSubGraph(node n,node startNode,std::set<node> &selected,unsigned int depth);
  int  maxDepth;
};
/*@}*/
#endif











