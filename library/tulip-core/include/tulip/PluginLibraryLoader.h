/*
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
///@cond DOXYGEN_HIDDEN

#ifndef PLUGINLIBLOADER_H
#define PLUGINLIBLOADER_H

#include <set>
#include <string>

#include <tulip/tulipconf.h>

namespace tlp {

struct PluginLoader;

/**
 * @ingroup Plugins
 *
 * @brief This class takes care of the actual loading of the libraries.
 * You can use it to load a single plugin (loadPluginLibrary) or all the plugins in a given folder
 *(loadPlugins).
 *
 * It is a singleton to guarantee the currentPluginLibrary member is initialized, but it only shows
 *static functions for syntactic sugar.
 **/
class TLP_SCOPE PluginLibraryLoader {
public:
  /**
   * @brief Loads all the plugins in each directory contained in TulipPluginsPath.
   * This function will not look into subfolders of the specified folder.
   *
   *
   * To load all the plugins in the following example, you need to call this function once for the
   *lib/tulip folder,
   * once for the glyph folder, and once for the interactors folder.
   *
   * lib/tulip/
   * -> glyphs
   *      |-> libBillboard-4.0.0.so
   *      |-> libWindow-4.0.0.so
   * -> interactors
   *      |-> libInteractorAddEdge-4.0.0.so
   *      |-> libInteractorSelectionModifier-4.0.0.so
   * -> libAdjacencyMatrixImport-4.0.0.so
   * -> libColorMapping-4.0.0.so
   * -> libCompleteGraph-4.0.0.so
   *
   *
   * @param loader A PluginLoader to output what is going on. Defaults to nullptr.
   * @param pluginPath A folder to append to each path in TulipPluginsPath (e.g. "glyphs/")
   *
   **/
  static void loadPlugins(PluginLoader *loader = nullptr, const std::string &pluginPath = "");

  /**
  * @brief Recursively loads plugins from a root directory.
  *
  * @since Tulip 5.1
  *
  * This function enables to recursively load Tulip plugins from a
  * provided root directory, thus visiting subdirectories of the provided
  * one and so forth. If a new version of a plugin exist in a user specific
  * local directory then the plugin is not loaded.
  *
  *
  * @param rootPath The root directory from which to look for plugins to load.
  * @param loader A PluginLoader to output what is going on. Defaults to nullptr.
  * @param userLocalPath A user specific local directory where some plugins may have been downloaded

  *
  **/
  static void loadPluginsFromDir(const std::string &rootPath, PluginLoader *loader = nullptr,
                                 const std::string &userLocalPath = "");

  /**
   * @brief Loads a single plugin library.
   *
   * @param filename The name of the plugin file to load.
   * @param loader A loader to report what is going on (only its loaded or aborted functions will be
   *called) Defaults to nullptr.
   * @return bool Whether the plugin was successfully loaded.
   **/
  static bool loadPluginLibrary(const std::string &filename, PluginLoader *loader = nullptr);

  /**
   * @brief Gets the name of the plug-in library being loaded.
   * If the plugin is statically linked into the tulip library, returns an empty string.
   *
   * @return :string& The name of the plugin library being loaded.
   **/
  static const std::string &getCurrentPluginFileName() {
    return _currentPluginLibrary;
  }

private:
  PluginLibraryLoader() {}
  static bool initPluginDir(PluginLoader *loader, bool recursive = false,
                            const std::string &userPluginsPath = "");
  static std::string _message;
  static std::string _pluginPath;
  static std::string _currentPluginLibrary;
};
} // namespace tlp

#endif // PLUGINLIBLOADER_H

///@endcond
