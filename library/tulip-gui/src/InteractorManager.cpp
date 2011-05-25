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
#include "tulip/InteractorManager.h"
#include "tulip/Interactor.h"

//====================================================
tlp::InteractorManager* tlp::InteractorManager::inst=0;

using namespace std;

namespace tlp
{

  bool InteractorManager::operator()(std::string first, std::string second) {
    return interactorsMap[first]->getPriority() >= interactorsMap[second]->getPriority();
  }
  
  InteractorManager::InteractorManager() {
  }
  //====================================================
  void InteractorManager::loadInteractorPlugins() {
    // if interactorMap is empty, put all interactors in the Map
    interactorsMap.clear();

    Iterator<string> *itS = InteractorLister::availablePlugins();
    while (itS->hasNext()) {
      string interactorName=itS->next();
      interactorsMap[interactorName]=InteractorLister::getPluginObject(interactorName, NULL);
    } delete itS;
  }
  
  //====================================================
  list<string> InteractorManager::getSortedCompatibleInteractors(const string &viewName) {
    list<string> compatibleInteractors;
    for(map<string,Interactor*>::iterator it=interactorsMap.begin();it!=interactorsMap.end();++it){
      if((*it).second->isCompatible(viewName))
        compatibleInteractors.push_back((*it).first);
    }
    compatibleInteractors.sort(*this);
    return compatibleInteractors;
  }
}
