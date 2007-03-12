#include <cassert>
#include <tulip/AcyclicTest.h>
#include <tulip/GraphMeasure.h>
#include "DagLevelMetric.h"

DOUBLEPLUGINOFGROUP(DagLevelMetric,"DagLevel","David Auber","10/03/2000","Alpha","1.0","Tree");

using namespace std;
using namespace tlp;

DagLevelMetric::DagLevelMetric(const PropertyContext &context):DoubleAlgorithm(context) {}

DagLevelMetric::~DagLevelMetric() {}
//====================================================== 
bool DagLevelMetric::run() {
  return computeDagLevelMetric(graph, doubleResult, pluginProgress);
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
