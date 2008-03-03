//-*-c++-*-
#ifndef Tulip_SPANNINGTREESELECTION_H
#define Tulip_SPANNINGTREESELECTION_H

#include <tulip/TulipPlugin.h>

/** \addtogroup selection */
/*@{*/

/**
 * This selection plugins enables to find a subgraph of G that is a forest (a set of trees).
 *
 *  \author David Auber, LaBRI University Bordeaux I France: 
 *   auber@tulip-software.org
 */
class SpanningTreeSelection:public tlp::BooleanAlgorithm { 
public:
  SpanningTreeSelection(const tlp::PropertyContext &);
  ~SpanningTreeSelection();
  bool run();
};
/*@}*/
#endif
