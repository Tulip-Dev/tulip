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
  template<typename Ty> void addParameter(	std::string str,
  											const char * inHelp=0,
  											std::string inDefValue = std::string()	)
  											{ parameters.template add<Ty>(str,inHelp,inDefValue); }
protected:
  ///
  StructDef parameters;
};
/*@}*/
#endif


