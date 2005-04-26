//-*-c++-*-
#ifndef _CIRCULARLAYOUT_H
#define _CIRCULARLAYOUT_H

#include <tulip/TulipPlugin.h>

/** \addtogroup layout */
/*@{*/
/// Circular.h - An implementation of a circurlar layout.
/** 
 * This plugin is an implementation of a circular layout
 * All nodes are palced arbitrary on circle. 
 *
 * HISTORY:
 *
 * 01/12/99 Verson 0.0.1: Initial release
 *
 * NOTES:
 *
 * This work on general graphs.
 * Let n be the number of nodes, the algorithm complexity is in O(n).
 *
 * AUTHOR:
 *
 * David Auber University Bordeaux I France: Email:auber@tulip-software.com
 */
class Circular:public Layout { 
public:
  Circular(const PropertyContext &);
  ~Circular();
  bool run();
};
/*@}*/
#endif
