/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
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
#include <cassert>
#include "InducedSubGraphSelection.h"
#include <tulip/BooleanProperty.h>
#include <tulip/MethodFactory.h>

using namespace tlp;

BOOLEANPLUGIN(InducedSubGraphSelection,"Induced Sub-Graph","David Auber","08/08/2001","Alpha","1.0");

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
  BooleanAlgorithm(context) {
  addParameter<BooleanProperty>("Nodes", paramHelp[0], "viewSelection");
}
//=================================================================================
bool InducedSubGraphSelection::run() {
  booleanResult->setAllNodeValue(false);
  booleanResult->setAllEdgeValue(false);
  BooleanProperty *entrySelection = 0;
  if (dataSet!=0) 
    dataSet->get("Nodes", entrySelection);  
  if (entrySelection == 0) 
    entrySelection = graph->getProperty<BooleanProperty>("viewSelection");

  Iterator<node> *itN = graph->getNodes();
  while (itN->hasNext()) {
    node itn=itN->next() ;
    if (entrySelection->getNodeValue(itn)) {
      booleanResult->setNodeValue(itn, true);
      Iterator<edge> *itE = graph->getOutEdges(itn);
      while (itE->hasNext()) {
	edge e = itE->next();
	node target = graph->target(e);
	if (entrySelection->getNodeValue(target))
	  booleanResult->setEdgeValue(e, true);
      } delete itE;
    }
  } delete itN;
  return true;
}
//=================================================================================
