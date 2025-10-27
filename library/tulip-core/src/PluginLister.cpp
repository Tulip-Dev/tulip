/**
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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

struct PluginDescription {
  tlp::PluginFactory *factory;
  std::string library;
  Plugin *info;
  bool pyPlugin;

  PluginDescription() : factory(nullptr), info(nullptr), pyPlugin(false) {}

  ~PluginDescription() {
    // avoid double-free for python plugins
    // as they are allocated from the python side
    if (!pyPlugin)
      delete info;
  }
};

// used to initialize the _plugins map
// it is first called by registerPlugin at the library loading time
// while _plugins is only 'zero' initialized
std::map<std::string, PluginDescription> &getPluginsMap() {
  static std::map<std::string, PluginDescription> plugins;
  return plugins;
}

// the _plugins map
std::map<std::string, PluginDescription> &_plugins = getPluginsMap();

struct PluginEventSender : public tlp::Observable {
  // there is only one instance of this class
  // It is first used by PluginLister::registerPlugin
  // at the tulip-core library loading time while
  // it is only 'zero' initialized
  bool created;
  PluginEventSender() : created(true) {}
  inline void sendPluginAddedEvent(const std::string &pluginName) {
    if (created)
      sendEvent(PluginEvent(*this, PluginEvent::TLP_ADD_PLUGIN, pluginName));
  }
  inline void sendPluginRemovedEvent(const std::string &pluginName) {
    if (created)
      sendEvent(PluginEvent(*this, PluginEvent::TLP_REMOVE_PLUGIN, pluginName));
  }
};

static PluginEventSender _pluginEventSender;

void PluginEvent::addListener(Observable *obs) {
  _pluginEventSender.addListener(obs);
}

void PluginLister::checkLoadedPluginsDependencies(tlp::PluginLoader *loader) {
  // plugins dependencies loop
  bool depsNeedCheck;

  do {
    depsNeedCheck = false;

    // loop over plugins
    std::list<std::string> &&plugins = PluginLister::availablePlugins();

    for (const string &pluginName : plugins) {
      const std::list<Dependency> &dependencies = PluginLister::getPluginDependencies(pluginName);

      // loop over dependencies
      for (const Dependency &dep : dependencies) {
        const std::string &pluginDepName = dep.pluginName;

        if (!PluginLister::pluginExists(pluginDepName)) {
          if (loader)
            loader->aborted(pluginName, " '" + pluginName +
                                            "' will be removed, it depends on missing " + "'" +
                                            pluginDepName + "'.");

          PluginLister::removePlugin(pluginName);
          depsNeedCheck = true;
          break;
        }

        const std::string &release = PluginLister::getPluginRelease(pluginDepName);
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

Plugin *PluginLister::registeredPluginObject(const std::string &name) {
  auto it = _plugins.find(name);
  if (it != _plugins.end())
    return it->second.info;
  return nullptr;
}

class PluginIterator : public Iterator<Plugin *> {
  std::map<std::string, PluginDescription>::iterator it;

public:
  PluginIterator() : it(_plugins.begin()) {}

  bool hasNext() {
    return it != _plugins.end();
  }

  Plugin *next() {
    Plugin *plugin = nullptr;
    // deprecated names are not listed
    while ((it != _plugins.end()) && (it->first != it->second.info->name()))
      ++it;
    if (it != _plugins.end()) {
      plugin = it->second.info;
      ++it;
    }
    return plugin;
  }
};

Iterator<Plugin *> *PluginLister::registeredPluginObjects() {
  return new PluginIterator;
}

std::list<std::string> PluginLister::availablePlugins() {
  std::list<std::string> keys;

  for (auto it = _plugins.begin(); it != _plugins.end(); ++it) {
    // deprecated names are not listed
    if (it->first == it->second.info->name())
      keys.emplace_back(it->first);
  }

  return keys;
}

const Plugin &PluginLister::pluginInformation(const std::string &name) {
  return *(_plugins.find(name)->second.info);
}

void PluginLister::registerPlugin(PluginFactory *factory) {
  // because the tulip-core library contains some import/export plugins
  // we must ensure plugins map initialization at the library loading time
  // while _plugins is only 'zero' initialized
  std::map<std::string, PluginDescription> &plugins = getPluginsMap();

  tlp::Plugin *information = factory->createPluginObject(nullptr);
  std::string pluginName = information->name();
  assert(!pluginName.empty());

  if (plugins.find(pluginName) == plugins.end()) {
    PluginDescription &description = plugins[pluginName];
    description.factory = factory;
    description.library = PluginLibraryLoader::getCurrentPluginFileName();
    description.info = information;
    description.pyPlugin = information->programmingLanguage() == "Python";

    if (PluginLoader::current != nullptr) {
      PluginLoader::current->loaded(information, information->dependencies());
    }

    _pluginEventSender.sendPluginAddedEvent(pluginName);

    // register under a deprecated name if needed
    std::string oldName = information->deprecatedName();
    if (!oldName.empty()) {
      if (!pluginExists(oldName)) {
        plugins[oldName] = plugins[pluginName];
        plugins[oldName].info = factory->createPluginObject(nullptr);
      } else if (PluginLoader::current != nullptr) {
        std::string tmpStr;
        tmpStr += "'" + oldName + "' cannot be a deprecated name of plugin '" + pluginName + "'";
        PluginLoader::current->aborted(tmpStr,
                                       "multiple definitions found; check your plugin libraries.");
      }
    }
  } else {
    if (PluginLoader::current != nullptr) {
      std::string tmpStr;
      tmpStr += "'" + pluginName + "' plugin";
      PluginLoader::current->aborted(tmpStr,
                                     "multiple definitions found; check your plugin libraries.");
    }

    delete information;
  }
}

void tlp::PluginLister::removePlugin(const std::string &name) {
  _plugins.erase(name);
  _pluginEventSender.sendPluginRemovedEvent(name);
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
