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
#include <iostream>
#include <tulip/PropertyInterface.h>
#include <tulip/Graph.h>

using namespace tlp;

PropertyInterface::~PropertyInterface() {
  // check if the current property is not registered
  // as a property of a graph
  if (graph && !name.empty() &&
      graph->existLocalProperty(name) &&
      graph->getProperty(name) == this) {
    std::cerr << "Warning : "  << __PRETTY_FUNCTION__ << " ... Serious bug; you have deleted a registered graph property named '"  << name.c_str() << "'" << std::endl;
    abort();
  }
  observableDeleted();
}

void PropertyInterface::addPropertyObserver(Observable *pObs) {
  addListener(pObs);
}

void PropertyInterface::removePropertyObserver(Observable *pObs) {
  removeListener(pObs);
}

void PropertyInterface::notifyBeforeSetNodeValue(const node n) {
  if (hasOnlookers())
    sendEvent(PropertyEvent(*this, PropertyEvent::TLP_BEFORE_SET_NODE_VALUE,
			    Event::TLP_INFORMATION, n));
}

void PropertyInterface::notifyAfterSetNodeValue(const node n) {
  if (hasOnlookers())
    sendEvent(PropertyEvent(*this, PropertyEvent::TLP_AFTER_SET_NODE_VALUE,
			    Event::TLP_MODIFICATION, n));
}

void PropertyInterface::notifyBeforeSetEdgeValue(const edge e) {
  if (hasOnlookers())
    sendEvent(PropertyEvent(*this, PropertyEvent::TLP_BEFORE_SET_EDGE_VALUE,
			    Event::TLP_INFORMATION, e));
}

void PropertyInterface::notifyAfterSetEdgeValue(const edge e) {
  if (hasOnlookers())
    sendEvent(PropertyEvent(*this, PropertyEvent::TLP_AFTER_SET_EDGE_VALUE,
			    Event::TLP_MODIFICATION, e));
}

void PropertyInterface::notifyBeforeSetAllNodeValue() {
  if (hasOnlookers())
    sendEvent(PropertyEvent(*this,
			    PropertyEvent::TLP_BEFORE_SET_ALL_NODE_VALUE,
			    Event::TLP_INFORMATION));
}

void PropertyInterface::notifyAfterSetAllNodeValue() {
  if (hasOnlookers())
    sendEvent(PropertyEvent(*this,
			    PropertyEvent::TLP_AFTER_SET_ALL_NODE_VALUE,
			    Event::TLP_MODIFICATION));
}

void PropertyInterface::notifyBeforeSetAllEdgeValue() {
  if (hasOnlookers())
    sendEvent(PropertyEvent(*this,
			    PropertyEvent::TLP_BEFORE_SET_ALL_EDGE_VALUE,
			    Event::TLP_INFORMATION));
}

void PropertyInterface::notifyAfterSetAllEdgeValue() {
  if (hasOnlookers())
    sendEvent(PropertyEvent(*this,
			    PropertyEvent::TLP_AFTER_SET_ALL_EDGE_VALUE,
			    Event::TLP_MODIFICATION));
}

