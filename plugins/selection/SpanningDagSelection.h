//-*-c++-*-
#ifndef Tulip_SpanningDagSelection_H
#define Tulip_SpanningDagSelection_H

#include <tulip/TulipPlugin.h>

template<typename T>
class MutableConatainer;

/** \addtogroup selection */
/*@{*/

/**
 * This selection plugins enables to find a subgraph of G that is acyclic.
 *
 *  \author David Auber, LaBRI University Bordeaux I France: 
 *   auber@tulip-software.org
 */
class SpanningDagSelection:public tlp::BooleanAlgorithm { 

public:
  SpanningDagSelection(const tlp::PropertyContext &);
  bool run();
};
/*@}*/
#endif
