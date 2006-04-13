//-*-c++-*-
#ifndef _RANDOMLAYOUT_H
#define _RANDOMLAYOUT_H

#include <tulip/TulipPlugin.h>

/// Random.h - An implementation of a random 3D layout.
/** This plugin is an implementation of a random 3D layout
 *
 *  <b>HISTORY</b>
 *
 *  01/12/99 Verson 0.0.1: Initial release
 *
 *
 *  \note This algorithm works on general graphs. \n
 *  Let n be the number of nodes, the algorithm complexity is in O(n).
 *
 *
 *  \author David Auber University Bordeaux I France, Email : auber@tulip-software.com
 */
class Random:public LayoutAlgorithm
{ 
public:
  Random(const PropertyContext &);
  ~Random();
  
  bool run();
  bool check(std::string &);
  void reset();
};

#endif










