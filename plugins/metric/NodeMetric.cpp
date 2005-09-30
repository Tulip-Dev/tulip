#include <tulip/AcyclicTest.h>
#include "NodeMetric.h"

METRICPLUGIN(NodeMetric,"Node","David Auber","20/12/1999","Alpha","0","1");

using namespace std;

//====================================================================
NodeMetric::NodeMetric(const PropertyContext &context):Metric(context) 
{}
//====================================================================
double NodeMetric::getNodeValue(const node n) {
  double result = 1;
  Iterator<node> *itN = superGraph->getOutNodes(n);
  while (itN->hasNext()) {
    node itn=itN->next();
    result += metricProxy->getNodeValue(itn);
  } delete itN; 
  return result;
}
//====================================================================
bool NodeMetric::check(string &erreurMsg) {
  if (AcyclicTest::isAcyclic(superGraph)) {
    erreurMsg="";
    return true;
  }
  else  {
    erreurMsg="The Graph must be acyclic";
    return false;
  }
}
//====================================================================
