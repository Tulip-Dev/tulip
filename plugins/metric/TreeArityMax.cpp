#include <tulip/AcyclicTest.h>
#include <tulip/ForEach.h>
#include "TreeArityMax.h"

DOUBLEPLUGINOFGROUP(TreeArityMax,"TreeArityMax","David Auber","20/12/1999","Alpha","0","1","Tree");

using namespace std;
using namespace tlp;

TreeArityMax::TreeArityMax(const PropertyContext &context):DoubleAlgorithm(context) 
{}
//======================================================
double TreeArityMax::getNodeValue(const node n) {
  if (graph->outdeg(n) == 0) return 0;
  if (doubleResult->getNodeValue(n) != 0) 
    return doubleResult->getNodeValue(n);
  
  double result = graph->outdeg(n);
  node _n;
  forEach(_n, graph->getOutNodes(n)) {
    if (getNodeValue(_n) > result)
      result = getNodeValue(_n);
  }
  
  doubleResult->setNodeValue(n, result);
  return result;
}
//======================================================
bool TreeArityMax::run() {
  doubleResult->setAllEdgeValue(0);
  doubleResult->setAllNodeValue(0);
  node n;
  forEach(n, graph->getNodes()) {
    doubleResult->setNodeValue(n, getNodeValue(n));
  }
  return true;
}
//======================================================
bool TreeArityMax::check(string &erreurMsg) {
   if (AcyclicTest::isAcyclic(graph)) {
     erreurMsg="";
     return true;
   }
   else {
     erreurMsg="The Graph must be Acyclic";
     return false;
   } 
}







