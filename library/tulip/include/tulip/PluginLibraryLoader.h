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
#ifndef PLUGINLIBLOADER_H
#define PLUGINLIBLOADER_H

#ifndef DOXYGEN_NOTFOR_USER

#include <tulip/PluginLoader.h>
#include <tulip/tulipconf.h>

namespace tlp {

class TLP_SCOPE PluginLibraryLoader {
 public:
  int n;
  std::string msg;
  std::string pluginPath;
  PluginLoader *loader;

  PluginLibraryLoader(std::string pluginPath, PluginLoader *loader);
  bool hasPluginLibraryToLoad() { return n >= 0; }
  bool loadNextPluginLibrary(PluginLoader *loader);
  static bool loadPluginLibrary(const std::string & filename,
				PluginLoader *loader = 0);
  static const std::string& getCurrentPluginFileName() {
    return currentPluginLibrary;
  }

 private:
  void *infos;
  static std::string currentPluginLibrary;
};

}
#endif //DOXYGEN_NOTFOR_USER
#endif //PLUGINLIBLOADER_H
