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
#include <tulip/PluginLister.h>

using namespace tlp;
using namespace std;

map<string, PluginListerInterface* > *PluginListerInterface::allFactories = NULL;
PluginLoader *PluginListerInterface::currentLoader = NULL;

void PluginListerInterface::addFactory(PluginListerInterface *factory, const std::string &name) {
  if (!allFactories)
    allFactories = new std::map<std::string, PluginListerInterface*>();

  //std::cerr << name.c_str() << " factory added" << std::endl;
  (*allFactories)[name] = factory;
}

bool PluginListerInterface::pluginExists(const std::string& factoryName, const std::string& pluginName) {
  assert(allFactories->find(factoryName) != allFactories->end());
  return (*allFactories)[factoryName]->pluginExists(pluginName);
}

void PluginListerInterface::checkLoadedPluginsDependencies(tlp::PluginLoader* loader) {
  // plugins dependencies loop
  bool depsNeedCheck;

  do {
    std::map<std::string, PluginListerInterface *>::const_iterator it = PluginListerInterface::allFactories->begin();
    depsNeedCheck = false;

    // loop over factories
    for (; it != PluginListerInterface::allFactories->end(); ++  it) {
      PluginListerInterface *tfi = (*it).second;
      // loop over plugins
      Iterator<std::string> *itP = tfi->availablePlugins();

      while(itP->hasNext()) {
        std::string pluginName = itP->next();
        std::list<Dependency> dependencies = tfi->getPluginDependencies(pluginName);
        std::list<Dependency>::const_iterator itD = dependencies.begin();

        // loop over dependencies
        for (; itD != dependencies.end(); ++itD) {
          std::string factoryDepName = (*itD).factoryName;
          std::string pluginDepName = (*itD).pluginName;

          if (!PluginListerInterface::pluginExists(factoryDepName, pluginDepName)) {
            if (loader)
              loader->aborted(pluginName, tfi->getPluginsClassName() +
                              " '" + pluginName + "' will be removed, it depends on missing " +
                              factoryDepName + " '" + pluginDepName + "'.");

            tfi->removePlugin(pluginName);
            depsNeedCheck = true;
            break;
          }

          std::string release = (*PluginListerInterface::allFactories)[factoryDepName]->getPluginRelease(pluginDepName);
          std::string releaseDep = (*itD).pluginRelease;

          if (tlp::getMajor(release) != tlp::getMajor(releaseDep) ||
              tlp::getMinor(release) != tlp::getMinor(releaseDep)) {
            if (loader) {
              loader->aborted(pluginName, tfi->getPluginsClassName() +
                              " '" + pluginName + "' will be removed, it depends on release " +
                              releaseDep + " of " + factoryDepName + " '" + pluginDepName + "' but " +
                              release + " is loaded.");
            }

            tfi->removePlugin(pluginName);
            depsNeedCheck = true;
            break;
          }
        }
      }

      delete itP;
    }
  }
  while (depsNeedCheck);
}
