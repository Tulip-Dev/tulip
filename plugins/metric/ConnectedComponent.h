//-*-c++-*-
#ifndef _ConnectedComponent_H
#define _ConnectedComponent_H
#include <tulip/TulipPlugin.h>
#include <tulip/MutableContainer.h>
/** \addtogroup metric */
/*@{*/
/// ConnectedComponent.h - An implementation of the connected component algorithm.
/** This plugin is an implementation of the connected componnent decompostion algorithm. each 
 *  node and edge that belongs to the same componnent receive the same value.
 *
 *  <b>HISTORY</b>
 *
 *  - 01/07/2002 Verson 0.0.1: Initial release
 *
 *
 *  \note This algorithm assigns to each node a value defined as following : If two nodes are in the same
 *  connected component they have the same value else they have a 
 *  different value.
 *
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
class ConnectedComponent:public MetricAlgorithm { 
public:
  ConnectedComponent(const PropertyContext &);
  ~ConnectedComponent();
  bool run();

private:
  void dfs(node n, MutableContainer<bool> &flag,double value);
};
/*@}*/
#endif
