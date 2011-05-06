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
//====================================================================================
#include <iostream>
#include <tulip/StlIterator.h>

#include <tulip/AbstractPluginInfo.h>

template<class ObjectType, class Context>
tlp::Iterator<std::string>* tlp::PluginManager< ObjectType, Context>::availablePlugins() {
  return new tlp::StlIterator<std::string,std::set<std::string>::const_iterator>(objNames.begin(), objNames.end());
}

template<class ObjectType, class Context>
bool tlp::PluginManager< ObjectType, Context>::pluginExists(const std::string &pluginName) {
  return (objMap.find(pluginName) != objMap.end());
}

template<class ObjectType, class Context>
void tlp::PluginManager<ObjectType,Context>::registerPlugin(FactoryInterface<ObjectType, Context> *objectFactory) {
  std::string pluginName = objectFactory->getName();
  if (!pluginExists(pluginName)) {
  objNames.insert(pluginName);
  objMap[pluginName]=objectFactory;
  ObjectType *withParam=objectFactory->createPluginObject(Context());
  objParam[pluginName] = withParam->getParameters();
  // loop over dependencies
  // to demangle the class names
  std::list<tlp::Dependency> dependencies = withParam->getDependencies();
  std::list<tlp::Dependency>::iterator itD = dependencies.begin();
  for (; itD != dependencies.end(); itD++) {
    const char *factoryDepName = (*itD).factoryName.c_str();
    (*itD).factoryName = tlp::demangleTlpClassName(factoryDepName);
  }
  objDeps[pluginName] = dependencies;
  delete withParam;
  objRels[pluginName] = objectFactory->getRelease();
  if (currentLoader!=0) currentLoader->loaded(
					      pluginName,
					      objectFactory->getAuthor(),
					      objectFactory->getDate(),
					      objectFactory->getInfo(),
					      objectFactory->getRelease(),
					      objectFactory->getTulipRelease(),
					      dependencies
					      );
  } else {
    if (currentLoader != 0) {
      std::string tmpStr;
      tmpStr += "'" + pluginName + "' " + getPluginsClassName() + " plugin";
      currentLoader->aborted(tmpStr, "multiple definitions found; check your plugin librairies.");
    }
  }
}

template<class ObjectType, class Context>
void tlp::PluginManager<ObjectType,Context>::removePlugin(const std::string &name) {
  objNames.erase(name);
  objMap.erase(name);
  objParam.erase(name);
  objDeps.erase(name);
  objRels.erase(name);
}

template<class ObjectType, class Context>
ObjectType * tlp::PluginManager<ObjectType,Context>::getPluginObject(const std::string& name, Context c) {
  typename ObjectCreator::iterator it;
  it=objMap.find(name);
  if (it!=objMap.end()) return (*it).second->createPluginObject(c);
  return 0;
}

template<class ObjectType, class Context>
tlp::StructDef tlp::PluginManager<ObjectType,Context>::getPluginParameters(std::string name) {
  assert(objMap.find(name)!=objMap.end());
  return objParam[name];
}

template<class ObjectType, class Context>
std::string tlp::PluginManager<ObjectType,Context>::getPluginRelease(std::string name) {
  assert(objMap.find(name)!=objMap.end());
  return objRels[name];
}

template<class ObjectType, class Context>
std::list<tlp::Dependency> tlp::PluginManager<ObjectType,Context>::getPluginDependencies(std::string name) {
  assert(objMap.find(name)!=objMap.end());
  return objDeps[name];
}

template<class ObjectType, class Context> std::string tlp::PluginManager<ObjectType,Context>::getPluginsClassName() {
  return demangleTlpClassName(typeid(ObjectType).name());
}
