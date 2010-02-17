//-*-c++-*-
/**
 Author: Morgan Mathiaut and Samuel Carruesco, Mickael Melas, Laurent Peyronnet, Michael Roche, Sabrina Wons
 Email : mathiaut@labri.fr
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.
*/

#ifndef _Tulip_PLUGININFO_H_
#define _Tulip_PLUGININFO_H_

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <tulip/tulipconf.h>

#include <string>
#include <vector>

#include <tulip/Algorithm.h>
#include <tulip/ColorProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/IntegerProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/LayoutProperty.h>
#include <tulip/StringProperty.h>
#include <tulip/BooleanProperty.h>
#include <tulip/ImportModule.h>
#include <tulip/ExportModule.h>
#include <tulip/Interactor.h>
#include <tulip/View.h>
#include <tulip/Controller.h>

namespace tlp {

  struct PluginDependency {

    std::string name;
    std::string type;
    std::string version;

    PluginDependency(const std::string &name,const std::string &type,const std::string& version):name(name),version(version){
      if(type=="Glyph")
	this->type="Glyph";
      else if(type=="Interactor")
	this->type="Interactor";
      else if(type=="View")
	this->type="View";
      else if(type=="Controller")
	this->type="Controller";
      else
	this->type="Algorithm";
    };

  };

  class TLP_PLUGINSMANAGER_SCOPE PluginInfo {

  public :

    static const std::string pluginsDirName;
    static std::string getInstallationSubDir(const std::string &str);

    virtual bool isInstalledInHome() {return false;}

    std::string name;
    std::string type;
    std::string displayType;
    std::string server;
    std::string version;
    std::vector<PluginDependency> dependencies;
    bool local;

    virtual ~PluginInfo() {};

  protected :
    PluginInfo(bool local):local(local){};
    PluginInfo(const std::string& name,
	       const std::string& type,
	       const std::string &displayType,
	       const std::string& server,
	       const std::string& version,
	       const std::vector<PluginDependency> &dependencies,
	       bool local)
      :name(name),
      type(type),
      displayType(displayType),
      server(server),
      version(version),
      dependencies(dependencies),
      local(local){};

  public :

    static std::string getPluginDisplayType(const std::string &name) {
      if(SizeProperty::factory && SizeProperty::factory->pluginExists(name))
	return "Size";
      if(IntegerProperty::factory &&
	 IntegerProperty::factory->pluginExists(name))
	return "Integer";
      if(LayoutProperty::factory &&
	 LayoutProperty::factory->pluginExists(name))
	return "Layout";
      if(ColorProperty::factory && ColorProperty::factory->pluginExists(name))
	return "Color";
      if(DoubleProperty::factory &&
	 DoubleProperty::factory->pluginExists(name))
	return "Metric";
      if(StringProperty::factory &&
	 StringProperty::factory->pluginExists(name))
	return "Label";
      if(BooleanProperty::factory &&
	 BooleanProperty::factory->pluginExists(name))
	return "Selection";
      if(ImportModuleFactory::factory &&
	 ImportModuleFactory::factory->pluginExists(name))
	return "Import";
      if(ExportModuleFactory::factory &&
	 ExportModuleFactory::factory->pluginExists(name))
	return "Export";
      if (AlgorithmFactory::factory &&
	  AlgorithmFactory::factory->pluginExists(name))
	return "General";
      if (InteractorFactory::factory &&
	  InteractorFactory::factory->pluginExists(name))
	return "Interactor";
      if (ViewFactory::factory &&
	  ViewFactory::factory->pluginExists(name))
	return "View";
      if (ControllerFactory::factory &&
	  ControllerFactory::factory->pluginExists(name))
	return "Controller";

      return "Glyph";
    }
  };

  // we define a specific code to return (must be < 256)
  // from tulip_check_pl
#define TLP_CHECK_PL_RETURN_SUCCESS 222

  class TLP_PLUGINSMANAGER_SCOPE DistPluginInfo : public PluginInfo{

