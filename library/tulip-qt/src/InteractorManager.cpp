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
#include "tulip/TlpQtTools.h"


//====================================================
tlp::InteractorManager* tlp::InteractorManager::inst=0;

using namespace std;

namespace tlp
{

  InteractorManager::InteractorManager() {
    InteractorFactory::initFactory();
  }
  //====================================================
  void InteractorManager::loadPlugins(PluginLoader *plug) {
    InteractorFactory::initFactory();
    string::const_iterator begin=tlp::TulipPluginsPath.begin();
    string::const_iterator end=begin;
    while (end!=tlp::TulipPluginsPath.end())
      if ((*end)==tlp::PATH_DELIMITER) {
        if (begin!=end)
          tlp::loadInteractorPluginsFromDir(string(begin,end)+"/interactors",plug);
        ++end;
        begin=end;
      } else
        ++end;
    if (begin!=end) {
      tlp::loadInteractorPluginsFromDir(string(begin,end)+"/interactors",plug);
    }

    // if interactorMap is empty, put all interactors in the Map
    interactorsMap.clear();
    InteractorContext ic;

    Iterator<string> *itS = InteractorFactory::factory->availablePlugins();
    while (itS->hasNext()) {
      string interactorName=itS->next();
      interactorsMap[interactorName]=InteractorFactory::factory->getPluginObject(interactorName, &ic);
    } delete itS;
  }
  //====================================================
  Interactor *InteractorManager::getInteractor(const string &name){
    InteractorContext ic;
    return InteractorFactory::factory->getPluginObject(name, &ic);
  }
  //====================================================
  list<string> InteractorManager::getCompatibleInteractors(const string &viewName) {
    loadPlugins();
    list<string> compatibleInteractors;
    for(map<string,Interactor*>::iterator it=interactorsMap.begin();it!=interactorsMap.end();++it){
      if((*it).second->isCompatible(viewName))
        compatibleInteractors.push_back((*it).first);
    }
    return compatibleInteractors;
  }

  //====================================================
  list<string> InteractorManager::getSortedCompatibleInteractors(const string &viewName) {
    multimap<int,string> sortedInteractors;
    list<string> result;
    list<string> compatibleInteractors=getCompatibleInteractors(viewName);
    for(list<string>::iterator it=compatibleInteractors.begin();it!=compatibleInteractors.end();++it){
    	sortedInteractors.insert(pair<int,string>(interactorsMap[*it]->getPriority(),(*it)));
    }
    for(multimap<int,string>::reverse_iterator it=sortedInteractors.rbegin();it!=sortedInteractors.rend();++it){
    	result.push_back(it->second);
    }
    return result;
  }
}
