#ifndef CLUSTERINGQUALITYMEASURE_H
#define CLUSTERINGQUALITYMEASURE_H
#include "tulip/Node.h"

namespace tlp {
  class Graph;
  class DoubleProperty;
  class ClusteringAlgorithmBase;

  class ClusteringQualityMeasure {
    public:
      ClusteringQualityMeasure(const tlp::ClusteringAlgorithmBase* const clustering);
      virtual double getQuality() = 0;
      virtual double getQualityIfMerged(tlp::node n1, tlp::node n2) = 0;
      virtual void beforeMergeNodes(tlp::node, tlp::node) {}
      virtual void afterMergeNodes(tlp::node) {}
      virtual void initialize()  {};
    protected:
      const tlp::ClusteringAlgorithmBase* const _clusteringAlgorithm;
  };
}
#endif //CLUSTERINGQUALITYMEASURE_H
