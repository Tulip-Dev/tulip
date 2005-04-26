#include <deque>
#include <tulip/GraphMeasure.h>
#include "Eccentricity.h"

METRICPLUGIN(EccentricityMetric,"Eccentricity","Auber/Munzner","18/06/2004","Alpha","0","1");

using namespace std;

EccentricityMetric::EccentricityMetric(const PropertyContext &context):Metric(context) {
}

EccentricityMetric::~EccentricityMetric() {
}

bool EccentricityMetric::run() {
  Iterator<node> *itN= superGraph->getNodes();
  for (unsigned int i=0; itN->hasNext(); ++i) {
    if (pluginProgress->progress(i,superGraph->numberOfNodes())!=TLP_CONTINUE) break;
    node n = itN->next();
    MutableContainer<unsigned int> distance;
    double max = tlp::maxDistance(superGraph, n, distance, 2);
    if (0 == max)
      metricProxy->setNodeValue(n,0);
    else
      metricProxy->setNodeValue(n, 1.0/max);
  } delete itN;
  return pluginProgress->state()!=TLP_CANCEL;
}
