#include <tulip/AcyclicTest.h>
#include <tulip/ForEach.h>
#include "LeafMetric.h"

DOUBLEPLUGIN(LeafMetric,"Leaf","David Auber","20/12/1999","Alpha","0","1");

using namespace std;

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



