#include <tulip/AcyclicTest.h>
#include "TreeArityMax.h"

METRICPLUGIN(TreeArityMax,"TreeArityMax","David Auber","20/12/1999","Alpha","0","1");

using namespace std;

TreeArityMax::TreeArityMax(const PropertyContext &context):Metric(context) 
{}
TreeArityMax::~TreeArityMax() {}

//======================================================
double TreeArityMax::getNodeValue(const node n) {

  double tmpDbl;
  double result=superGraph->outdeg(n);
  Iterator<node> *itN=superGraph->getOutNodes(n);
  while (itN->hasNext()) {
    node itn=itN->next();
    tmpDbl=metricProxy->getNodeValue(itn);
    if (tmpDbl>result)
      result=tmpDbl;
  }  delete itN;
  return result;
}

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







