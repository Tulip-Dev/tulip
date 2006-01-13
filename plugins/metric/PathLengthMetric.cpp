#include <tulip/AcyclicTest.h>
#include "PathLengthMetric.h"

METRICPLUGIN(PathLengthMetric,"Path Length","David Auber","15/02/2001","Alpha","0","2");

using namespace std;

PathLengthMetric::PathLengthMetric(const PropertyContext &context):Metric(context) {
}
PathLengthMetric::~PathLengthMetric() {}

double PathLengthMetric::getNodeValue(const node n) {
  if (superGraph->outdeg(n)==0) return 0.0;
  bool cached,resultBool;
  string erreurMsg;
  double result=0;
  MetricProxy *leafMetric= new MetricProxy(superGraph);
  resultBool = superGraph->computeProperty("Leaf", leafMetric, erreurMsg);
  assert (resultBool);
  Iterator<node> *itN=superGraph->getOutNodes(n);
  while (itN->hasNext()) {
    node child=itN->next();
    result += metricProxy->getNodeValue(child);
  } delete itN;
  result += leafMetric->getNodeValue(n);
  delete leafMetric;
  return result;
}

bool PathLengthMetric::check(string &erreurMsg) {
  if (AcyclicTest::isAcyclic(superGraph))
      return true;
  else {
    erreurMsg="The Graph must be acyclic";
    return false;
  }
}
