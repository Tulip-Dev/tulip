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
#include "tulip/ControllerPluginsManager.h"
#include "tulip/TlpQtTools.h"

//====================================================
#ifdef _WIN32
#ifdef DLL_EXPORT
tlp::ControllerPluginsManager* tlp::ControllerPluginsManager::inst=0;
#endif
#else
tlp::ControllerPluginsManager* tlp::ControllerPluginsManager::inst=0;
#endif

using namespace std;

namespace tlp
{

  ControllerPluginsManager::ControllerPluginsManager() {
    ControllerFactory::initFactory();
  }
  //====================================================
  void ControllerPluginsManager::loadPlugins(PluginLoader *plug) {
    ControllerFactory::initFactory();
    string::const_iterator begin=tlp::TulipPluginsPath.begin();
    string::const_iterator end=begin;
    while (end!=tlp::TulipPluginsPath.end())
      if ((*end)==tlp::PATH_DELIMITER) {
	if (begin!=end)
	  tlp::loadControllerPluginsFromDir(string(begin,end)+"/controller", plug);
	++end;
	begin=end;
      } else
	++end;
    if (begin!=end) {
      tlp::loadControllerPluginsFromDir(string(begin,end)+"/controller", plug);
    }
  }
  //====================================================
  void ControllerPluginsManager::initControllerPluginsList(MutableContainer<Controller *> &controllers) {
    ControllerContext ic;
    controllers.setAll(0);

    Iterator<string> *itS = ControllerFactory::factory->availablePlugins();
    while (itS->hasNext()) {
      string controllerPluginName=itS->next();
      Controller *newControllerPlugin = ControllerFactory::factory->getPluginObject(controllerPluginName, &ic);
    } delete itS;
  }
  //====================================================
  Controller* ControllerPluginsManager::createController(const string &name) {
    ControllerContext ic;
    return ControllerFactory::factory->getPluginObject(name, &ic);
  }
}
