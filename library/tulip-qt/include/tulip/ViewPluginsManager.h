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
     * Get the name of plugin with id : id
     */
    std::string viewPluginName(int id);
    /**
     * Get the id of plugin with name : name
     */
    int viewPluginId(std::string name);
    /**
     * Create internal maps of plugins
     */
    void loadViewPlugins();
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

#endif // DOXYGEN_NOTFOR_DEVEL

#endif // Tulip_VIEWPLUGINSMANAGER_H
