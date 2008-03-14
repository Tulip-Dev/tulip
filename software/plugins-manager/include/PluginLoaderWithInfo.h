#ifndef _Tulip_PLUGINLOADERWITHINFO_H_
#define _Tulip_PLUGINLOADERWITHINFO_H_

#include <vector>
#include <iostream>

#include <tulip/PluginLoaderTxt.h>

#include "PluginInfo.h"

namespace tlp {

  //==============================================================
  struct TLP_PLUGINSMANAGER_SCOPE PluginLoaderWithInfo:public PluginLoader {
    std::vector<LocalPluginInfo> pluginsList;
    std::string currentType;
    std::string currentDisplayType;
    std::string currentPath;
    std::string currentFileName;
    std::string errorMsgs;

    virtual void start(const std::string &path,const std::string &type) {
      currentType=type;
      currentPath=path;
    }

    virtual void loading(const std::string &filename)
    {
      currentFileName=filename;
    }

    virtual void loaded(const std::string &name,
			const std::string &author,
			const std::string &date, 
			const std::string &info,
			const std::string &release,
			const std::string &version,
			const std::list <Dependency> &deps)
    {
      LocalPluginInfo plugin;
      plugin.name=name;
      plugin.type=currentType;
      plugin.version=version+" "+release;
      plugin.fileName=currentFileName.substr(0,currentFileName.rfind("."));
      plugin.author=author;
      plugin.date=date;
      plugin.info=info;
      plugin.server="Installed";
      
      for(std::list<Dependency>::const_iterator it=deps.begin();it!=deps.end();++it) {
	PluginDependency dep((*it).pluginName,(*it).factoryName,version+" "+(*it).pluginRelease);
	plugin.dependencies.push_back(dep);
      }
      
      pluginsList.push_back(plugin);
    }

    virtual void aborted(const std::string &filename,const  std::string &errormsg) {
      errorMsgs += errormsg + '\n';
      std::cerr << "Loading error: " << errormsg << std::endl;
    }

    virtual void finished(bool state,const std::string &msg){}
  };
  
}
#endif
