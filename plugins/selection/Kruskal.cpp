#include "Kruskal.h"

BOOLEANPLUGIN(Kruskal,"Kruskal","Anthony DON","14/04/03","Alpha","1.0");

using namespace std;
using namespace tlp;

namespace {
  const char * paramHelp[] = {
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "DoubleProperty" ) \
    HTML_HELP_DEF( "default", "\"viewMetric\"" ) \
    HTML_HELP_BODY() \
    "This parameter defines the metric used for edges weight." \
    HTML_HELP_CLOSE(),
  };
}
//======================================================
Kruskal::Kruskal(const PropertyContext &context):BooleanAlgorithm(context) {
  addParameter<DoubleProperty> ("edge weight", paramHelp[0], "viewMetric");
}
//======================================================
Kruskal::~Kruskal() {
}
//======================================================
#include <tulip/ConnectedTest.h>
bool Kruskal::check(string &erreurMsg) {
  if (ConnectedTest::isConnected(graph)) {
    erreurMsg = "";
    return true;
  }
  else {
    erreurMsg = "The Graph must be connected";
    return false;
  }
}
//======================================================
/// Compute the Minimum Spanning Tree
bool Kruskal::run(){
  /* Initialisation */
  DoubleProperty *edgeWeight = 0;
  if ( dataSet!=0) {
    dataSet->get("edge weight", edgeWeight);
  }
  if (edgeWeight == 0)
    edgeWeight = graph->getProperty<DoubleProperty>("viewMetric");

  selectMinimumSpanningTree(graph, booleanResult, edgeWeight, pluginProgress);

  return true;
}
//=======================================================================
