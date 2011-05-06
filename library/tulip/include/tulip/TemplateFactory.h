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
#ifndef _TEMPLATEFACTORY_H
#define _TEMPLATEFACTORY_H

#include <list>
#include <string>
#include <typeinfo>
#include <set>

#include <tulip/PluginLoader.h>
#include <tulip/WithParameter.h>
#include <tulip/WithDependency.h>
#include <tulip/Iterator.h>
#include <tulip/Plugin.h>
#include <tulip/TlpTools.h>
#include <tulip/PluginContext.h>

namespace tlp {

/** @addtogroup plugins
 * @brief The Tulip plug-in system allows plug-ins to be loaded dynamically at run-time, and can check dependencies on other plug-ins, as well as multiple definitions.
 *
 * The Tulip plug-in system can be decomposed in 4 layers:
 *
 * 1: Tulip, who knows about TemplateFactories
 *
 * 2: TemplateFactories, who know about Plugin subclasses, usually called factories.
 *
 * 3: Plugin subclasses, who know about the plugin itself
 *
 * 4: The plugin itself, a subclass of Algorithm (more likely IntegerAlgorithm, DoubleAlgorithm, ...), View, ControllerViewsManager, ...
 */

/*@{*/

/**
 * @brief This interface lists functions used to regroup plug-ins.
 *
 * It is used to list plug-ins that register themselves into it.
 * 
 * The TemplateFactory's role is to list plug-ins, and retrive their dependencies for Tulip to check if they are met.
 * The only check performed should be the unicity of a plug-in in the system.
 * 
 * Each Tulip plug-in has a factory, which needs to be registered into a TemplateFactory.
 * TemplateFactories register themselves in the Tulip plug-in system, and Tulip lists the plug-ins of each TemplateFactory.
 * 
 **/
class PluginManagerInterface {
public:
  static TLP_SCOPE std::map< std::string, PluginManagerInterface* > *allFactories;
  static TLP_SCOPE PluginLoader *currentLoader;

  virtual ~PluginManagerInterface(){}

  /**
   * @brief Gets the list of plug-ins that registered themselves in this factory.
   *
   * @return :Iterator< std::string >* An iterator over the names of the plug-ins registered in this factory.
   **/
  virtual Iterator<std::string>* availablePlugins()=0;
  
  /**
   * @brief Checks if a given name is registered in this factory.
   *
   * @param pluginName The name of the plug-in to look for.
   * @return bool Whether there is a plug-in with the given name registered in this factory.
   **/
  virtual bool pluginExists(const std::string &pluginName)=0;
  
  /**
   * @brief Gets the list of parameters for the given plug-in.
   *
   * @param name The name of the plug-in to retrieve the parameters of.
   * @return :StructDef The parameters of the plug-in.
   **/
  virtual StructDef getPluginParameters(std::string name)=0;

  /**
   * @brief Gets the release number of the given plug-in.
   *
   * @param name The name of the plug-in to retrieve the version number of.
   * @return :string The version number, ussually formatted as X[.Y], where X is the major, and Y the minor.
   **/
  virtual std::string getPluginRelease(std::string name)=0;

  /**
   * @brief Gets the dependencies of a plug-in.
   *
   * @param name The name of the plug-in to retrieve the dependencies of.
   * @return :list< tlp::Dependency, std::allocator< tlp::Dependency > > The list of dependencies of the plug-in.
   **/
  virtual std::list<tlp::Dependency> getPluginDependencies(std::string name)=0;

  /**
   * @brief Gets the class name for the plug-in's registered class.
   *  If the class is in the tlp namespace, the 'tlp::' prefix is removed.
   *
   * @return :string The class name of the plug-in.
   **/
  virtual std::string getPluginsClassName()=0;

  /**
   * @brief Removes a plug-in from this factory.
   * This is usefull when a plug-in has unmet dependencies, or appears more than once.
   *
   * @param name The name of the plug-in to remove.
   * @return void
   **/
  virtual void removePlugin(const std::string& name)=0;

  /**
   * @brief Adds a factory to a static map of factories.
   * This map is then used to list all the factories, and all the plug-ins for each factory.
   *
   * @param factory The factory to add.
   * @param name The name of the factory to add, used as key.   
   * @return void
   **/
  static void addFactory(PluginManagerInterface *factory, const std::string &name)  {
    if (!allFactories)
      allFactories = new std::map<std::string, PluginManagerInterface*>();
    //std::cerr << name.c_str() << " factory added" << std::endl;
    (*allFactories)[name] = factory;
  }

  /**
   * @brief Checks if a plug-in exists in a specific factory.
   * In debug mode, an assert checks the factory is registered in the factory map before accessing it.
   * 
   * @param factoryName The name of the factory to look into.
   * @param pluginName The name of the plugin to look for.
   * @return bool Whether the plug-in exists in the specified factory.
   **/
  static bool pluginExists(const std::string& factoryName, const std::string& pluginName) {
    assert(allFactories->find(factoryName) != allFactories->end());
    return (*allFactories)[factoryName]->pluginExists(pluginName);
  }
};

template <class PluginObject, class PluginContext>
class FactoryInterface;

/**
 * @brief This template class takes 3 parameters :
 * 
 * * The type of factory that it will list,
 * 
 * * The type of object said factories build,
 * 
 * * The type of object to pass as parameter to the objects when building them.
 *
 * When constructed it registers itself into the factories map automatically.
 **/
template<class ObjectType, class Context> class PluginManager: public PluginManagerInterface {
public:
  PluginManager() {
    PluginManagerInterface::addFactory(this, tlp::demangleTlpClassName(typeid(ObjectType).name()));
  }

  typedef std::map< std::string , FactoryInterface<ObjectType, Context> * > ObjectCreator;

  /**
   * @brief Stores the factories that register into this TemplateFactory.
   **/
  ObjectCreator objMap;
  /**
   * @brief Stores the parameters of each registered plug-in.
   **/
  std::map<std::string,StructDef> objParam;
  /**
   * @brief Stores the list of registered plug-ins.
   **/
  std::set<std::string> objNames;
  /**
   * @brief Stores the dependencies of the registered plug-ins.
   **/
  std::map<std::string, std::list<tlp::Dependency> > objDeps;
  /**
   * @brief Stores the release version of the registered plug-ins.
   **/
  std::map<std::string, std::string> objRels;

  /**
   * @brief Constructs a plug-in.
   *
   * @param name The name of the plug-in to instantiate.
   * @param p The context to give to the plug-in.
   * @return ObjectType* The newly constructed plug-in.
   **/
  ObjectType *getPluginObject(const std::string& name, Context p);

  //the following function are inherited from TemplateFactoryInterface, and by default inherit the doc.
  Iterator<std::string>* availablePlugins();
  bool pluginExists(const std::string& pluginName);
  StructDef getPluginParameters(std::string name);
  std::string getPluginRelease(std::string name);
  std::list<tlp::Dependency> getPluginDependencies(std::string name);
  std::string getPluginsClassName();
  void registerPlugin(FactoryInterface<ObjectType, Context>* objectFactory);
  void removePlugin(const std::string &name);
};

template <class PropertyAlgorithm> class PropertyFactory : public FactoryInterface<PropertyAlgorithm, PropertyContext> {
public:
  virtual ~PropertyFactory() {}
};

/*@}*/

}
#include "cxx/TemplateFactory.cxx"
#endif
