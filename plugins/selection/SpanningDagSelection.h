//-*-c++-*-
#ifndef Tulip_SpanningDagSelection_H
#define Tulip_SpanningDagSelection_H

#include <tulip/TulipPlugin.h>


/** \addtogroup selection */
/*@{*/

/// This selection plugins enables to find a subgraph of G that is acyclic.
/**
 * This selection plugins enables to find a subgraph of G that is acyclic.
 *
 *  \author David Auber, LaBRI University Bordeaux I France: 
 *   auber@tulip-software.org
 */
class SpanningDagSelection:public Selection { 

public:
  SpanningDagSelection(const PropertyContext &);
  ~SpanningDagSelection();
  bool run();
  bool check(std::string &);
  void reset();

private :
  void dfsAcyclicTest(const node &,SelectionProxy *,SelectionProxy *);
};
/*@}*/
#endif
