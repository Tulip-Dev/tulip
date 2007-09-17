#include <stdio.h>
#include <math.h>
#include <sstream>

#include <tulip/Graph.h>
#include "EqualValueClustering.h"

//================================================================================
using namespace std;
using namespace tlp;

ALGORITHMPLUGIN(EqualValueClustering,"Equal Value","David Auber","13/06/2001","Alpha","1.0");

namespace {
  const char * paramHelp[] = {
    // selectedNodes
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "PropertyInterface*" ) \
    HTML_HELP_BODY() \
    "Specify the property that will be used to partition the graph" \
    HTML_HELP_CLOSE(),
    HTML_HELP_OPEN()				 \
    HTML_HELP_DEF( "type", "String Collection" ) \
    HTML_HELP_DEF("Values", "nodes <BR> edges") \
    HTML_HELP_DEF( "default", "nodes" )	 \
    HTML_HELP_BODY() \
    "This parameter enables to choose the type of graph elements to partition"	\
    HTML_HELP_CLOSE()
  };
}
#define ELT_TYPE "Type"
#define ELT_TYPES "nodes;edges;"
#define NODE_ELT 0
#define EDGE_ELT 1
//================================================================================
EqualValueClustering::EqualValueClustering(AlgorithmContext context):Algorithm(context) {
  addParameter<PropertyInterface*>("Property", paramHelp[0], "viewMetric");
  addParameter<StringCollection>(ELT_TYPE, paramHelp[1], ELT_TYPES);
}
//===============================================================================
bool EqualValueClustering::run() {
  string tmp1,tmp2;
  PropertyInterface *property=0;
  StringCollection eltTypes(ELT_TYPES);
  eltTypes.setCurrent(0);
  if (dataSet!=0) {
    dataSet->get("Property", property);  
    dataSet->get(ELT_TYPE, eltTypes);
  }

  if (property == 0)
    property = graph->getProperty("viewMetric");

  return computeEqualValueClustering(graph, property,
				     eltTypes.getCurrent() == NODE_ELT,
				     pluginProgress);
}
//================================================================================
