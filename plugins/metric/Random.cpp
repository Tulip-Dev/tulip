#include <tulip/ForEach.h>
#include "Random.h"

METRICPLUGIN(RandomMetric,"Random","David Auber","04/10/2001","Alpha","0","1")

//===========================================
RandomMetric::RandomMetric(const PropertyContext &context):MetricAlgorithm(context) 
{}
//===========================================
bool RandomMetric::run() {
  node n;
  forEach(n, superGraph->getNodes())
    metricObj->setNodeValue(n, (double)rand() / double(RAND_MAX));
  edge e;
  forEach(e, superGraph->getEdges())
    metricObj->setEdgeValue(e, (double)rand() / double(RAND_MAX));
  return true;
}
//===========================================
