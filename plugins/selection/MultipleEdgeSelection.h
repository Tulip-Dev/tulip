//-*-c++-*-
#ifndef Tulip_ParaSelection_H
#define Tulip_ParaSelection_H

#include <tulip/TulipPlugin.h>
/** \addtogroup selection */
/*@{*/
///
class MultipleEdgeSelection:public Selection
{ 
public:
  ///
  MultipleEdgeSelection(const PropertyContext &);
  ///
  ~MultipleEdgeSelection();
  ///
  bool run();
};
/*@}*/
#endif




