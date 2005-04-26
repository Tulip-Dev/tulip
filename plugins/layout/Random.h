//-*-c++-*-
#ifndef _RANDOMLAYOUT_H
#define _RANDOMLAYOUT_H

#include <tulip/TulipPlugin.h>

/// Random.h - An implementation of a random 3D layout.
/** This plugin is an implementation of a random 3D layout
 *
 *  HISTORY:
 *
 *  01/12/99 Verson 0.0.1: Initial release
 *
 *  NOTES:
 *
 *  This algorithm works on general graphs.
 *  Let n be the number of nodes, the algorithm complexity is in O(n).
 *
 *  AUTHOR:
 *
 *  David Auber University Bordeaux I France: Email:auber@tulip-software.com
 */
class Random:public Layout
{ 
public:
  Random(const PropertyContext &);
  ~Random();
  
  bool run();
  bool check(std::string &);
  void reset();
};

#endif










