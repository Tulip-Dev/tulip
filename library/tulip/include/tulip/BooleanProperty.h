/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
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
#ifndef TULIP_SELECTION_H
#define TULIP_SELECTION_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "tulip/AbstractProperty.h"
#include "tulip/TemplateFactory.h"
#include "tulip/BooleanAlgorithm.h"

namespace tlp {

class PropertyContext;
/** \addtogroup properties */ 
/*@{*/
class TLP_SCOPE BooleanProperty:public AbstractProperty<BooleanType,BooleanType, BooleanAlgorithm> { 
  friend class BooleanAlgorithm;

public :
  BooleanProperty (Graph *g, std::string n="") :AbstractProperty<BooleanType,BooleanType, BooleanAlgorithm>(g, n) {}
  PropertyInterface* clonePrototype(Graph *, const std::string&);
  void reverse();
  ///reverse all the direction of edges of the visible graph which are true in this BooleanProperty
  void reverseEdgeDirection();
  Iterator<node> *getNodesEqualTo(const bool val, Graph * = 0);
  Iterator<edge> *getEdgesEqualTo(const bool val, Graph * = 0);
};

class TLP_SCOPE BooleanVectorProperty:public AbstractProperty<BooleanVectorType,BooleanVectorType> { 
public :
  BooleanVectorProperty(Graph *g, std::string n="") :AbstractProperty<BooleanVectorType, BooleanVectorType>(g, n) {}
  PropertyInterface* clonePrototype(Graph *, const std::string&);
};

}
/*@}*/
#endif
