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
#ifndef PROPERTYOBSERVABLE_H
#define PROPERTYOBSERVABLE_H
#include <list>
#include <map>
#include <algorithm>

#include "tulip/tulipconf.h"
#include "tulip/Node.h"
#include "tulip/Edge.h"
#include "tulip/Observable.h"

namespace tlp {

class PropertyInterface;
//=========================================================

/** \addtogroup properties */ 
/*@{*/
/// Observer for Property
/**
 * The Observer pattern is described in the lecture notes and pp293-304 
 * of Design Patterns by Gamma, Helm, Johnson, and Vlissides. It is a 
 * framework for handling state dependency between observer and observed 
 * object.
 */
class  TLP_SCOPE PropertyObserver :public Observable {

 public:
  virtual ~PropertyObserver() {}
  virtual void beforeSetNodeValue(PropertyInterface*, const node){}
  virtual void afterSetNodeValue(PropertyInterface*, const node){}
  virtual void beforeSetEdgeValue(PropertyInterface*, const edge){}
  virtual void afterSetEdgeValue(PropertyInterface*, const edge){}
  virtual void beforeSetAllNodeValue(PropertyInterface*){}
  virtual void afterSetAllNodeValue(PropertyInterface*){}
  virtual void beforeSetAllEdgeValue(PropertyInterface*){}
  virtual void afterSetAllEdgeValue(PropertyInterface*){}
  virtual void destroy(const PropertyInterface*){}
 protected:
  virtual void treatEvent(const Event& ev);
};
/*@}*/
}

#endif

