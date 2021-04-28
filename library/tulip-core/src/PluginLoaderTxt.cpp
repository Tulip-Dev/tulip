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

#include <iostream>
#include <tulip/Plugin.h>
#include <tulip/PluginLoaderTxt.h>

using namespace std;
using namespace tlp;

PluginLoader *PluginLoader::current = nullptr;

void PluginLoaderTxt::start(const string &path) {
  std::cout << "Start loading plug-ins in " << path << std::endl;
}

void PluginLoaderTxt::loading(const string &filename) {
  std::cout << "loading file: " << filename << endl;
}

void PluginLoaderTxt::loaded(const Plugin *info,
                             const std::list<Dependency> &deps) {
  std::cout << "Plug-in " << info->name()
            << " loaded, Author: " << info->author()
            << ", Date: " << info->date() << ", Release: " << info->release()
            << ", Tulip Version: " << info->tulipRelease() << endl;

  // output dependencies if any
  if (deps.size()) {
    unsigned int i = deps.size();
    std::cout << "depending on ";
    list<Dependency>::const_iterator itD = deps.begin();

    for (i--; itD != deps.end(); ++itD, --i)
      std::cout << itD->pluginName << (i ? ", " : "");
    std::cout << endl;
  }
}

void PluginLoaderTxt::aborted(const string &filename, const string &erreurmsg) {
  std::cout << "Aborted loading of " << filename << " Error:" << erreurmsg
            << endl;
}

void PluginLoaderTxt::finished(bool state, const string &msg) {
  if (state)
    std::cout << "Loading complete" << endl;
  else
    std::cout << "Loading error " << msg << endl;
}
