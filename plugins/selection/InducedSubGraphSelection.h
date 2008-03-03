//-*-c++-*-
#ifndef Tulip_InducedSubGraphSelection_H
#define Tulip_InducedSubGraphSelection_H

#include <tulip/TulipPlugin.h>

/** \addtogroup selection */
/*@{*/
/// This selection plugins enables to find a subgraph induced by a set of nodes.
/**
 * Let V' a subset of nodes, G' is an induced subgraph of G if all edges 
 * (and only these ones) element of G that link
 * two nodes (only one in case of loop) of V' are in G'.
 *
 *  \author David Auber, LaBRI University Bordeaux I France: 
 *   auber@tulip-software.org
 */
class InducedSubGraphSelection:public tlp::BooleanAlgorithm { 
public:
  InducedSubGraphSelection(const tlp::PropertyContext &);
  bool run();
};
/*@}*/
#endif




