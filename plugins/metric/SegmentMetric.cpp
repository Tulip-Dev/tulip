#include <tulip/AcyclicTest.h>
#include <tulip/MetricProxy.h>
#include "SegmentMetric.h"


METRICPLUGIN(SegmentMetric,"Segment","David Auber","19/02/2000","Alpha","0","1");

using namespace std;

SegmentMetric::SegmentMetric(const PropertyContext &context):Metric(context) {}

SegmentMetric::~SegmentMetric() {}

int SegmentMetric::segNode(node n,int &curSeg,int &segMax) {
  curSeg=0;segMax=0;
  if (superGraph->outdeg(n)!=0) {
    int resCurSeg=0,resSegMax=0;
    Iterator<node> *itN=superGraph->getOutNodes(n);
    while (itN->hasNext()) {
      node itn=itN->next();
      segNode(itn,resCurSeg,resSegMax);
      ++resCurSeg;
      if (resCurSeg>curSeg)
	curSeg=resCurSeg;
      resSegMax=max(resCurSeg,resSegMax);
      if (segMax<resSegMax) segMax=resSegMax;
    } delete itN;
    if (superGraph->outdeg(n)>1) curSeg=0;
    return segMax;
  }
  return 0;
}

double SegmentMetric::getNodeValue(const node n) {
  if (superGraph->outdeg(n)==0) return 0.0;
  int seg,segMax;
  return (double)segNode(n,seg,segMax);
}

bool SegmentMetric::check(string &erreurMsg) {
  if (AcyclicTest::isAcyclic(superGraph)) {
    erreurMsg="";
    return true;
    }
  else {
    erreurMsg="The Graph must be Acyclic";
    return false;
  }
}





