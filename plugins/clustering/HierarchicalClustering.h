//-*-c++-*-
#ifndef _HierarchicalClustering_H
#define _HierarchicalClustering_H

#include <list>
#include <string>

#include <tulip/TulipPlugin.h>

/** \addtogroup clustering */
/*@{*/
class HierarchicalClustering:public tlp::Clustering { 
public:
  HierarchicalClustering(tlp::ClusterContext);
  ~HierarchicalClustering();
  bool run();
  bool check(std::string &);
  void reset();
private :
  bool split (tlp::DoubleProperty*, std::list<tlp::node> &);
};
/*@}*/

#endif





