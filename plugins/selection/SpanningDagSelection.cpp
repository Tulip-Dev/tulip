#include <cassert>

#include <tulip/BooleanProperty.h>
#include <tulip/AcyclicTest.h>

#include "SpanningDagSelection.h"

BOOLEANPLUGIN(SpanningDagSelection,"SpanningDag","David Auber","01/12/1999","Alpha","1.0");

using namespace std;
using namespace tlp;

//=================================================================
SpanningDagSelection::SpanningDagSelection(const PropertyContext &context):BooleanAlgorithm(context) {}
//=================================================================
bool SpanningDagSelection::run() {
  booleanResult->setAllNodeValue(true);
  booleanResult->setAllEdgeValue(true);
  vector<edge> obstructions;
  AcyclicTest::acyclicTest(graph, &obstructions);
  for (vector<edge>::const_iterator it = obstructions.begin(); it != obstructions.end(); ++it) {
    booleanResult->setEdgeValue(*it, false);
  }
  return true;
}
//=================================================================
