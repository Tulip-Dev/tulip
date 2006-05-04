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
  double computeMQValue(const std::vector< std::set<node> > & partition, Graph *);
  double findBestThreshold(int numberOfSteps);
  Graph* buildSubGraphs(const std::vector< std::set<node > > &);
  void recursiveCall(Graph *, std::map<Graph *,Graph *> &);
  Graph* buildQuotientGraph(Graph *);
  void adjustMetaGraphProtperty(Graph *, std::map<Graph *,Graph *> &);
  DoubleProperty* values;
};
/*@}*/
#endif
