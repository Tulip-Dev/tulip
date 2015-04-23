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

    * :const:`tlp.GraphEvent.TLP_ADD_NODE` : a node has been added in the graph (use :meth:`tlp.GraphEvent.getNode` to get the concerned node)
    * :const:`tlp.GraphEvent.TLP_DEL_NODE` : a node has been deleted in the graph (use :meth:`tlp.GraphEvent.getNode` to get the concerned node)
    * :const:`tlp.GraphEvent.TLP_ADD_EDGE` : an edge has been added in the graph (use :meth:`tlp.GraphEvent.getEdge` to get the concerned edge)
    * :const:`tlp.GraphEvent.TLP_DEL_EDGE` : an edge has been deleted in the graph (use :meth:`tlp.GraphEvent.getEdge` to get the concerned edge)
    * :const:`tlp.GraphEvent.TLP_REVERSE_EDGE` : the direction of an edge has been reversed in the graph (use :meth:`tlp.GraphEvent.getEdge` to get the concerned edge)
    * :const:`tlp.GraphEvent.TLP_BEFORE_SET_ENDS` : the extremities of an edge is about to be modified (use :meth:`tlp.GraphEvent.getEdge` to get the concerned edge)
    * :const:`tlp.GraphEvent.TLP_AFTER_SET_ENDS` : the extremities of an edge have been modified (use :meth:`tlp.GraphEvent.getEdge` to get the concerned edge)
    * :const:`tlp.GraphEvent.TLP_ADD_NODES` : several nodes have been added in the graph (use :meth:`tlp.GraphEvent.getNodes` to get the list of concerned nodes)
    * :const:`tlp.GraphEvent.TLP_ADD_EDGES` : several edges have been added in the graph (use :meth:`tlp.GraphEvent.getEdges` to get the list of concerned edges)
    * :const:`tlp.GraphEvent.TLP_BEFORE_ADD_DESCENDANTGRAPH` : a descendant graph (i.e. not necessarily a direct sub-graph) is about to be added in the sub-graphs hierarchy. Use :meth:`tlp.GraphEvent.getSubGraph` to get it.
    * :const:`tlp.GraphEvent.TLP_AFTER_ADD_DESCENDANTGRAPH` : a descendant graph (i.e. not necessarily a direct sub-graph) has been added in the sub-graphs hierarchy. Use :meth:`tlp.GraphEvent.getSubGraph` to get it.
    * :const:`tlp.GraphEvent.TLP_BEFORE_DEL_DESCENDANTGRAPH` : a descendant graph (i.e. not necessarily a direct sub-graph) is about to be removed in the sub-graphs hierarchy. Use :meth:`tlp.GraphEvent.getSubGraph` to get it.
    * :const:`tlp.GraphEvent.TLP_AFTER_DEL_DESCENDANTGRAPH` : a descendant graph (i.e. not necessarily a direct sub-graph) has been removed in the sub-graphs hierarchy. Use :meth:`tlp.GraphEvent.getSubGraph` to get it.
    * :const:`tlp.GraphEvent.TLP_BEFORE_ADD_SUBGRAPH` : a sub-graph is about to be added in the graph. Use :meth:`tlp.GraphEvent.getSubGraph` to get it.
    * :const:`tlp.GraphEvent.TLP_AFTER_ADD_SUBGRAPH` : a sub-graph has been added in the graph. Use :meth:`tlp.GraphEvent.getSubGraph` to get it.
    * :const:`tlp.GraphEvent.TLP_BEFORE_DEL_SUBGRAPH` : a sub-graph is about to be removed in the graph. Use :meth:`tlp.GraphEvent.getSubGraph` to get it.
    * :const:`tlp.GraphEvent.TLP_AFTER_DEL_SUBGRAPH` : a sub-graph has been removed in the graph. Use :meth:`tlp.GraphEvent.getSubGraph` to get it.
    * :const:`tlp.GraphEvent.TLP_BEFORE_ADD_LOCAL_PROPERTY` : a local property is about to be added in the graph. Use :meth:`tlp.GraphEvent.getPropertyName` to get the name of the concerned property.
    * :const:`tlp.GraphEvent.TLP_ADD_LOCAL_PROPERTY` : a local property has been added in the graph. Use :meth:`tlp.GraphEvent.getPropertyName` to get the name of the concerned property.
    * :const:`tlp.GraphEvent.TLP_BEFORE_DEL_LOCAL_PROPERTY` : a local property is about to be deleted in the graph. Use :meth:`tlp.GraphEvent.getPropertyName` to get the name of the concerned property.
    * :const:`tlp.GraphEvent.TLP_AFTER_DEL_LOCAL_PROPERTY` : a local property has been deleted in the graph. Use :meth:`tlp.GraphEvent.getPropertyName` to get the name of the concerned property.
    * :const:`tlp.GraphEvent.TLP_BEFORE_ADD_INHERITED_PROPERTY` : an inherited property (not attached to the graph itself but one of its parent) is about to be added in the graph. Use :meth:`tlp.GraphEvent.getPropertyName` to get the name of the concerned property.
    * :const:`tlp.GraphEvent.TLP_ADD_INHERITED_PROPERTY` : an inherited property (not attached to the graph itself but one of its parent) has been added in the graph. Use :meth:`tlp.GraphEvent.getPropertyName` to get the name of the concerned property.
    * :const:`tlp.GraphEvent.TLP_BEFORE_DEL_INHERITED_PROPERTY` : an inherited property (not attached to the graph itself but one of its parent) is about to be deleted in the graph. Use :meth:`tlp.GraphEvent.getPropertyName` to get the name of the concerned property.
    * :const:`tlp.GraphEvent.TLP_AFTER_DEL_INHERITED_PROPERTY` : an inherited property (not attached to the graph itself but one of its parent) has been deleted in the graph. Use :meth:`tlp.GraphEvent.getPropertyName` to get the name of the concerned property.
    * :const:`tlp.GraphEvent.TLP_BEFORE_SET_ATTRIBUTE` : an attribute of the graph is about to be set/modified. Use :meth:`tlp.GraphEvent.getAttributeName` to get the name of the concerned attribute.
    * :const:`tlp.GraphEvent.TLP_AFTER_SET_ATTRIBUTE` : an attribute of the graph has been set/modified. Use :meth:`tlp.GraphEvent.getAttributeName` to get the name of the concerned attribute.
    * :const:`tlp.GraphEvent.TLP_REMOVE_ATTRIBUTE` : an attribute of the graph has been removed. Use :meth:`tlp.GraphEvent.getAttributeName` to get the name of the concerned attribute.

