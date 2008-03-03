#include "NodeMetric.h"

DOUBLEPLUGINOFGROUP(NodeMetric,"Node","David Auber","20/12/1999","Alpha","1.0","Tree");

using namespace std;
using namespace tlp;

//====================================================================
NodeMetric::NodeMetric(const PropertyContext &context):DoubleAlgorithm(context) 
{}
//====================================================================
double NodeMetric::getNodeValue(const node n) {
  if (doubleResult->getNodeValue(n)!=0)
    return doubleResult->getNodeValue(n);
  double result = 1;
  node _n;
  forEach(_n, graph->getOutNodes(n))
    result += getNodeValue(_n);
  doubleResult->setNodeValue(n, result);
  return result;
}
//====================================================================
bool NodeMetric::run() {
  doubleResult->setAllEdgeValue(0);
  doubleResult->setAllNodeValue(0);
  node _n;
  forEach(_n, graph->getNodes())
    doubleResult->setNodeValue(_n,getNodeValue(_n));
  return true;
}
//====================================================================
bool NodeMetric::check(string &erreurMsg) {
  if (AcyclicTest::isAcyclic(graph)) {
    erreurMsg="";
    return true;
  }
  else  {
    erreurMsg="The Graph must be acyclic";
    return false;
  }
}
//====================================================================
