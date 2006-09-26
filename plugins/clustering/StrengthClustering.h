//-*-c++-*-
#ifndef _StrengthClustering_H
#define _StrengthClustering_H

#include <string>
#include <set>
#include <vector>
#include <map>
#include <tulip/TulipPlugin.h>

/** \addtogroup clustering */
/*@{*/
class StrengthClustering:public tlp::Clustering { 
public:
  StrengthClustering(tlp::ClusterContext);
  ~StrengthClustering();
  bool run();
  bool check(std::string &);
  void reset();
private:
  std::vector< std::set<tlp::node> > computeNodePartition(double threshold);
  double computeMQValue(const std::vector< std::set<tlp::node> > & partition, tlp::Graph *);
  double findBestThreshold(int numberOfSteps);
  tlp::Graph* buildSubGraphs(const std::vector< std::set<tlp::node > > &);
  void recursiveCall(tlp::Graph *, std::map<tlp::Graph *, tlp::Graph *> &);
  tlp::Graph* buildQuotientGraph(tlp::Graph *);
  void adjustMetaGraphProtperty(tlp::Graph *, std::map<tlp::Graph *, tlp::Graph *> &);
  tlp::DoubleProperty* values;
};
/*@}*/
#endif
