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
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <cassert>
#include <iostream>
#include <tulip/ObservableGraph.h>
#include <tulip/Graph.h>

using namespace tlp;

void GraphObserver::RealGraphObserver::treatEvent(const Event& ev) {
  const GraphEvent* gEvt = dynamic_cast<const GraphEvent*>(&ev);
  if (gEvt) {
    Graph* graph = gEvt->getGraph();
    switch(gEvt->getType()) {
    case GraphEvent::TLP_ADD_NODE:
      visibleObserver->addNode(graph, gEvt->getNode());
      break;
    case GraphEvent::TLP_DEL_NODE:
      visibleObserver->delNode(graph, gEvt->getNode());
      break;
    case GraphEvent::TLP_ADD_EDGE:
      visibleObserver->addEdge(graph, gEvt->getEdge());
      break;
    case GraphEvent::TLP_DEL_EDGE:
      visibleObserver->delEdge(graph, gEvt->getEdge());
      break;
    case GraphEvent::TLP_REVERSE_EDGE:
      visibleObserver->reverseEdge(graph, gEvt->getEdge());
      break;
    case GraphEvent::TLP_BEFORE_SET_ENDS:
      visibleObserver->beforeSetEnds(graph, gEvt->getEdge());
      break;
    case GraphEvent::TLP_AFTER_SET_ENDS:
      visibleObserver->afterSetEnds(graph, gEvt->getEdge());
      break;
    case GraphEvent::TLP_ADD_NODES: {
      const std::vector<node>& nodes = gEvt->getNodes();
      for (unsigned int i = 0; i < nodes.size(); ++i)
	visibleObserver->addNode(graph, nodes[i]);
      break;
    }
    case GraphEvent::TLP_ADD_EDGES: {
      const std::vector<edge>& edges = gEvt->getEdges();
      for (unsigned int i = 0; i < edges.size(); ++i)
	visibleObserver->addEdge(graph, edges[i]);
      break;
    }
    case GraphEvent::TLP_ADD_SUBGRAPH:
      visibleObserver->addSubGraph(graph, const_cast<Graph *>(gEvt->getSubGraph()));
      break;
    case GraphEvent::TLP_DEL_SUBGRAPH:
      visibleObserver->delSubGraph(graph, const_cast<Graph *>(gEvt->getSubGraph()));
      break;
    case GraphEvent::TLP_ADD_LOCAL_PROPERTY:
      visibleObserver->addLocalProperty(graph, gEvt->getPropertyName());
      break;
    case GraphEvent::TLP_BEFORE_DEL_LOCAL_PROPERTY:
      visibleObserver->beforeDelLocalProperty(graph, gEvt->getPropertyName());
      break;
    case GraphEvent::TLP_AFTER_DEL_LOCAL_PROPERTY:
      visibleObserver->afterDelLocalProperty(graph, gEvt->getPropertyName());
      break;
    case GraphEvent::TLP_BEFORE_SET_ATTRIBUTE:
      visibleObserver->beforeSetAttribute(graph, gEvt->getAttributeName());
      break;
    case GraphEvent::TLP_AFTER_SET_ATTRIBUTE:
      visibleObserver->afterSetAttribute(graph, gEvt->getAttributeName());
      break;
    case GraphEvent::TLP_REMOVE_ATTRIBUTE:
      visibleObserver->removeAttribute(graph, gEvt->getAttributeName());
      break;
    case GraphEvent::TLP_ADD_INHERITED_PROPERTY:
      visibleObserver->addInheritedProperty(graph, gEvt->getPropertyName());
      break;
    case GraphEvent::TLP_BEFORE_DEL_INHERITED_PROPERTY:
      visibleObserver->beforeDelInheritedProperty(graph, gEvt->getPropertyName());
      break;
    case GraphEvent::TLP_AFTER_DEL_INHERITED_PROPERTY:
      visibleObserver->afterDelInheritedProperty(graph, gEvt->getPropertyName());
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
      visibleObserver->destroy(graph);
  }
}
