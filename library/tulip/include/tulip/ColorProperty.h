//-*-c++-*-
#ifndef TULIP_COLORS_H
#define TULIP_COLORS_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "tulip/AbstractProperty.h"
#include "tulip/TemplateFactory.h"
#include "tulip/ColorAlgorithm.h"

namespace tlp {

class PropertyContext;
/** \addtogroup properties */ 
/*@{*/
class TLP_SCOPE ColorProperty:public AbstractProperty<ColorType,ColorType, ColorAlgorithm> { 
  friend class ColorAlgorithm;
public :
  ColorProperty (Graph *g, std::string n="") :AbstractProperty<ColorType,ColorType, ColorAlgorithm>(g, n) {}
  PropertyInterface* clonePrototype(Graph *, const std::string&);
  void copy( const node, const node, PropertyInterface * );
  void copy( const edge, const edge, PropertyInterface * );
};

class TLP_SCOPE ColorVectorProperty:public AbstractProperty<ColorVectorType,ColorVectorType> { 
public :
  ColorVectorProperty(Graph *g, std::string n="") :AbstractProperty<ColorVectorType, ColorVectorType>(g, n) {}
  PropertyInterface* clonePrototype(Graph *, const std::string&);
  void copy( const node, const node, PropertyInterface * );
  void copy( const edge, const edge, PropertyInterface * );
};
/*@}*/

}
#endif

