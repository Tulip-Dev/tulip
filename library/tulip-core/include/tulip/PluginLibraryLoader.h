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

#ifndef _WIN32
#include <dirent.h>
#endif

namespace tlp {

  /**
   * @brief This class takes care of the actual loading of the libraries.
   **/
  class TLP_SCOPE PluginLibraryLoader {
 public:
   
  /**
  * @brief Loads all the plugins in each directory contained in TulipPluginsPath.
  *
  * @param plug A PluginLoader to output what is going on. Defaults to 0.
  * @param folder A folder to append to each path in TulipPluginsPath (e.g. "views/")
  * @return void
  **/
  static void loadPlugins(PluginLoader *loader = NULL, std::string pluginPath = "");
  
  static bool loadPluginLibrary(const std::string & filename, PluginLoader *loader = 0);
  
  static const std::string& getCurrentPluginFileName() {
    return getInstance()->currentPluginLibrary;
  }

 private:
  bool hasPluginLibraryToLoad() { return nbUnloadedPluginLibraries >= 0; }
  bool loadNextPluginLibrary(PluginLoader *loader);

  void initPluginDir(PluginLoader *loader);
   
  static PluginLibraryLoader* getInstance() {
    if(_instance == NULL) {
      _instance = new PluginLibraryLoader();
    }
    return _instance;
  }
  static PluginLibraryLoader* _instance;
   
  std::string message;
  int nbUnloadedPluginLibraries;
  std::string pluginPath;
  std::string currentPluginLibrary;
  
#ifdef _WIN32
  IteratorInfos* infos;
#else
  struct dirent ** infos;
#endif
};

}
#endif //DOXYGEN_NOTFOR_USER
#endif //PLUGINLIBLOADER_H
