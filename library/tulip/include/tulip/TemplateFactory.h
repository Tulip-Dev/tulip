//-*-c++-*-
#ifndef _TEMPLATEFACTORY_H
#define _TEMPLATEFACTORY_H

#include "PluginLoader.h"
#include "WithParameter.h"
#include "Iterator.h"
#include "Plugin.h"

#include "tulip/PluginsCreation.h"

/** \addtogroup plugins */ 
/*@{*/
template<class ObjectFactory, class ObjectType, class Parameter> class TemplateFactory {
public:
  //typedef void *(*func)();
  typedef std::map< std::string , ObjectFactory * > ObjectCreator;
  void *handle;
  PluginLoader *currentLoader;

  ObjectCreator objMap;
  std::map<std::string,StructDef> objParam;
  std::set<std::string> objNames;

  Iterator<std::string>* availablePlugins();
  bool exists(const std::string &pluginName);
  void load(std::string pluginPath,std::string type, PluginLoader *loader=0);
  //bool load(std::string file);
  ObjectType *getObject(std::string name,Parameter p);
  StructDef getParam(std::string name);
  void getPluginParameters(ObjectFactory* objectFactory);
};


template <class T> class TLP_SCOPE PropertyFactory:public Plugin {
public:
  PropertyFactory(){}
  virtual ~PropertyFactory() {}
  virtual T* createObject(const PropertyContext &context)=0;
};

/*@}*/
#include "./cxx/TemplateFactory.cxx"
#endif































