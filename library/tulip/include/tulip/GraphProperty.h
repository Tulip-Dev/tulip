//-*-c++-*-
#ifndef TULIP_METAGRAPH_H
#define TULIP_METAGRAPH_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "AbstractProperty.h"
#include "TemplateFactory.h"
#include "GraphAlgorithm.h"

namespace tlp {

class PropertyContext;

/** \addtogroup properties */ 
/*@{*/
class TLP_SCOPE GraphProperty:public AbstractProperty<GraphType,GraphType>, public GraphObserver { 
  friend class GraphAlgorithm;

public :
  GraphProperty (Graph *);
  virtual ~GraphProperty();
  PropertyInterface* clonePrototype(Graph *, std::string );
  void copy( const node, const node, PropertyInterface * );
  void copy( const edge, const edge, PropertyInterface * );
  void destroy(Graph *);
  
private:
  MutableContainer<std::set<node> > referencedGraph;
  void setNodeValue_handler(const node n, const GraphType::RealType &);
  void setAllNodeValue_handler(const GraphType::RealType &);

};
/*@}*/

}
#endif

