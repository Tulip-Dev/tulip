//-*-c++-*-
#ifndef Tulip_REACHABLESUBGRAPHELECTION_H
#define Tulip_REACHABLESUBGRAPHELECTION_H

#include <tulip/TulipPlugin.h>

struct ReachableSubGraphSelection:public Selection { 
  ReachableSubGraphSelection(const PropertyContext &);
  ~ReachableSubGraphSelection();
  bool run();
};
#endif



