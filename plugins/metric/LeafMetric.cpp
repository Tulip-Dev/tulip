#include <tulip/AcyclicTest.h>
#include <tulip/ForEach.h>
#include "LeafMetric.h"

METRICPLUGINOFGROUP(LeafMetric,"Leaf","David Auber","20/12/1999","Alpha","0","1","Tree");

using namespace std;

//=======================================================================
LeafMetric::LeafMetric(const PropertyContext &context):Metric(context) {}
//=======================================================================
double LeafMetric::getNodeValue(const node n) {
  if (metricProxy->getNodeValue(n) != 0) 
    return metricProxy->getNodeValue(n);
  double result=0;
  node _n;
  forEach(_n, superGraph->getOutNodes(n)) 
    result += getNodeValue(_n);
  if (result==0) result=1.0;
  metricProxy->setNodeValue(n, result);
  return result;
}
//=======================================================================
bool LeafMetric::run() {
  metricProxy->setAllNodeValue(0);
  metricProxy->setAllEdgeValue(0);
  node n;
  forEach(n, superGraph->getNodes())
    metricProxy->setNodeValue(n, getNodeValue(n));
  
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



