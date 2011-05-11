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
#include <tulip/PluginLister.h>
#include <tulip/AbstractPluginInfo.h>

template<typename KEY, typename VALUE>
struct StlMapKeyIterator : public tlp::Iterator<KEY> {
  StlMapKeyIterator(typename std::map<KEY,VALUE>::const_iterator startIt, typename std::map<KEY,VALUE>::const_iterator endIt):
  it(startIt),
  itEnd(endIt)
  {}
  KEY next() {
    const KEY tmp = it->first;
    ++it;
    return tmp;
  }
  bool hasNext() {
    return it != itEnd;
  }
private:
  typename std::map<KEY,VALUE>::const_iterator it, itEnd;
};

template<class ObjectType, class Context>
tlp::Iterator<std::string>* tlp::PluginLister< ObjectType, Context>::availablePlugins() const {
  return new StlMapKeyIterator<std::string, PluginDescription>(objMap.begin(), objMap.end());
}

template<class ObjectType, class Context>
bool tlp::PluginLister< ObjectType, Context>::pluginExists(const std::string &pluginName) const {
  return (objMap.find(pluginName) != objMap.end());
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
    objMap[pluginName] = description;
    
    delete withParam;
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
void tlp::PluginLister<ObjectType,Context>::removePlugin(const std::string &name) {
  objMap.erase(name);
}

template<class ObjectType, class Context>
ObjectType * tlp::PluginLister<ObjectType,Context>::getPluginObject(const std::string& name, Context c) const {
  typename ObjectCreator::const_iterator it = objMap.find(name);
  if (it!=objMap.end()) {
    return (*it).second.factory->createPluginObject(c);
  }
  return 0;
}

template<class ObjectType, class Context>
const tlp::StructDef tlp::PluginLister<ObjectType,Context>::getPluginParameters(std::string name) const {
  assert(objMap.find(name)!=objMap.end());
  return objMap.at(name).parameters;
}

template<class ObjectType, class Context>
std::string tlp::PluginLister<ObjectType,Context>::getPluginRelease(std::string name) const {
  assert(objMap.find(name)!=objMap.end());
  return objMap.at(name).factory->getRelease();
}

template<class ObjectType, class Context>
std::list<tlp::Dependency> tlp::PluginLister<ObjectType,Context>::getPluginDependencies(std::string name) const {
  assert(objMap.find(name)!=objMap.end());
  return objMap.at(name).dependencies;
}

template<class ObjectType, class Context> std::string tlp::PluginLister<ObjectType,Context>::getPluginsClassName() const {
  return demangleTlpClassName(typeid(ObjectType).name());
}
