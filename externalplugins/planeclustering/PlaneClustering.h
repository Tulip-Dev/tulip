//-*-c++-*-
#ifndef _PLANECLUSTERING_H
#define _PLANECLUSTERING_H

#include <list>
#include <string>

// the header below is not public
#include "../../library/tulip/include/tulip/Plane.h"
#include <tulip/TulipPlugin.h>

/** \addtogroup clustering */
/*@{*/

class PlaneClustering : public tlp::Algorithm 
{ 
 public:

  PlaneClustering(tlp::AlgorithmContext);
  ~PlaneClustering();

  bool run();
  bool check(std::string &);
  void reset();

 private :

  tlp::Plane plane;
};
/*@}*/

#endif





