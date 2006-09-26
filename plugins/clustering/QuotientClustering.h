//-*-c++-*-
#ifndef _QuotientClustering_H
#define _QuotientClustering_H

#include <tulip/TulipPlugin.h>

/** \addtogroup clustering */
/*@{*/
class QuotientClustering:public tlp::Clustering { 
public:
  QuotientClustering(tlp::ClusterContext);
  ~QuotientClustering();
  bool run();
  bool check(std::string &);
  void reset();

};
/*@}*/
#endif





