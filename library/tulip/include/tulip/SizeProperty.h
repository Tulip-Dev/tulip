//-*-c++-*-
#ifndef TULIP_SIZES_H
#define TULIP_SIZES_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

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
  SizeProperty (Graph *);

  Size getMax(Graph *sg=0);
  Size getMin(Graph *sg=0);
  PropertyInterface* clonePrototype(Graph *, std::string );
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
  stdext::hash_map<unsigned long, Size> max,min;
  stdext::hash_map<unsigned long, bool> minMaxOk;
  void computeMinMax(Graph * sg=NULL);
};
/*@}*/

}
#endif

