#include <cassert>
#include <deque>
#include <tulip/AcyclicTest.h>
#include "DagLevelMetric.h"

DOUBLEPLUGINOFGROUP(DagLevelMetric,"DagLevel","David Auber","10/03/2000","Alpha","1.0","Tree");

using namespace std;
using namespace tlp;

DagLevelMetric::DagLevelMetric(const PropertyContext &context):DoubleAlgorithm(context) {}

DagLevelMetric::~DagLevelMetric() {}
//====================================================== 
bool DagLevelMetric::run() {
  MutableContainer<unsigned int> totreat;
  deque<node> fifo;
  //===============================================
  Iterator<node> *itN = graph->getNodes();
  while(itN->hasNext()) {
    node itn=itN->next();
    unsigned int indegree = graph->indeg(itn);
    if (indegree==0) {
      fifo.push_back(itn);
      doubleResult->setNodeValue(itn,0);
    }
    else
      totreat.set(itn.id, indegree - 1);
  } delete itN;
  //==============================================
  while (!fifo.empty()) {
    node current = fifo.front();
    fifo.pop_front();
    itN=graph->getOutNodes(current);
    while(itN->hasNext()) {
      node child=itN->next();
      if (totreat.get(child.id)>0)
	totreat.set(child.id,totreat.get(child.id)-1);
      else {
	doubleResult->setNodeValue(child,doubleResult->getNodeValue(current)+1);
	fifo.push_back(child);
      }
    } delete itN;
  }
  //==============================================
  return true;
}

bool DagLevelMetric::check(string &erreurMsg) {
  if (AcyclicTest::isAcyclic(graph)) {
    erreurMsg="";
    return true;
  }
  else {
    erreurMsg="The Graph must be a acylic";
    return false;
  }
}
