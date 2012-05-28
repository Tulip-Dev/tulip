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
#include <tulip/PropertyInterface.h>
#include <tulip/Observable.h>
namespace tlp {
// Use implementation from 3.7 branch to keep the Python API consistent between version 3.7 and 4.0
class  PropertyObserverPython : public Observable {

public:

  PropertyObserverPython() {}
  virtual ~PropertyObserverPython() {}
  PropertyObserverPython(const tlp::PropertyObserverPython&) {}
  PropertyObserverPython& operator=(const tlp::PropertyObserverPython &) {
    return *this;
  }

  virtual void beforeSetNodeValue(tlp::PropertyInterface*, const tlp::node) {}
  virtual void afterSetNodeValue(tlp::PropertyInterface*, const tlp::node) {}
  virtual void beforeSetEdgeValue(tlp::PropertyInterface*, const tlp::edge) {}
  virtual void afterSetEdgeValue(tlp::PropertyInterface*, const tlp::edge) {}
  virtual void beforeSetAllNodeValue(tlp::PropertyInterface*) {}
  virtual void afterSetAllNodeValue(tlp::PropertyInterface*) {}
  virtual void beforeSetAllEdgeValue(tlp::PropertyInterface*) {}
  virtual void afterSetAllEdgeValue(tlp::PropertyInterface*) {}
  virtual void destroy(tlp::PropertyInterface*) {}

protected:

  void treatEvent(const Event& ev) {
    const PropertyEvent* propEvt = dynamic_cast<const PropertyEvent*>(&ev);

    if (propEvt) {
      PropertyInterface* prop = propEvt->getProperty();

      switch(propEvt->getType()) {
      case PropertyEvent::TLP_BEFORE_SET_NODE_VALUE:
        beforeSetNodeValue(prop, propEvt->getNode());
        break;

      case PropertyEvent::TLP_AFTER_SET_NODE_VALUE:
        afterSetNodeValue(prop, propEvt->getNode());
        break;

      case PropertyEvent::TLP_BEFORE_SET_ALL_NODE_VALUE:
        beforeSetAllNodeValue(prop);
        break;

      case PropertyEvent::TLP_AFTER_SET_ALL_NODE_VALUE:
        afterSetAllNodeValue(prop);
        break;

      case PropertyEvent::TLP_BEFORE_SET_ALL_EDGE_VALUE:
        beforeSetAllEdgeValue(prop);
        break;

      case PropertyEvent::TLP_AFTER_SET_ALL_EDGE_VALUE:
        afterSetAllEdgeValue(prop);
        break;

      case PropertyEvent::TLP_BEFORE_SET_EDGE_VALUE:
        beforeSetEdgeValue(prop, propEvt->getEdge());
        break;

      case PropertyEvent::TLP_AFTER_SET_EDGE_VALUE:
        afterSetEdgeValue(prop, propEvt->getEdge());
        break;

      default:
        // this should not happen
        assert(false);
      }
    }
    else {
      PropertyInterface* prop = dynamic_cast<PropertyInterface *>(ev.sender());

      if (prop && ev.type() == Event::TLP_DELETE)
        destroy(prop);
    }
  }
};
}
%End

namespace tlp {

class PropertyObserverPython : tlp::Observable /PyName=PropertyObserver/ {
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

  PropertyObserverPython();
  virtual ~PropertyObserverPython();
  PropertyObserverPython(const tlp::PropertyObserverPython&);

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

//===========================================================================================

protected:

    virtual void treatEvent(const tlp::Event & /NoCopy/);

};

};


