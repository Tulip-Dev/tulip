#include <tulip/ForEach.h>
#include "ArityMetric.h"

METRICPLUGIN(ArityMetric,"Degree","David Auber","04/10/2001","Stable","0","1");

ArityMetric::ArityMetric(const PropertyContext &context):Metric(context) {
}
//==================================================================
bool ArityMetric::run() {
  node n;
  forEach(n, superGraph->getNodes())
    metricProxy->setNodeValue(n, superGraph->deg(n));
  metricProxy->setAllEdgeValue(0);
  return true;
}
//==================================================================




