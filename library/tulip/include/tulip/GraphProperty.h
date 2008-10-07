//-*-c++-*-
#ifndef TULIP_METAGRAPH_H
#define TULIP_METAGRAPH_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "tulip/AbstractProperty.h"
#include "tulip/TemplateFactory.h"
#include "tulip/GraphAlgorithm.h"

namespace tlp {

class PropertyContext;
class GraphAbstract;

/** \addtogroup properties */ 
/*@{*/
class TLP_SCOPE GraphProperty:public AbstractProperty<GraphType,EdgeSetType>, public GraphObserver { 
  friend class GraphAlgorithm;
  friend class GraphAbstract;

public :
  GraphProperty (Graph *);
  virtual ~GraphProperty();
  PropertyInterface* clonePrototype(Graph *, std::string );
  void copy( const node, const node, PropertyInterface * );
  void copy( const edge, const edge, PropertyInterface * );
  void destroy(Graph *);
  bool setNodeStringValue(const node n, const std::string & v);
  bool setAllNodeStringValue(const std::string & v);
  bool setEdgeStringValue( const edge e, const std::string & v);
  bool setAllEdgeStringValue(const std::string & v);

private:
  MutableContainer<std::set<node> > referencedGraph;
  void setNodeValue_handler(const node n, const GraphType::RealType &);
  void setAllNodeValue_handler(const GraphType::RealType &);
  const std::set<edge>& getReferencedEdges(const edge) const;
  
};
/*@}*/

}
#endif

