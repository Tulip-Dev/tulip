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
  if (metricObj->getNodeValue(n) != 0) 
    return metricObj->getNodeValue(n);
  
  double result = superGraph->outdeg(n);
  node _n;
  forEach(_n, superGraph->getOutNodes(n)) {
    if (getNodeValue(_n) > result)
      result = getNodeValue(_n);
  }
  
  metricObj->setNodeValue(n, result);
  return result;
}
//======================================================
bool TreeArityMax::run() {
  metricObj->setAllEdgeValue(0);
  metricObj->setAllNodeValue(0);
  node n;
  forEach(n, superGraph->getNodes()) {
    metricObj->setNodeValue(n, getNodeValue(n));
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







