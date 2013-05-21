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

#include <cstring>
#include <string>
#include <sstream>
#include <locale.h>
#include <errno.h>
#include <sys/stat.h>

#ifndef _WIN32
#include <sys/types.h>
#include <unistd.h>
#endif

#ifdef _WIN32
#include <windows.h>
#ifdef _MSC_VER
#include <dbghelp.h>
#endif
#else
#include <dirent.h>
#include <dlfcn.h>
#endif

#include <gzstream.h>
#include <tulip/TulipException.h>
#include <tulip/TlpTools.h>
#include <tulip/PluginLoader.h>
#include <tulip/PropertyTypes.h>
#include <tulip/TulipRelease.h>

using namespace std;
using namespace tlp;

#ifndef _TULIP_LIB_DIR
#ifdef _WIN32
#define _TULIP_LIB_DIR "c:\\Tulip\\lib\\"
#else
#define _TULIP_LIB_DIR "/usr/local/lib/"
#endif
#endif

static const char *TULIP_PLUGINS_PATH_VARIABLE="TLP_PLUGINS_PATH";

string tlp::TulipLibDir;
string tlp::TulipPluginsPath;
string tlp::TulipDocProfile;
string tlp::TulipUserHandBookIndex;
string tlp::TulipBitmapDir;
string tlp::TulipShareDir;
#ifdef _WIN32
const char tlp::PATH_DELIMITER = ';';
#else
const char tlp::PATH_DELIMITER = ':';
#endif

#ifndef EMSCRIPTEN
// A function that retrieves the Tulip libraries directory based on
// the path of the loaded shared library libtulip-core-X.Y.[dll, so, dylib]
extern "C" {

  std::string getTulipLibDir() {
    std::string tulipLibDir;
    std::string libTulipName;

#ifdef _WIN32
#ifdef __MINGW32__
    libTulipName = "libtulip-core-" + getMajor(TULIP_RELEASE) + "." + getMinor(TULIP_RELEASE) + ".dll";
#else
    libTulipName = "tulip-core-" + getMajor(TULIP_RELEASE) + "_" + getMinor(TULIP_RELEASE) + ".dll";
#endif
    HMODULE hmod = GetModuleHandle(libTulipName.c_str());

    if (hmod != NULL) {
      TCHAR szPath[512 + 1];
      DWORD dwLen = GetModuleFileName(hmod, szPath, 512);

      if (dwLen > 0) {
        std::string tmp = szPath;
        std::replace(tmp.begin(), tmp.end(), '\\', '/');
        tulipLibDir = tmp.substr(0, tmp.rfind('/')+1) + "../lib";
      }
    }

#else
#ifdef __APPLE__
    libTulipName = "libtulip-core-" + getMajor(TULIP_RELEASE) + "." + getMinor(TULIP_RELEASE) + ".dylib";
#else
    libTulipName = "libtulip-core-" + getMajor(TULIP_RELEASE) + "." + getMinor(TULIP_RELEASE) + ".so";
#endif
    void *ptr;
    Dl_info info;

    ptr = dlopen(libTulipName.c_str(), RTLD_LAZY);

    if (ptr != NULL) {
      void* symbol = dlsym(ptr, "getTulipLibDir");

      if (symbol != NULL) {
        if (dladdr(symbol, &info)) {
          std::string tmp = info.dli_fname;
          tulipLibDir = tmp.substr(0, tmp.rfind('/')+1) + "../lib";
        }
      }
    }

#endif
    return tulipLibDir;
  }

}

// throw an exception if an expected directory does not exist
static void checkDirectory(const std::string& dir) {
  struct stat infoEntry;

  if (stat(dir.c_str(),&infoEntry) != 0) {
    std::stringstream ess;
    ess << "Error - " << dir << ": " << std::endl << strerror(errno);
    ess << std::endl << "Check your TLP_DIR environment variable";

    throw TulipException(ess.str());
  }
}

