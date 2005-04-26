//-*-c++-*-
#ifndef Tulip_SpanningDagSelection_H
#define Tulip_SpanningDagSelection_H

#include <tulip/TulipPlugin.h>


/** \addtogroup selection */
/*@{*/
///

class SpanningDagSelection:public Selection { 

public:
  ///
  SpanningDagSelection(const PropertyContext &);
  ///
  ~SpanningDagSelection();
  ///
  bool run();
  ///
  bool check(std::string &);
  ///
  void reset();

private :
  ///
  void dfsAcyclicTest(const node &,SelectionProxy *,SelectionProxy *);
};
/*@}*/
#endif
