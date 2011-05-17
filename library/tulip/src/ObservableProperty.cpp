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
#include <tulip/ObservableProperty.h>
#include <tulip/PropertyInterface.h>

using namespace stdext;
using namespace tlp;

void PropertyObserver::RealPropertyObserver::treatEvent(const Event& ev) {
  const PropertyEvent* propEvt = dynamic_cast<const PropertyEvent*>(&ev);
  if (propEvt) {
    PropertyInterface* prop = propEvt->getProperty();
    switch(propEvt->getType()) {
    case PropertyEvent::TLP_BEFORE_SET_NODE_VALUE:
      visibleObserver->beforeSetNodeValue(prop, propEvt->getNode());
      break;
    case PropertyEvent::TLP_AFTER_SET_NODE_VALUE:
      visibleObserver->afterSetNodeValue(prop, propEvt->getNode());
      break;
    case PropertyEvent::TLP_BEFORE_SET_ALL_NODE_VALUE:
      visibleObserver->beforeSetAllNodeValue(prop);
      break;
    case PropertyEvent::TLP_AFTER_SET_ALL_NODE_VALUE:
      visibleObserver->afterSetAllNodeValue(prop);
      break;
    case PropertyEvent::TLP_BEFORE_SET_ALL_EDGE_VALUE:
      visibleObserver->beforeSetAllEdgeValue(prop);
      break;
    case PropertyEvent::TLP_AFTER_SET_ALL_EDGE_VALUE:
      visibleObserver->afterSetAllEdgeValue(prop);
      break;
    case PropertyEvent::TLP_BEFORE_SET_EDGE_VALUE:
      visibleObserver->beforeSetEdgeValue(prop, propEvt->getEdge());
      break;
    case PropertyEvent::TLP_AFTER_SET_EDGE_VALUE:
      visibleObserver->afterSetEdgeValue(prop, propEvt->getEdge());
      break;
    default:
      // this should not happen
      assert(false);
    }
  } else {
    PropertyInterface* prop = dynamic_cast<PropertyInterface *>(ev.sender());
    if (prop && ev.type() == Event::TLP_DELETE)
      visibleObserver->destroy(prop);
  }
}
