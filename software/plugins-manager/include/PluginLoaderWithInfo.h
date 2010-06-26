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
#ifndef _Tulip_PLUGINLOADERWITHINFO_H_
#define _Tulip_PLUGINLOADERWITHINFO_H_

#include <vector>
#include <iostream>

#include <tulip/PluginLoaderTxt.h>

#include "PluginInfo.h"
#include <QtCore/QFileInfo>

namespace tlp {

  //==============================================================
  struct PluginLoaderWithInfo:public PluginLoader {
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
      currentFileName = QFileInfo(filename.c_str()).fileName().toStdString();
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
    }

    virtual void finished(bool state,const std::string &msg){}
  };
  
}
#endif
