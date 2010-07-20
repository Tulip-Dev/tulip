#ifndef QCLUSTERINGQUALITYMEASURE_H
#define QCLUSTERINGQUALITYMEASURE_H

#include "ClusteringQualityMeasure.h"

namespace tlp {
  class ClusteringAlgorithmBase;

  class QClusteringQualityMeasure : public ClusteringQualityMeasure {
    public:
      QClusteringQualityMeasure(const tlp::ClusteringAlgorithmBase* const clustering);
      virtual double getQuality();
      virtual double getQualityIfMerged(tlp::node n1, tlp::node n2);
      virtual void initialize();
    private:
      double _sumEdges;  
  };
}

#endif //QCLUSTERINGQUALITYMEASURE_H
