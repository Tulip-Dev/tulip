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
Iterator<std::string>* TemplateFactory<ObjectFactory, ObjectType, Parameter>::availablePlugins() {
  return new StlIterator<std::string,std::set<std::string>::const_iterator>(objNames.begin(), objNames.end());
}

template<class ObjectFactory, class ObjectType, class Parameter>
bool TemplateFactory<ObjectFactory, ObjectType, Parameter>::exists(const std::string &pluginName) {
  return (objMap.find(pluginName) != objMap.end());
}

template<class ObjectFactory, class ObjectType, class Parameter>
void TemplateFactory<ObjectFactory,ObjectType,Parameter>::getPluginParameters(PluginLoader *loader) {
  Plugin *tmpObjectFactory=(Plugin *)createObj();
  if(dynamic_cast< ObjectFactory* >(tmpObjectFactory) != 0) {
    ObjectFactory *objectFactory=(ObjectFactory *)tmpObjectFactory;
    objNames.insert(objectFactory->getName());
    objMap[objectFactory->getName()]=objectFactory;
    if (loader!=0) loader->loaded(
				  objectFactory->getName(),
				  objectFactory->getAuthor(),
				  objectFactory->getDate(),
				  objectFactory->getInfo(),
				  objectFactory->getRelease(),
				  objectFactory->getVersion()
				  );
    Parameter p = Parameter();
    WithParameter *withParam=objectFactory->createObject(p);
    objParam[objectFactory->getName()]=withParam->getParameters();
  }
}

template<class ObjectFactory, class ObjectType, class Parameter>
void TemplateFactory<ObjectFactory,ObjectType,Parameter>::load(std::string pluginPath,std::string type,PluginLoader *loader) {
  if (loader!=0)
    loader->start(pluginPath.c_str(),type);

  PluginIterator iterator(pluginPath, loader);

  if (iterator.isValid()) {
    while(createObj = iterator.nextPluginObj(loader)) {
      getPluginParameters(loader);
    }
    if (loader)
      loader->finished(true, iterator.msg);
  } else {
    if (loader)
      loader->finished(false, iterator.msg);
  }
}

template<class ObjectFactory, class ObjectType, class Parameter>
bool TemplateFactory<ObjectFactory,ObjectType,Parameter>::load(std::string file) {
  createObj = getCreationFunc(file);
  if(createObj == NULL)
    return false;
  else {
    ObjectFactory *tmpObjectFactory=(ObjectFactory *)createObj();
    objMap[tmpObjectFactory->getName()]=tmpObjectFactory;
    Parameter p;
    WithParameter *withParam=tmpObjectFactory->createObject(p);
    objParam[tmpObjectFactory->getName()]=withParam->getParameters();
  }
 return true;
}

template<class ObjectFactory, class ObjectType, class Parameter>
ObjectType * TemplateFactory<ObjectFactory,ObjectType,Parameter>::getObject(std::string name,Parameter p)
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
StructDef TemplateFactory<ObjectFactory,ObjectType,Parameter>::getParam(std::string name){
  assert(objMap.find(name)!=objMap.end());
  return objParam[name];
}
