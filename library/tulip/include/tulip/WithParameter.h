#ifndef _TULIPWITHPARAMETER
#define _TULIPWITHPARAMETER

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "tulip/Reflect.h"

namespace tlp {

/** \addtogroup plugins */ 
/*@{*/
struct TLP_SCOPE WithParameter {
  ///
  StructDef getParameters();
  ///
  template<typename Ty> void addParameter(const char* str,
					  const char* inHelp=0,
					  const char* inDefValue = 0,
					  bool isMandatory = true)
  { parameters.template add<Ty>(str,inHelp,inDefValue,isMandatory); }
protected:
  ///
  StructDef parameters;
};
/*@}*/

}
#endif


