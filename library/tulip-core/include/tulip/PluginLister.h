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
#ifndef TULIP_PLUGINLISTER_H
#define TULIP_PLUGINLISTER_H

#include <list>
#include <string>
#include <set>

#include <tulip/Plugin.h>
#include <tulip/TlpTools.h>
#include <tulip/PluginLibraryLoader.h>

namespace tlp {
class PluginContext;

/**
 * @ingroup Plugins
 * @brief This abstract class provides a more complete interface for plugin factories, including plugin creation.
 *
 **/
class FactoryInterface {
public:
  /**
   * @brief Creates a new Algorithm object.
   *
   * @param context The context for the new plug-in.
   * @return PluginObject* A newly created algorithm plug-in.
   **/
  virtual tlp::Plugin* createPluginObject(tlp::PluginContext* context) = 0;
};

/**
 * @ingroup Plugins
 * @brief The PluginLister class
 */
class TLP_SCOPE PluginLister {
private:
  struct PluginDescription {
    FactoryInterface* factory;
    std::string library;
  };

public:
  static PluginLoader *currentLoader;

  /**
   * @brief Checks if all registered plug-ins have their dependencies met.
   *
   * @param loader If there are errors, the loader is informed about them so they can be displayed.
   * @return void
   **/
  static void checkLoadedPluginsDependencies(tlp::PluginLoader* loader);

  /**
   * @brief Gets the static instance of this class. If not already done, creates it beforehand.
   *
   * @return PluginLister< ObjectType, Context >* The only instance of this object that exists in the whole program.
   **/
  static tlp::PluginLister* instance();

  /**
   * @brief Constructs a plug-in.
   *
   * @param name The name of the plug-in to instantiate.
   * @param p The context to give to the plug-in.
   * @return ObjectType* The newly constructed plug-in.
   **/
  static tlp::Plugin* getPluginObject(const std::string& name, tlp::PluginContext* context);

  template<typename PluginType>
  bool pluginExists(const std::string &pluginName) {
    const Plugin* p = getPluginObject(pluginName,NULL);

    if (p == NULL)
      return false;

    bool result = dynamic_cast<const PluginType*>(p) != NULL;
    delete p;
    return result;
  }

  template<typename PluginType>
  PluginType* getPluginObject(const std::string& name, tlp::PluginContext* context) {
    return dynamic_cast<PluginType*>(getPluginObject(name, context));
  }


  /**
   * @brief Gets the list of plug-ins that registered themselves in this factory.
   *
   * @return :Iterator< std::string >* An iterator over the names of the plug-ins registered in this factory.
   **/
  static std::list<std::string> availablePlugins();

  template<typename PluginType>
  std::list<std::string> availablePlugins() {
    std::list<std::string> keys;

    for(std::map<std::string , PluginDescription>::const_iterator it = _plugins.begin(); it != _plugins.end(); ++it) {
      PluginType* plugin = dynamic_cast<PluginType*>(it->second.factory->createPluginObject(NULL));

      if(plugin != NULL) {
        keys.push_back(it->first);
      }

      delete plugin;
    }

    return keys;
  }

  /**
   * @brief Gets more detailed informations about one specific plug-in.
   *
   * @param name The name of the plugin to retrieve informations for.
   * @return :Plugin* The informations on the plugin.
   **/
  static const Plugin* pluginInformations(const std::string& name);

  /**
   * @brief Checks if a given name is registered in this factory.
   *
   * @param pluginName The name of the plug-in to look for.
   * @return bool Whether there is a plug-in with the given name registered in this factory.
   **/
  static bool pluginExists(const std::string& pluginName);

  /**
   * @brief Gets the list of parameters for the given plug-in.
   *
   * @param name The name of the plug-in to retrieve the parameters of.
   * @return :ParameterDescriptionList The parameters of the plug-in.
   **/
  static const ParameterDescriptionList getPluginParameters(std::string name);

  /**
   * @brief Gets the dependencies of a plug-in.
   *
   * @param name The name of the plug-in to retrieve the dependencies of.
   * @return :list< tlp::Dependency, std::allocator< tlp::Dependency > > The list of dependencies of the plug-in.
   **/
  static std::list<tlp::Dependency> getPluginDependencies(std::string name);

  /**
   * @brief Gets the library from which a plug-in has been loaded.
   *
   * @param name The name of the plug-in to retrieve the library of.
   * @return std::string The library from which the plug-in was loaded.
   **/
  static std::string getPluginLibrary(const std::string& name);

  /**
   * @brief Removes a plug-in from this factory.
   * This is usefull when a plug-in has unmet dependencies, or appears more than once.
   *
   * @param name The name of the plug-in to remove.
   * @return void
   **/
  static void removePlugin(const std::string &name);

  static void registerPlugin(FactoryInterface* objectFactory);

protected:
  static PluginLister* _instance;

  /**
   * @brief Stores the factory, dependencies, and parameters of all the plugins that register into this factory.
   **/
  std::map<std::string , PluginDescription> _plugins;

  /**
   * @brief Gets the release number of the given plug-in.
   *
   * @param name The name of the plug-in to retrieve the version number of.
   * @return :string The version number, ussually formatted as X[.Y], where X is the major, and Y the minor.
   **/
  static std::string getPluginRelease(std::string name);
};

}

#endif //TULIP_PLUGINLISTER_H
