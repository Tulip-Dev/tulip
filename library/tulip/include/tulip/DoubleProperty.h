//-*-c++-*-
#ifndef TULIP_METRIC_H
#define TULIP_METRIC_H

#include "tulip/AbstractProperty.h"
#include "tulip/TemplateFactory.h"
#include "tulip/DoubleAlgorithm.h"

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

namespace tlp {

class PropertyContext;

/** \addtogroup properties */ 
/*\@{*/
  class TLP_SCOPE DoubleProperty:public AbstractProperty<DoubleType,DoubleType,DoubleAlgorithm>, public PropertyObserver { 

  friend class DoubleAlgorithm;

public :
  DoubleProperty (Graph *);
  DoubleType::RealType  getNodeMin(Graph *sg=0);
  DoubleType::RealType  getNodeMax(Graph *sg=0);
  DoubleType::RealType  getEdgeMin(Graph *sg=0);
  DoubleType::RealType  getEdgeMax(Graph *sg=0);
  PropertyInterface* clonePrototype(Graph *, std::string );
  void copy( const node, const node, PropertyInterface * );
  void copy( const edge, const edge, PropertyInterface * );
  void uniformQuantification(unsigned int);

  void clone_handler(AbstractProperty<DoubleType,DoubleType> &);

  // redefinition of some PropertyObserver methods 
  virtual void beforeSetNodeValue(PropertyInterface* prop, const node n);
  virtual void beforeSetEdgeValue(PropertyInterface* prop, const edge e);
  virtual void beforeSetAllNodeValue(PropertyInterface* prop);
  virtual void beforeSetAllEdgeValue(PropertyInterface* prop);

private:
  stdext::hash_map<unsigned long, double> maxN,minN,maxE,minE;
  stdext::hash_map<unsigned long, bool> minMaxOkNode;
  stdext::hash_map<unsigned long, bool> minMaxOkEdge;
  void computeMinMaxNode(Graph *sg=0);
  void computeMinMaxEdge(Graph *sg=0);
};

class TLP_SCOPE DoubleVectorProperty:public AbstractProperty<DoubleVectorType,DoubleVectorType> { 
public :
  DoubleVectorProperty(Graph *g):AbstractProperty<DoubleVectorType, DoubleVectorType>(g) {}
  PropertyInterface* clonePrototype(Graph *, std::string );
  void copy( const node, const node, PropertyInterface * );
  void copy( const edge, const edge, PropertyInterface * );
};
/*@}*/

}
#endif
