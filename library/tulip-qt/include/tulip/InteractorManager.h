//-*-c++-*-
/**
 Author: Morgan Mathiaut
 Email : mathiaut@labri.fr
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.
*/

#ifndef Tulip_INTERACTORMANAGER_H
#define Tulip_INTERACTORMANAGER_H

#ifndef DOXYGEN_NOTFOR_DEVEL

#include <string>
#include <cassert>

#include <tulip/tulipconf.h>
#include <tulip/MutableContainer.h>
#include <tulip/PluginLoader.h>
#include <tulip/GWInteractor.h>

namespace tlp {

  /** \brief Interactor plugins manager
   *
   * Interactor plugins manager
   */
  class TLP_QT_SCOPE InteractorManager {

  public:

    static InteractorManager &getInst() {
      if(!inst)
	inst=new InteractorManager();
      return *inst;
    }

    /**
     * return the name of plugin with id : id
     */
    std::string interactorName(int id);
    /**
     * return id of plugin with name : name
     */
    int interactorId(std::string name);
    /**
     * Create map of pair<id,name> and pair<name,id>
     */
    void loadInteractorPlugins();
    /**
     * Load interactor plugins with PluginLoader : plug
     */
    void loadPlugins(PluginLoader *plug=0);

    /**
     * Get the list of available plugins
     */
    void initInteractorList(MutableContainer<Interactor *>& interactors);

  private:

    InteractorManager();

    static InteractorManager* inst;

  };

}

#endif // DOXYGEN_NOTFOR_DEVEL

#endif // Tulip_INTERACTORMANAGER_H
