#include <tulip/AcyclicTest.h>
#include <tulip/ForEach.h>
#include "PathLengthMetric.h"

METRICPLUGIN(PathLengthMetric,"Path Length","David Auber","15/02/2001","Alpha","0","2");

using namespace std;

//=======================================
PathLengthMetric::PathLengthMetric(const PropertyContext &context):MetricAlgorithm(context) {
}
//=======================================
double PathLengthMetric::getNodeValue(const node n) {
  if (superGraph->outdeg(n)==0) return 0.0;
  if (metricResult->getNodeValue(n)!=0)
    return metricResult->getNodeValue(n);
  double result=0;
  node child;
  forEach(child, superGraph->getOutNodes(n)) {
    result += metricResult->getNodeValue(child);
  }
  result += leafMetric->getNodeValue(n);
  metricResult->setNodeValue(n, result);
  return result;
}
//=======================================
bool PathLengthMetric::run() {
  metricResult->setAllNodeValue(0);
  metricResult->setAllEdgeValue(0);
  leafMetric = new Metric(superGraph);
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
