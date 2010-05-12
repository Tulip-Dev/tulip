//-*-c++-*-
#ifndef TULIP_STRING_H
#define TULIP_STRING_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "tulip/AbstractProperty.h"
#include "tulip/TemplateFactory.h"
#include "tulip/StringAlgorithm.h"

namespace tlp {

class PropertyContext;

  typedef AbstractProperty<StringType,StringType, StringAlgorithm> AbstractStringProperty;
/** \addtogroup properties */ 
/*\@{*/
class TLP_SCOPE StringProperty:public AbstractStringProperty{ 

  friend class StringAlgorithm;

public :
  StringProperty (Graph *g, std::string n="") :AbstractStringProperty(g, n) {}

  PropertyInterface* clonePrototype(Graph *, const std::string&);
};

class TLP_SCOPE StringVectorProperty:public AbstractProperty<StringVectorType,StringVectorType>{ 
public :
  StringVectorProperty(Graph *g, std::string n=""):AbstractProperty<StringVectorType, StringVectorType>(g, n) {}

  PropertyInterface* clonePrototype(Graph *, const std::string&);
};
/*\@}*/
}
#endif
