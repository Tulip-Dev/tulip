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

#ifndef TULIP_PLUGINLISTER_H
#define TULIP_PLUGINLISTER_H

#include <list>
#include <string>
#include <map>

#include <tulip/Plugin.h>
#include <tulip/PluginLoader.h>
#include <tulip/Observable.h>


namespace tlp {
class PluginContext;

/**
 * @ingroup Plugins
 * @brief The base class for plugin factories.
 *
 * A plugin factory handles the creation process of a tlp::Plugin subclass. This class should never be used directly. See the PLUGIN macro for additional information.
 * @see PLUGIN
 **/
class FactoryInterface {
public:
  virtual tlp::Plugin* createPluginObject(tlp::PluginContext* context) = 0;
};

/**
 * @ingroup Plugins
 *
 * @brief The PluginLister class is a singleton used to list plugins currently loaded into Tulip and retrieve information about them.
 *
 * This class holds various methods to check information about plugins currently loaded into Tulip. You can use it to list plugins, get dependencies information or create an instance of a plugin.
 *
 * @note Since a plugin name is unique, Plugins are mainly identified by their name (tlp::Plugin::name()) when interfaced with the plugin lister.
 *
 * @see tlp::Plugin
 * @see tlp::PluginLoader
 * @see tlp::PluginLibraryLoader
 */
class TLP_SCOPE PluginLister : public Observable {
private:
  struct PluginDescription {
    FactoryInterface* factory;
    std::string library;
    Plugin* infos;

    PluginDescription(): factory(NULL), infos(NULL) {}
    ~PluginDescription() {
      delete infos;
    }
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

  /**
   * @brief Checks if a plugin of a given type is loaded
   * This method checks the plugin "pluginName" is currently loaded into Tulip and if it's of type PluginType.
   * @param PluginType the class type of the plugin
   * @param pluginName the name of the plugin
   * @return true if a matching plugin is currently loaded into Tulip.
   */
  template<typename PluginType>
  bool pluginExists(const std::string &pluginName) {
    std::map<std::string, PluginDescription>::const_iterator it =
      _plugins.find(pluginName);
    return (it != _plugins.end() &&
            (dynamic_cast<const PluginType*>(it->second.infos) != NULL));
  }

  /**
   * @brief Similar to tlp::PluginLister::getPluginObject() but returns a typed instance
   *
   * This method instantiate a plugin from its name and returns it casted into the given type.
   *
   * @param name The plugin's name
   * @param context The context to give to the plugin
   *
   * @return The plugin instance. If there is no such plugin or if the plugin does not match the required type, this method returns NULL
   */
  template<typename PluginType>
  PluginType* getPluginObject(const std::string& name,
                              tlp::PluginContext* context) {
    std::map<std::string, PluginDescription>::const_iterator it =
      _plugins.find(name);
    return (it != _plugins.end() &&
            (dynamic_cast<const PluginType*>(it->second.infos) != NULL))
           ? static_cast<PluginType*>(it->second.factory->createPluginObject(context))
           : NULL;
  }


  /**
   * @brief Gets the list of plugins of a given type (template parameter).   *
   * @return A std::list<std::string> containing the names of the plugins.
   **/
  static std::list<std::string> availablePlugins();


  template<typename PluginType>
  std::list<std::string> availablePlugins() {
    std::list<std::string> keys;

    for(std::map<std::string, PluginDescription>::const_iterator it = _plugins.begin(); it != _plugins.end(); ++it) {
      PluginType* plugin = dynamic_cast<PluginType*>(it->second.infos);

      if(plugin != NULL) {
        keys.push_back(it->first);
      }
    }

    return keys;
  }

  /**
   * @brief Gets more detailed information about one specific plug-in.
   *
   * @param name The name of the plugin to retrieve information for.
   * @return :const Plugin& The information on the plugin.
   **/
  static const Plugin& pluginInformation(const std::string& name);

  /**
   * @brief Gets more detailed information about one specific plug-in.
   * @deprecated this function should not be used anymore, please use PluginLister::pluginInformation() instead.
   *
   * @param name The name of the plugin to retrieve information for.
   * @return :const Plugin& The information on the plugin.
   **/
  static _DEPRECATED const Plugin& pluginInformations(const std::string& name);

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
  static const ParameterDescriptionList& getPluginParameters(const std::string& name);

  /**
   * @brief Gets the dependencies of a plug-in.
   *
   * @param name The name of the plug-in to retrieve the dependencies of.
   * @return :list< tlp::Dependency, std::allocator< tlp::Dependency > > The list of dependencies of the plug-in.
   **/
  static const std::list<tlp::Dependency>& getPluginDependencies(const std::string& name);

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
  static void removePlugin(const std::string& name);

  /**
   * @brief Registers a plugin into Tulip
   *
   * @warning This method should only be called by tlp::FactoryInterface subclasses
   * @see PLUGIN
   */
  static void registerPlugin(FactoryInterface* objectFactory);

protected:


  void sendPluginAddedEvent(const std::string &pluginName);
  void sendPluginRemovedEvent(const std::string &pluginName);

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
  static std::string getPluginRelease(const std::string& name);
};

class TLP_SCOPE PluginEvent : public Event {
public:

  enum PluginEventType {
    TLP_ADD_PLUGIN = 0,
    TLP_REMOVE_PLUGIN = 1
  };

  // constructor for node/edge events
  PluginEvent(PluginEventType pluginEvtType, const std::string& pluginName)
    : Event(*(tlp::PluginLister::instance()), Event::TLP_MODIFICATION),
      evtType(pluginEvtType), pluginName(pluginName) {}

  PluginEventType getType() const {
    return evtType;
  }

  std::string getPluginName() const {
    return pluginName;
  }

protected:

  PluginEventType evtType;
  std::string pluginName;

};

}

#endif //TULIP_PLUGINLISTER_H
