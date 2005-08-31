//-*-c++-*-
#ifndef _ConnectedComponent_H
#define _ConnectedComponent_H

#if (__GNUC__ < 3)
#include <hash_map>
#else
#include <ext/hash_map>
#endif

#include <tulip/TulipPlugin.h>
/** \addtogroup metric */
/*@{*/
/// StrongComponent.h - An implementation of the Stongly connected component algorithm.
/** This plugin is an implementation of the Stongly connected algorithm.
 *
 *  <b>HISTORY</b>
 *
 *  - 01/12/1999 Verson 0.0.1: Initial release
 *
 *
 *  \note This algorithm assigns to each node a value defined as following : If two nodes are in the same
 *  connected component they have the same value else they have a 
 *  different value.
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
class ConnectedComponent:public Metric
{ 
public:
  ConnectedComponent(const PropertyContext &);
  ~ConnectedComponent();
  bool run();

private:
  void dfs(node n, stdext::hash_map<node,bool> &flag,double value);
};
/*@}*/
#endif
