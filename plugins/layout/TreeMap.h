//-*-c++-*-
#ifndef _TREELEAFLAYOUT_H
#define _TREELEAFLAYOUT_H

#if (__GNUC__ < 3)
#include <hash_map>
#else
#include <ext/hash_map>
#endif

#include <tulip/TulipPlugin.h>

class DoubleProperty;
class SizeProperty;


/** \addtogroup layout */
/*@{*/
/// TreeMap.h - An implementation of the tree map layout.
/** This plugin is an implementation of the tree map layout
 *  algorithm first published as:
 *
 *  Shneiderman, B. (March 1991) \n
 *  Tree visualization with treemaps: a 2-d space-filling approach \n
 *  ACM Transactions on Graphics, vol. 11, 1 (Jan. 1992) 92-99. \n
 *  HCIL-91-03, CS-TR-2645, CAR-TR-548  
 *
 *  <b>HISTORY</b>
 *
 *  - 01/04/99 Verson 0.0.1: Initial release
 *
 *  \note This algorithm only works on trees.
 *  This version use the "viewMetric" property to determine
 *  The size of elements. \n
 *  Let n be the number of nodes, the algorithm complexity is in O(n).
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
class TreeMap:public LayoutAlgorithm { 
public:
  TreeMap(const PropertyContext &);
  ~TreeMap();
  bool run();
  bool check(std::string &);
  void reset();


private:
  void dfsPlacement(node n, int depth, double x, double y, double width, double height,
		    bool direction, stdext::hash_map<node,double> &value);
  double initVal(node n, stdext::hash_map<node,double> &value);
  SizeProperty *size;
  DoubleProperty *metric;
  ColorProperty *color;
};
/*@}*/
#endif










