#include "IdMetric.h"

using namespace tlp;

DOUBLEPLUGINOFGROUP(IdMetric,"Id","David Auber","06/04/2000","Alpha","1.0","Misc");

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
