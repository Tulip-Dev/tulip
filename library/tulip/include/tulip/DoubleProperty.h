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
class TLP_SCOPE DoubleProperty:public AbstractProperty<DoubleType,DoubleType,DoubleAlgorithm> { 

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

protected:
  virtual void setNodeValue_handler(const node n, const DoubleType::RealType &);
  virtual void setEdgeValue_handler(const edge e, const DoubleType::RealType &);
  virtual void setAllNodeValue_handler(const DoubleType::RealType &);
  virtual void setAllEdgeValue_handler(const DoubleType::RealType &);
  void clone_handler(AbstractProperty<DoubleType,DoubleType> &);

private:
  stdext::hash_map<unsigned long, double> maxN,minN,maxE,minE;
  stdext::hash_map<unsigned long, bool> minMaxOkNode;
  stdext::hash_map<unsigned long, bool> minMaxOkEdge;
  void computeMinMaxNode(Graph *sg=0);
  void computeMinMaxEdge(Graph *sg=0);
};
/*@}*/

}
#endif
