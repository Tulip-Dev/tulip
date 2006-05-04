#include <tulip/ForEach.h>

#include "IdMetric.h"

DOUBLEPLUGIN(IdMetric,"Id","David Auber","06/04/2000","Alpha","0","1");

//==================================================================
IdMetric::IdMetric(const PropertyContext &context):DoubleAlgorithm(context) {
}
//==================================================================
bool IdMetric::run() {
  node n;
  forEach(n, graph->getNodes())
    doubleResult->setNodeValue(n, n.id);
  edge e;
  forEach(e, graph->getEdges())
    doubleResult->setEdgeValue(e, e.id);
  return true;
}
//=================================================================
