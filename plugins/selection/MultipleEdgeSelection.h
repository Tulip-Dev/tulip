//-*-c++-*-
#ifndef Tulip_ParaSelection_H
#define Tulip_ParaSelection_H

#include <tulip/TulipPlugin.h>
/** \addtogroup selection */
/*@{*/

/// This selection plugins enables to find the multiple-edges and parallel-edges in a graph.
/**
 * This selection plugins enables to find the multiple-edges and parallel-edges in a graph.
 * Let e1, e2 two edges, e1 and e2 are parallel edges if source(e1)=source(e2) and 
 * target(e1) = target(e2).
 * If it exists n edges between two nodes, only n-1 edges will be selected. 
 *
 *  \author David Auber, LaBRI University Bordeaux I France: 
 *   auber@tulip-software.org
 */
class MultipleEdgeSelection:public tlp::BooleanAlgorithm { 
public:
  MultipleEdgeSelection(const tlp::PropertyContext &);
  bool run();
};
/*@}*/
#endif




