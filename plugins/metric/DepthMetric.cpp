#include <tulip/AcyclicTest.h>
#include <tulip/ForEach.h>
#include "DepthMetric.h"

DOUBLEPLUGINOFGROUP(DepthMetric,"Depth","David Auber","15/02/2001","Alpha","1.0","Tree");

using namespace std;
using namespace tlp;

DepthMetric::DepthMetric(const PropertyContext &context):DoubleAlgorithm(context) {}

//=================================================
double DepthMetric::getNodeValue(const node n) {
  if (graph->outdeg(n)==0) return 0.0;
  if (doubleResult->getNodeValue(n) > 0.1)
    return doubleResult->getNodeValue(n);
  double max = 0;
  node _n2;
  forEach(_n2, graph->getOutNodes(n))
    max = std::max(max, getNodeValue(_n2));
  doubleResult->setNodeValue(n, max + 1);
  return max + 1;
}
//====================================================================
bool DepthMetric::run() {
  doubleResult->setAllEdgeValue(0);
  doubleResult->setAllNodeValue(0);
  node _n;
  forEach(_n, graph->getNodes())
    doubleResult->setNodeValue(_n, getNodeValue(_n));
  return true;
}
//=================================================
bool DepthMetric::check(string &erreurMsg) {
  if (AcyclicTest::isAcyclic(graph))
    return true;
  else {
    erreurMsg="The Graph must be acyclic";
    return false;
  }
}
//=================================================
