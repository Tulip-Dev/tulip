//-*-c++-*-
/**
 Author: David Auber
 Email : auber@labri.fr
 Last modification : 20/08/2001
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/
#include "ReachableSubGraphSelection.h"
#include <tulip/GraphMeasure.h>

using namespace tlp;

BOOLEANPLUGIN(ReachableSubGraphSelection,"ReachableSubGraph","David Auber","01/12/1999","Alpha","0","1");

namespace {
  const char * paramHelp[] = {
    // direction
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "int" ) \
    HTML_HELP_DEF( "values", "{0,1,2}" ) \
    HTML_HELP_DEF( "default", "0" ) \
    HTML_HELP_BODY() \
    "This parameter defines the navigation direction. Following values are corrects :" \
    "<ul><li>0: follow ouput edges (directed);</li>" \
    "<li>1: follow input edges (reverse-directed);</li>" \
    "<li>2: all edges (undirected).</li></ul>" \
    HTML_HELP_CLOSE(),

    // startingNodes
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "Selection" ) \
    HTML_HELP_DEF( "default", "\"viewSelection\"" ) \
    HTML_HELP_BODY() \
    "This parameter defines the starting set of nodes used to walk in the graph." \
    HTML_HELP_CLOSE(),

    // maxdepth
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "int" ) \
    HTML_HELP_DEF( "values", "[0,1000000]" ) \
    HTML_HELP_DEF( "default", "10" ) \
    HTML_HELP_BODY() \
    "This parameter defines the maximal distance of reachable nodes." \
    HTML_HELP_CLOSE(),
  };
}


ReachableSubGraphSelection::ReachableSubGraphSelection(const PropertyContext &context):BooleanAlgorithm(context) {
  addParameter<int> ("direction",paramHelp[0],"0");
  addParameter<BooleanProperty> ("startingnodes",paramHelp[1],"viewSelection");
  addParameter<int> ("distance",paramHelp[2],"5");
}

ReachableSubGraphSelection::~ReachableSubGraphSelection() {}

///===========================================================
bool ReachableSubGraphSelection::run() {
  int maxDepth = 5;
  int direction = 0;
  BooleanProperty * startNodes=graph->getProperty<BooleanProperty>("viewSelection");
  if ( dataSet!=0) {
    dataSet->get("distance", maxDepth);
    dataSet->get("direction", direction);
    dataSet->get("startingnodes", startNodes);
  }

  booleanResult->setAllEdgeValue(false);
  booleanResult->setAllNodeValue(false);

  if ( startNodes ) {
    Iterator<node> *itN = graph->getNodes();
    while (itN->hasNext()) { 
      node current = itN->next();
      if (startNodes->getNodeValue(current)) {
	MutableContainer<unsigned int> distance;
	tlp::maxDistance(graph, current, distance, direction);
	Iterator<node> *itN = graph->getNodes();
	while (itN->hasNext()) { 
	  node itn = itN->next();
	  if (distance.get(itn.id) <= maxDepth && distance.get(itn.id)<graph->numberOfNodes() ) 
	    booleanResult->setNodeValue(itn,true);
	} delete itN;
      }
    } delete itN;

    Iterator<edge> *itE = graph->getEdges();
    while(itE->hasNext()) {
      edge e = itE->next();
      node source = graph->source(e);
      node target = graph->target(e);
      if (booleanResult->getNodeValue(source) && booleanResult->getNodeValue(target))
	booleanResult->setEdgeValue(e,true);
    }delete itE;
  }
 
  return true;
}




