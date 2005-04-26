//-*-c++-*-
#ifndef Tulip_SPANNINGTREESELECTION_H
#define Tulip_SPANNINGTREESELECTION_H

#include <tulip/TulipPlugin.h>

///
class SpanningTreeSelection:public Selection
{ 


public:
  ///
  SpanningTreeSelection(const PropertyContext &);
  ///
  ~SpanningTreeSelection();
  ///
  bool run();
};

#endif
