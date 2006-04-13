#include <tulip/AcyclicTest.h>
#include <tulip/ForEach.h>
#include "NodeMetric.h"

METRICPLUGIN(NodeMetric,"Node","David Auber","20/12/1999","Alpha","0","1");

using namespace std;

//====================================================================
NodeMetric::NodeMetric(const PropertyContext &context):MetricAlgorithm(context) 
{}
//====================================================================
double NodeMetric::getNodeValue(const node n) {
  if (metricResult->getNodeValue(n)!=0)
    return metricResult->getNodeValue(n);
  double result = 1;
  node _n;
  forEach(_n, superGraph->getOutNodes(n))
    result += getNodeValue(_n);
  metricResult->setNodeValue(n, result);
  return result;
}
//====================================================================
bool NodeMetric::run() {
  metricResult->setAllEdgeValue(0);
  metricResult->setAllNodeValue(0);
  node _n;
  forEach(_n, superGraph->getNodes())
    metricResult->setNodeValue(_n,getNodeValue(_n));
  return true;
}
//====================================================================
bool NodeMetric::check(string &erreurMsg) {
  if (AcyclicTest::isAcyclic(superGraph)) {
    erreurMsg="";
    return true;
  }
  else  {
    erreurMsg="The Graph must be acyclic";
    return false;
  }
}
//====================================================================
