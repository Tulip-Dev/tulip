#include <tulip/ForEach.h>
#include "Random.h"

METRICPLUGINOFGROUP(RandomMetric,"Random","David Auber","04/10/2001","Alpha","0","1","Misc")

//===========================================
RandomMetric::RandomMetric(const PropertyContext &context):Metric(context) 
{}
//===========================================
bool RandomMetric::run() {
  node n;
  forEach(n, superGraph->getNodes())
    metricProxy->setNodeValue(n, (double)rand() / double(RAND_MAX));
  edge e;
  forEach(e, superGraph->getEdges())
    metricProxy->setEdgeValue(e, (double)rand() / double(RAND_MAX));
  return true;
}
//===========================================
