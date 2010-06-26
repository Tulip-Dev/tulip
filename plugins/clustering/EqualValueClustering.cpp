/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */
#include <stdio.h>
#include <math.h>
#include <sstream>

#include <tulip/Graph.h>
#include "EqualValueClustering.h"

//================================================================================
using namespace std;
using namespace tlp;

ALGORITHMPLUGIN(EqualValueClustering,"Equal Value","David Auber","20/05/2008","Beta","1.1");

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
    HTML_HELP_DEF("values", "nodes <BR> edges") \
    HTML_HELP_DEF( "default", "nodes" )	 \
    HTML_HELP_BODY() \
    "This parameter enables to choose the type of graph elements to partition"	\
    HTML_HELP_CLOSE(),
    HTML_HELP_OPEN()				 \
    HTML_HELP_DEF( "type", "bool" ) \
    HTML_HELP_DEF( "values", "[true, false]" ) \
    HTML_HELP_DEF( "default", "false" ) \
    HTML_HELP_BODY() \
    "This parameter indicates whether the subgraphs have to be connected." \
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
  addParameter<bool>("Connected", paramHelp[2], "false");
}
//===============================================================================
bool EqualValueClustering::run() {
  string tmp1,tmp2;
  PropertyInterface *property=0;
  StringCollection eltTypes(ELT_TYPES);
  bool connected = false;
  eltTypes.setCurrent(0);
  if (dataSet!=0) {
    dataSet->get("Property", property);  
    dataSet->get(ELT_TYPE, eltTypes);
    dataSet->get("Connected", connected);
  }

  if (property == 0)
    property = graph->getProperty("viewMetric");

  return computeEqualValueClustering(graph, property, eltTypes.getCurrent() == NODE_ELT,
				     connected, pluginProgress);
}
//================================================================================
