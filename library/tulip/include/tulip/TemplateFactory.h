//-*-c++-*-
#ifndef _TEMPLATEFACTORY_H
#define _TEMPLATEFACTORY_H

#include <list>
#include <string>
#include <typeinfo>
#include "PluginLoader.h"
#include "WithParameter.h"
#include "WithDependency.h"
#include "Iterator.h"
#include "Plugin.h"
#include "TlpTools.h"

#include "tulip/PluginsCreation.h"

namespace tlp {

/** \addtogroup plugins */ 
/*@{*/
class TemplateFactoryInterface {
public:
  static TLP_SCOPE std::map< std::string, TemplateFactoryInterface* > *allFactories;
  static TLP_SCOPE PluginLoader *currentLoader;

  virtual ~TemplateFactoryInterface(){}
  virtual Iterator<std::string>* availablePlugins()=0;
  virtual bool pluginExists(const std::string &pluginName)=0;
  virtual StructDef getPluginParameters(std::string name)=0;
  virtual std::string getPluginRelease(std::string name)=0;
  virtual std::list<tlp::Dependency> getPluginDependencies(std::string name)=0;
  virtual std::string getPluginsClassName()=0;
  virtual void removePlugin(const std::string& name)=0;

  static void addFactory(TemplateFactoryInterface *factory, const std::string &name)  {
    if (!allFactories)
      allFactories = new std::map<std::string, TemplateFactoryInterface*>();
    //std::cerr << name.c_str() << " factory added" << std::endl;
    (*allFactories)[name] = factory;
  }
  static bool pluginExists(const std::string& factoryName, const std::string& pluginName) {
    assert(allFactories->find(factoryName) != allFactories->end());
    return (*allFactories)[factoryName]->pluginExists(pluginName);
  }
};

template<class ObjectFactory, class ObjectType, class Context> class TemplateFactory: public TemplateFactoryInterface {
public:
  TemplateFactory() {
    TemplateFactoryInterface::addFactory(this,
					 tlp::demangleTlpClassName(typeid(ObjectType).name()));
  }
  //typedef void *(*func)();
  typedef std::map< std::string , ObjectFactory * > ObjectCreator;

  ObjectCreator objMap;
  std::map<std::string,StructDef> objParam;
  std::set<std::string> objNames;
  std::map<std::string, std::list<tlp::Dependency> > objDeps;
  std::map<std::string, std::string> objRels;

  Iterator<std::string>* availablePlugins();
  bool pluginExists(const std::string& pluginName);
  ObjectType *getPluginObject(const std::string& name, Context p);
  StructDef getPluginParameters(std::string name);
  std::string getPluginRelease(std::string name);
  std::list<tlp::Dependency> getPluginDependencies(std::string name);
  std::string getPluginsClassName();
  void registerPlugin(ObjectFactory* objectFactory);
  void removePlugin(const std::string &name);
};


template <class T> class TLP_SCOPE PropertyFactory:public Plugin {
public:
  PropertyFactory(){}
  virtual ~PropertyFactory() {}
  virtual T* createPluginObject(const PropertyContext &context)=0;
  virtual  std::string getMajor() const {
    return tlp::getMajor(getRelease());
  }
  virtual  std::string getMinor() const  {
    return tlp::getMinor(getRelease());
  }
  virtual  std::string getTulipMajor() const {
    return tlp::getMajor(getTulipRelease());
  }
  virtual  std::string getTulipMinor() const  {
    return tlp::getMinor(getTulipRelease());
  }
};

/*@}*/

}
#include "./cxx/TemplateFactory.cxx"
#endif































