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
/** \addtogroup properties */ 
/*\@{*/
class TLP_SCOPE StringProperty:public AbstractProperty<StringType,StringType, StringAlgorithm>{ 

  friend class StringAlgorithm;

public :
  StringProperty (Graph *);
  ~StringProperty();
  PropertyInterface* clonePrototype(Graph *, std::string );
  void copy( const node, const node, PropertyInterface * );
  void copy( const edge, const edge, PropertyInterface * );

protected:
  void reset_handler();
  void recompute_handler();  
};
/*\@}*/
}
#endif
