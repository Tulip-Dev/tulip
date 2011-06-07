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
#include <tulip/PluginLibraryLoader.h>
#include <tulip/TulipRelease.h>
#include <tulip/tulipconf.h>
#include <tulip/PluginLister.h>
#include <string.h>
#include <sstream>
#include <set>
#include <vector>

#ifdef _WIN32
#include <stdio.h>
#include <windows.h>
#else
#include <stdlib.h>
#include <dlfcn.h>
#endif

#include <QtCore/QString>
#include <QtCore/QDir>
#include <QtCore/QLibrary>

using namespace tlp;

PluginLibraryLoader* PluginLibraryLoader::_instance = NULL;

void PluginLibraryLoader::loadPlugins(PluginLoader *loader, std::string folder) {
  std::vector<std::string> paths;
  std::stringstream ss(TulipPluginsPath);
  std::string item;
  while(getline(ss, item, PATH_DELIMITER)) {
    paths.push_back(item);
  }
  
  //load the plugins in 'folder' for each path in TulipPluginsPath (TulipPluginsPath/folder)
  for(std::vector<std::string>::const_iterator it = paths.begin(); it != paths.end(); ++it) {
    std::string dir = (*it) + "/" + folder;
    if (loader!=0)
      loader->start(dir.c_str());
    
    PluginListerInterface::currentLoader = loader;
    getInstance()->pluginPath = dir;
    getInstance()->initPluginDir(loader);
    
    if (loader) {
      loader->finished(true, getInstance()->message);
    }
  }
}

bool PluginLibraryLoader::loadPluginLibrary(const std::string &filename, PluginLoader *loader) {
  QLibrary lib(filename.c_str());
  bool loaded = lib.load();
  if (loader != 0 && !loaded) {
    loader->aborted(filename, lib.errorString().toStdString());
  }
  return loaded;
}

void PluginLibraryLoader::initPluginDir(PluginLoader *loader) {
  #if defined(_WIN32)
  const QString suffix = "*.dll";
  #elif defined(__APPLE__)
  const QString suffix = "*.dylib";
  #else
  const QString suffix = "*.so";
  #endif
  
  QDir pluginDir(pluginPath.c_str());
  QStringList filters(suffix);
  QStringList plugins = pluginDir.entryList(filters, QDir::Files);
  int nbUnloadedPluginLibraries = plugins.size();
  
  if (loader!=0)
    loader->numberOfFiles(nbUnloadedPluginLibraries);
  
  if (nbUnloadedPluginLibraries < 0) {
    message = "No libraries in folder";
    return;
  }
  
  foreach(const QString& library, plugins) {
    std::string lib = library.toStdString();
    currentPluginLibrary = pluginPath +"/"+ lib;
    if(library.contains(TULIP_RELEASE)) {
      if (loader!=0) {
        loader->loading(lib);
      }
      loadPluginLibrary(currentPluginLibrary, loader);
    }
    else {
      //the library is not a tulip plug-in
      if (loader) {
        loader->aborted(currentPluginLibrary, currentPluginLibrary + " is not a Tulip plugin library");
      }
    }
  }
}

