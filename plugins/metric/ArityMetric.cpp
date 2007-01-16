#include <tulip/ForEach.h>
#include "ArityMetric.h"

using namespace tlp;

DOUBLEPLUGINOFGROUP(ArityMetric,"Degree","David Auber","04/10/2001","Stable","0","1","Graph");

namespace {
  const char * paramHelp[] = {
    //Degree type
    HTML_HELP_OPEN()				 \
    HTML_HELP_DEF( "type", "String Collection" ) \
    HTML_HELP_DEF( "default", "InOut" )	 \
    HTML_HELP_BODY() \
    "This parameter enables to choose the type of degree to use"	\
    HTML_HELP_CLOSE(),
  };
}
#define DEGREE_TYPE "Type"
#define DEGREE_TYPES "InOut;In;Out;"
#define INOUT 0
#define IN 1
#define OUT 2
//==============================================================================
ArityMetric::ArityMetric(const PropertyContext &context):DoubleAlgorithm(context) {
   addParameter<StringCollection>(DEGREE_TYPE, paramHelp[0], DEGREE_TYPES);
}
//==================================================================
bool ArityMetric::run() {
  StringCollection degreeTypes(DEGREE_TYPES);
  degreeTypes.setCurrent(0);
  if (dataSet!=0) {
    dataSet->get(DEGREE_TYPE, degreeTypes);
  }
  node n;
  switch(degreeTypes.getCurrent()) {
  case INOUT:
    forEach(n, graph->getNodes())
      doubleResult->setNodeValue(n, graph->deg(n));
    break;
  case IN:
    forEach(n, graph->getNodes())
      doubleResult->setNodeValue(n, graph->indeg(n));
    break;
  case OUT:
    forEach(n, graph->getNodes())
      doubleResult->setNodeValue(n, graph->outdeg(n));
    break;
  }
  doubleResult->setAllEdgeValue(0);
  return true;
}
//==================================================================
