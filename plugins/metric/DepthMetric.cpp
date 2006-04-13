#include <tulip/AcyclicTest.h>
#include <tulip/ForEach.h>
#include "DepthMetric.h"

METRICPLUGIN(DepthMetric,"Depth","David Auber","15/02/2001","Alpha","0","2");

using namespace std;

DepthMetric::DepthMetric(const PropertyContext &context):MetricAlgorithm(context) {}

//=================================================
double DepthMetric::getNodeValue(const node n) {
  if (superGraph->outdeg(n)==0) return 0.0;
  if (metricResult->getNodeValue(n) != 0)
    return metricResult->getNodeValue(n);
  double max = 0;
  node _n;
  forEach(_n, superGraph->getOutNodes(n))
    max = std::max(max, getNodeValue(_n));
  metricResult->setNodeValue(_n, max + 1);
  return max + 1;
}
//====================================================================
bool DepthMetric::run() {
  metricResult->setAllEdgeValue(0);
  metricResult->setAllNodeValue(0);
  node _n;
  forEach(_n, superGraph->getNodes())
    getNodeValue(_n);
  return true;
}
//=================================================
bool DepthMetric::check(string &erreurMsg) {
  if (AcyclicTest::isAcyclic(superGraph))
    return true;
  else {
    erreurMsg="The Graph must be acyclic";
    return false;
  }
}
//=================================================
