/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
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

#ifndef Tulip_VIEWPLUGINSMANAGER_H
#define Tulip_VIEWPLUGINSMANAGER_H

#include <string>
#include <cassert>

#include <tulip/tulipconf.h>
#include <tulip/MutableContainer.h>
#include <tulip/PluginLoader.h>

#include "tulip/View.h"

namespace tlp {

  /** \brief Manager of view's plugins
   *
   * Class to manage view's plugins
   */
  class TLP_QT_SCOPE ViewPluginsManager {

  public:

    /**
     * Singleton system
     */
    static ViewPluginsManager &getInst() {
      if(!inst)
	inst=new ViewPluginsManager();
      return *inst;
    }


    /**
     * Load all view's plugins available
     * \param plug : Use this plugin loader
     */
    void loadPlugins(PluginLoader *plug=0);

    /**
     * Get available plugins list
     */
    void initViewPluginsList(MutableContainer<View *> &views);
    /**
     * Create view with given name
     * \return The view
     */
    View *createView(const std::string &name);

  private:

    /**
     * Singleton system
     */
    ViewPluginsManager();

    static ViewPluginsManager* inst;

  };

}

#endif // Tulip_VIEWPLUGINSMANAGER_H
