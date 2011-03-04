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

    void loadPlugins(PluginLoader *plug=0);

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
