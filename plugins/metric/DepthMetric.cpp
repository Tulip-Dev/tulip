#include <tulip/AcyclicTest.h>
#include <tulip/ForEach.h>
#include "DepthMetric.h"

METRICPLUGINOFGROUP(DepthMetric,"Depth","David Auber","15/02/2001","Alpha","0","2","Tree");

using namespace std;

DepthMetric::DepthMetric(const PropertyContext &context):Metric(context) {}

//=================================================
double DepthMetric::getNodeValue(const node n) {
  if (superGraph->outdeg(n)==0) return 0.0;
  if (metricProxy->getNodeValue(n) != 0)
    return metricProxy->getNodeValue(n);
  double max = 0;
  node _n;
  forEach(_n, superGraph->getOutNodes(n))
    max = std::max(max, getNodeValue(_n));
  metricProxy->setNodeValue(_n, max + 1);
  return max + 1;
}
//====================================================================
bool DepthMetric::run() {
  metricProxy->setAllEdgeValue(0);
  metricProxy->setAllNodeValue(0);
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
