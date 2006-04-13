#include <tulip/ForEach.h>
#include "ArityMetric.h"

METRICPLUGIN(ArityMetric,"Degree","David Auber","04/10/2001","Stable","0","1");

ArityMetric::ArityMetric(const PropertyContext &context):MetricAlgorithm(context) {
}
//==================================================================
bool ArityMetric::run() {
  node n;
  forEach(n, superGraph->getNodes())
    metricResult->setNodeValue(n, superGraph->deg(n));
  metricResult->setAllEdgeValue(0);
  return true;
}
//==================================================================




