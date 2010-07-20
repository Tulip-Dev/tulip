#include <tulip/ClusteringQualityMeasure.h>

#include "tulip/Graph.h"
#include <tulip/DoubleProperty.h>
#include <tulip/ForEach.h>

using namespace tlp;

ClusteringQualityMeasure::ClusteringQualityMeasure(const ClusteringAlgorithmBase* const clustering) 
  :_clusteringAlgorithm(clustering) {
}
