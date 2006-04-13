//-*-c++-*-
#ifndef _TREELEAFLAYOUT_H
#define _TREELEAFLAYOUT_H

#include <tulip/TulipPlugin.h>

/** \addtogroup layout */
/*@{*/
/// TreeLeaf.h - An implementation of a simple tree layout.
/** 
 * This plugin is an implementation of a simple tree layout.
 * All leaves are placed at a distance one (x-coordinates) and the order
 * is the one of a suffix ordering. The y coordinates is the depth in the
 * tree. The other nodes are placed at the center of their children
 * (x-coordinates), and the y-coordinate is their depth in the tree.
 *
 * <b>HISTORY</b>
 *
 * - 01/12/99 Verson 0.0.1: Initial release
 *
 *
 * \note This works on tree. \n
 * Let n be the number of nodes, the algorithm complexity is in O(n).
 *
 *
 * \author David Auber University Bordeaux I France: Email:auber@tulip-software.org
 */
class TreeLeaf:public LayoutAlgorithm { 
public:
  TreeLeaf(const PropertyContext &);
  ~TreeLeaf();
  bool run();
  bool check(std::string &);
  void reset();
  int dfsPlacement(node,int &,int );
};
/*@}*/
#endif










