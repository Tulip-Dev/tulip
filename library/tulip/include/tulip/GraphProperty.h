//-*-c++-*-
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

/** \addtogroup properties */ 
/*@{*/
  class TLP_SCOPE GraphProperty:public AbstractProperty<GraphType,EdgeSetType>, public GraphObserver, public PropertyObserver { 
  friend class GraphAbstract;

public :
  GraphProperty (Graph *, std::string n="");
  virtual ~GraphProperty();
  PropertyInterface* clonePrototype(Graph *, const std::string&);
  void copy( const node, const node, PropertyInterface * );
  void copy( const edge, const edge, PropertyInterface * );
  void destroy(Graph *);
  bool setNodeStringValue(const node n, const std::string & v);
  bool setAllNodeStringValue(const std::string & v);
  bool setEdgeStringValue( const edge e, const std::string & v);
  bool setAllEdgeStringValue(const std::string & v);

  // redefinition of some PropertyObserver methods 
  virtual void beforeSetNodeValue(PropertyInterface* prop, const node n);
  virtual void afterSetNodeValue(PropertyInterface* prop, const node n);
  virtual void beforeSetAllNodeValue(PropertyInterface* prop);
  virtual void afterSetAllNodeValue(PropertyInterface* prop);

  // for optimizations purpose
  bool hasNonDefaultValue(const node n) const {
    return nodeProperties.hasNonDefaultValue(n.id);
  }
  bool hasNonDefaultValue(const edge e) const  {
    return nodeProperties.hasNonDefaultValue(e.id);
  }

private:
  MutableContainer<std::set<node> > referencedGraph;
  const std::set<edge>& getReferencedEdges(const edge) const;
  
};
/*@}*/

}
#endif

