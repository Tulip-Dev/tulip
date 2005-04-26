//-*-c++-*-
#ifndef Tulip_InducedSubGraphSelection_H
#define Tulip_InducedSubGraphSelection_H

#include <tulip/TulipPlugin.h>

class InducedSubGraphSelection:public Selection { 
public:
  InducedSubGraphSelection(const PropertyContext &);
  bool run();
};

#endif




