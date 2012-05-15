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

class PropertyObserver {
%TypeHeaderCode
#include <tulip/ObservableProperty.h>
%End

%Docstring
.. versionadded:: 3.8

Derived implementation of that class receives a message (through a callback/handler method)
after each modification of an object deriving from :class:`tlp.PropertyInterface`.

To receive notifications, the :class:`tlp.PropertyObserver` object must
first be connected to the :class:`tlp.Graph` object.
This is done by calling the :meth:`tlp.PropertyInterface.addPropertyObserver` method.

Depending on which messages you want to receive, you need to override
different methods of :class:`tlp.PropertyObserver`.

The Observer pattern is described in the lecture notes and pp293-304
of Design Patterns by Gamma, Helm, Johnson, and Vlissides. It is a
framework for handling state dependency between observer and observed
object.
%End

public:

  PropertyObserver();
  virtual ~PropertyObserver();
  PropertyObserver(const tlp::PropertyObserver&);

//===========================================================================================

  virtual void beforeSetNodeValue(tlp::PropertyInterface*, const tlp::node);
%Docstring
tlp.PropertyObserver.beforeSetNodeValue(property, node)

This method is called before the modification of the value associated to
a node in an observed property.

:param property: the observed property
:type property: sub-classes of :class:`tlp.PropertyInterface`
:param node: the node that will have its value modified
:type node: :class:`tlp.node`
%End

//===========================================================================================

  virtual void afterSetNodeValue(tlp::PropertyInterface*, const tlp::node);
%Docstring
tlp.PropertyObserver.afterSetNodeValue(property, node)

This method is called after the modification of the value associated to
a node in an observed property.

:param property: the observed property
:type property: sub-classes of :class:`tlp.PropertyInterface`
:param node: the node that had its value modified
:type node: :class:`tlp.node`
%End

//===========================================================================================

  virtual void beforeSetEdgeValue(tlp::PropertyInterface*, const tlp::edge);
%Docstring
tlp.PropertyObserver.beforeSetEdgeValue(property, edge)

This method is called before the modification of the value associated to
an edge in an observed property.

:param property: the observed property
:type property: sub-classes of :class:`tlp.PropertyInterface`
:param edge: the edge that will have its value modified
:type edge: :class:`tlp.edge`
%End

//===========================================================================================

  virtual void afterSetEdgeValue(tlp::PropertyInterface*, const tlp::edge);
%Docstring
tlp.PropertyObserver.afterSetEdgeValue(property, edge)

This method is called after the modification of the value associated to
an edge in an observed property.

:param property: the observed property
:type property: sub-classes of :class:`tlp.PropertyInterface`
:param edge: the edge that had its value modified
:type edge: :class:`tlp.edge`
%End

//===========================================================================================

  virtual void beforeSetAllNodeValue(tlp::PropertyInterface*);
%Docstring
tlp.PropertyObserver.beforeSetAllNodeValue(property)

This method is called before setting the same value on all nodes in an observed property.

:param property: the observed property
:type property: sub-classes of :class:`tlp.PropertyInterface`
%End

//===========================================================================================

  virtual void afterSetAllNodeValue(tlp::PropertyInterface*);
%Docstring
tlp.PropertyObserver.afterSetAllNodeValue(property)

This method is called after setting the same value on all nodes in an observed property.

:param property: the observed property
:type property: sub-classes of :class:`tlp.PropertyInterface`
%End

//===========================================================================================

  virtual void beforeSetAllEdgeValue(tlp::PropertyInterface*);
%Docstring
tlp.PropertyObserver.beforeSetAllEdgeValue(property)

This method is called before setting the same value on all edges in an observed property.

:param property: the observed property
:type property: sub-classes of :class:`tlp.PropertyInterface`
%End

//===========================================================================================

  virtual void afterSetAllEdgeValue(tlp::PropertyInterface*);
%Docstring
tlp.PropertyObserver.afterSetAllEdgeValue(property)

This method is called after setting the same value on all edges in an observed property.

:param property: the observed property
:type property: sub-classes of :class:`tlp.PropertyInterface`
%End

//===========================================================================================

  virtual void destroy(tlp::PropertyInterface*);
%Docstring
tlp.PropertyObserver.destroy(property)

This method is called when an observed property is deleted.

:param property: the observed property
:type property: sub-classes of :class:`tlp.PropertyInterface`
%End

};

};


