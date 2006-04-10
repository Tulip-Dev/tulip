#include <tulip/MetricProxy.h>

#include "IdMetric.h"

METRICPLUGIN(IdMetric,"Id","David Auber","06/04/2000","Alpha","0","1");

IdMetric::IdMetric(const PropertyContext &context):Metric(context) {
}
double IdMetric::getEdgeValue(const edge e ) {
  return e.id;
}
double IdMetric::getNodeValue(const node n ) {
  return n.id;
}




