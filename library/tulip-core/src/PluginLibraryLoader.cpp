/**
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
#include <tulip/PluginLibraryLoader.h>
#include <tulip/TulipRelease.h>
#include <tulip/tulipconf.h>
#include <tulip/PluginLister.h>
#include <tulip/TlpTools.h>

#include <cstring>
#include <sstream>
#include <set>
#include <vector>

#ifdef _WIN32
#include <stdio.h>
#include <windows.h>
#else
#include <cstdlib>
#include <dlfcn.h>
#include <dirent.h>
#include <errno.h>
#endif

using namespace tlp;

PluginLibraryLoader* PluginLibraryLoader::_instance = NULL;

#ifndef EMSCRIPTEN
void PluginLibraryLoader::loadPlugins(PluginLoader *loader, const std::string& folder) {
  std::vector<std::string> paths;
  std::stringstream ss(TulipPluginsPath);
  std::string item;

  while(getline(ss, item, PATH_DELIMITER)) {
    paths.push_back(item);
  }

  //load the plugins in 'folder' for each path in TulipPluginsPath (TulipPluginsPath/folder)
  for(std::vector<std::string>::const_iterator it = paths.begin(); it != paths.end(); ++it) {
    std::string dir = (*it) + "/" + folder;

    if (loader!=NULL)
      loader->start(dir.c_str());

    PluginLister::currentLoader = loader;
    getInstance()->pluginPath = dir;

    // ensure message is empty before plugins directory loading
    getInstance()->message.clear();

    if (getInstance()->initPluginDir(loader)) {
      if (loader)
        loader->finished(true, getInstance()->message);
    }

#ifndef NDEBUG
    else
      tlp::debug() << "loadPlugins info: " << getInstance()->message.c_str() << std::endl;

#endif

    PluginLister::currentLoader = NULL;
  }
}

#ifdef _WIN32
bool PluginLibraryLoader::loadPluginLibrary(const std::string & filename, PluginLoader *loader) {
  HINSTANCE hDLL = LoadLibrary(filename.c_str());

  if (hDLL == NULL) {
    if (loader!=NULL) {
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
        sprintf(scode, "%s: unable to load(error %d)",
                filename.c_str(), (int) dwErrCode);
        loader->aborted(filename, std::string(scode));
      }
      else {
        loader->aborted(filename, filename + ": " + msg);
        LocalFree(msg);
      }
    }

    return false;
  }

  return true;
}

#else

bool PluginLibraryLoader::loadPluginLibrary(const std::string &filename, PluginLoader *loader) {
  void *handle = dlopen (filename.c_str() , RTLD_NOW);

  if (!handle) {
    if (loader!=NULL)
      loader->aborted(filename, std::string(dlerror()));

    return false;
  }

  return true;
}

// accepts only file whose name matches *.so or *.dylib
int __tulip_select_libs(struct dirent *ent) {
#if !defined(__APPLE__)
  const char *suffix = ".so";
  const unsigned long suffix_len = 3;
#else
  const char *suffix = ".dylib";
  const unsigned long suffix_len = 6;
#endif
  int idx = strlen(ent->d_name) - suffix_len;

  if (idx < 0) return 0;

  for (unsigned long i=0; i < suffix_len; ++i) {
    if ((ent->d_name[idx + i]) != suffix[i]) return 0;
  }

  return 1;
}

#endif

bool PluginLibraryLoader::initPluginDir(PluginLoader *loader) {
  std::string tulip_mm_version(TULIP_MM_VERSION);
  std::string tulip_version(TULIP_VERSION);

#if defined(_WIN32)

  HANDLE hFind;
  WIN32_FIND_DATA findData;
  TCHAR currentDirectory[FILENAME_MAX];
  DWORD dwRet;
  dwRet = GetCurrentDirectory(FILENAME_MAX, currentDirectory);

  if(dwRet == 0) {
    message = pluginPath + " - Scandir error";
    return false;
  }
  else {
    // first check is pluginPath exists
    DWORD fileAttr = GetFileAttributes(pluginPath.c_str());

    if (fileAttr == 0xFFFFFFFF) {
      message += std::string("Directory not found: ") + pluginPath.c_str();
      return false;
    }

    SetCurrentDirectory (pluginPath.c_str());
    hFind = FindFirstFile ("*.dll", &findData);

    if (loader != NULL) {
      // count files loop
      unsigned long nbFiles = 0;

      if (hFind != INVALID_HANDLE_VALUE)
        nbFiles = 1;

      while (FindNextFile (hFind, &findData)) {
        ++nbFiles;
      }

      loader->numberOfFiles(nbFiles);
      hFind = FindFirstFile ("*.dll", &findData);
    }

    BOOL success = hFind != INVALID_HANDLE_VALUE;

    while (success) {
      std::string currentPluginLibrary = pluginPath +"/"+ findData.cFileName;
      std::string lib(findData.cFileName);
      // looking for a suffix matching -A.B.C.dll
      size_t idx = lib.rfind('.');

      if (idx != std::string::npos) {
        if (idx == (lib.find(tulip_mm_version) + tulip_version.size())) {
          if (loader)
            loader->loading(findData.cFileName);

          loadPluginLibrary(currentPluginLibrary, loader);
        }
        else if (loader)
          loader->aborted(currentPluginLibrary, currentPluginLibrary + " is not compatible with Tulip "
                          + TULIP_VERSION);
      }
      else if (loader)
        loader->aborted(currentPluginLibrary, currentPluginLibrary + " is not a Tulip plugin library");

      success = FindNextFile (hFind, &findData);
    }

    SetCurrentDirectory(currentDirectory);
  }

#else

  struct dirent **namelist;
  int n = scandir((const char *) pluginPath.c_str(),
                  &namelist,
#if !(defined(__APPLE__) || defined(__FreeBSD__)) || (defined(__APPLE__) && __MAC_OS_X_VERSION_MIN_REQUIRED >= 1080)
                  (int (*) (const dirent *))
#endif
                  __tulip_select_libs,
                  alphasort);

  if (loader!=NULL)
    loader->numberOfFiles(n);

  if (n < 0) {
    message += pluginPath + " - " + std::string(strerror(errno));
    return false;
  }

  while (n > 0) {
    n--;
    std::string lib(namelist[n]->d_name);
    free(namelist[n]);

    if (n == 0)
      free(namelist);

    std::string currentPluginLibrary = pluginPath +"/"+ lib;
    // looking for a suffix matching -A.B.C.(so/dylib)
    unsigned long idx = lib.rfind('.');

    if (idx != std::string::npos) {
      if (idx == (lib.find(tulip_mm_version) + tulip_version.size())) {
        if (loader!=NULL)
          loader->loading(lib);

        loadPluginLibrary(currentPluginLibrary, loader);
        continue;
      }

      std::string suffix = lib.substr(idx + 1);
      idx = suffix.find('.');

      if (idx != std::string::npos) {
        bool isNumber = true;

        for (unsigned long i = 0; i < idx; ++i) {
          if (!isdigit(suffix[i])) {
            isNumber = false;
            break;
          }
        }

        if (isNumber && suffix.size() > idx + 1) {
          suffix = suffix.substr(idx + 1);
          idx = suffix.find(STRINGIFY(VERSION_SEPARATOR));

          if (idx != std::string::npos) {
            for (unsigned long i = 0; i < idx; ++i) {
              if (!isdigit(suffix[i])) {
                isNumber = false;
                break;
              }
            }

            if (isNumber && loader) {
              loader->aborted(currentPluginLibrary,  currentPluginLibrary + " is not compatible with Tulip " + TULIP_VERSION);
              return n > 0;
            }
          }
        }
      }
    }

    if (loader)
      loader->aborted(currentPluginLibrary, currentPluginLibrary + " is not a Tulip plugin library");
  }

#endif
  return true;
}
#endif //EMSCRIPTEN