//=========================================================
void tlp::initTulipLib(const char* appDirPath) {
  // first we must ensure that the parsing of float or double
  // doest not depend on locale
  setlocale(LC_NUMERIC, "C");

  char *getEnvTlp;
  string::size_type pos;

  getEnvTlp=getenv("TLP_DIR");

  if (getEnvTlp==0) {
    if (appDirPath) {
#ifdef _WIN32
      TulipLibDir = std::string(appDirPath) + "/../lib";
#else
      // one dir up to initialize the lib dir
      TulipLibDir.append(appDirPath,
                         strlen(appDirPath) -
                         strlen(strrchr(appDirPath, '/') + 1));
#ifdef I64
      // check for lib64
      string tlpPath64 = TulipLibDir + "lib64/tulip";
      struct stat statInfo;

      if (stat(tlpPath64.c_str(), &statInfo) == 0)
        TulipLibDir.append("lib64");
      else
#endif
        TulipLibDir.append("lib");

#endif
    }
    else {
      // if no appDirPath is provided, retrieve dynamically the Tulip lib dir
      TulipLibDir = getTulipLibDir();

      // if no results (should not happen with a clean Tulip install), fall back in the default value provided during compilation
      if (TulipLibDir.empty())
        TulipLibDir = string(_TULIP_LIB_DIR);
    }
  }
  else
    TulipLibDir=string(getEnvTlp);

#ifdef _WIN32
  // ensure it is a unix-style path
  pos = TulipLibDir.find('\\', 0);

  while(pos != string::npos) {
    TulipLibDir[pos] = '/';
    pos = TulipLibDir.find('\\', pos);
  }

#endif

  // ensure it is '/' terminated
  if (TulipLibDir[TulipLibDir.length() - 1] != '/')
    TulipLibDir+='/';

  // check TulipLibDir exists
  bool tlpDirSet = (getEnvTlp!=NULL);

  if (tlpDirSet)
    checkDirectory(TulipLibDir);

  getEnvTlp=getenv(TULIP_PLUGINS_PATH_VARIABLE);

  if (getEnvTlp!=NULL) {
    TulipPluginsPath=string(getEnvTlp);
#ifdef _WIN32
    // ensure it is a unix-style path
    pos = TulipPluginsPath.find('\\', 0);

    while(pos != string::npos) {
      TulipPluginsPath[pos] = '/';
      pos = TulipPluginsPath.find('\\', pos);
    }

#endif
    TulipPluginsPath= TulipLibDir + "tulip" + PATH_DELIMITER + TulipPluginsPath;
  }
  else
    TulipPluginsPath= TulipLibDir + "tulip";


  // one dir up to initialize the share dir
  pos = TulipLibDir.length() - 2;
  pos = TulipLibDir.rfind("/", pos);
  TulipShareDir=TulipLibDir.substr(0, pos + 1)+"share/tulip/";

  // check it exists
  if (tlpDirSet)
    checkDirectory(TulipShareDir);

  TulipDocProfile=TulipShareDir+"tulip.qhc";
  TulipUserHandBookIndex=TulipShareDir+"userHandbook/html/index.html";

  TulipBitmapDir=TulipShareDir+"bitmaps/";

  // check it exists
  if (tlpDirSet)
    checkDirectory(TulipBitmapDir);

  // initialize serializers
  initTypeSerializers();
}
//=========================================================

// tlp class names demangler
#if defined(__GNUC__)
#include <cxxabi.h>
std::string tlp::demangleClassName(const char* className,
				   bool hideTlp) {
  static char demangleBuffer[256];
  int status;
  size_t length = 256;
  abi::__cxa_demangle((char *) className, (char *) demangleBuffer,
                      &length, &status);
  // skip tlp::
  if (hideTlp && strstr(demangleBuffer, "tlp::") == demangleBuffer)
    return std::string(demangleBuffer + 5);
  return std::string(demangleBuffer);
}
#elif defined(_MSC_VER)
// With Visual Studio, typeid(tlp::T).name() does not return a mangled type name
// but a human readable type name in the form "class tlp::T"
// so just remove the first 11 characters to return T
std::string tlp::demangleClassName(const char* className,
				   bool hideTlp) {
  char* clName = const_cast<char*>(className);
  if (strstr(className, "class ") == className)
    clName += 6;
  if (hideTlp && strstr(clName, "tlp::") == clName)
    return std::string(clName + 5);
  return std::string(clName);
}
#else
#error define symbols demangling function
#endif
#endif // EMSCRIPTEN

//=========================================================
std::istream *tlp::getIgzstream(const char *name, int open_mode) {
  return new igzstream(name, open_mode);
}

std::ostream *tlp::getOgzstream(const char *name, int open_mode) {
  return new ogzstream(name, open_mode);
}

