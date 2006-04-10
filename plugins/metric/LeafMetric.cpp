#include <tulip/AcyclicTest.h>
#include "LeafMetric.h"

METRICPLUGIN(LeafMetric,"Leaf","David Auber","20/12/1999","Alpha","0","1");

using namespace std;

//=======================================================================
LeafMetric::LeafMetric(const PropertyContext &context):Metric(context) {}
//=======================================================================
double LeafMetric::getNodeValue(const node n) {
  double result=0;
  Iterator<node> *itN=superGraph->getOutNodes(n);
  while(itN->hasNext()) {
    result+=metricProxy->getNodeValue(itN->next());
  } delete itN;
  if (result==0) result=1.0;
  return result;
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