  public :
    std::string fileName;
    std::string localVersion;
    bool linuxVersion;
    bool macVersion_ppc;
    bool macVersion_intel;
    bool windowsVersion;
    bool linuxVersion_i64;
    bool windowsVersion_i64;
    bool installIsOK;

    virtual ~DistPluginInfo() {};

    DistPluginInfo():PluginInfo(false){};
    DistPluginInfo(const std::string& name,
		   const std::string& type,
		   const std::string& displayType,
		   const std::string& server,
		   const std::string& version,
		   const std::string& fileName,
		   const std::string& localVersion,
		   const std::vector<PluginDependency>& dependencies,
		   bool linuxVersion,
           bool macVersion_ppc,
           bool macVersion_intel,
		   bool windowsVersion,
		   bool linuxVersion_i64,
		   bool windowsVersion_i64)
      : PluginInfo(name,type,displayType,server,version,dependencies,false),
      fileName(fileName),
      localVersion(localVersion),
      linuxVersion(linuxVersion),
      macVersion_ppc(macVersion_ppc),
      macVersion_intel(macVersion_intel),
      windowsVersion(windowsVersion),
      linuxVersion_i64(linuxVersion_i64),
      windowsVersion_i64(windowsVersion_i64)
      {
        local=false;
      };
  };

  class TLP_PLUGINSMANAGER_SCOPE LocalPluginInfo : public PluginInfo{

  public :
    std::string fileName;
    std::string author;
    std::string date;
    std::string info;

    virtual ~LocalPluginInfo() {};

    LocalPluginInfo():PluginInfo(true){};
    LocalPluginInfo(const std::string& name,
	       const std::string& type,
	       const std::string& displayType,
	       const std::string& server,
	       const std::string& version,
	       const std::string& fileName,
	       const std::string& author,
	       const std::string& date,
	       const std::string& info,
	       const std::vector<PluginDependency> &dependencies)
      :PluginInfo(name,type,displayType,server,version,dependencies,true),
      fileName(fileName),
      author(author),
      date(date),
      info(info){
	local=true;};
      virtual bool isInstalledInHome();
  };

  class TLP_PLUGINSMANAGER_SCOPE PluginCmp {
  public :
    bool operator()(const PluginInfo& p1,const PluginInfo &p2) {
      if(p1.name!=p2.name)
	return p1.name.compare(p2.name) < 0;
      if(p1.type!=p2.type)
	return p1.type.compare(p2.type) < 0;
      if(p1.server!=p2.server)
	return p1.server.compare(p2.server) < 0;
      if(p1.version!=p2.version)
	return p1.version.compare(p2.version) < 0;
      if(p1.server!=p2.server)
	return p1.server.compare(p2.server) < 0;
      return false;
    }
  };

  class TLP_PLUGINSMANAGER_SCOPE PluginTypeCmp {
    public :
      bool operator()(const PluginInfo& p1,const PluginInfo &p2) {
        if(p1.type!=p2.type)
          return p1.type.compare(p2.type) < 0;
        if(p1.name!=p2.name)
          return p1.name.compare(p2.name) < 0;
        if(p1.server!=p2.server)
          return p1.server.compare(p2.server) < 0;
        if(p1.version!=p2.version)
          return p1.version.compare(p2.version) < 0;
        if(p1.server!=p2.server)
          return p1.server.compare(p2.server) < 0;
        return false;
      }
    };

  class TLP_PLUGINSMANAGER_SCOPE PluginDependencyCmp {
    public :
    bool operator()(const PluginDependency& p1,const PluginDependency &p2) {
      if(p1.name!=p2.name)
	return p1.name.compare(p2.name) < 0;
      if(p1.type!=p2.type)
	return p1.type.compare(p2.type) < 0;
      if(p1.version!=p2.version)
	return p1.version.compare(p2.version) < 0;
      return false;
    }
  };

}


#endif //PLUGININFO

