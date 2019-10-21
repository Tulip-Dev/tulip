/**
 *
 * This file is part of Tulip (http://tulip.labri.fr)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
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
#include <cstdlib>
#include <tulip/PluginLister.h>
#include <tulip/PluginLoader.h>
#include <tulip/PluginLibraryLoader.h>

using namespace tlp;
using namespace std;

PluginLoader *PluginLister::currentLoader = nullptr;

struct PluginListerInstance : public PluginLister {
  // there is only one instance of this class
  // It is first used by PluginLister::registerPlugin
  // at the tulip-core library loading time while
  // it is only 'zero' initialized
  bool created;
  PluginListerInstance() : created(true) {}
  inline void sendPluginAddedEvent(const std::string &pluginName) {
    if (created)
      sendEvent(PluginEvent(PluginEvent::TLP_ADD_PLUGIN, pluginName));
  }
  inline void sendPluginRemovedEvent(const std::string &pluginName) {
    if (created)
      sendEvent(PluginEvent(PluginEvent::TLP_REMOVE_PLUGIN, pluginName));
  }
};

PluginListerInstance _instance;

PluginLister *PluginLister::instance() {
  return &_instance;
}

void PluginLister::checkLoadedPluginsDependencies(tlp::PluginLoader *loader) {
  // plugins dependencies loop
  bool depsNeedCheck;

  do {
    depsNeedCheck = false;

    // loop over plugins
    std::list<std::string> plugins = PluginLister::availablePlugins();

    for (const string &pluginName : plugins) {
      std::list<Dependency> dependencies = PluginLister::getPluginDependencies(pluginName);

      // loop over dependencies
      for (const Dependency &dep : dependencies) {
        std::string pluginDepName = dep.pluginName;

        if (!PluginLister::pluginExists(pluginDepName)) {
          if (loader)
            loader->aborted(pluginName, " '" + pluginName +
                                            "' will be removed, it depends on missing " + "'" +
                                            pluginDepName + "'.");

          PluginLister::removePlugin(pluginName);
          depsNeedCheck = true;
          break;
        }

        std::string release = PluginLister::getPluginRelease(pluginDepName);
        const std::string &releaseDep = dep.pluginRelease;

        if (tlp::getMajor(release) != tlp::getMajor(releaseDep) ||
            tlp::getMinor(release) < tlp::getMinor(releaseDep)) {
          if (loader) {
            loader->aborted(pluginName, " '" + pluginName +
                                            "' will be removed, it depends on release " +
                                            releaseDep + " of" + " '" + pluginDepName + "' but " +
                                            release + " is loaded.");
          }

          PluginLister::removePlugin(pluginName);
          depsNeedCheck = true;
          break;
        }
      }
    }
  } while (depsNeedCheck);
}

std::list<std::string> PluginLister::availablePlugins() {
  std::list<std::string> keys;

  for (auto it = _plugins.begin(); it != _plugins.end(); ++it) {
    // deprecated names are not listed
    if (it->first == it->second.info->name())
      keys.push_back(it->first);
  }

  return keys;
}

const Plugin &PluginLister::pluginInformation(const std::string &name) {
  return *(_plugins.find(name)->second.info);
}

// used to initialize the _plugins map
// it is first called by registerPlugin at the library loading time
// while _plugins is only 'zero' initialized
std::map<std::string, PluginLister::PluginDescription> &PluginLister::getPluginsMap() {
  static std::map<std::string, PluginDescription> plugins;
  return plugins;
}

// the _plugins map
std::map<std::string, PluginLister::PluginDescription> &PluginLister::_plugins =
    PluginLister::getPluginsMap();

void PluginLister::registerPlugin(FactoryInterface *objectFactory) {
  // because the tulip-core library contains some import/export plugins
  // we must ensure plugins map initialization at the library loading time
  // while _plugins is only 'zero' initialized
  std::map<std::string, PluginLister::PluginDescription> &plugins = getPluginsMap();

  tlp::Plugin *information = objectFactory->createPluginObject(nullptr);
  std::string pluginName = information->name();
  assert(!pluginName.empty());

  if (plugins.find(pluginName) == plugins.end()) {
    PluginDescription &description = plugins[pluginName];
    description.factory = objectFactory;
    description.library = PluginLibraryLoader::getCurrentPluginFileName();
    description.info = information;

    if (currentLoader != nullptr) {
      currentLoader->loaded(information, information->dependencies());
    }

    _instance.sendPluginAddedEvent(pluginName);

    // register under a deprecated name if needed
    std::string oldName = information->deprecatedName();
    if (!oldName.empty()) {
      if (!pluginExists(oldName)) {
        plugins[oldName] = plugins[pluginName];
        plugins[oldName].info = objectFactory->createPluginObject(nullptr);
      } else if (currentLoader != nullptr) {
        std::string tmpStr;
        tmpStr += "'" + oldName + "' cannot be a deprecated name of plugin '" + pluginName + "'";
        currentLoader->aborted(tmpStr, "multiple definitions found; check your plugin librairies.");
      }
    }
  } else {
    if (currentLoader != nullptr) {
      std::string tmpStr;
      tmpStr += "'" + pluginName + "' plugin";
      currentLoader->aborted(tmpStr, "multiple definitions found; check your plugin librairies.");
    }

    delete information;
  }
}

void tlp::PluginLister::removePlugin(const std::string &name) {
  _plugins.erase(name);
  _instance.sendPluginRemovedEvent(name);
}

tlp::Plugin *PluginLister::getPluginObject(const std::string &name, PluginContext *context) {
  auto it = _plugins.find(name);

  if (it != _plugins.end()) {
    std::string pluginName = it->second.info->name();
    if (name != pluginName)
      tlp::warning() << "Warning: '" << name << "' is a deprecated plugin name. Use '" << pluginName
                     << "' instead." << std::endl;

    return it->second.factory->createPluginObject(context);
  }

  return nullptr;
}

const tlp::ParameterDescriptionList &PluginLister::getPluginParameters(const std::string &name) {
  return pluginInformation(name).getParameters();
}

std::string PluginLister::getPluginRelease(const std::string &name) {
  return pluginInformation(name).release();
}

const std::list<tlp::Dependency> &PluginLister::getPluginDependencies(const std::string &name) {
  return pluginInformation(name).dependencies();
}

std::string PluginLister::getPluginLibrary(const std::string &name) {
  return _plugins.find(name)->second.library;
}

bool PluginLister::pluginExists(const std::string &pluginName) {
  return _plugins.find(pluginName) != _plugins.end();
}
