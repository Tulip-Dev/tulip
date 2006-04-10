#include "ArityMetric.h"

METRICPLUGIN(ArityMetric,"Degree","David Auber","04/10/2001","Stable","0","1");

ArityMetric::ArityMetric(const PropertyContext &context):Metric(context) {
}

ArityMetric::~ArityMetric() {}

double ArityMetric::getNodeValue(const node n ) {
  return superGraph->deg(n);
}




