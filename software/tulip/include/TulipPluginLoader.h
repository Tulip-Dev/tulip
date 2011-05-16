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
#ifndef _Tulip_TULIPPLUGINLOADER_H_
#define _Tulip_TULIPPLUGINLOADER_H_

#include <vector>

#include <PluginLoaderWithInfo.h>

#include "AppStartUp.h"
#include <tulip/AbstractPluginInfo.h>

namespace tlp {

  //==============================================================
  struct TulipPluginLoader:public PluginLoaderWithInfo {
    AppStartUp *appStartUp;
    int progress;

    virtual void start(const std::string &path) {
      appStartUp->setProgress(0);
      progress=0;
      PluginLoaderWithInfo::start(path);
    }

    virtual void numberOfFiles(int nbFile) {
      appStartUp->setTotalSteps(nbFile);
      qApp->processEvents();
    }

    virtual void loaded(const AbstractPluginInfo* infos, const std::list <Dependency>& deps)
    {
      progress++;
      appStartUp->setLabel(infos->getName());
      appStartUp->setProgress(progress);
      qApp->processEvents();

      PluginLoaderWithInfo::loaded(infos, deps);
    }

    virtual void aborted(const std::string &filename,const  std::string &errormsg) {
      progress++;
      std::string msg("Error when loading ");
      msg += tlp::getCurrentPluginFileName() + ":\n" + filename + '\n' + errormsg;
      appStartUp->addErrorMsg(msg);
      appStartUp->setLabel("Error");
      appStartUp->setProgress(progress);
      qApp->processEvents();

      PluginLoaderWithInfo::aborted(filename,errormsg);
    }
  };
  
}
#endif
