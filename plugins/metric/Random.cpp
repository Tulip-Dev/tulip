#include <tulip/ForEach.h>
#include "Random.h"

using namespace tlp;

DOUBLEPLUGINOFGROUP(RandomMetric,"Random","David Auber","04/10/2001","Alpha","0","1","Misc")

//===========================================
RandomMetric::RandomMetric(const PropertyContext &context):DoubleAlgorithm(context) 
{}
//===========================================
bool RandomMetric::run() {
  node n;
  forEach(n, graph->getNodes())
    doubleResult->setNodeValue(n, (double)rand() / double(RAND_MAX));
  edge e;
  forEach(e, graph->getEdges())
    doubleResult->setEdgeValue(e, (double)rand() / double(RAND_MAX));
  return true;
}
//===========================================
