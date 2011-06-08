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
template<class ObjectType, class Context>
tlp::Iterator<std::string>* tlp::PluginLister< ObjectType, Context>::availablePlugins() const {
  return new StlMapKeyIterator<std::string, PluginDescription>(plugins.begin(), plugins.end());
}

template<class ObjectType, class Context>
const tlp::AbstractPluginInfo* tlp::PluginLister< ObjectType, Context>::pluginInformations(const std::string& name) const {
  return plugins.find(name)->second.factory;
}

template<class ObjectType, class Context>
bool tlp::PluginLister< ObjectType, Context>::pluginExists(const std::string &pluginName) const {
  return (plugins.find(pluginName) != plugins.end());
}

template<class ObjectType, class Context>
void tlp::PluginLister<ObjectType,Context>::registerPlugin(FactoryInterface<ObjectType, Context> *objectFactory) {
  std::string pluginName = objectFactory->getName();
  if (!pluginExists(pluginName)) {
    ObjectType *withParam=objectFactory->createPluginObject(Context());
    
    // loop over dependencies
    // to demangle the class names
    std::list<tlp::Dependency> dependencies = withParam->getDependencies();
    std::list<tlp::Dependency>::iterator itD = dependencies.begin();
    for (; itD != dependencies.end(); itD++) {
      const char *factoryDepName = (*itD).factoryName.c_str();
      (*itD).factoryName = tlp::demangleTlpClassName(factoryDepName);
    }
    
    PluginDescription description;
    description.factory = objectFactory;
    description.parameters = withParam->getParameters();
    description.dependencies = dependencies;
    description.library = PluginLibraryLoader::getCurrentPluginFileName();
    plugins[pluginName] = description;
    
    delete withParam;
    if (currentLoader!=0) {
      currentLoader->loaded(objectFactory, dependencies);
    }
  }
  else {
    if (currentLoader != 0) {
      std::string tmpStr;
      tmpStr += "'" + pluginName + "' " + getPluginsClassName() + " plugin";
      currentLoader->aborted(tmpStr, "multiple definitions found; check your plugin librairies.");
    }
  }
}

template<class ObjectType, class Context>
void tlp::PluginLister<ObjectType,Context>::removePlugin(const std::string &name) {
  plugins.erase(name);
}

template<class ObjectType, class Context>
ObjectType * tlp::PluginLister<ObjectType,Context>::getPluginObject(const std::string& name, Context c) const {
  typename std::map<std::string , PluginDescription>::const_iterator it = plugins.find(name);
  if (it!=plugins.end()) {
    return (*it).second.factory->createPluginObject(c);
  }
  return 0;
}

template<class ObjectType, class Context>
const tlp::ParameterList tlp::PluginLister<ObjectType,Context>::getPluginParameters(std::string name) const {
  assert(plugins.find(name)!=plugins.end());
  return plugins.find(name)->second.parameters;
}

template<class ObjectType, class Context>
std::string tlp::PluginLister<ObjectType,Context>::getPluginRelease(std::string name) const {
  assert(plugins.find(name)!=plugins.end());
  return plugins.find(name)->second.factory->getRelease();
}

template<class ObjectType, class Context>
std::list<tlp::Dependency> tlp::PluginLister<ObjectType,Context>::getPluginDependencies(std::string name) const {
  assert(plugins.find(name)!=plugins.end());
  return plugins.find(name)->second.dependencies;
}

template<class ObjectType, class Context> std::string tlp::PluginLister<ObjectType,Context>::getPluginsClassName() const {
  return demangleTlpClassName(typeid(ObjectType).name());
}

template<class ObjectType, class Context> std::string tlp::PluginLister<ObjectType,Context>::getPluginLibrary(const std::string& name) const {
  return plugins.find(name)->second.library;
}
