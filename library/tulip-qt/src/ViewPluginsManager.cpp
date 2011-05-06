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
#include "tulip/ViewPluginsManager.h"
#include "tulip/TlpQtTools.h"

//====================================================
tlp::ViewPluginsManager* tlp::ViewPluginsManager::inst=0;

using namespace std;

namespace tlp
{

  ViewPluginsManager::ViewPluginsManager() {
    ViewFactory::initFactory();
  }
  //====================================================
  void ViewPluginsManager::loadPlugins(PluginLoader *plug) {
    ViewFactory::initFactory();
    tlp::loadPlugins(plug, "/view");
  }
  //====================================================
  void ViewPluginsManager::initViewPluginsList(MutableContainer<View *> &views) {
    ViewContext ic;
    views.setAll(0);

    Iterator<string> *itS = ViewFactory::factory->availablePlugins();
    while (itS->hasNext()) {
      string viewPluginName=itS->next();
      ViewFactory::factory->getPluginObject(viewPluginName, &ic);
    } delete itS;
  }
  //====================================================
  View* ViewPluginsManager::createView(const string &name) {
  	ViewContext ic;
  	return ViewFactory::factory->getPluginObject(name, &ic);
  }
}
