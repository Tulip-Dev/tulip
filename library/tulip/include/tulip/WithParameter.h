#ifndef _TULIPWITHPARAMETER
#define _TULIPWITHPARAMETER

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "Reflect.h"
/** \addtogroup plugins */ 
/*@{*/
struct TLP_SCOPE WithParameter {
  ///
  StructDef getParameters();
  ///
  template<typename Ty> void addParameter(const char* str,
					  const char* inHelp=0,
					  const char* inDefValue = 0)
  { parameters.template add<Ty>(str,inHelp,inDefValue); }
protected:
  ///
  StructDef parameters;
};
/*@}*/
#endif


