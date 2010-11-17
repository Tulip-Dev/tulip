#ifndef MQCLUSTERINGQUALITYMEASURE_H
#define MQCLUSTERINGQUALITYMEASURE_H

#include "ClusteringQualityMeasure.h"

namespace tlp {
  class ClusteringAlgorithmBase;

  class TLP_SCOPE MQClusteringQualityMeasure : public ClusteringQualityMeasure {
    public:
      MQClusteringQualityMeasure(const tlp::ClusteringAlgorithmBase* const clustering);
      virtual double getQuality();
      virtual double getQualityIfMerged(tlp::node n1, tlp::node n2);
      void removePositive(tlp::node n1, tlp::node n2, double& positive);
      void removeNegative(tlp::node n1, tlp::node n2, double& negative);
      virtual void beforeMergeNodes(tlp::node n1, tlp::node n2);
      virtual void afterMergeNodes(tlp::node n1);
      virtual void beforeSplitNode(tlp::node n);
      //oldNode is the old node, but now represents a new cluster, newnode is a new node and represents a new cluster
      virtual void afterSplitNode(tlp::node oldNode, tlp::node newNode);
      virtual void initialize();
    private:
      double _positive;
      double _negative;
  };
}

#endif //MQCLUSTERINGQUALITYMEASURE_H
