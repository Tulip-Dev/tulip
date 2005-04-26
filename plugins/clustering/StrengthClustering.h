//-*-c++-*-
#ifndef _StrengthClustering_H
#define _StrengthClustering_H

#include <string>
#include <set>
#include <vector>
#include <map>
#include <tulip/TulipPlugin.h>

class MetricProxy;

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
  double e(std::set<node> &U);
  double e(std::set<node> &U,std::set<node> &V);
  double s(std::set<node> &U,std::set<node> &V);
  double s(std::set<node> &U);
  std::vector< std::set<node> > computeNodePartition(double threshold);
  double computeMQValue(std::vector< std::set<node> > partition);
  double findBestThreshold(int numberOfSteps);
  SuperGraph* buildSubGraphs(const std::vector< std::set<node > > &);
  void recursiveCall(SuperGraph *, std::map<SuperGraph *,SuperGraph *> &);
  SuperGraph* buildQuotientGraph(SuperGraph *graph);
  void adjustMetaGraphProtperty(SuperGraph *, std::map<SuperGraph *,SuperGraph *> &);
  MetricProxy * values;
};
/*@}*/
#endif
