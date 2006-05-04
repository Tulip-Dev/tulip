//-*-c++-*-
#ifndef _HierarchicalClustering_H
#define _HierarchicalClustering_H

#include <list>
#include <string>

#include <tulip/TulipPlugin.h>

/** \addtogroup clustering */
/*@{*/
class HierarchicalClustering:public Clustering { 
public:
  HierarchicalClustering(ClusterContext);
  ~HierarchicalClustering();
  bool run();
  bool check(std::string &);
  void reset();
private :
  bool split (DoubleProperty*, std::list<node> &);
};
/*@}*/

#endif





