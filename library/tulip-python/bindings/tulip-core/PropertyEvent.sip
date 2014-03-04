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

class PropertyEvent : tlp::Event {
%TypeHeaderCode
#include <tulip/PropertyInterface.h>
%End

%Docstring
That class represents an event that happened on a :class:`tlp.PropertyInterface` derived instance.
To get the property that sent the event, use the :meth:`tlp.PropertyEvent.getProperty`.
The type of property event can be retrieved through the :meth:`tlp.PropertyEvent.getType` method.
Below is an exhaustive list of those types :

    * :const:`tlp.PropertyEvent.TLP_BEFORE_SET_NODE_VALUE` : the value of a node is about to be modified. Use :meth:`tlp.PropertyEvent.getNode()` to get the concerned node.
    * :const:`tlp.PropertyEvent.TLP_AFTER_SET_NODE_VALUE` : the value of a node has been modified. Use :meth:`tlp.PropertyEvent.getNode()` to get the concerned node.
    * :const:`tlp.PropertyEvent.TLP_BEFORE_SET_EDGE_VALUE` : the value of an edge is about to be modified. Use :meth:`tlp.PropertyEvent.getEdge()` to get the concerned edge.
    * :const:`tlp.PropertyEvent.TLP_AFTER_SET_EDGE_VALUE` : the value of an edge has been modified. Use :meth:`tlp.PropertyEvent.getEdge()` to get the concerned edge.
    * :const:`tlp.PropertyEvent.TLP_BEFORE_SET_ALL_NODE_VALUE` : the value of all nodes is about to be modified.
    * :const:`tlp.PropertyEvent.TLP_AFTER_SET_ALL_NODE_VALUE` : the value of all nodes has been modified.
    * :const:`tlp.PropertyEvent.TLP_BEFORE_SET_ALL_EDGE_VALUE` : the value of all edges is about to be modified.
    * :const:`tlp.PropertyEvent.TLP_AFTER_SET_ALL_EDGE_VALUE` : the value of all edges has been modified.

%End

%ConvertToSubClassCode
   if (dynamic_cast<tlp::PropertyEvent*>(sipCpp)) {
        sipType = sipFindType("tlp::PropertyEvent");
   } else {
        sipType = NULL;
    }
%End

public:

  enum PropertyEventType {TLP_BEFORE_SET_NODE_VALUE = 0,
                          TLP_AFTER_SET_NODE_VALUE,
                          TLP_BEFORE_SET_ALL_NODE_VALUE,
                          TLP_AFTER_SET_ALL_NODE_VALUE,
                          TLP_BEFORE_SET_ALL_EDGE_VALUE,
                          TLP_AFTER_SET_ALL_EDGE_VALUE,
                          TLP_BEFORE_SET_EDGE_VALUE,
                          TLP_AFTER_SET_EDGE_VALUE
                         };

  PropertyEvent(const tlp::PropertyInterface& prop, PropertyEventType propEvtType,
                tlp::Event::EventType evtType = tlp::Event::TLP_MODIFICATION,
                unsigned int id = UINT_MAX);

// ========================================================================================

  tlp::PropertyInterface* getProperty() const;
%Docstring
tlp.PropertyEvent.getProperty()

Returns the property concerned by the event.

:rtype: derived instances of :class:`tlp.PropertyInterface`
%End

// ========================================================================================

  tlp::node getNode() const;
%Docstring
tlp.PropertyEvent.getNode()

Returns the node concerned by the event (if any, otherwise return an invalid node)

:rtype: :class:`tlp.node`
%End

// ========================================================================================

  tlp::edge getEdge() const;
%Docstring
tlp.PropertyEvent.getEdge()

Returns the edge concerned by the event (if any, otherwise return an invalid edge)

:rtype: :class:`tlp.edge`
%End

// ========================================================================================

  PropertyEventType getType() const;
%Docstring
tlp.Graph.getType()

Returns the type of graph event.

:rtype: :const:`tlp.GraphEvent.TLP_*` (cf. class description above)
%End

};

};
