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
#include <cassert>
#include <iostream>
#include <tulip/ObservableGraph.h>
#include "tulip/Graph.h"

using namespace tlp;

void GraphObserver::treatEvent(const Event& ev) {
  const GraphEvent* gEvt = dynamic_cast<const GraphEvent*>(&ev);
  if (gEvt) {
    Graph* graph = gEvt->getGraph();
    switch(gEvt->getType()) {
    case GraphEvent::TLP_ADD_NODE:
      addNode(graph, gEvt->getNode());
      break;
    case GraphEvent::TLP_DEL_NODE:
      delNode(graph, gEvt->getNode());
      break;
    case GraphEvent::TLP_ADD_EDGE:
      addEdge(graph, gEvt->getEdge());
      break;
    case GraphEvent::TLP_DEL_EDGE:
      delEdge(graph, gEvt->getEdge());
      break;
    case GraphEvent::TLP_REVERSE_EDGE:
      reverseEdge(graph, gEvt->getEdge());
      break;
    case GraphEvent::TLP_BEFORE_SET_ENDS:
      beforeSetEnds(graph, gEvt->getEdge());
      break;
    case GraphEvent::TLP_AFTER_SET_ENDS:
      afterSetEnds(graph, gEvt->getEdge());
      break;
    case GraphEvent::TLP_ADD_SUBGRAPH:
      addSubGraph(graph, const_cast<Graph *>(gEvt->getSubGraph()));
      break;
    case GraphEvent::TLP_DEL_SUBGRAPH:
      delSubGraph(graph, const_cast<Graph *>(gEvt->getSubGraph()));
      break;
    case GraphEvent::TLP_ADD_LOCAL_PROPERTY:
      addLocalProperty(graph, gEvt->getPropertyName());
      break;
    case GraphEvent::TLP_DEL_LOCAL_PROPERTY:
      delLocalProperty(graph, gEvt->getPropertyName());
      break;
    case GraphEvent::TLP_BEFORE_SET_ATTRIBUTE:
      beforeSetAttribute(graph, gEvt->getAttributeName());
      break;
    case GraphEvent::TLP_AFTER_SET_ATTRIBUTE:
      afterSetAttribute(graph, gEvt->getAttributeName());
      break;
    case GraphEvent::TLP_REMOVE_ATTRIBUTE:
      removeAttribute(graph, gEvt->getAttributeName());
    case GraphEvent::TLP_ADD_INHERITED_PROPERTY:
    case GraphEvent::TLP_DEL_INHERITED_PROPERTY:
      break;
    default:
      // this should not happen
      assert(false);
    }
  } else {
    Graph* graph =
      // From my point of view the use of dynamic_cast should be correct
      // but it fails, so I use reinterpret_cast (pm)
      reinterpret_cast<Graph *>(ev.sender());
    if (graph && ev.type() == Event::TLP_DELETE)
      destroy(graph);
  }
}