%End

%ConvertToSubClassCode
   if (dynamic_cast<tlp::GraphEvent*>(sipCpp)) {
        sipType = sipFindType("tlp::GraphEvent");
   } else {
        sipType = NULL;
   }
%End

  public:

    enum GraphEventType {
      TLP_ADD_NODE = 0,
      TLP_DEL_NODE = 1,
      TLP_ADD_EDGE = 2,
      TLP_DEL_EDGE = 3,
      TLP_REVERSE_EDGE = 4,
      TLP_BEFORE_SET_ENDS = 5,
      TLP_AFTER_SET_ENDS = 6,
      TLP_ADD_NODES = 7,
      TLP_ADD_EDGES = 8,
      TLP_BEFORE_ADD_DESCENDANTGRAPH = 9,
      TLP_AFTER_ADD_DESCENDANTGRAPH = 10,
      TLP_BEFORE_DEL_DESCENDANTGRAPH = 11,
      TLP_AFTER_DEL_DESCENDANTGRAPH = 12,
      TLP_BEFORE_ADD_SUBGRAPH = 13,
      TLP_AFTER_ADD_SUBGRAPH = 14,
      TLP_BEFORE_DEL_SUBGRAPH = 15,
      TLP_AFTER_DEL_SUBGRAPH = 16,
      TLP_ADD_LOCAL_PROPERTY = 17,
      TLP_BEFORE_DEL_LOCAL_PROPERTY = 18,
      TLP_AFTER_DEL_LOCAL_PROPERTY = 19,
      TLP_ADD_INHERITED_PROPERTY = 20,
      TLP_BEFORE_DEL_INHERITED_PROPERTY = 21,
      TLP_AFTER_DEL_INHERITED_PROPERTY = 22,
      TLP_BEFORE_SET_ATTRIBUTE = 23,
      TLP_AFTER_SET_ATTRIBUTE = 24,
      TLP_REMOVE_ATTRIBUTE = 25,
      TLP_BEFORE_ADD_LOCAL_PROPERTY = 26,
      TLP_BEFORE_ADD_INHERITED_PROPERTY = 27
    };

    GraphEvent(const tlp::Graph& g, GraphEventType graphEvtType, unsigned int id,
	       tlp::Event::EventType evtType = tlp::Event::TLP_MODIFICATION);
      
    GraphEvent(const tlp::Graph& g, GraphEventType graphEvtType,
	       const tlp::Graph* sg);

    GraphEvent(const tlp::Graph& g, GraphEventType graphEvtType,
	       const std::string& str,
	       tlp::Event::EventType evtType = tlp::Event::TLP_MODIFICATION);
    
    ~GraphEvent() ;

// ========================================================================================

    tlp::Graph* getGraph() const;
%Docstring
tlp.GraphEvent.getGraph()

Returns the graph that sent the event.

:rtype: :class:`tlp.Graph`
%End

// ========================================================================================

    tlp::node getNode() const ;
%Docstring
tlp.GraphEvent.getNode()

Returns the node concerned by the event (if any, otherwise return an invalid node)

:rtype: :class:`tlp.node`
%End

// ========================================================================================

    tlp::edge getEdge() const ;
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

    const tlp::Graph* getSubGraph() const ;
%Docstring
tlp.GraphEvent.getSubGraph()

Returns the sub-graph or descendant graph concerned by the event (if any, return :const:`None` otherwise)

:rtype: :class:`tlp.Graph`
%End

// ========================================================================================

    const std::string& getAttributeName() const ;
%Docstring
tlp.GraphEvent.getAttributeName()

Returns the name of the attribute concerned by the event (if any, otherwise returns an empty string)

:rtype: string
%End

// ========================================================================================

    const std::string& getPropertyName() const ;
%Docstring
tlp.Graph.getPropertyName()

Returns the name of the property concerned by the event (if any, otherwise returns an empty string)

:rtype: string
%End

// ========================================================================================

    GraphEventType getType() const ;
%Docstring
tlp.Graph.getType()

Returns the type of graph event.

:rtype: :const:`tlp.GraphEvent.TLP_*` (cf. class description above)
%End

};

};
