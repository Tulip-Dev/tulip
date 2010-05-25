#include <cassert>
#include "DagLevelMetric.h"

DOUBLEPLUGINOFGROUP(DagLevelMetric,"Dag Level","David Auber","10/03/2000","Alpha","1.0","Tree");

using namespace std;
using namespace tlp;

DagLevelMetric::DagLevelMetric(const PropertyContext &context):DoubleAlgorithm(context) {}

DagLevelMetric::~DagLevelMetric() {}
//====================================================== 
bool DagLevelMetric::run() {
  MutableContainer<unsigned int> level;
  dagLevel(graph, level, pluginProgress);
  node n;
  forEach(n, graph->getNodes())
    doubleResult->setNodeValue(n, level.get(n.id));
  return true;
}
//====================================================== 
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
