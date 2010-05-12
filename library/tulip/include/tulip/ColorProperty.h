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

typedef AbstractProperty<ColorType, ColorType, ColorAlgorithm> AbstractColorProperty;

/** \addtogroup properties */ 
/*@{*/
class TLP_SCOPE ColorProperty:public AbstractColorProperty { 
  friend class ColorAlgorithm;
public :
  ColorProperty (Graph *g, std::string n="") :AbstractColorProperty(g, n) {}
  PropertyInterface* clonePrototype(Graph *, const std::string&);
};

class TLP_SCOPE ColorVectorProperty:public AbstractProperty<ColorVectorType,ColorVectorType> { 
public :
  ColorVectorProperty(Graph *g, std::string n="") :AbstractProperty<ColorVectorType, ColorVectorType>(g, n) {}
  PropertyInterface* clonePrototype(Graph *, const std::string&);
};
/*@}*/

}
#endif

