//-*-c++-*
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

template<class ObjectFactory, class ObjectType, class Parameter>
tlp::Iterator<std::string>* tlp::TemplateFactory<ObjectFactory, ObjectType, Parameter>::availablePlugins() {
  return new tlp::StlIterator<std::string,std::set<std::string>::const_iterator>(objNames.begin(), objNames.end());
}

template<class ObjectFactory, class ObjectType, class Parameter>
bool tlp::TemplateFactory<ObjectFactory, ObjectType, Parameter>::exists(const std::string &pluginName) {
  return (objMap.find(pluginName) != objMap.end());
}

template<class ObjectFactory, class ObjectType, class Parameter>
void tlp::TemplateFactory<ObjectFactory,ObjectType,Parameter>::getPluginParameters(ObjectFactory *objectFactory) {
  objNames.insert(objectFactory->getName());
  objMap[objectFactory->getName()]=objectFactory;
  if (currentLoader!=0) currentLoader->loaded(
				objectFactory->getName(),
				objectFactory->getAuthor(),
				objectFactory->getDate(),
				objectFactory->getInfo(),
				objectFactory->getRelease(),
				objectFactory->getVersion()
				);
  Parameter p = Parameter();
  tlp::WithParameter *withParam=objectFactory->createObject(p);
  objParam[objectFactory->getName()]=withParam->getParameters();
}

template<class ObjectFactory, class ObjectType, class Parameter>
void tlp::TemplateFactory<ObjectFactory,ObjectType,Parameter>::load(std::string pluginPath,std::string type,tlp::PluginLoader *loader) {
  if (loader!=0)
    loader->start(pluginPath.c_str(),type);

  tlp::PluginIterator iterator(pluginPath, loader);

  currentLoader = loader;
  if (iterator.isValid()) {
    while(iterator.nextPlugin(loader)) {
    }
    if (loader)
      loader->finished(true, iterator.msg);
  } else {
    if (loader)
      loader->finished(false, iterator.msg);
  }
}

template<class ObjectFactory, class ObjectType, class Parameter>
ObjectType * tlp::TemplateFactory<ObjectFactory,ObjectType,Parameter>::getObject(std::string name,Parameter p)
{
  typename ObjectCreator::iterator it;
#ifndef NDEBUG 
  std::cerr << "TemplateFactory::GetObject :" << name << std::endl;  
#endif
  it=objMap.find(name);
  if (it!=objMap.end()) return (*it).second->createObject(p);
  return 0;
}
template<class ObjectFactory, class ObjectType, class Parameter>
tlp::StructDef tlp::TemplateFactory<ObjectFactory,ObjectType,Parameter>::getParam(std::string name){
  assert(objMap.find(name)!=objMap.end());
  return objParam[name];
}
