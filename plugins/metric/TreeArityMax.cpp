#include <tulip/AcyclicTest.h>
#include <tulip/ForEach.h>
#include "TreeArityMax.h"

METRICPLUGIN(TreeArityMax,"TreeArityMax","David Auber","20/12/1999","Alpha","0","1");

using namespace std;

TreeArityMax::TreeArityMax(const PropertyContext &context):MetricAlgorithm(context) 
{}
//======================================================
double TreeArityMax::getNodeValue(const node n) {
  if (superGraph->outdeg(n) == 0) return 0;
  if (metricResult->getNodeValue(n) != 0) 
    return metricResult->getNodeValue(n);
  
  double result = superGraph->outdeg(n);
  node _n;
  forEach(_n, superGraph->getOutNodes(n)) {
    if (getNodeValue(_n) > result)
      result = getNodeValue(_n);
  }
  
  metricResult->setNodeValue(n, result);
  return result;
}
//======================================================
bool TreeArityMax::run() {
  metricResult->setAllEdgeValue(0);
  metricResult->setAllNodeValue(0);
  node n;
  forEach(n, superGraph->getNodes()) {
    metricResult->setNodeValue(n, getNodeValue(n));
  }
  return true;
}
//======================================================
bool TreeArityMax::check(string &erreurMsg) {
   if (AcyclicTest::isAcyclic(superGraph)) {
     erreurMsg="";
     return true;
   }
   else {
     erreurMsg="The Graph must be Acyclic";
     return false;
   } 
}







