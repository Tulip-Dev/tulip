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
#include <string.h>
#include <set>

#ifdef _WIN32
#include <windows.h>
#include <stdio.h>
#else
#include <stdlib.h>
#include <dlfcn.h>
#include <dirent.h>
#endif

using namespace tlp;

std::string PluginLibraryLoader::currentPluginLibrary;

static std::set<std::string> previouslyLoadedLib;

static bool isPreviouslyLoaded(const std::string &lib){
  unsigned long idx = lib.rfind('-', lib.rfind('.') - 1);
  std::string libName=lib.substr(0,idx);
  if(previouslyLoadedLib.count(libName)!=0)
    return true;

  previouslyLoadedLib.insert(libName);
  return false;
}

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
  sprintf(scode, "%s: unable to load(error %d)",
    filename.c_str(), (int) dwErrCode);
  loader->aborted(filename, std::string(scode));
      } else {
  loader->aborted(filename, filename + ": " + msg);
  LocalFree(msg);
      }
    }
    return false;
  }
  return true;
}

struct IteratorInfos {
#define BUFSIZE 256
  HANDLE hFind;
  WIN32_FIND_DATA FindData;
  TCHAR currentDirectory[BUFSIZE];
};

PluginLibraryLoader::PluginLibraryLoader(std::string _pluginPath, PluginLoader *loader) {
  DWORD dwRet;
  IteratorInfos *_infos = new IteratorInfos();
  dwRet = GetCurrentDirectory(BUFSIZE, _infos ->currentDirectory);
  //printf("Plugins from: %s\n", _pluginPath.c_str());
  n = 0;
  pluginPath = _pluginPath;
  if(dwRet == 0) {
    n = -1;
    msg = std::string("Scandir error");//perror("scandir");
    delete _infos;
  } else {
    // first check is pluginPath exists
    DWORD fileAttr = GetFileAttributes(pluginPath.c_str());
    if (fileAttr == 0xFFFFFFFF) {
      msg = std::string("Directory not found: ") + pluginPath;
      n = -1;
      delete _infos;
      return;
    }
    SetCurrentDirectory (_pluginPath.c_str());
    _infos->hFind = FindFirstFile ("*.dll", &_infos->FindData);
    if (loader != 0) {
      // count files loop
      unsigned long nbFiles = 0;
      if (_infos->hFind != INVALID_HANDLE_VALUE)
	nbFiles = 1;
      while (FindNextFile (_infos->hFind, &_infos->FindData)) {
	++nbFiles;
      }
      loader->numberOfFiles(nbFiles);
      _infos->hFind = FindFirstFile ("*.dll", &_infos->FindData);
    }
    infos = (void *) _infos;
  }
}

bool PluginLibraryLoader::loadNextPluginLibrary(PluginLoader *loader) {
  bool pluginFound = false;
  IteratorInfos *_infos = (IteratorInfos *) infos;
  if (_infos->hFind != INVALID_HANDLE_VALUE) {
    //printf("\t%s\n",  _infos->FindData.cFileName);
    n++;
    currentPluginLibrary = pluginPath +"/"+ _infos->FindData.cFileName;
    std::string lib(_infos->FindData.cFileName);
    unsigned long idx = lib.rfind('-');
    if (idx != std::string::npos) {
      std::string tulip_release(TULIP_MM_RELEASE);
      if (lib.find(tulip_release, idx) == idx + 1) {
        if(!isPreviouslyLoaded(lib)){
          if (loader)
            loader->loading(_infos->FindData.cFileName);
          loadPluginLibrary(currentPluginLibrary, loader);
        }
      }
      else if (loader)
        loader->aborted(currentPluginLibrary,currentPluginLibrary + " is not compatible with Tulip "
            + TULIP_RELEASE);
    }
    else if (loader)
      loader->aborted(currentPluginLibrary, currentPluginLibrary + " is not a Tulip plugin library");
    pluginFound = FindNextFile (_infos->hFind, &_infos->FindData);
  }
  if (!pluginFound) {
    //printf("*** no more plugins ***\n");
    SetCurrentDirectory(_infos->currentDirectory);
    delete _infos;
  }
  return pluginFound;
}

#else

bool PluginLibraryLoader::loadPluginLibrary(const std::string &filename, PluginLoader *loader) {
  void *handle = dlopen (filename.c_str() , RTLD_NOW);
  if (!handle) {
    if (loader!=0)
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

PluginLibraryLoader::PluginLibraryLoader(std::string _pluginPath,
           PluginLoader *loader) {
  struct dirent **namelist;
  n = scandir((const char *) _pluginPath.c_str(),
        &namelist,
#if !(defined(__APPLE__) || defined(__FreeBSD__))
        (int (*) (const dirent *))
#endif
        __tulip_select_libs,
        alphasort);
  pluginPath = _pluginPath;
  if (loader!=0)
    loader->numberOfFiles(n);
  if (n < 0) {
    msg=std::string("Scandir error");//perror("scandir");
    return;
  }
  infos = (void *) namelist;
}

bool PluginLibraryLoader::loadNextPluginLibrary(PluginLoader *loader) {
  if (n > 0) {
    struct dirent **namelist = (struct dirent **) infos;
    n--;
    std::string lib(namelist[n]->d_name);
    free(namelist[n]);
    if (n == 0)
      free(infos);
    currentPluginLibrary = pluginPath +"/"+ lib;
    // looking for a suffix matching -A.B.C
    unsigned long idx = lib.rfind('-', lib.rfind('.') - 1);
    if (idx != std::string::npos) {
      std::string tulip_release(TULIP_RELEASE);
      tulip_release = tulip_release.substr(0, tulip_release.rfind('.') + 1);
      if (lib.find(tulip_release, idx) == idx + 1) {
        if(!isPreviouslyLoaded(lib)){
          if (loader!=0)
            loader->loading(lib);
          loadPluginLibrary(currentPluginLibrary, loader);
        }
        return n > 0;
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
              loader->aborted(currentPluginLibrary,  currentPluginLibrary + " is not compatible with Tulip " + TULIP_RELEASE);
              return n > 0;
            }
          }
        }
      }
    }
    if (loader)
      loader->aborted(currentPluginLibrary, currentPluginLibrary + " is not a Tulip plugin library");
  }
  return (n > 0); //return pluginLoaded;
}

#endif
