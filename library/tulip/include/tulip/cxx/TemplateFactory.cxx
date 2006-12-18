//-*-c++-*-
/**
 Author: David Auber
 Email : auber@labri.fr
 Last modification : 26/09/2001
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/
//====================================================================================
#include <iostream>
#include <tulip/StlIterator.h>

#include <tulip/Plugin.h>

template<class ObjectFactory, class ObjectType, class Context>
tlp::Iterator<std::string>* tlp::TemplateFactory<ObjectFactory, ObjectType, Context>::availablePlugins() {
  return new tlp::StlIterator<std::string,std::set<std::string>::const_iterator>(objNames.begin(), objNames.end());
}

template<class ObjectFactory, class ObjectType, class Context>
bool tlp::TemplateFactory<ObjectFactory, ObjectType, Context>::pluginExists(const std::string &pluginName) {
  return (objMap.find(pluginName) != objMap.end());
}

template<class ObjectFactory, class ObjectType, class Context>
void tlp::TemplateFactory<ObjectFactory,ObjectType,Context>::registerPlugin(ObjectFactory *objectFactory) {
  std::string pluginName = objectFactory->getName();
  objNames.insert(pluginName);
  objMap[pluginName]=objectFactory;
  ObjectType *withParam=objectFactory->createPluginObject(Context());
  objParam[pluginName] = withParam->getParameters();
  // loop over dependencies
  // to demangle the class names
  std::list<std::pair < std::string, std::string > > dependencies =
    withParam->getDependencies();
  std::list<std::pair < std::string, std::string > >::iterator itD =
    dependencies.begin();
  for (; itD != dependencies.end(); itD++) {
    const char *factoryDepName = (*itD).first.c_str();
    (*itD).first = std::string(demangleTlpClassName(factoryDepName));
  }
  objDeps[pluginName] = dependencies;
  if (currentLoader!=0) currentLoader->loaded(
					      pluginName,
					      objectFactory->getAuthor(),
					      objectFactory->getDate(),
					      objectFactory->getInfo(),
					      objectFactory->getRelease(),
					      objectFactory->getVersion(),
					      dependencies
					      );
}

template<class ObjectFactory, class ObjectType, class Context>
void tlp::TemplateFactory<ObjectFactory,ObjectType,Context>::removePlugin(const std::string &name) {
  objNames.erase(name);
  objMap.erase(name);
  objParam.erase(name);
  objDeps.erase(name);
}

template<class ObjectFactory, class ObjectType, class Context>
void tlp::TemplateFactory<ObjectFactory,ObjectType,Context>::loadPluginsFromDir(std::string pluginPath, std::string type,tlp::PluginLoader *loader) {
  if (loader!=0)
    loader->start(pluginPath.c_str(),type);

  tlp::PluginLibraryLoader plLoader(pluginPath, loader);

  currentLoader = loader;
  if (plLoader.hasPluginLibraryToLoad()) {
    while(plLoader.loadNextPluginLibrary(loader)) {
    }
    if (loader)
      loader->finished(true, plLoader.msg);
  } else {
    if (loader)
      loader->finished(false, plLoader.msg);
  }
}

template<class ObjectFactory, class ObjectType, class Context>
ObjectType * tlp::TemplateFactory<ObjectFactory,ObjectType,Context>::getPluginObject(const std::string& name, Context c) {
  typename ObjectCreator::iterator it;
#ifndef NDEBUG 
  std::cerr << "TemplateFactory::GetObject :" << name << std::endl;  
#endif
  it=objMap.find(name);
  if (it!=objMap.end()) return (*it).second->createPluginObject(c);
  return 0;
}

template<class ObjectFactory, class ObjectType, class Context>
tlp::StructDef tlp::TemplateFactory<ObjectFactory,ObjectType,Context>::getPluginParameters(std::string name) {
  assert(objMap.find(name)!=objMap.end());
  return objParam[name];
}

template<class ObjectFactory, class ObjectType, class Context>
std::list< std::pair < std::string, std::string > > tlp::TemplateFactory<ObjectFactory,ObjectType,Context>::getPluginDependencies(std::string name) {
  assert(objMap.find(name)!=objMap.end());
  return objDeps[name];
}

template<class ObjectFactory, class ObjectType, class Context> std::string tlp::TemplateFactory<ObjectFactory,ObjectType,Context>::getPluginsClassName() {
  return std::string(demangleTlpClassName(typeid(ObjectType).name()));
}
