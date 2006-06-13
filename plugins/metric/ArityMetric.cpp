#include <tulip/ForEach.h>
#include "ArityMetric.h"

DOUBLEPLUGINOFGROUP(ArityMetric,"Degree","David Auber","04/10/2001","Stable","0","1","Graph");

ArityMetric::ArityMetric(const PropertyContext &context):DoubleAlgorithm(context) {
}
//==================================================================
bool ArityMetric::run() {
  node n;
  forEach(n, graph->getNodes())
    doubleResult->setNodeValue(n, graph->deg(n));
  doubleResult->setAllEdgeValue(0);
  return true;
}
//==================================================================




