//-*-c++-*-
#ifndef _TREELEAFLAYOUT_H
#define _TREELEAFLAYOUT_H

#include <tulip/tuliphash.h>
#include <tulip/TulipPlugin.h>

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
 *  \note This algorithm only works on trees.
 *  This version use a predefined metric property to determine
 *  The size of the elements. \n
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
class TreeMap:public tlp::LayoutAlgorithm { 
public:
  TreeMap(const tlp::PropertyContext &);
  ~TreeMap();
  bool run();
  bool check(std::string &);

private:
  void dfsPlacement(tlp::node n, int depth, double x, double y, double width, double height,
		    bool direction, TLP_HASH_MAP<tlp::node,double> &value);
  double initVal(tlp::node n, TLP_HASH_MAP<tlp::node,double> &value);
  tlp::SizeProperty *size;
  tlp::DoubleProperty *metric;
  tlp::ColorProperty *color;
};
/*@}*/
#endif










