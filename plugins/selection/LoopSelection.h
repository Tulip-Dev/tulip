//-*-c++-*-
#ifndef Tulip_LoopSelection_H
#define Tulip_LoopSelection_H

#include <tulip/TulipPlugin.h>

/** \addtogroup selection */
/*@{*/
///
class LoopSelection:public Selection { 
public:
  ///
  LoopSelection(const PropertyContext &);
  ///
  ~LoopSelection();
  ///
  bool getEdgeValue(edge e);
};
/*@}*/
#endif




