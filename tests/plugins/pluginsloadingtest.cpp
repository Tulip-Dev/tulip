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
#include <cstdlib>

#include <QApplication>

#include <tulip/TlpTools.h>
#include <tulip/PluginLoaderTxt.h>
#include <tulip/PluginLibraryLoader.h>
#include <tulip/PluginLister.h>

// Custom loader to catch if there was some issues
// when loading plugins
class PluginLoaderTest : public tlp::PluginLoaderTxt {
public:
  PluginLoaderTest() : allPluginsLoaded(true) {}

  void aborted(const std::string &filename, const std::string &errormsg) {
    const std::string &libName = tlp::PluginLibraryLoader::getCurrentPluginFileName();
    // plugins may be loaded twice because it may exist an other version
    // of the plugins in a CMakeFiles sub dir (/CMakeRelink.dir)
    // So set the failure flag only if the plugin was not found
    // under the CMakeFiles dir
    if (libName.find("CMakeFiles") == std::string::npos)
      allPluginsLoaded = false;
    tlp::PluginLoaderTxt::aborted(filename, errormsg);
  }

  bool allPluginsLoaded;
};

// Simple test that will try to load all compiled plugins from the Tulip trunk
// in order to detect possible issues.
// The test will then try to create and destroy an instance of each plugin.
// This can be helpfull to catch possible segfaults or memory leaks.
int main(int argc, char **argv) {

  // we need to create a QApplication as some plugins (view, perspective, interactor)
  // need one to load correctly
  QApplication app(argc, argv);

  std::string tulipBuildDir = TULIP_BUILD_DIR;

  tlp::initTulipLib();

  // load all plugins from the Tulip build folder
  PluginLoaderTest pLoader;
  tlp::PluginLibraryLoader::loadPluginsFromDir(tulipBuildDir + "/plugins", &pLoader);

  // create an instance of each of them, then destroy it
  std::list<std::string> pluginNames = tlp::PluginLister::instance()->availablePlugins();
  std::list<std::string>::const_iterator it = pluginNames.begin();
  for (; it != pluginNames.end(); ++it) {
    tlp::Plugin *plugin = tlp::PluginLister::instance()->getPluginObject(*it, NULL);
    delete plugin;
  }

  // test is successfull if there was no plugin loading issue
  if (pLoader.allPluginsLoaded) {
    return EXIT_SUCCESS;
  } else {
    return EXIT_FAILURE;
  }
}
