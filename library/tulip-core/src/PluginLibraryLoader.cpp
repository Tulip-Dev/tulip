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

using namespace tlp;

PluginLibraryLoader* PluginLibraryLoader::_instance = NULL;
std::set<std::string> PluginLibraryLoader::libraries;

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
    
    bool hasPluginToLoad = getInstance()->hasPluginLibraryToLoad();
    if (hasPluginToLoad) {
      while(getInstance()->loadNextPluginLibrary(loader)) {
      }
    }
    if (loader) {
      loader->finished(hasPluginToLoad, getInstance()->message);
    }
  }
}

#if defined(_WIN32)
QString suffix = "*.dll";
#elif defined(__APPLE__) 
QString suffix = "*.dylib";
#else
QString suffix = "*.so";
#endif

#ifdef _WIN32
bool PluginLibraryLoader::loadPluginLibrary(const std::string & filename, PluginLoader *loader) {   
  HINSTANCE hDLL = LoadLibrary(filename.c_str());   
  if (hDLL == NULL) {   
    if (loader!=0) {   
      char *msg;   
      DWORD dwErrCode = GetLastError();   
      FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |   
      FORMAT_MESSAGE_FROM_SYSTEM,   
      NULL,               // no source buffer needed   
      dwErrCode,          // error code for this message   
      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),   
                    (LPTSTR)&msg,       // allocated by fcn   
                    0,               // minimum size of buffer   
                    NULL);              // no inserts   
      if (!msg) {   
        char scode[128];   
        sprintf(scode, "%s: unable to load(error %d)", filename.c_str(), (int) dwErrCode);   
        loader->aborted(filename, std::string(scode));   
      }   
      else {   
        loader->aborted(filename, filename + ": " + msg);   
        LocalFree(msg);   
      }   
    }   
  }   
  return hDLL != NULL;   
}
#else 
bool PluginLibraryLoader::loadPluginLibrary(const std::string &filename, PluginLoader *loader) {
  void *handle = dlopen (filename.c_str() , RTLD_NOW);
  if (!handle) {
    if (loader!=0)
      loader->aborted(filename, std::string(dlerror()));
  }
  return handle;
}
#endif

void PluginLibraryLoader::initPluginDir(PluginLoader *loader) {
  
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
  
  //Qt is not allowed in tulip-core headers, so convert to std types.
  foreach(const QString& library, plugins) {
    libraries.insert(library.toStdString());
  }
  currentLibrary = libraries.begin();
}

bool PluginLibraryLoader::loadNextPluginLibrary(PluginLoader *loader) {
  if(currentLibrary != libraries.end()) {
    std::string lib = *currentLibrary;
    QString library(lib.c_str());
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
    
    currentLibrary++;
  }
  return currentLibrary != libraries.end(); //return whether there are more plugins to load
}
