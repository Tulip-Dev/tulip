//-*-c++-*-
#ifndef _StrengthClustering_H
#define _StrengthClustering_H

#include <string>
#include <set>
#include <vector>
#include <map>
#include <tulip/TulipPlugin.h>

class Metric;

/** \addtogroup clustering */
/*@{*/
class StrengthClustering:public Clustering { 
public:
  StrengthClustering(ClusterContext);
  ~StrengthClustering();
  bool run();
  bool check(std::string &);
  void reset();
private:
  std::vector< std::set<node> > computeNodePartition(double threshold);
  double computeMQValue(const std::vector< std::set<node> > & partition, SuperGraph *);
  double findBestThreshold(int numberOfSteps);
  SuperGraph* buildSubGraphs(const std::vector< std::set<node > > &);
  void recursiveCall(SuperGraph *, std::map<SuperGraph *,SuperGraph *> &);
  SuperGraph* buildQuotientGraph(SuperGraph *graph);
  void adjustMetaGraphProtperty(SuperGraph *, std::map<SuperGraph *,SuperGraph *> &);
  Metric* values;
};
/*@}*/
#endif
