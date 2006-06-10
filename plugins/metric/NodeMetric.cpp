#include <tulip/AcyclicTest.h>
#include <tulip/ForEach.h>
#include "NodeMetric.h"

METRICPLUGINOFGROUP(NodeMetric,"Node","David Auber","20/12/1999","Alpha","0","1","Tree");

using namespace std;

//====================================================================
NodeMetric::NodeMetric(const PropertyContext &context):Metric(context) 
{}
//====================================================================
double NodeMetric::getNodeValue(const node n) {
  if (metricProxy->getNodeValue(n)!=0)
    return metricProxy->getNodeValue(n);
  double result = 1;
  node _n;
  forEach(_n, superGraph->getOutNodes(n))
    result += getNodeValue(_n);
  metricProxy->setNodeValue(n, result);
  return result;
}
//====================================================================
bool NodeMetric::run() {
  metricProxy->setAllEdgeValue(0);
  metricProxy->setAllNodeValue(0);
  node _n;
  forEach(_n, superGraph->getNodes())
    metricProxy->setNodeValue(_n,getNodeValue(_n));
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
