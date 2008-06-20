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
  
  class TLP_QT_SCOPE InteractorManager {
  
  public:

    static InteractorManager &getInst() {
      if(!inst)
	inst=new InteractorManager();
      return *inst;
    }

    std::string interactorName(int id);
    int interactorId(std::string name);
    void loadInteractorPlugins();
    void loadPlugins(PluginLoader *plug=0);

    void initInteractorList(MutableContainer<Interactor *>& interactors);

  private:

    InteractorManager();

    static InteractorManager* inst;

  };
 
}

#endif // DOXYGEN_NOTFOR_DEVEL

#endif // Tulip_INTERACTORMANAGER_H
