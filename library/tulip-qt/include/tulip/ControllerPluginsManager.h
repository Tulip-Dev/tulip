//-*-c++-*-
/**
 Author: Morgan Mathiaut
 Email : mathiaut@labri.fr
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.
*/

#ifndef Tulip_CONTROLLERPLUGINSMANAGER_H
#define Tulip_CONTROLLERPLUGINSMANAGER_H

#include <string>
#include <cassert>

#include <tulip/tulipconf.h>
#include <tulip/MutableContainer.h>
#include <tulip/PluginLoader.h>

#include "tulip/Controller.h"

namespace tlp {

  /** \brief Controller plugins manager
   *
   * Controller plugins manager
   */
  class TLP_QT_SCOPE ControllerPluginsManager {

  public:

    static ControllerPluginsManager &getInst() {
      if(!inst)
	inst=new ControllerPluginsManager();
      return *inst;
    }

    /**
     * return the name of plugin with id : id
     */
    std::string controllerPluginName(int id);
    /**
     * return id of plugin with name : name
     */
    int controllerPluginId(std::string name);
    /**
     * Create map of pair<id,name> and pair<name,id>
     */
    void loadControllerPlugins();
    /**
     * Load controller plugins with PluginLoader : plug
     */
    void loadPlugins(PluginLoader *plug=0);

    /**
     * Get the list of available plugins
     */
    void initControllerPluginsList(MutableContainer<Controller *> &controllers);
    /**
     * return Controller with given name
     */
    Controller *createController(const std::string &name);

  private:

    ControllerPluginsManager();

    static ControllerPluginsManager* inst;

  };

}

#endif // Tulip_CONTROLLERPLUGINSMANAGER_H
