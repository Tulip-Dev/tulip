//-*-c++-*-
#ifndef _BUBBLE_CIRCULARLAYOUT_H
#define _BUBBLE_CIRCULARLAYOUT_H

#include <tulip/TulipPlugin.h>

/** \addtogroup layout */
/*@{*/
/// Circular.h - An implementation of a circurlar layout.
/** 
 * This plugin is an implementation of a circular layout that takes
 * node size into account.  It uses a varient of the BubbleTree algorithm
 * developed by S. Grivet, D. Auber, J.P. Domenger, and G. Melancon.
 *
 * HISTORY:
 *
 * 25/11/2004 version 0.0.2: size and order
 * 01/12/1999 Version 0.0.1: Initial release
 * 
 *
 * NOTES:
 *
 * This work on general graphs.
 * Let n be the number of nodes, the algorithm complexity is in O(n).
 *
 * AUTHORS:
 *
 * David Auber University Bordeaux I France: Email:auber@tulip-software.org
 * Daniel Archambault The University of British Columbia:
 * Email:  archam@cs.ubc.ca
 */
class Circular:public Layout { 
public:
  Circular(const PropertyContext &);
  bool run();
};
/*@}*/
#endif
