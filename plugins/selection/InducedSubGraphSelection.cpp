/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
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

#include "InducedSubGraphSelection.h"

using namespace tlp;

PLUGIN(InducedSubGraphSelection)

//=================================================================================
namespace {
const char * paramHelp[] = {
  // selectedNodes
  HTML_HELP_OPEN() \
  HTML_HELP_DEF( "type", "Selection" ) \
  HTML_HELP_BODY() \
  "Set of nodes for which the induced sub-graph is computed." \
  HTML_HELP_CLOSE(),
};
}
//=================================================================================
InducedSubGraphSelection::InducedSubGraphSelection(const tlp::PluginContext* context):
  BooleanAlgorithm(context) {
  addInParameter<BooleanProperty>("Nodes", paramHelp[0], "viewSelection");
}
//=================================================================================
bool InducedSubGraphSelection::run() {
  result->setAllNodeValue(false);
  result->setAllEdgeValue(false);
  BooleanProperty *entrySelection = NULL;

  if (dataSet!=NULL)
    dataSet->get("Nodes", entrySelection);

  if (entrySelection == NULL)
    entrySelection = graph->getProperty<BooleanProperty>("viewSelection");

  Iterator<node> *itN = graph->getNodes();

  while (itN->hasNext()) {
    node itn=itN->next() ;

    if (entrySelection->getNodeValue(itn)) {
      result->setNodeValue(itn, true);
      Iterator<edge> *itE = graph->getOutEdges(itn);

      while (itE->hasNext()) {
        edge e = itE->next();
        node target = graph->target(e);

        if (entrySelection->getNodeValue(target))
          result->setEdgeValue(e, true);
      }

      delete itE;
    }
  }

  delete itN;
  return true;
}
//=================================================================================
