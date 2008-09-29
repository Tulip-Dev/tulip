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
class TLP_SCOPE SizeProperty:public AbstractProperty<SizeType,SizeType, SizeAlgorithm> { 

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
protected:
  void reset_handler();
  void resetMinMax();
  virtual void setNodeValue_handler(const node n, const SizeType::RealType &);
  virtual void setEdgeValue_handler(const edge e, const SizeType::RealType &);
  virtual void setAllNodeValue_handler( const SizeType::RealType &);
  virtual void setAllEdgeValue_handler( const SizeType::RealType &);

private:
  stdext::hash_map<unsigned long, Size> max,min;
  stdext::hash_map<unsigned long, bool> minMaxOk;
  void computeMinMax(Graph * sg=NULL);
};
/*@}*/

}
#endif

