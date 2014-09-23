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

#include "ReachableSubGraphSelection.h"

#include <tulip/StringCollection.h>

using namespace tlp;

PLUGIN(ReachableSubGraphSelection)

namespace {
const char * paramHelp[] = {
  // direction
  HTML_HELP_OPEN() \
  HTML_HELP_DEF( "type", "StringCollection" ) \
  HTML_HELP_DEF( "values", "{output edges, input edges, all edges}" ) \
  HTML_HELP_DEF( "default", "output edges" ) \
  HTML_HELP_BODY() \
  "This parameter defines the navigation direction. Following values are corrects :" \
  "<ul><li>output edges: follow ouput edges (directed);</li>" \
  "<li>input edges: follow input edges (reverse-directed);</li>" \
  "<li>all edges: all edges (undirected).</li></ul>" \
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

std::string edgesDirectionLabels[] = {
  "output edges",
  "input edges",
  "all edges",
};
}



ReachableSubGraphSelection::ReachableSubGraphSelection(const tlp::PluginContext *context):BooleanAlgorithm(context) {
  addInParameter<StringCollection> ("edges direction",paramHelp[0],"output edges;input edges;all edges");
  addInParameter<BooleanProperty> ("startingnodes",paramHelp[1],"viewSelection");
  addInParameter<int> ("distance",paramHelp[2],"5");
}

ReachableSubGraphSelection::~ReachableSubGraphSelection() {}

///===========================================================
bool ReachableSubGraphSelection::run() {
  unsigned int maxDistance = 5;
  StringCollection edgeDirectionCollecion;
  EDGE_TYPE edgeDirection=DIRECTED;
  BooleanProperty * startNodes=graph->getProperty<BooleanProperty>("viewSelection");

  if ( dataSet!=NULL) {
    dataSet->get("distance", maxDistance);

    //Get the edge orientation
    if(dataSet->get("edges direction",edgeDirectionCollecion)) {
      if(edgeDirectionCollecion.getCurrentString() == edgesDirectionLabels[0]) {
        edgeDirection = DIRECTED;
      }
      else if(edgeDirectionCollecion.getCurrentString()== edgesDirectionLabels[1]) {
        edgeDirection = INV_DIRECTED;
      }
      else if(edgeDirectionCollecion.getCurrentString()== edgesDirectionLabels[2]) {
        edgeDirection = UNDIRECTED;
      }
    }
    else {
      //If the new parameter is not defined search for the old one.
      int direction=0;

      if(dataSet->get("direction",direction)) {
        switch(direction) {
        case 0:
          edgeDirection = DIRECTED;
          break;

        case 1:
          edgeDirection = INV_DIRECTED;
          break;

        case 2:
          edgeDirection = UNDIRECTED;
        }
      }
    }

    dataSet->get("startingnodes", startNodes);
  }

  result->setAllEdgeValue(false);
  result->setAllNodeValue(false);

  if (startNodes) {
    Iterator<node>* itN = startNodes->getNodesEqualTo(true);
    std::set<node> reachables;

    // iterate on startNodes add them and their reachables
    while (itN->hasNext()) {
      node current = itN->next();
      reachables.insert(current);
      reachableNodes(graph, current, reachables, maxDistance,
                     edgeDirection);
    }

    delete itN;

    std::set<node>::const_iterator itr = reachables.begin();
    std::set<node>::const_iterator ite = reachables.end();

    // select nodes
    while (itr != ite) {
      result->setNodeValue((*itr), true);
      ++itr;
    }

    // select corresponding edges
    Iterator<edge> *itE = graph->getEdges();

    while(itE->hasNext()) {
      edge e = itE->next();
      const std::pair<node, node>& ends = graph->ends(e);

      if (result->getNodeValue(ends.first) &&
          result->getNodeValue(ends.second))
        result->setEdgeValue(e, true);
    }

    delete itE;
  }

  return true;
}




