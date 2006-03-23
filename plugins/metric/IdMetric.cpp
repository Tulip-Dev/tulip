#include <tulip/ForEach.h>

#include "IdMetric.h"

METRICPLUGIN(IdMetric,"Id","David Auber","06/04/2000","Alpha","0","1");

//==================================================================
IdMetric::IdMetric(const PropertyContext &context):Metric(context) {
}
//==================================================================
bool IdMetric::run() {
  node n;
  forEach(n, superGraph->getNodes())
    metricProxy->setNodeValue(n, n.id);
  edge e;
  forEach(e, superGraph->getEdges())
    metricProxy->setEdgeValue(e, e.id);
  return true;
}
//=================================================================
