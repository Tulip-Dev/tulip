#include <tulip/AcyclicTest.h>
#include "DepthMetric.h"

METRICPLUGIN(DepthMetric,"Depth","David Auber","15/02/2001","Alpha","0","2");

using namespace std;

DepthMetric::DepthMetric(const PropertyContext &context):Metric(context) {}

//=================================================
double DepthMetric::getNodeValue(const node n) {
  if (superGraph->outdeg(n)==0) return 0.0;
  double max = 0;
  Iterator<node> *itN=superGraph->getOutNodes(n);
  while (itN->hasNext()) {
    max = std::max(max, metricProxy->getNodeValue(itN->next()));
  } delete itN;
  return max + 1;
}
//=================================================
bool DepthMetric::check(string &erreurMsg) {
  if (AcyclicTest::isAcyclic(superGraph))
      return true;
  else {
    erreurMsg="The Graph must be acyclic";
    return false;
  }
}
//=================================================






