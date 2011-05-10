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
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

#if _MSC_VER
#include <windows.h>
#include <Dbghelp.h>
#else
#include <dirent.h>
#endif
#include <thirdparty/gzstream/gzstream.h>

#include <tulip/TlpTools.h>
#include <tulip/PropertyTypes.h>
#include <tulip/LayoutProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/StringProperty.h>
#include <tulip/BooleanProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/IntegerProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/ExportModule.h>
#include <tulip/Algorithm.h>
#include <tulip/ImportModule.h>
#include <tulip/PluginLibraryLoader.h>

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
  std::string tulipDocDir;
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
    } else
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
  if (getEnvTlp!=0) {
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
  } else
    TulipPluginsPath= TulipLibDir + "tulip";
    

  // one dir up to initialize the doc dir
  pos = TulipLibDir.length() - 2;
  pos = TulipLibDir.rfind("/", pos);
  tulipDocDir=TulipLibDir.substr(0, pos + 1)+"share/tulip/";
  TulipDocProfile=tulipDocDir+"tulip.qhc";
  TulipUserHandBookIndex=tulipDocDir+"userHandbook/html/index.html";

  TulipBitmapDir=TulipLibDir.substr(0, pos + 1)+"share/tulip/bitmaps/";

  // initialize factories
  PropertyFactory<TemplateAlgorithm<SizeProperty> >::initFactory();
  PropertyFactory<TemplateAlgorithm<IntegerProperty> >::initFactory();
  PropertyFactory<TemplateAlgorithm<LayoutProperty> >::initFactory();
  PropertyFactory<TemplateAlgorithm<ColorProperty> >::initFactory();
  PropertyFactory<TemplateAlgorithm<DoubleProperty> >::initFactory();
  PropertyFactory<TemplateAlgorithm<StringProperty> >::initFactory();
  PropertyFactory<TemplateAlgorithm<BooleanProperty> >::initFactory();
  AlgorithmFactory::initFactory();
  ImportModuleFactory::initFactory();
  ExportModuleFactory::initFactory();

  // initialize serializers
  initTypeSerializers();
}
//=========================================================
istream *tlp::getIgzstream(const char *name, int open_mode) {
  return new igzstream(name, open_mode);
}

ostream *tlp::getOgzstream(const char *name, int open_mode) {
  return new ogzstream(name, open_mode);
}
//=========================================================
map<string, PluginManagerInterface* > *PluginManagerInterface::allFactories = 0;
//==========================================================
PluginLoader *PluginManagerInterface::currentLoader = NULL;
//==========================================================
void tlp::loadPluginsFromDir(std::string dir, std::string type, PluginLoader *loader) {
  if (loader!=0)
    loader->start(dir.c_str(), type);

  tlp::PluginLibraryLoader plLoader(dir, loader);

  PluginManagerInterface::currentLoader = loader;
  if (plLoader.hasPluginLibraryToLoad()) {
    while(plLoader.loadNextPluginLibrary(loader)) {
    }
    if (loader)
      loader->finished(true, plLoader.msg);
  } else {
    if (loader)
      loader->finished(false, plLoader.msg);
  }
}  

//=========================================================
void tlp::loadPlugins(PluginLoader *plug, std::string folder) {
  vector<string> paths;
  stringstream ss(TulipPluginsPath);
  string item;
  while(getline(ss, item, PATH_DELIMITER)) {
    paths.push_back(item);
  }
  
  for(vector<string>::const_iterator it = paths.begin(); it != paths.end(); ++it) {
    loadPluginsFromDir((*it) + "/" + folder, "Algorithm", plug);
  }
}
//=========================================================
bool tlp::loadPlugin(const std::string & filename, PluginLoader *plug) {
  PluginManagerInterface::currentLoader = plug;
  if (plug)
    plug->loading(filename);
  return PluginLibraryLoader::loadPluginLibrary(filename, plug);
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
#elif _MSC_VER

std::string tlp::demangleTlpClassName(const char* className) {
	std::string result;
	static char demangleBuffer[256];
	UnDecorateSymbolName(className, demangleBuffer, 256, UNDNAME_32_BIT_DECODE);
	result = std::string(demangleBuffer + 5);
	return result;
}
#else
#error define symbols demangling function
#endif

//=========================================================
std::string tlp::getMajor(const std::string& v) {
  unsigned int pos = v.find('.');
  return v.substr(0, pos);
}

//=========================================================
std::string tlp::getMinor(const std::string& v) {
  size_t pos = v.find('.');
  if (pos == string::npos)
    return string("0");
  unsigned int rpos = v.rfind('.');
  if (pos == rpos)
    return v.substr(pos+1);
  return v.substr(pos + 1, rpos - pos - 1);
}
//=========================================================
const std::string& tlp::getCurrentPluginFileName() {
  return PluginLibraryLoader::getCurrentPluginFileName();
}
