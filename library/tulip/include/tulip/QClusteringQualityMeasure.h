#ifndef QCLUSTERINGQUALITYMEASURE_H
#define QCLUSTERINGQUALITYMEASURE_H

#include "ClusteringQualityMeasure.h"

namespace tlp {
  class ClusteringAlgorithmBase;

  class TLP_SCOPE QClusteringQualityMeasure : public ClusteringQualityMeasure {
    public:
      QClusteringQualityMeasure(const tlp::ClusteringAlgorithmBase* const clustering);
      virtual double getQuality();
      virtual double getQualityIfMerged(tlp::node n1, tlp::node n2);
      virtual void initialize();
      virtual void removeFromQ(tlp::node n1, double &measure);

    private:
      double _sumEdges;  
      double _q;
  };
}

#endif //QCLUSTERINGQUALITYMEASURE_H
