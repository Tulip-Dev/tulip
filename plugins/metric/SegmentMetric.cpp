#include <tulip/AcyclicTest.h>
#include <tulip/Metric.h>
#include <tulip/ForEach.h>
#include "SegmentMetric.h"


METRICPLUGIN(SegmentMetric,"Segment","David Auber","19/02/2000","Alpha","0","1");

using namespace std;

//=============================
SegmentMetric::SegmentMetric(const PropertyContext &context):MetricAlgorithm(context) {}
//=============================
int SegmentMetric::segNode(node n,int &curSeg,int &segMax) {
  curSeg=0;
  segMax=0;
  if (superGraph->outdeg(n)!=0) {
    int resCurSeg=0,resSegMax=0;
    node _n;
    forEach(_n, superGraph->getOutNodes(n)) {
      segNode(_n, resCurSeg, resSegMax);
      ++resCurSeg;
      if (resCurSeg>curSeg)
	curSeg = resCurSeg;
      resSegMax = max(resCurSeg,resSegMax);
      if (segMax<resSegMax) segMax=resSegMax;
    }
    if (superGraph->outdeg(n)>1) curSeg=0;
    return segMax;
  }
  return 0;
}
/*
double SegmentMetric::getNodeValue(const node n) {
  if (superGraph->outdeg(n)==0) return 0.0;
  int seg,segMax;
  return (double)segNode(n,seg,segMax);
}
*/
//=============================
bool SegmentMetric::run() {
  metricObj->setAllNodeValue(0);
  metricObj->setAllEdgeValue(0);
  node n;
  forEach(n, superGraph->getNodes()){
    int seg,segMax;
    metricObj->setNodeValue(n, segNode(n, seg, segMax));
  }
  return true;
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





