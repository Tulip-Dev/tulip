//-*-c++-*-
#ifndef _CIRCULARLAYOUT_H
#define _CIRCULARLAYOUT_H

#include <tulip/TulipPlugin.h>

/** \addtogroup layout */
/*@{*/
/// Circular.h - An implementation of a circular layout.
/** 
 * This plugin is an implementation of a circular layout
 * All nodes are placed arbitrary on circle. 
 *
 * <b>HISTORY</b>
 *
 * - 01/12/99 Verson 0.0.1: Initial release
 *
 *
 * \note This works on general graphs.
 * Let n be the number of nodes, the algorithm complexity is in O(n).
 *
 *
 * \author David Auber University Bordeaux I France: Email:auber@tulip-software.org
 */
class Circular:public Layout { 
public:
  Circular(const PropertyContext &);
  ~Circular();
  bool run();
};
/*@}*/
#endif
