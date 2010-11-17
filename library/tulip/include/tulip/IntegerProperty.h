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
#ifndef TULIP_INT_H
#define TULIP_INT_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "tulip/AbstractProperty.h"
#include "tulip/TemplateFactory.h"
#include "tulip/IntegerAlgorithm.h"

namespace tlp {

class Graph;
class PropertyContext;

typedef AbstractProperty<tlp::IntegerType, tlp::IntegerType, tlp::IntegerAlgorithm> AbstractIntegerProperty;
/** \addtogroup properties */ 
/*@{*/
 class TLP_SCOPE IntegerProperty:public AbstractIntegerProperty, public PropertyObserver, public GraphObserver { 

  friend class IntegerAlgorithm;

public :
  IntegerProperty(Graph *, std::string n = "");
  IntegerType::RealType  getNodeMin(Graph *sg=0);
  IntegerType::RealType  getNodeMax(Graph *sg=0);
  IntegerType::RealType  getEdgeMin(Graph *sg=0);
  IntegerType::RealType  getEdgeMax(Graph *sg=0);
  PropertyInterface* clonePrototype(Graph *, const std::string&);

  // redefinition of some PropertyObserver methods 
  virtual void beforeSetNodeValue(PropertyInterface* prop, const node n);
  virtual void beforeSetEdgeValue(PropertyInterface* prop, const edge e);
  virtual void beforeSetAllNodeValue(PropertyInterface* prop);
  virtual void beforeSetAllEdgeValue(PropertyInterface* prop);

  // redefinition of GraphObserver methods
  virtual void addNode(Graph* graph, const node n);
  virtual void addEdge(Graph* graph, const edge e);

protected:
  void clone_handler(AbstractProperty<IntegerType,IntegerType> &);

private:
  TLP_HASH_MAP<unsigned int, int> maxN,minN,maxE,minE;
  TLP_HASH_MAP<unsigned int, bool> minMaxOkNode;
  TLP_HASH_MAP<unsigned long, bool> minMaxOkEdge;
  void computeMinMaxNode(Graph *sg=0);
  void computeMinMaxEdge(Graph *sg=0);
};

class TLP_SCOPE IntegerVectorProperty:public AbstractProperty<tlp::IntegerVectorType, tlp::IntegerVectorType> { 
public :
  IntegerVectorProperty(Graph *g, std::string n =""):AbstractProperty<IntegerVectorType, IntegerVectorType>(g, n) {}
  PropertyInterface* clonePrototype(Graph *, const std::string&);
};
/*@}*/

}
#endif
