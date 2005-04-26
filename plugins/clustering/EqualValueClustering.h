//-*-c++-*-
#ifndef _EqualValueClustering_H
#define _EqualValueClustering_H

#include <tulip/TulipPlugin.h>

class EqualValueClustering:public Clustering { 
public:
  EqualValueClustering(ClusterContext);
  bool run();
};

#endif





