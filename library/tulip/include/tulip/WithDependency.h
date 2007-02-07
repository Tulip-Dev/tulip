#ifndef _TULIPWITHDEPENDENCY
#define _TULIPWITHDEPENDENCY

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <list>
#include <string>
#include <typeinfo>
#include <tulip/tulipconf.h>

namespace tlp {

  /** \addtogroup plugins */ 
  /*@{*/
  struct Dependency {
    std::string factoryName;
    std::string pluginName;
    char** pluginParams;

    Dependency(std::string fName, std::string pName, char* pParams[]) {
      factoryName = fName;
      pluginName = pName;
      pluginParams = pParams;
    }
  };

  class WithDependency {
  protected:
    ///
    std::list<Dependency> dependencies;
    /// 
    void addDependency(const char* factory, const char *name, char* params[]) {
      dependencies.push_back(Dependency(factory, name, params));
    }

  public:
    ///
    std::list<Dependency> getDependencies() {
      return dependencies;
    }
    /** Indicates that the current algorithm depends on the named algorithm of type 'Ty'
     *  which is supposing to have the parameters specified as second argument.
     *  If non null the second argument needs to be a null terminated array of character strings.
     */
    template<typename Ty> void addDependency(const char* name, char* params[] = 0) {
      addDependency(typeid(Ty).name(), name, params);
    }
  };
  /*@}*/

}
#endif


