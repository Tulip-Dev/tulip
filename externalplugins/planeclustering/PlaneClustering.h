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

class PlaneClustering : public Clustering 
{ 
 public:

  PlaneClustering(ClusterContext);
  ~PlaneClustering();

  bool run();
  bool check(std::string &);
  void reset();

 private :

  Plane plane;
};
/*@}*/

#endif





