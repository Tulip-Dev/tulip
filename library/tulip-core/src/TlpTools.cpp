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

#include <string>
#include <locale.h>

#ifndef _WIN32
#include <sys/stat.h>
#include <unistd.h>
#endif

#include <thirdparty/gzstream/gzstream.h>
#include <tulip/TlpTools.h>
#include <tulip/PluginLoader.h>
#include <tulip/PropertyTypes.h>

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
    else
      TulipLibDir=string(_TULIP_LIB_DIR);
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
  TulipDocProfile=TulipShareDir+"tulip.qhc";
  TulipUserHandBookIndex=TulipShareDir+"userHandbook/html/index.html";

  TulipBitmapDir=TulipShareDir+"bitmaps/";

  // initialize serializers
  initTypeSerializers();
}
//=========================================================

// tlp class names demangler
#if defined(__GNUC__)
#include <cxxabi.h>
std::string tlp::demangleTlpClassName(const char* className) {
  static char demangleBuffer[256];
  int status;
  size_t length = 256;
  abi::__cxa_demangle((char *) className, (char *) demangleBuffer,
                      &length, &status);
  // skip tlp::
  return std::string(demangleBuffer + 5);
}
#elif defined(_MSC_VER)
// With Visual Studio, typeid(tlp::T).name() does not return a mangled type name
// but a human readable type name in the form "class tlp::T"
// so just remove the first 11 characters to return T
std::string tlp::demangleTlpClassName(const char* className) {
  return std::string(className + 11);
}
#else
#error define symbols demangling function
#endif
