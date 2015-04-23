/*
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
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

#include <list>
#include <string>
#include <typeinfo>
#include <tulip/tulipconf.h>

namespace tlp {

/**
 * @ingroup Plugins
 *
 * @brief Represents a plugin's dependency to another plugin.
 * In addition to maganing plugin registration, Tulip also handles a dependency mechanism between plugins.
 * Every Tulip plugin inherits from the tlp::WithDependency interface which allows to declare that another plugin should be loaded in order for this plugin to run.
 * When declaring a dependency, a plugin state the name and the version of the dependecy. This is done by calling tlp::WithDependency::addDependency()
 *
 * @see tlp::WithDependency
 */
struct Dependency {
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
   * @param pName The name of the plug-in, as registered in the Tulip plug-in system.
   * @param pRelease The required version of the plug-in.
   */
  Dependency(std::string pName,
             std::string pRelease):pluginName(pName),pluginRelease(pRelease) {

  }
};

/**
 * @ingroup Plugins
 * @brief Describes the dependencies of a plug-in on other plug-ins, identified by their name and their version number.
 *
 * This allows to have a plug-in inner workings depend on other plug-ins without linking them statically, or hoping depended plug-in will be there.
 */
class WithDependency {
protected:
  /**
   * @brief The inner list of dependencies.
   */
  std::list<Dependency> _dependencies;
public:
  /**
   * @brief Adds a dependency upon another plug-in.
   *
   * @param factory The type name of the plug-in (e.g. 'DoubleAlgorithm')
   * @param name The name of the plug-in, as registered in the Tulip plug-in system.
   * @param release The required version of the depended plug-in.
   */
  void addDependency(const char *name,const char *release) {
    _dependencies.push_back(Dependency(name, release));
  }

  /**
   * @brief Gets the list of Dependencies of this plug-in.
   *
   * @return list<Dependency> The list of dependencies of this plug-in.
   */
  const std::list<Dependency>& dependencies() const {
    return _dependencies;
  }

};


}
#endif
