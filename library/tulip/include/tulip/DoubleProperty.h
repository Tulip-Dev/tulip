//-*-c++-*-
#ifndef TULIP_METRIC_H
#define TULIP_METRIC_H

#include "tulip/tuliphash.h"
#include "tulip/AbstractProperty.h"
#include "tulip/TemplateFactory.h"
#include "tulip/DoubleAlgorithm.h"

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

namespace tlp {

class PropertyContext;

typedef AbstractProperty<DoubleType, DoubleType, DoubleAlgorithm> AbstractDoubleProperty;
/** \addtogroup properties */ 
/*\@{*/
  class TLP_SCOPE DoubleProperty:public AbstractDoubleProperty, public PropertyObserver { 

  friend class DoubleAlgorithm;

public :
  DoubleProperty (Graph *, std::string n="");
  DoubleType::RealType  getNodeMin(Graph *sg=0);
  DoubleType::RealType  getNodeMax(Graph *sg=0);
  DoubleType::RealType  getEdgeMin(Graph *sg=0);
  DoubleType::RealType  getEdgeMax(Graph *sg=0);
  PropertyInterface* clonePrototype(Graph *, const std::string& );
  void uniformQuantification(unsigned int);

  void clone_handler(AbstractProperty<DoubleType,DoubleType> &);

  // redefinition of some PropertyObserver methods 
  virtual void beforeSetNodeValue(PropertyInterface* prop, const node n);
  virtual void beforeSetEdgeValue(PropertyInterface* prop, const edge e);
  virtual void beforeSetAllNodeValue(PropertyInterface* prop);
  virtual void beforeSetAllEdgeValue(PropertyInterface* prop);

  enum PredefinedMetaValueCalculator {NO_CALC = 0, AVG_CALC = 1, SUM_CALC = 2,
				      MAX_CALC = 3, MIN_CALC = 4};

  // setMetaValueCalculator overloading
  virtual void setMetaValueCalculator(MetaValueCalculator* calc);
  void setMetaValueCalculator(PredefinedMetaValueCalculator nodeCalc = AVG_CALC,
			      PredefinedMetaValueCalculator edgeCalc = AVG_CALC);

private:
  TLP_HASH_MAP<unsigned int, double> maxN,minN,maxE,minE;
  TLP_HASH_MAP<unsigned int, bool> minMaxOkNode;
  TLP_HASH_MAP<unsigned int, bool> minMaxOkEdge;
  void computeMinMaxNode(Graph *sg=0);
  void computeMinMaxEdge(Graph *sg=0);
};

class TLP_SCOPE DoubleVectorProperty:public AbstractProperty<DoubleVectorType,DoubleVectorType> { 
public :
  DoubleVectorProperty(Graph *g, std::string n=""):AbstractProperty<DoubleVectorType, DoubleVectorType>(g, n) {}
  PropertyInterface* clonePrototype(Graph *, const std::string& );
};
/*@}*/

}
#endif
