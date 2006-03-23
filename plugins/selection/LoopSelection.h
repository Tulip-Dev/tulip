//-*-c++-*-
#ifndef Tulip_LoopSelection_H
#define Tulip_LoopSelection_H

#include <tulip/TulipPlugin.h>

/** \addtogroup selection */
/*@{*/

/// This selection plugins enables to find loops in a graph
/**
 * This selection plugins enables to find loops in a graph
 * A loop is an edge that has the same source and target.
 *
 *  \author David Auber, LaBRI University Bordeaux I France: 
 *   auber@tulip-software.org
 */
class LoopSelection:public Selection { 
public:
  LoopSelection(const PropertyContext &);
  bool run();
};
/*@}*/
#endif




