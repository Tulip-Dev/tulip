/*
 *
 * This file is part of Tulip (www.tulip-software.org)
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

#ifndef TLP_PLUGINLOADER
#define TLP_PLUGINLOADER
#include <list>
#include <string>
#include <tulip/WithDependency.h>

namespace tlp {

class Plugin;

/**
 * @ingroup Plugins
 *
 * @brief A callback class when loading plugins into Tulip
 *
 * This interface can be subclassed and passed to the tlp::PluginLibraryLoader to implement custom event handling when loading plugins into Tulip
 *
 * @see tlp::PluginLibraryLoader
 * @see tlp::PluginLoaderTxt
 */
struct TLP_SCOPE PluginLoader {
  virtual ~PluginLoader() {}

  /**
   * @brief Called when starting to load plugins into a given directory
   * @param path The absolute path of the directory
   */
  virtual void start(const std::string &path)=0;

  /**
   * @brief Indicates the number of files to be loaded
   * @param int the number of files
   */
  virtual void numberOfFiles(int) {}

  /**
   * @brief Indicates that a new file is being loaded
   * @param filename The absolute path of the file
   */
  virtual void loading(const std::string &filename)=0;

  /**
   * @brief Indicates that a plugin has been loaded sucessfully
   * @param infos The Plugin object that has just been loaded
   * @param dependencies The plugin dependencies
   *
   * @see tlp::Dependency
   */
  virtual void loaded(const Plugin* infos, const std::list <Dependency>& dependencies)=0;

  /**
   * @brief Indicates that an error occurred when trying to load a file.
   * @param filename The absolute path of where the error occured.
   * @param errormsg A human-readable error message.
   */
  virtual void aborted(const std::string &filename,const  std::string &errormsg)=0;

  /**
   * @brief Indicates that a file has finished being loaded
   * @param state true if the file was loaded sucessfully
   * @param msg An additional human-readable message about the load state
   */
  virtual void finished(bool state,const std::string &msg)=0;
};

}
#endif
