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
    std::string pluginRelease;

    Dependency(std::string fName, std::string pName,
	       std::string pRelease) {
      factoryName = fName;
      pluginName = pName;
      pluginRelease = pRelease;
    }
  };

  class WithDependency {
  protected:
    ///
    std::list<Dependency> dependencies;
    /// 
    void addDependency(const char* factory, const char *name,
		       const char *release) {
      dependencies.push_back(Dependency(factory, name, release));
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
    template<typename Ty> void addDependency(const char* name, const char* release) {
      addDependency(typeid(Ty).name(), name, release);
    }
  };
  /*@}*/

}
#endif


