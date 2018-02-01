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
#include <tulip/PluginLister.h>
#include <tulip/PluginLoader.h>
#include <tulip/PluginLibraryLoader.h>

using namespace tlp;
using namespace std;

PluginLoader *PluginLister::currentLoader = nullptr;
PluginLister *PluginLister::_instance = nullptr;

tlp::PluginLister *PluginLister::instance() {
  if (_instance == nullptr) {
    _instance = new PluginLister();
  }

  return _instance;
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

std::list<std::string> tlp::PluginLister::availablePlugins() {
  std::list<std::string> keys;

  for (auto it = instance()->_plugins.begin();
       it != instance()->_plugins.end(); ++it) {
    // deprecated names are not listed
    if (it->first == it->second.info->name())
      keys.push_back(it->first);
  }

  return keys;
}

const tlp::Plugin &tlp::PluginLister::pluginInformation(const std::string &name) {
  return *(instance()->_plugins.find(name)->second.info);
}

void tlp::PluginLister::registerPlugin(FactoryInterface *objectFactory) {
  tlp::Plugin *information = objectFactory->createPluginObject(nullptr);
  std::string pluginName = information->name();

  if (!pluginExists(pluginName)) {
    PluginDescription &description = instance()->_plugins[pluginName];
    description.factory = objectFactory;
    description.library = PluginLibraryLoader::getCurrentPluginFileName();
    description.info = information;

    if (currentLoader != nullptr) {
      currentLoader->loaded(information, information->dependencies());
    }

    instance()->sendPluginAddedEvent(pluginName);

    // register under a deprecated name if needed
    std::string oldName = information->deprecatedName();
    if (!oldName.empty()) {
      if (!pluginExists(oldName))
        instance()->_plugins[oldName] = description;
      else if (currentLoader != nullptr) {
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

void tlp::PluginLister::sendPluginAddedEvent(const std::string &pluginName) {
  sendEvent(PluginEvent(PluginEvent::TLP_ADD_PLUGIN, pluginName));
}

void tlp::PluginLister::sendPluginRemovedEvent(const std::string &pluginName) {
  sendEvent(PluginEvent(PluginEvent::TLP_REMOVE_PLUGIN, pluginName));
}

void tlp::PluginLister::removePlugin(const std::string &name) {
  instance()->_plugins.erase(name);
  instance()->sendPluginRemovedEvent(name);
}

tlp::Plugin *tlp::PluginLister::getPluginObject(const std::string &name, PluginContext *context) {
  auto it = instance()->_plugins.find(name);

  if (it != instance()->_plugins.end()) {
    std::string pluginName = it->second.info->name();
    if (name != pluginName)
      tlp::warning() << "Warning: '" << name << "' is a deprecated plugin name. Use '" << pluginName
                     << "' instead." << std::endl;

    return it->second.factory->createPluginObject(context);
  }

  return nullptr;
}

const tlp::ParameterDescriptionList &
tlp::PluginLister::getPluginParameters(const std::string &name) {
  return pluginInformation(name).getParameters();
}

std::string tlp::PluginLister::getPluginRelease(const std::string &name) {
  return pluginInformation(name).release();
}

const std::list<tlp::Dependency> &
tlp::PluginLister::getPluginDependencies(const std::string &name) {
  return pluginInformation(name).dependencies();
}

std::string tlp::PluginLister::getPluginLibrary(const std::string &name) {
  return instance()->_plugins.find(name)->second.library;
}

bool tlp::PluginLister::pluginExists(const std::string &pluginName) {
  return instance()->_plugins.find(pluginName) != instance()->_plugins.end();
}
