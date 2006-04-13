/**
 Author: David Auber
 Email : auber@labri.fr
 Last modification : 20/08/2001
 This program is free software; you can redistribute it and/or modify  *
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/
#include <cassert>
#include "InducedSubGraphSelection.h"
#include <tulip/Selection.h>
#include <tulip/MethodFactory.h>

SELECTIONPLUGIN(InducedSubGraphSelection,"Induced sub-graph","David Auber","08/08/2001","Alpha","0","1");

//=================================================================================
namespace {
  const char * paramHelp[] = {
    // selectedNodes
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "Selection" ) \
    HTML_HELP_BODY() \
    "This selection defines the originally set of nodes used to extend the current selection to the induced sub-graph." \
    HTML_HELP_CLOSE(),
  };
}
//=================================================================================
InducedSubGraphSelection::InducedSubGraphSelection(const PropertyContext &context):
  SelectionAlgorithm(context) {
  addParameter<Selection>("Nodes", paramHelp[0], "viewSelection");
}
//=================================================================================
bool InducedSubGraphSelection::run() {
  selectionResult->setAllNodeValue(false);
  selectionResult->setAllEdgeValue(false);
  Selection *entrySelection = 0;
  if (dataSet!=0) 
    dataSet->get("Nodes", entrySelection);  
  if (entrySelection == 0) 
    entrySelection = superGraph->getProperty<Selection>("viewSelection");

  Iterator<node> *itN = superGraph->getNodes();
  while (itN->hasNext()) {
    node itn=itN->next() ;
    if (entrySelection->getNodeValue(itn)) {
      selectionResult->setNodeValue(itn, true);
      Iterator<edge> *itE = superGraph->getOutEdges(itn);
      while (itE->hasNext()) {
	edge e = itE->next();
	node target = superGraph->target(e);
	if (entrySelection->getNodeValue(target))
	  selectionResult->setEdgeValue(e, true);
      } delete itE;
    }
  } delete itN;
  return true;
}
//=================================================================================
