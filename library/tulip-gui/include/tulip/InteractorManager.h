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

#ifndef Tulip_INTERACTORMANAGER_H
#define Tulip_INTERACTORMANAGER_H

#ifndef DOXYGEN_NOTFOR_DEVEL

#include <string>
#include <cassert>
#include <map>

#include <tulip/tulipconf.h>
#include <tulip/MutableContainer.h>

namespace tlp {

  class Interactor;

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
     * Loads interactor plugins
     */
    void loadInteractorPlugins();

    /**
     * Returns a list of compatible interactors with the view identified by viewName
     */
    std::list<std::string> getSortedCompatibleInteractors(const std::string &viewName);

    bool operator() (std::string first, std::string second);
  private:
//     bool comparePriority(std::string first, std::string second);
    
    InteractorManager();

    std::map<std::string,Interactor *> interactorsMap;

    static InteractorManager* inst;

  };

}

#endif // DOXYGEN_NOTFOR_DEVEL

#endif // Tulip_INTERACTORMANAGER_H
