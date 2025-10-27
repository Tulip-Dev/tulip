/*
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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

// +-------------------------------------------------------------------------+
// | Tulip Python Bindings                                                   |
// | inspired from bindings by the Booggie project development team          |
// | (https://sourceforge.net/projects/booggie/)                             |
// +-------------------------------------------------------------------------+

namespace tlp {

class GraphEvent : tlp::Event {
%TypeHeaderCode
#include <tulip/Graph.h>
%End

%Docstring
That class represents an event that happened on a :class:`tlp.Graph` instance.
To get the graph that sent the event, use the :meth:`tlp.GraphEvent.getGraph`.
The type of graph event can be retrieved through the :meth:`tlp.GraphEvent.getType` method.
Below is an exhaustive list of those types :

  * :const:`tlp.GraphEvent.GraphEventType.TLP_ADD_NODE` : a node has been added in the graph (use :meth:`tlp.GraphEvent.getNode` to get the concerned node)
  * :const:`tlp.GraphEvent.GraphEventType.TLP_BEFORE_DEL_NODE` : a node is about to be deleted in the graph (use :meth:`tlp.GraphEvent.getNode` to get the concerned node)
  * :const:`tlp.GraphEvent.GraphEventType.TLP_AFTER_DEL_NODE` : a node has been deleted in the graph (use :meth:`tlp.GraphEvent.getNode` to get the concerned node)
  * :const:`tlp.GraphEvent.GraphEventType.TLP_ADD_EDGE` : an edge has been added in the graph (use :meth:`tlp.GraphEvent.getEdge` to get the concerned edge)
  * :const:`tlp.GraphEvent.GraphEventType.TLP_BEFORE_DEL_EDGE` : an edge is about to be deleted in the graph (use :meth:`tlp.GraphEvent.getEdge` to get the concerned edge)
  * :const:`tlp.GraphEvent.GraphEventType.TLP_AFTER_DEL_EDGE` : an edge has been deleted in the graph (use :meth:`tlp.GraphEvent.getEdge` to get the concerned edge)
  * :const:`tlp.GraphEvent.GraphEventType.TLP_REVERSE_EDGE` : the direction of an edge has been reversed in the graph (use :meth:`tlp.GraphEvent.getEdge` to get the concerned edge)
  * :const:`tlp.GraphEvent.GraphEventType.TLP_BEFORE_SET_ENDS` : the extremities of an edge is about to be modified (use :meth:`tlp.GraphEvent.getEdge` to get the concerned edge)
  * :const:`tlp.GraphEvent.GraphEventType.TLP_AFTER_SET_ENDS` : the extremities of an edge have been modified (use :meth:`tlp.GraphEvent.getEdge` to get the concerned edge)
  * :const:`tlp.GraphEvent.GraphEventType.TLP_ADD_NODES` : several nodes have been added in the graph (use :meth:`tlp.GraphEvent.getNodes` to get the list of concerned nodes)
  * :const:`tlp.GraphEvent.GraphEventType.TLP_ADD_EDGES` : several edges have been added in the graph (use :meth:`tlp.GraphEvent.getEdges` to get the list of concerned edges)
  * :const:`tlp.GraphEvent.GraphEventType.TLP_BEFORE_ADD_DESCENDANTGRAPH` : a descendant graph (i.e. not necessarily a direct subgraph) is about to be added in the subgraphs hierarchy. Use :meth:`tlp.GraphEvent.getSubGraph` to get it.
  * :const:`tlp.GraphEvent.GraphEventType.TLP_AFTER_ADD_DESCENDANTGRAPH` : a descendant graph (i.e. not necessarily a direct subgraph) has been added in the subgraphs hierarchy. Use :meth:`tlp.GraphEvent.getSubGraph` to get it.
  * :const:`tlp.GraphEvent.GraphEventType.TLP_BEFORE_DEL_DESCENDANTGRAPH` : a descendant graph (i.e. not necessarily a direct subgraph) is about to be removed in the subgraphs hierarchy. Use :meth:`tlp.GraphEvent.getSubGraph` to get it.
  * :const:`tlp.GraphEvent.GraphEventType.TLP_AFTER_DEL_DESCENDANTGRAPH` : a descendant graph (i.e. not necessarily a direct subgraph) has been removed in the subgraphs hierarchy. Use :meth:`tlp.GraphEvent.getSubGraph` to get it.
  * :const:`tlp.GraphEvent.GraphEventType.TLP_BEFORE_ADD_SUBGRAPH` : a subgraph is about to be added in the graph. Use :meth:`tlp.GraphEvent.getSubGraph` to get it.
  * :const:`tlp.GraphEvent.GraphEventType.TLP_AFTER_ADD_SUBGRAPH` : a subgraph has been added in the graph. Use :meth:`tlp.GraphEvent.getSubGraph` to get it.
  * :const:`tlp.GraphEvent.GraphEventType.TLP_BEFORE_DEL_SUBGRAPH` : a subgraph is about to be removed in the graph. Use :meth:`tlp.GraphEvent.getSubGraph` to get it.
  * :const:`tlp.GraphEvent.GraphEventType.TLP_AFTER_DEL_SUBGRAPH` : a subgraph has been removed in the graph. Use :meth:`tlp.GraphEvent.getSubGraph` to get it.
  * :const:`tlp.GraphEvent.GraphEventType.TLP_BEFORE_ADD_LOCAL_PROPERTY` : a local property is about to be added in the graph. Use :meth:`tlp.GraphEvent.getPropertyName` to get the name of the concerned property.
  * :const:`tlp.GraphEvent.GraphEventType.TLP_ADD_LOCAL_PROPERTY` : a local property has been added in the graph. Use :meth:`tlp.GraphEvent.getPropertyName` to get the name of the concerned property.
  * :const:`tlp.GraphEvent.GraphEventType.TLP_BEFORE_DEL_LOCAL_PROPERTY` : a local property is about to be deleted in the graph. Use :meth:`tlp.GraphEvent.getPropertyName` to get the name of the concerned property.
  * :const:`tlp.GraphEvent.GraphEventType.TLP_AFTER_DEL_LOCAL_PROPERTY` : a local property has been deleted in the graph. Use :meth:`tlp.GraphEvent.getPropertyName` to get the name of the concerned property.
  * :const:`tlp.GraphEvent.GraphEventType.TLP_BEFORE_ADD_INHERITED_PROPERTY` : an inherited property (not attached to the graph itself but one of its parent) is about to be added in the graph. Use :meth:`tlp.GraphEvent.getPropertyName` to get the name of the concerned property.
  * :const:`tlp.GraphEvent.GraphEventType.TLP_ADD_INHERITED_PROPERTY` : an inherited property (not attached to the graph itself but one of its parent) has been added in the graph. Use :meth:`tlp.GraphEvent.getPropertyName` to get the name of the concerned property.
  * :const:`tlp.GraphEvent.GraphEventType.TLP_BEFORE_DEL_INHERITED_PROPERTY` : an inherited property (not attached to the graph itself but one of its parent) is about to be deleted in the graph. Use :meth:`tlp.GraphEvent.getPropertyName` to get the name of the concerned property.
  * :const:`tlp.GraphEvent.GraphEventType.TLP_AFTER_DEL_INHERITED_PROPERTY` : an inherited property (not attached to the graph itself but one of its parent) has been deleted in the graph. Use :meth:`tlp.GraphEvent.getPropertyName` to get the name of the concerned property.
  * :const:`tlp.GraphEvent.GraphEventType.TLP_BEFORE_SET_ATTRIBUTE` : an attribute of the graph is about to be set/modified. Use :meth:`tlp.GraphEvent.getAttributeName` to get the name of the concerned attribute.
  * :const:`tlp.GraphEvent.GraphEventType.TLP_AFTER_SET_ATTRIBUTE` : an attribute of the graph has been set/modified. Use :meth:`tlp.GraphEvent.getAttributeName` to get the name of the concerned attribute.
  * :const:`tlp.GraphEvent.GraphEventType.TLP_REMOVE_ATTRIBUTE` : an attribute of the graph has been removed. Use :meth:`tlp.GraphEvent.getAttributeName` to get the name of the concerned attribute.
  * :const:`tlp.GraphEvent.GraphEventType.TLP_BEFORE_RENAME_LOCAL_PROPERTY` : a local property of the graph is about to be renamed. Use :meth:`tlp.GraphEvent.getProperty` to get the concerned property and :meth:`tlp.GraphEvent.getPropertyNewName` to get the new property name.
  * :const:`tlp.GraphEvent.GraphEventType.TLP_AFTER_RENAME_LOCAL_PROPERTY` : a local property of the graph has been renamed. Use :meth:`tlp.GraphEvent.getProperty` to get the concerned property and :meth:`tlp.GraphEvent.getPropertyOldName` to get the old property name.

%End

%ConvertToSubClassCode
  if (dynamic_cast<tlp::GraphEvent*>(sipCpp)) {
    sipType = sipFindType("tlp::GraphEvent");
  } else {
    sipType = NULL;
  }
%End

public:

  enum GraphEventType /BaseType=IntEnum/ {
    TLP_ADD_NODE = GraphEvent::GraphEventType::TLP_ADD_NODE,
    TLP_BEFORE_DEL_NODE = GraphEvent::GraphEventType::TLP_BEFORE_DEL_NODE,
    TLP_AFTER_DEL_NODE = GraphEvent::GraphEventType::TLP_AFTER_DEL_NODE,
    TLP_ADD_EDGE = GraphEvent::GraphEventType::TLP_ADD_EDGE,
    TLP_BEFORE_DEL_EDGE = GraphEvent::GraphEventType::TLP_BEFORE_DEL_EDGE,
    TLP_AFTER_DEL_EDGE = GraphEvent::GraphEventType::TLP_AFTER_DEL_EDGE,
    TLP_REVERSE_EDGE = GraphEvent::GraphEventType::TLP_REVERSE_EDGE,
    TLP_BEFORE_SET_ENDS = GraphEvent::GraphEventType::TLP_BEFORE_SET_ENDS,
    TLP_AFTER_SET_ENDS = GraphEvent::GraphEventType::TLP_AFTER_SET_ENDS,
    TLP_ADD_NODES = GraphEvent::GraphEventType::TLP_ADD_NODES,
    TLP_ADD_EDGES = GraphEvent::GraphEventType::TLP_ADD_EDGES,
    TLP_BEFORE_ADD_DESCENDANTGRAPH = GraphEvent::GraphEventType::TLP_BEFORE_ADD_DESCENDANTGRAPH,
    TLP_AFTER_ADD_DESCENDANTGRAPH = GraphEvent::GraphEventType::TLP_AFTER_ADD_DESCENDANTGRAPH,
    TLP_BEFORE_DEL_DESCENDANTGRAPH = GraphEvent::GraphEventType::TLP_BEFORE_DEL_DESCENDANTGRAPH,
    TLP_AFTER_DEL_DESCENDANTGRAPH = GraphEvent::GraphEventType::TLP_AFTER_DEL_DESCENDANTGRAPH,
    TLP_BEFORE_ADD_SUBGRAPH = GraphEvent::GraphEventType::TLP_BEFORE_ADD_SUBGRAPH,
    TLP_AFTER_ADD_SUBGRAPH = GraphEvent::GraphEventType::TLP_AFTER_ADD_SUBGRAPH,
    TLP_BEFORE_DEL_SUBGRAPH = GraphEvent::GraphEventType::TLP_BEFORE_DEL_SUBGRAPH,
    TLP_AFTER_DEL_SUBGRAPH = GraphEvent::GraphEventType::TLP_AFTER_DEL_SUBGRAPH,
    TLP_ADD_LOCAL_PROPERTY = GraphEvent::GraphEventType::TLP_ADD_LOCAL_PROPERTY,
    TLP_BEFORE_DEL_LOCAL_PROPERTY = GraphEvent::GraphEventType::TLP_BEFORE_DEL_LOCAL_PROPERTY,
    TLP_AFTER_DEL_LOCAL_PROPERTY = GraphEvent::GraphEventType::TLP_AFTER_DEL_LOCAL_PROPERTY,
    TLP_ADD_INHERITED_PROPERTY = GraphEvent::GraphEventType::TLP_ADD_INHERITED_PROPERTY,
    TLP_BEFORE_DEL_INHERITED_PROPERTY = GraphEvent::GraphEventType::TLP_BEFORE_DEL_INHERITED_PROPERTY,
    TLP_AFTER_DEL_INHERITED_PROPERTY = GraphEvent::GraphEventType::TLP_AFTER_DEL_INHERITED_PROPERTY,
    TLP_BEFORE_RENAME_LOCAL_PROPERTY = GraphEvent::GraphEventType::TLP_BEFORE_RENAME_LOCAL_PROPERTY,
    TLP_AFTER_RENAME_LOCAL_PROPERTY = GraphEvent::GraphEventType::TLP_AFTER_RENAME_LOCAL_PROPERTY,
    TLP_BEFORE_SET_ATTRIBUTE = GraphEvent::GraphEventType::TLP_BEFORE_SET_ATTRIBUTE,
    TLP_AFTER_SET_ATTRIBUTE = GraphEvent::GraphEventType::TLP_AFTER_SET_ATTRIBUTE,
    TLP_REMOVE_ATTRIBUTE = GraphEvent::GraphEventType::TLP_REMOVE_ATTRIBUTE,
    TLP_BEFORE_ADD_LOCAL_PROPERTY = GraphEvent::GraphEventType::TLP_BEFORE_ADD_LOCAL_PROPERTY,
    TLP_BEFORE_ADD_INHERITED_PROPERTY = GraphEvent::GraphEventType::TLP_BEFORE_ADD_INHERITED_PROPERTY
  };

  GraphEvent(const tlp::Graph& g, GraphEventType graphEvtType, unsigned int id,
             tlp::Event::EventType evtType = tlp::Event::TLP_MODIFICATION);
      
  GraphEvent(const tlp::Graph& g, GraphEventType graphEvtType, const tlp::Graph* sg);

  GraphEvent(const tlp::Graph& g, GraphEventType graphEvtType,
             const std::string& str, tlp::Event::EventType evtType = tlp::Event::TLP_MODIFICATION);
    
  ~GraphEvent() ;

// ========================================================================================

  tlp::Graph* getGraph() const;
%Docstring
tlp.GraphEvent.getGraph()

Returns the graph that sent the event.

:rtype: :class:`tlp.Graph`
%End

// ========================================================================================

  tlp::node getNode() const;
%Docstring
tlp.GraphEvent.getNode()

Returns the node concerned by the event (if any, otherwise return an invalid node)

:rtype: :class:`tlp.node`
%End

// ========================================================================================

  tlp::edge getEdge() const;
%Docstring
tlp.GraphEvent.getEdge()

Returns the edge concerned by the event (if any, otherwise return an invalid edge)

:rtype: :class:`tlp.edge`
%End

// ========================================================================================

  const std::vector<tlp::node>& getNodes() const;
%Docstring
tlp.GraphEvent.getNodes()

Returns the nodes concerned by the event (if any, otherwise return an empty list)

:rtype: list of :class:`tlp.node`
%End

// ========================================================================================

  const std::vector<tlp::edge>& getEdges() const;
%Docstring
tlp.GraphEvent.getEdges()

Returns the edges concerned by the event (if any, otherwise return an empty list)

:rtype: list of :class:`tlp.edge`
%End

// ========================================================================================

  const tlp::Graph* getSubGraph() const;
%Docstring
tlp.GraphEvent.getSubGraph()

Returns the subgraph or descendant graph concerned by the event (if any, return :const:`None` otherwise)

:rtype: :class:`tlp.Graph`
%End

// ========================================================================================

  const std::string& getAttributeName() const;
%Docstring
tlp.GraphEvent.getAttributeName()

Returns the name of the attribute concerned by the event (if any, otherwise returns an empty string)

:rtype: string
%End

// ========================================================================================

    const std::string& getPropertyName() const;
%Docstring
tlp.GraphEvent.getPropertyName()

Returns the name of the property concerned by the event (if any, otherwise returns an empty string)

:rtype: string
%End

// ========================================================================================

    GraphEventType getType() const;
%Docstring
tlp.GraphEvent.getType()

Returns the type of graph event.

:rtype: :const:`tlp.GraphEvent.GraphEventType.TLP_*` (cf. class description above)
%End

// ========================================================================================

  tlp::PropertyInterface* getProperty() const;
%Docstring
tlp.GraphEvent.getProperty()

Returns the property that is about to be or has been renamed.

:rtype: :class:`tlp.PropertyInterface`
%End

// ========================================================================================

    const std::string& getPropertyOldName() const;
%Docstring
tlp.GraphEvent.getPropertyOldName()

Returns the old name of the property when it has been renamed.

:rtype: string
%End

// ========================================================================================

  const std::string& getPropertyNewName() const;
%Docstring
tlp.GraphEvent.getPropertyNewName()

Returns the new name of the property that it is about to be renamed.

:rtype: string
%End

};

};
