//-*-c++-*-
#ifndef _PLANECLUSTERING_H
#define _PLANECLUSTERING_H

#include <list>
#include <string>

#include <tulip/Plane.h>
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

  tlp::Plane plane;
};
/*@}*/

#endif





