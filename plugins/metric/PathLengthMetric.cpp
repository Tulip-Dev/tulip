#include <tulip/AcyclicTest.h>
#include <tulip/ForEach.h>
#include "PathLengthMetric.h"

METRICPLUGIN(PathLengthMetric,"Path Length","David Auber","15/02/2001","Alpha","0","2");

using namespace std;

//=======================================
PathLengthMetric::PathLengthMetric(const PropertyContext &context):Metric(context) {
}
//=======================================
double PathLengthMetric::getNodeValue(const node n) {
  if (superGraph->outdeg(n)==0) return 0.0;
  if (metricProxy->getNodeValue(n)!=0)
    return metricProxy->getNodeValue(n);
  double result=0;
  node child;
  forEach(child, superGraph->getOutNodes(n)) {
    result += metricProxy->getNodeValue(child);
  }
  result += leafMetric->getNodeValue(n);
  metricProxy->setNodeValue(n, result);
  return result;
}
//=======================================
bool PathLengthMetric::run() {
  metricProxy->setAllNodeValue(0);
  metricProxy->setAllEdgeValue(0);
  leafMetric = new MetricProxy(superGraph);
  string erreurMsg;
  if (!superGraph->computeProperty("Leaf", leafMetric, erreurMsg)) {
    cerr << erreurMsg << endl;
    return false;
  }
  node _n;
  forEach(_n, superGraph->getNodes())
    getNodeValue(_n);
  delete leafMetric;
}
//=======================================
bool PathLengthMetric::check(string &erreurMsg) {
  if (AcyclicTest::isAcyclic(superGraph))
    return true;
  else {
    erreurMsg="The Graph must be acyclic";
    return false;
  }
}
//=======================================
