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

std::map<std::string, PluginLister::PluginDescription> PluginLister::plugins = std::map<std::string, PluginLister::PluginDescription>();
PluginLoader* PluginLister::currentLoader = NULL;
PluginLister* PluginLister::_instance = NULL;

void PluginLister::checkLoadedPluginsDependencies(tlp::PluginLoader* loader) {
  // plugins dependencies loop
  bool depsNeedCheck;

  do {
    depsNeedCheck = false;

    // loop over plugins
    Iterator<std::string> *itP = _instance->availablePlugins();

    while(itP->hasNext()) {
      std::string pluginName = itP->next();
      std::list<Dependency> dependencies = _instance->getPluginDependencies(pluginName);
      std::list<Dependency>::const_iterator itD = dependencies.begin();

      // loop over dependencies
      for (; itD != dependencies.end(); ++itD) {
        std::string factoryDepName = (*itD).factoryName;
        std::string pluginDepName = (*itD).pluginName;

        if (!PluginLister::pluginExists(pluginDepName)) {
          if (loader)
            loader->aborted(pluginName, " '" + pluginName + "' will be removed, it depends on missing " +
                            factoryDepName + " '" + pluginDepName + "'.");

            _instance->removePlugin(pluginName);
          depsNeedCheck = true;
          break;
        }

        std::string release = _instance->getPluginRelease(pluginName);
        std::string releaseDep = (*itD).pluginRelease;

        if (tlp::getMajor(release) != tlp::getMajor(releaseDep) ||
            tlp::getMinor(release) != tlp::getMinor(releaseDep)) {
          if (loader) {
            loader->aborted(pluginName, " '" + pluginName + "' will be removed, it depends on release " +
                            releaseDep + " of " + factoryDepName + " '" + pluginDepName + "' but " +
                            release + " is loaded.");
          }

          _instance->removePlugin(pluginName);
          depsNeedCheck = true;
          break;
        }
      }
    }

    delete itP;
  }
  while (depsNeedCheck);
}

tlp::Iterator<std::string>* tlp::PluginLister::availablePlugins() {
  return new StlMapKeyIterator<std::string, PluginDescription>(plugins.begin(), plugins.end());
}

const tlp::Plugin* tlp::PluginLister::pluginInformations(const std::string& name) {
  return plugins.find(name)->second.factory->createPluginObject(NULL);
}

void tlp::PluginLister::registerPlugin(FactoryInterface *objectFactory) {
  tlp::Plugin* informations = objectFactory->createPluginObject(NULL);
  std::string pluginName = informations->getName();
  
  if (!pluginExists(pluginName)) {
    
    // loop over dependencies
    // to demangle the class names
    std::list<tlp::Dependency> dependencies = informations->getDependencies();
    std::list<tlp::Dependency>::iterator itD = dependencies.begin();
    
    for (; itD != dependencies.end(); itD++) {
      const char *factoryDepName = (*itD).factoryName.c_str();
      (*itD).factoryName = tlp::demangleTlpClassName(factoryDepName);
    }
    
    PluginDescription description;
    description.factory = objectFactory;
    description.parameters = informations->getParameters();
    description.dependencies = dependencies;
    description.library = PluginLibraryLoader::getCurrentPluginFileName();
    plugins[pluginName] = description;
    
    
    if (currentLoader!=0) {
      currentLoader->loaded(informations, dependencies);
    }
    delete informations;
  }
  else {
    if (currentLoader != 0) {
      std::string tmpStr;
      tmpStr += "'" + pluginName + "' plugin";
      currentLoader->aborted(tmpStr, "multiple definitions found; check your plugin librairies.");
    }
  }
}

void tlp::PluginLister::removePlugin(const std::string &name) {
  plugins.erase(name);
}

tlp::Plugin* tlp::PluginLister::getPluginObject(const std::string& name, PluginContext* context) {
  typename std::map<std::string, PluginDescription>::const_iterator it = plugins.find(name);
  
  if (it!=plugins.end()) {
    return (*it).second.factory->createPluginObject(context);
  }
  
  return 0;
}

const tlp::ParameterList tlp::PluginLister::getPluginParameters(std::string name) {
  assert(plugins.find(name)!=plugins.end());
  return plugins.find(name)->second.parameters;
}

std::string tlp::PluginLister::getPluginRelease(std::string name) {
  assert(plugins.find(name)!=plugins.end());
  return plugins.find(name)->second.factory->createPluginObject(NULL)->getRelease();
}

std::list<tlp::Dependency> tlp::PluginLister::getPluginDependencies(std::string name) {
  assert(plugins.find(name)!=plugins.end());
  return plugins.find(name)->second.dependencies;
}

std::string tlp::PluginLister::getPluginLibrary(const std::string& name) {
  return plugins.find(name)->second.library;
}

bool tlp::PluginLister::pluginExists(const std::string& pluginName) {
  return plugins.find(pluginName) != plugins.end();
}
