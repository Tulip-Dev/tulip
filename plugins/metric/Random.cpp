#include "Random.h"

METRICPLUGIN(RandomMetric,"Random","David Auber","04/10/2001","Alpha","0","1")

RandomMetric::RandomMetric(const PropertyContext &context):Metric(context) 
{}

RandomMetric::~RandomMetric() {}

double RandomMetric::getNodeValue(const node n ) {
  return (double)rand();
}

