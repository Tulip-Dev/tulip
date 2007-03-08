#include <tulip/AcyclicTest.h>
#include <tulip/ForEach.h>
#include "LeafMetric.h"

DOUBLEPLUGINOFGROUP(LeafMetric,"Leaf","David Auber","20/12/1999","Alpha","1.0","Tree");

using namespace std;
using namespace tlp;

//=======================================================================
LeafMetric::LeafMetric(const PropertyContext &context):DoubleAlgorithm(context) {}
//=======================================================================
double LeafMetric::getNodeValue(const node n) {
  if (doubleResult->getNodeValue(n) != 0) 
    return doubleResult->getNodeValue(n);
  double result=0;
  node _n;
  forEach(_n, graph->getOutNodes(n)) 
    result += getNodeValue(_n);
  if (result==0) result=1.0;
  doubleResult->setNodeValue(n, result);
  return result;
}
//=======================================================================
bool LeafMetric::run() {
  doubleResult->setAllNodeValue(0);
  doubleResult->setAllEdgeValue(0);
  node n;
  forEach(n, graph->getNodes())
    doubleResult->setNodeValue(n, getNodeValue(n));
  return true;
}
//=======================================================================
bool LeafMetric::check(string &erreurMsg) {
  if (AcyclicTest::isAcyclic(graph))
    return true;
  else {
    erreurMsg="The Graph must be a Acyclic";
    return false;
  }
}
//=======================================================================



