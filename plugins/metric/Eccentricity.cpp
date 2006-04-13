#include <deque>
#include <tulip/GraphMeasure.h>
#include <tulip/ForEach.h>
#include "Eccentricity.h"

METRICPLUGIN(EccentricityMetric,"Eccentricity","Auber/Munzner","18/06/2004","Alpha","0","1");

using namespace std;

namespace {
  const char * paramHelp[] = {
    // property
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "bool" ) \
    HTML_HELP_DEF( "values", "[true , false]" ) \
    HTML_HELP_DEF( "default", "false" ) \
    HTML_HELP_BODY() \
    "If true the eccentricity will be computed using all path between a node to the other (not only the maximum one)"\
    HTML_HELP_CLOSE(),
  };
}

EccentricityMetric::EccentricityMetric(const PropertyContext &context):MetricAlgorithm(context) {
  addParameter<bool>("all paths",paramHelp[0],"false");
}

EccentricityMetric::~EccentricityMetric() {
}

bool EccentricityMetric::run() {
  bool allPaths = false;
  if (dataSet!=0) dataSet->get("all paths", allPaths);
  double maxV = 0;
  double minV = DBL_MAX;
  Iterator<node> *itN= superGraph->getNodes();
  for (unsigned int i=0; itN->hasNext(); ++i) {
    if (pluginProgress->progress(i,superGraph->numberOfNodes())!=TLP_CONTINUE) break;
    node n = itN->next();
    MutableContainer<unsigned int> distance;
    double val = tlp::maxDistance(superGraph, n, distance, 2);
    if (allPaths) {
      node n2;
      val = 0;
      forEach(n2, superGraph->getNodes()) 
	val += double(distance.get(n2.id)) / double(superGraph->numberOfNodes()) ;
    }
    metricResult->setNodeValue(n, val);
    maxV = std::max(maxV, val);
    minV = std::min(minV, val);
  } delete itN;
  if (maxV>0) {
    node n;
    forEach(n, superGraph->getNodes()) {
      double val = maxV - metricResult->getNodeValue(n); //shift to zero
      double newMax = maxV - minV;
      metricResult->setNodeValue(n, val / newMax);
    }
  }
  return pluginProgress->state()!=TLP_CANCEL;
}
