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
#ifndef TULIP_METAGRAPH_H
#define TULIP_METAGRAPH_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "tulip/AbstractProperty.h"
#include "tulip/TemplateFactory.h"

namespace tlp {

class PropertyContext;
class GraphAbstract;

typedef AbstractProperty<tlp::GraphType, tlp::EdgeSetType> AbstractGraphProperty;

/** \addtogroup properties */ 
/*@{*/
  class TLP_SCOPE GraphProperty:public AbstractGraphProperty,
  public GraphObserver { 
  friend class GraphAbstract;

public :
  GraphProperty (Graph *, std::string n="");
  virtual ~GraphProperty();
  PropertyInterface* clonePrototype(Graph *, const std::string&);
  void destroy(Graph *);
  bool setNodeStringValue(const node n, const std::string & v);
  bool setAllNodeStringValue(const std::string & v);
  bool setEdgeStringValue( const edge e, const std::string & v);
  bool setAllEdgeStringValue(const std::string & v);

  // redefinition of some AbstractProperty methods 
  virtual void setNodeValue(const node n, const GraphType::RealType& g);
  virtual void setAllNodeValue(const GraphType::RealType& g);

  // for optimizations purpose
  bool hasNonDefaultValue(const node n) const {
    return nodeProperties.hasNonDefaultValue(n.id);
  }
  bool hasNonDefaultValue(const edge e) const  {
    return !edgeProperties.get(e.id).empty();
  }

private:
  MutableContainer<std::set<node> > referencedGraph;
  const std::set<edge>& getReferencedEdges(const edge) const;
};
/*@}*/

}
#endif

