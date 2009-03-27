//-*-c++-*-
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

/** \addtogroup properties */ 
/*@{*/
class TLP_SCOPE IntegerProperty:public AbstractProperty<IntegerType,IntegerType, IntegerAlgorithm>, public PropertyObserver{ 

  friend class IntegerAlgorithm;

public :
  IntegerProperty (Graph *);
  IntegerType::RealType  getNodeMin();
  IntegerType::RealType  getNodeMax();
  IntegerType::RealType  getEdgeMin();
  IntegerType::RealType  getEdgeMax();
  PropertyInterface* clonePrototype(Graph *, std::string );
  void copy( const node, const node, PropertyInterface * );
  void copy( const edge, const edge, PropertyInterface * );

  // redefinition of some PropertyObserver methods 
  virtual void afterSetNodeValue(PropertyInterface* prop, const node n);
  virtual void afterSetEdgeValue(PropertyInterface* prop, const edge e);
  virtual void afterSetAllNodeValue(PropertyInterface* prop);
  virtual void afterSetAllEdgeValue(PropertyInterface* prop);

protected:
  void clone_handler(AbstractProperty<IntegerType,IntegerType> &);

private:
  int maxN,minN,maxE,minE;
  bool minMaxOk;
  void computeMinMax();
};

class TLP_SCOPE IntegerVectorProperty:public AbstractProperty<IntegerVectorType,IntegerVectorType> { 
public :
  IntegerVectorProperty(Graph *g):AbstractProperty<IntegerVectorType, IntegerVectorType>(g) {}
  PropertyInterface* clonePrototype(Graph *, std::string );
  void copy( const node, const node, PropertyInterface * );
  void copy( const edge, const edge, PropertyInterface * );
};
/*@}*/

}
#endif
