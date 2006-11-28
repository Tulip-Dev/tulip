#ifndef _TULIPWITHDEPENDENCY
#define _TULIPWITHDEPENDENCY

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <vector>
#include <string>

namespace tlp {

/** \addtogroup plugins */ 
/*@{*/
class TLP_SCOPE WithDependency {
protected:
  ///
  std::vector< std::pair < std::string, std::string > > dependencies;

public:
  ///
  std::vector< std::pair < std::string, std::string > > getDependencies() {
    return dependencies;
  }
  ///
  void addDependency(const char* factory, const char *name) {
    dependencies.push_back(std::pair<std::string, std::string>(factory, name));
  }
  ///
  template<typename Ty> void addDependency(const char* str) {
    addDependency(Ty::getClassName(), str);
  }
};
/*@}*/

}
#endif


