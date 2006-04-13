#include <tulip/AcyclicTest.h>
#include <tulip/ForEach.h>
#include "LeafMetric.h"

METRICPLUGIN(LeafMetric,"Leaf","David Auber","20/12/1999","Alpha","0","1");

using namespace std;

//=======================================================================
LeafMetric::LeafMetric(const PropertyContext &context):MetricAlgorithm(context) {}
//=======================================================================
double LeafMetric::getNodeValue(const node n) {
  if (metricResult->getNodeValue(n) != 0) 
    return metricResult->getNodeValue(n);
  double result=0;
  node _n;
  forEach(_n, superGraph->getOutNodes(n)) 
    result += getNodeValue(_n);
  if (result==0) result=1.0;
  metricResult->setNodeValue(n, result);
  return result;
}
//=======================================================================
bool LeafMetric::run() {
  metricResult->setAllNodeValue(0);
  metricResult->setAllEdgeValue(0);
  node n;
  forEach(n, superGraph->getNodes())
    metricResult->setNodeValue(n, getNodeValue(n));
  
}
//=======================================================================
bool LeafMetric::check(string &erreurMsg) {
  if (AcyclicTest::isAcyclic(superGraph))
    return true;
  else {
    erreurMsg="The Graph must be a Acyclic";
    return false;
  }
}
//=======================================================================



