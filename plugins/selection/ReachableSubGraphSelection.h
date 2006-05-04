//-*-c++-*-
#ifndef Tulip_REACHABLESUBGRAPHELECTION_H
#define Tulip_REACHABLESUBGRAPHELECTION_H

#include <tulip/TulipPlugin.h>

/** \addtogroup selection */
/*@{*/
/**
 * This selection plugin enables to find all nodes and edges at a fixed distance of a set of nodes.
 * It takes three parameter :
 * - <int> distance 
 * - <int> direction : 0 means directed, 1 reverse directed, 2 undirected
 * - <SelectionProxy *> startingnodes : the selected nodes of this selection proxy will be used as 
 *   starting nodes.
 *
 *  \author David Auber, LaBRI University Bordeaux I France: 
 *   auber@tulip-software.org
 */
class ReachableSubGraphSelection:public BooleanAlgorithm {
public:
  ReachableSubGraphSelection(const PropertyContext &);
  ~ReachableSubGraphSelection();
  bool run();
};
/*@}*/
#endif



