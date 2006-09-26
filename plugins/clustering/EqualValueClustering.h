//-*-c++-*-
#ifndef _EqualValueClustering_H
#define _EqualValueClustering_H

#include <tulip/TulipPlugin.h>

class EqualValueClustering:public tlp::Clustering { 
public:
  EqualValueClustering(tlp::ClusterContext);
  bool run();
};

#endif





