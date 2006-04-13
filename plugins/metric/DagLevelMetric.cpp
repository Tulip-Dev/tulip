#include <cassert>
#include <deque>
#include <tulip/AcyclicTest.h>
#include "DagLevelMetric.h"

METRICPLUGIN(DagLevelMetric,"DagLevel","David Auber","10/03/2000","Alpha","0","1");

using namespace std;

DagLevelMetric::DagLevelMetric(const PropertyContext &context):MetricAlgorithm(context) {}

DagLevelMetric::~DagLevelMetric() {}
//====================================================== 
bool DagLevelMetric::run() {
  MutableContainer<unsigned int> totreat;
  deque<node> fifo;
  //===============================================
  Iterator<node> *itN = superGraph->getNodes();
  while(itN->hasNext()) {
    node itn=itN->next();
    unsigned int indegree = superGraph->indeg(itn);
    if (indegree==0) {
      fifo.push_back(itn);
      metricResult->setNodeValue(itn,0);
    }
    else
      totreat.set(itn.id, indegree - 1);
  } delete itN;
  //==============================================
  while (!fifo.empty()) {
    node current = fifo.front();
    fifo.pop_front();
    itN=superGraph->getOutNodes(current);
    while(itN->hasNext()) {
      node child=itN->next();
      if (totreat.get(child.id)>0)
	totreat.set(child.id,totreat.get(child.id)-1);
      else {
	metricResult->setNodeValue(child,metricResult->getNodeValue(current)+1);
	fifo.push_back(child);
      }
    } delete itN;
  }
  //==============================================
  return true;
}

bool DagLevelMetric::check(string &erreurMsg) {
  if (AcyclicTest::isAcyclic(superGraph)) {
    erreurMsg="";
    return true;
  }
  else {
    erreurMsg="The Graph must be a acylic";
    return false;
  }
}
