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

%ModuleHeaderCode
#include <tulip/Observable.h>
namespace tlp {

// Use implementation from 3.7 branch to keep the Python API consistent between version 3.7 and 4.0
class GraphObserverPython : public Observable {

public:
  GraphObserverPython() {}
  virtual ~GraphObserverPython() {}
  GraphObserverPython(const GraphObserverPython &) {}
  GraphObserverPython& operator=(const GraphObserverPython &) {
    return *this;
  }
  virtual void addNode(Graph *, const node ) {}
  virtual void addEdge(Graph *, const edge ) {}
  virtual void beforeSetEnds(Graph *, const edge) {}
  virtual void afterSetEnds(Graph *, const edge) {}
  virtual void delNode(Graph *,const node ) {}
  virtual void delEdge(Graph *,const edge ) {}
  virtual void reverseEdge(Graph *,const edge ) {}
  virtual void destroy(Graph *) {}
  virtual void addSubGraph(Graph *, Graph *) {}
  virtual void delSubGraph(Graph *, Graph *) {}
  virtual void addLocalProperty(Graph*, const std::string&) {}
  virtual void beforeDelLocalProperty(Graph* g, const std::string& pName) {
    // to ensure compatibility
    delLocalProperty(g, pName);
  }
  virtual void afterDelLocalProperty(Graph*, const std::string&) {}
  virtual void delLocalProperty(Graph*, const std::string&) {}
  virtual void addInheritedProperty(Graph*, const std::string&) {}
  virtual void beforeDelInheritedProperty(Graph*, const std::string&) {}
  virtual void afterDelInheritedProperty(Graph*, const std::string&) {}
  virtual void beforeSetAttribute(Graph*, const std::string&) {}
  virtual void afterSetAttribute(Graph*, const std::string&) {}
  virtual void removeAttribute(Graph*, const std::string&) {}

protected:

  virtual void treatEvent(const Event& ev) {
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

    case GraphEvent::TLP_ADD_NODES: {
      const std::vector<node>& nodes = gEvt->getNodes();

      for (unsigned int i = 0; i < nodes.size(); ++i)
        addNode(graph, nodes[i]);

      break;
    }

    case GraphEvent::TLP_ADD_EDGES: {
      const std::vector<edge>& edges = gEvt->getEdges();

      for (unsigned int i = 0; i < edges.size(); ++i)
        addEdge(graph, edges[i]);

      break;
    }

    case GraphEvent::TLP_AFTER_ADD_SUBGRAPH:
      addSubGraph(graph, const_cast<Graph *>(gEvt->getSubGraph()));
      break;

    case GraphEvent::TLP_AFTER_DEL_SUBGRAPH:
      delSubGraph(graph, const_cast<Graph *>(gEvt->getSubGraph()));
      break;

    case GraphEvent::TLP_ADD_LOCAL_PROPERTY:
      addLocalProperty(graph, gEvt->getPropertyName());
      break;

    case GraphEvent::TLP_BEFORE_DEL_LOCAL_PROPERTY:
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

    case GraphEvent::TLP_AFTER_DEL_LOCAL_PROPERTY:
    case GraphEvent::TLP_ADD_INHERITED_PROPERTY:
    case GraphEvent::TLP_AFTER_DEL_INHERITED_PROPERTY:
    case GraphEvent::TLP_BEFORE_DEL_INHERITED_PROPERTY:
    case GraphEvent::TLP_BEFORE_ADD_DESCENDANTGRAPH:
    case GraphEvent::TLP_AFTER_ADD_DESCENDANTGRAPH:
    case GraphEvent::TLP_BEFORE_DEL_DESCENDANTGRAPH:
    case GraphEvent::TLP_AFTER_DEL_DESCENDANTGRAPH:
    case GraphEvent::TLP_BEFORE_ADD_SUBGRAPH:
    case GraphEvent::TLP_BEFORE_DEL_SUBGRAPH:
    case GraphEvent::TLP_BEFORE_ADD_LOCAL_PROPERTY:
    case GraphEvent::TLP_BEFORE_ADD_INHERITED_PROPERTY:
      break;

    default:
      // this should not happen
      assert(false);
    }
  }
  else {
    Graph* graph = reinterpret_cast<Graph *>(ev.sender());

    if (graph && ev.type() == Event::TLP_DELETE)
      destroy(graph);
  }
}

};
}
%End

