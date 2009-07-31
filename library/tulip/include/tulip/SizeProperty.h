//-*-c++-*-
#ifndef TULIP_SIZES_H
#define TULIP_SIZES_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "tulip/tuliphash.h"
#include "tulip/AbstractProperty.h"
#include "tulip/TemplateFactory.h"
#include "tulip/SizeAlgorithm.h"

namespace tlp {

class PropertyContext;
/**
 * \addtogroup properties
 */ 
/*@{*/
  class TLP_SCOPE SizeProperty:public AbstractProperty<SizeType,SizeType, SizeAlgorithm>,
  public PropertyObserver { 

  friend class SizeAlgorithm;

public :
  SizeProperty (Graph *, std::string n="");

  Size getMax(Graph *sg=0);
  Size getMin(Graph *sg=0);
  PropertyInterface* clonePrototype(Graph *, const std::string&);
  void copy( const node, const node, PropertyInterface * );
  void copy( const edge, const edge, PropertyInterface * );
  void scale( const tlp::Vector<float,3>&, Graph *sg=0 );
  void scale( const tlp::Vector<float,3>&, Iterator<node> *, Iterator<edge> *);

  // redefinition of some PropertyObserver methods 
  virtual void beforeSetNodeValue(PropertyInterface* prop, const node n);
  virtual void beforeSetEdgeValue(PropertyInterface* prop, const edge e);
  virtual void beforeSetAllNodeValue(PropertyInterface* prop);
  virtual void beforeSetAllEdgeValue(PropertyInterface* prop);

protected:
  void resetMinMax();

private:
  TLP_HASH_MAP<unsigned long, Size> max,min;
  TLP_HASH_MAP<unsigned long, bool> minMaxOk;
  void computeMinMax(Graph * sg=NULL);
};

class TLP_SCOPE SizeVectorProperty:public AbstractProperty<SizeVectorType,SizeVectorType> { 
public :
  SizeVectorProperty(Graph *g, std::string n=""):AbstractProperty<SizeVectorType, SizeVectorType>(g, n) {}

  PropertyInterface* clonePrototype(Graph *, const std::string&);
  void copy( const node, const node, PropertyInterface * );
  void copy( const edge, const edge, PropertyInterface * );
};
/*@}*/

}
#endif

