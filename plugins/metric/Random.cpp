#include "Random.h"

METRICPLUGIN(RandomMetric,"Random","David Auber","04/10/2001","Alpha","0","1")

RandomMetric::RandomMetric(const PropertyContext &context):Metric(context) 
{}

double RandomMetric::getNodeValue(const node n ) {
  return (double)rand() / double(RAND_MAX);
}

double RandomMetric::getEdgeValue(const edge e ) {
  return (double)rand() / double(RAND_MAX);
}