namespace tlp {

class GraphObserverPython : tlp::Observable /PyName=GraphObserver/ {

%Docstring
.. versionadded:: 3.8

Derived implementation of that class receives a message (through a callback/handler method)
after each modification of a :class:`tlp.Graph` object.

To receive notifications, the :class:`tlp.GraphObserver` object must
first be connected to the :class:`tlp.Graph` object.
This is done by calling the :meth:`tlp.Graph.addGraphObserver` method.

Depending on which messages you want to receive, you need to override
different methods of :class:`tlp.GraphObserver`.

The Observer pattern is described in the lecture notes and pp293-304
of Design Patterns by Gamma, Helm, Johnson, and Vlissides. It is a
framework for handling state dependency between observer and observed
object.

Below is a small example of an observer that displays a message on the
standard output each time a node is added to a graph::

    class PrintObs(tlp.GraphObserver):
        def addNode(self, graph, node):
            print node, " added in ", graph

    printObs = PrintObs()

    def main(graph):
        graph.addGraphObserver(printObs)
        graph.addNode() // output : "<node ?> added in <graph ?>"


%End

public:

//===========================================================================================

  GraphObserverPython();
  virtual ~GraphObserverPython();
  GraphObserverPython(const tlp::GraphObserverPython&);

//===========================================================================================

  virtual void addNode(tlp::Graph *, const tlp::node);
%Docstring
tlp.GraphObserver.addNode(graph, node)

This method is called each time a node is added
in an observed graph.

:param graph: the observed graph
:type graph: :class:`tlp.Graph`
:param node: the newly added node
:type node: :class:`tlp.node`
%End

//===========================================================================================

  virtual void addEdge(tlp::Graph *, const tlp::edge);
%Docstring
tlp.GraphObserver.addEdge(graph, edge)

This method is called each time an edge is added
in an observed graph.

:param graph: the observed graph
:type graph: :class:`tlp.Graph`
:param edge: the newly added edge
:type edge: :class:`tlp.edge`
%End

//===========================================================================================

  virtual void beforeSetEnds(tlp::Graph *, const tlp::edge);
%Docstring
tlp.GraphObserver.beforeSetEnds(graph, edge)

This method is called before the modification of the
ends of an edge in an observed graph.

:param graph: the observed graph
:type graph: :class:`tlp.Graph`
:param edge: the edge whose ends will be modified
:type edge: :class:`tlp.edge`
%End

//===========================================================================================

  virtual void afterSetEnds(tlp::Graph *, const tlp::edge);
%Docstring
tlp.GraphObserver.afterSetEnds(graph, edge)

This method is called after the modification of the
ends of an edge in an observed graph.

:param graph: the observed graph
:type graph: :class:`tlp.Graph`
:param edge: the edge whose ends have been modified
:type edge: :class:`tlp.edge`
%End

//===========================================================================================

  virtual void delNode(tlp::Graph *,const tlp::node);
%Docstring
tlp.GraphObserver.delNode(graph, node)

This method is called each time a node is deleted
in an observed graph.

:param graph: the observed graph
:type graph: :class:`tlp.Graph`
:param node: the deleted node
:type node: :class:`tlp.node`
%End


//===========================================================================================

  virtual void delEdge(tlp::Graph *,const tlp::edge);
%Docstring
tlp.GraphObserver.delEdge(graph, edge)

This method is called each time an edge is deleted
in an observed graph.

:param graph: the observed graph
:type graph: :class:`tlp.Graph`
:param edge: the deleted edge
:type edge: :class:`tlp.edge`
%End

//===========================================================================================

  virtual void reverseEdge(tlp::Graph *,const tlp::edge);
%Docstring
tlp.GraphObserver.reverseEdge(graph, edge)

This method is called each time the orientation of an edge is reversed
in an observed graph.

:param graph: the observed graph
:type graph: :class:`tlp.Graph`
:param edge: the reversed edge
:type edge: :class:`tlp.edge`
%End

//===========================================================================================

  virtual void destroy(tlp::Graph *);
%Docstring
tlp.GraphObserver.destroy(graph)

This method is called when an observed graph is deleted.

:param graph: the observed graph
:type graph: :class:`tlp.Graph`
%End
//===========================================================================================

