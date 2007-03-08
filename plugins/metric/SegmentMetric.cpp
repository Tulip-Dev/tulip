#include <tulip/AcyclicTest.h>
#include <tulip/DoubleProperty.h>
#include <tulip/ForEach.h>
#include "SegmentMetric.h"


DOUBLEPLUGINOFGROUP(SegmentMetric,"Segment","David Auber","19/02/2000","Alpha","1.0","Tree");

using namespace std;
using namespace tlp;

//=============================
SegmentMetric::SegmentMetric(const PropertyContext &context):DoubleAlgorithm(context) {}
//=============================
int SegmentMetric::segNode(node n,int &curSeg,int &segMax) {
  curSeg=0;
  segMax=0;
  if (graph->outdeg(n)!=0) {
    int resCurSeg=0,resSegMax=0;
    node _n;
    forEach(_n, graph->getOutNodes(n)) {
      segNode(_n, resCurSeg, resSegMax);
      ++resCurSeg;
      if (resCurSeg>curSeg)
	curSeg = resCurSeg;
      resSegMax = max(resCurSeg,resSegMax);
      if (segMax<resSegMax) segMax=resSegMax;
    }
    if (graph->outdeg(n)>1) curSeg=0;
    return segMax;
  }
  return 0;
}
/*
double SegmentMetric::getNodeValue(const node n) {
  if (graph->outdeg(n)==0) return 0.0;
  int seg,segMax;
  return (double)segNode(n,seg,segMax);
}
*/
//=============================
bool SegmentMetric::run() {
  doubleResult->setAllNodeValue(0);
  doubleResult->setAllEdgeValue(0);
  node n;
  forEach(n, graph->getNodes()){
    int seg,segMax;
    doubleResult->setNodeValue(n, segNode(n, seg, segMax));
  }
  return true;
}
bool SegmentMetric::check(string &erreurMsg) {
  if (AcyclicTest::isAcyclic(graph)) {
    erreurMsg="";
    return true;
    }
  else {
    erreurMsg="The Graph must be Acyclic";
    return false;
  }
}





