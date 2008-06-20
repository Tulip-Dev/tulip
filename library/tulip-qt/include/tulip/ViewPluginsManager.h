//-*-c++-*-
/**
 Author: Morgan Mathiaut
 Email : mathiaut@labri.fr
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/

#ifndef Tulip_VIEWPLUGINSMANAGER_H
#define Tulip_VIEWPLUGINSMANAGER_H

#ifndef DOXYGEN_NOTFOR_DEVEL

#include <string>
#include <cassert>

#include <tulip/tulipconf.h>
#include <tulip/MutableContainer.h>
#include <tulip/PluginLoader.h>

#include "tulip/View.h"

namespace tlp {
  
  class TLP_QT_SCOPE ViewPluginsManager {
  
  public:

    static ViewPluginsManager &getInst() {
      if(!inst)
	inst=new ViewPluginsManager();
      return *inst;
    }

    std::string viewPluginName(int id);
    int viewPluginId(std::string name);
    void loadViewPlugins();
    void loadPlugins(PluginLoader *plug=0);

    void initViewPluginsList(MutableContainer<ViewCreator *>& views);

  private:

    ViewPluginsManager();

    static ViewPluginsManager* inst;

  };
 
}

#endif // DOXYGEN_NOTFOR_DEVEL

#endif // Tulip_VIEWPLUGINSMANAGER_H