  virtual void addSubGraph(tlp::Graph *, tlp::Graph *);
%Docstring
tlp.GraphObserver.addSubGraph(graph, subgraph)

This method is called each time a sub-graph is added
in an observed graph.

:param graph: the observed graph
:type graph: :class:`tlp.Graph`
:param subgraph: the newly added sub-graph
:type subgraph: :class:`tlp.Graph`
%End

//===========================================================================================

  virtual void delSubGraph(tlp::Graph *, tlp::Graph *);
%Docstring
tlp.GraphObserver.delSubGraph(graph, subgraph)

This method is called each time a sub-graph is deleted
in an observed graph.

:param graph: the observed graph
:type graph: :class:`tlp.Graph`
:param subgraph: the deleted sub-graph
:type subgraph: :class:`tlp.Graph`
%End

//===========================================================================================

  virtual void addLocalProperty(tlp::Graph*, const std::string&);
%Docstring
tlp.GraphObserver.addLocalProperty(graph, propertyName)

This method is called each time a new local property
is attached to an observed graph.

:param graph: the observed graph
:type graph: :class:`tlp.Graph`
:param propertyName: the name of the new local property
:type propertyName: string
%End

//===========================================================================================

  virtual void beforeDelLocalProperty(tlp::Graph* g, const std::string& pName);
%Docstring
tlp.GraphObserver.beforeDelLocalProperty(graph, propertyName)

This method is called before the deletion
of a local property attached to an observed graph.

:param graph: the observed graph
:type graph: :class:`tlp.Graph`
:param propertyName: the name of the local property that will be deleted
:type propertyName: string
%End

//===========================================================================================

  virtual void afterDelLocalProperty(tlp::Graph*, const std::string&);
%Docstring
tlp.GraphObserver.afterDelLocalProperty(graph, propertyName)

This method is called after the deletion
of a local property attached to an observed graph.

:param graph: the observed graph
:type graph: :class:`tlp.Graph`
:param propertyName: the name of the local property that has been deleted
:type propertyName: string
%End

//===========================================================================================

  virtual void addInheritedProperty(tlp::Graph*, const std::string&);
%Docstring
tlp.GraphObserver.addInheritedProperty(graph, propertyName)

This method is called each time a new inherited property
is attached to an observed graph.

:param graph: the observed graph
:type graph: :class:`tlp.Graph`
:param propertyName: the name of the new inherited property
:type propertyName: string
%End

//===========================================================================================

  virtual void beforeDelInheritedProperty(tlp::Graph*, const std::string&);
%Docstring
tlp.GraphObserver.beforeDelInheritedProperty(graph, propertyName)

This method is called before the deletion
of an inherited property attached to an observed graph.

:param graph: the observed graph
:type graph: :class:`tlp.Graph`
:param propertyName: the name of the inherited property that will be deleted
:type propertyName: string
%End

//===========================================================================================

  virtual void afterDelInheritedProperty(tlp::Graph*, const std::string&);
%Docstring
tlp.GraphObserver.afterDelInheritedProperty(graph, propertyName)

This method is called after the deletion
of an inherited property attached to an observed graph.

:param graph: the observed graph
:type graph: :class:`tlp.Graph`
:param propertyName: the name of the inherited property that has been deleted
:type propertyName: string
%End

//===========================================================================================

  virtual void beforeSetAttribute(tlp::Graph*, const std::string&);
%Docstring
tlp.GraphObserver.beforeSetAttribute(graph, attributeName)

This method is called before the creation/modification of
an attribute in an observed graph.

:param graph: the observed graph
:type graph: :class:`tlp.Graph`
:param attributeName: the name of the attribute that will be created/modified
:type attributeName: string
%End

//===========================================================================================

  virtual void afterSetAttribute(tlp::Graph*, const std::string&);
%Docstring
tlp.GraphObserver.afterSetAttribute(graph, attributeName)

This method is called after the creation/modification of
an attribute in an observed graph.

:param graph: the observed graph
:type graph: :class:`tlp.Graph`
:param attributeName: the name of the attribute that has been created/modified
:type attributeName: string
%End

//===========================================================================================

  virtual void removeAttribute(tlp::Graph*, const std::string&);
%Docstring
tlp.GraphObserver.removeAttribute(graph, attributeName)

This method is called each time an attribute of an observed graph
is removed.

:param graph: the observed graph
:type graph: :class:`tlp.Graph`
:param attributeName: the name of the removed attribute
:type attributeName: string
%End

//===========================================================================================

protected:

  virtual void treatEvent(const tlp::Event & /NoCopy/);

};

};
