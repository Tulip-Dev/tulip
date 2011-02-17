/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */
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
  /**
   * @brief Represents a single Tulip plug-in.
   */
  struct Dependency {
    /**
     * @brief The typename of the dependency (e.g. DoubleAlgorithm)
     */
    std::string factoryName;
    /**
     * @brief The name of the plug-in, as registered in the Tulip plug-in system.
     */
    std::string pluginName;
    /**
     * @brief The required version of the plug-in.
     */
    std::string pluginRelease;

    /**
     * @brief Constructs a new dependency.
     *
     * @param fName The typename of the dependency (e.g. DoubleAlgorithm)
     * @param pName The name of the plug-in, as registered in the Tulip plug-in system.
     * @param pRelease The required version of the plug-in.
     */
    Dependency(std::string fName, std::string pName,
	       std::string pRelease) {
      factoryName = fName;
      pluginName = pName;
      pluginRelease = pRelease;
    }
  };

  /**
   * @brief Describes the dependencies of a plug-in on other plug-ins, identified by their name.
   * This allows to have a plug-in inner workings depend on other plug-ins without linking them
   * statically, or hoping depended plug-in will be there.
   */
  class WithDependency {
  protected:
    /**
     * @brief The inner list of dependencies.
     */ 
    std::list<Dependency> dependencies;

    /**
     * @brief Adds a dependency upon another plug-in.
     *
     * @param factory The type name of the plug-in (e.g. 'DoubleAlgorithm')
     * @param name The name of the plug-in, as registered in the Tulip plug-in system.
     * @param release The required version of the plug-in.
     */
    void addDependency(const char* factory, const char *name,
		       const char *release) {
      dependencies.push_back(Dependency(factory, name, release));
    }

  public:
    /**
     * @brief Gets the list of Dependencies of this plug-in.
     *
     * @return list<Dependency> The list of dependencies of this plug-in.
     */
    std::list<Dependency> getDependencies() {
      return dependencies;
    }
    
    /**
     * @brief Adds a dependency upon another plug-in.
     * The parameters should be null-terminated strings, or NULL.
     * @param name The name of the plug-in, as registered in the Tulip plug-in system.
     * @param release The required version of the plug-in.
     */
    template<typename Ty> void addDependency(const char* name, const char* release) {
      addDependency(typeid(Ty).name(), name, release);
    }
  };
  /*@}*/

}
#endif


