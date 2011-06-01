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

#include "PluginInfo.h"

#include <vector>
#include <iostream>

#include <QtCore/QFileInfo>

#include <tulip/PluginLoaderTxt.h>
#include <tulip/AbstractPluginInfo.h>

namespace tlp {

  //==============================================================
  struct PluginLoaderWithInfo:public PluginLoader {
    std::vector<LocalPluginInfo> pluginsList;
    std::string currentDisplayType;
    std::string currentPath;
    std::string currentFileName;
    std::string errorMsgs;

    virtual void start(const std::string &path) {
      currentPath=path;
    }

    virtual void loading(const std::string &filename)
    {
      currentFileName = QFileInfo(filename.c_str()).fileName().toStdString();
    }

    virtual void loaded(const AbstractPluginInfo* infos, const std::list <Dependency>& deps)
    {
      LocalPluginInfo plugin;
      plugin.name = infos->getName();
      plugin.type = "Algorithm";
      plugin.version = infos->getTulipRelease() + " " + infos->getRelease();
      plugin.fileName = currentFileName.substr(0,currentFileName.rfind("."));
      plugin.author = infos->getAuthor();
      plugin.date = infos->getDate();
      plugin.info = infos->getInfo();
      plugin.server = "Installed";
      
      for(std::list<Dependency>::const_iterator it=deps.begin();it!=deps.end();++it) {
        PluginDependency dep((*it).pluginName,(*it).factoryName,infos->getTulipRelease() + " " + (*it).pluginRelease);
        plugin.dependencies.push_back(dep);
      }
      
      pluginsList.push_back(plugin);
    }

    virtual void aborted(const std::string& filename,const std::string &errormsg) {
      errorMsgs += filename + ":\n    " + errormsg + '\n';
    }

    virtual void finished(bool,const std::string&){}
  };
  
}
#endif
