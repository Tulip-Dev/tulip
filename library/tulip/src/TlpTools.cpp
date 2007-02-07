#include <dirent.h>
#include <string.h>

#include "thirdparty/gzstream/gzstream.h"

#include "tulip/TlpTools.h"
#include "tulip/LayoutProperty.h"
#include "tulip/DoubleProperty.h"
#include "tulip/StringProperty.h"
#include "tulip/BooleanProperty.h"
#include "tulip/ColorProperty.h"
#include "tulip/IntegerProperty.h"
#include "tulip/SizeProperty.h"
#include "tulip/GraphProperty.h"
#include "tulip/ExportModule.h"
#include "tulip/Algorithm.h"
#include "tulip/ImportModule.h"
#include "tulip/PluginsCreation.h"

using namespace std;
using namespace tlp;

#ifndef _TULIP_LIB_DIR
#ifdef _WIN32
#define _TULIP_LIB_DIR "c:\\Program Files\\Tulip\\lib\\"
#else
#define _TULIP_LIB_DIR "/usr/local/lib/"
#endif
#endif

static const char *TULIP_PLUGINS_PATH_VARIABLE="TLP_PLUGINS_PATH";

string tlp::TulipLibDir;
string tlp::TulipPluginsPath;
string tlp::TulipDocProfile;
string tlp::TulipUserHandBookIndex;
#ifdef _WIN32
const char tlp::PATH_DELIMITER = ';';
#else
const char tlp::PATH_DELIMITER = ':';
#endif
//=========================================================
void tlp::initTulipLib(char* appDirPath) {
  char *getEnvTlp;
  std::string tulipDocDir;
  string::size_type pos;

  getEnvTlp=getenv("TLP_DIR");
  if (getEnvTlp==0) {
    if (appDirPath) {
      // one dir up to initialize the lib dir
      char *last = strrchr(appDirPath, '/');
      last[1] = 0;
      TulipLibDir = std::string(appDirPath) + "lib";
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
    TulipPluginsPath= TulipLibDir + "tlp/plugins" + PATH_DELIMITER + TulipPluginsPath;
  } else
    TulipPluginsPath= TulipLibDir + "tlp/plugins";
    

  // one dir up to initialize the doc dir
  pos = TulipLibDir.length() - 2;
  pos = TulipLibDir.rfind("/", pos);
  tulipDocDir=TulipLibDir.substr(0, pos + 1)+"share/tulip/";
  TulipDocProfile=tulipDocDir+"profile.adp";
  TulipUserHandBookIndex=tulipDocDir+"userHandbook/html/index.html";
}
//=========================================================
istream *tlp::getIgzstream(const char *name, int open_mode) {
  return new igzstream(name, open_mode);
}

ostream *tlp::getOgzstream(const char *name, int open_mode) {
  return new ogzstream(name, open_mode);
}
//=========================================================
map<string, TemplateFactoryInterface* > *TemplateFactoryInterface::allFactories = 0;
TemplateFactory<AlgorithmFactory, Algorithm,AlgorithmContext > *AlgorithmFactory::factory = 0;
TemplateFactory<ImportModuleFactory,ImportModule,AlgorithmContext > *ImportModuleFactory::factory = 0;
TemplateFactory<ExportModuleFactory,ExportModule,AlgorithmContext > *ExportModuleFactory::factory = 0;
// initialize and export the factories needed to manage
// our different kinds of plugins
#if !defined( __APPLE__) || __GNUC__ > 3
template <class Tnode, class Tedge, class TPROPERTY>
  TemplateFactory<PropertyFactory<TPROPERTY >, TPROPERTY, PropertyContext > *AbstractProperty<Tnode,Tedge,TPROPERTY>::factory = 0;
 #else
TemplateFactory<PropertyFactory<ColorAlgorithm>, ColorAlgorithm, PropertyContext> *AbstractProperty<ColorType, ColorType, ColorAlgorithm>::factory = 0;
TemplateFactory<PropertyFactory<IntegerAlgorithm>, IntegerAlgorithm, PropertyContext> *AbstractProperty<IntegerType, IntegerType, IntegerAlgorithm>::factory = 0;
TemplateFactory<PropertyFactory<LayoutAlgorithm>, LayoutAlgorithm, PropertyContext> *AbstractProperty<PointType, LineType, LayoutAlgorithm>::factory = 0;
TemplateFactory<PropertyFactory<DoubleAlgorithm>, DoubleAlgorithm, PropertyContext> *AbstractProperty<DoubleType, DoubleType, DoubleAlgorithm>::factory = 0;
TemplateFactory<PropertyFactory<BooleanAlgorithm>, BooleanAlgorithm, PropertyContext> *AbstractProperty<BooleanType, BooleanType, BooleanAlgorithm>::factory = 0;
TemplateFactory<PropertyFactory<SizeAlgorithm>, SizeAlgorithm, PropertyContext> *AbstractProperty<SizeType,SizeType, SizeAlgorithm>::factory = 0;
TemplateFactory<PropertyFactory<StringAlgorithm>, StringAlgorithm, PropertyContext> *AbstractProperty<StringType, StringType, StringAlgorithm>::factory = 0;
#endif
//=========================================================
static void loadPluginsFromDir(std::string dir, PluginLoader *plug) {
  SizeProperty::initFactory();
  SizeProperty::factory->loadPluginsFromDir(dir + "sizes", "Size",plug);
  IntegerProperty::initFactory();
  IntegerProperty::factory->loadPluginsFromDir(dir + "int", "Integer",plug);
  LayoutProperty::initFactory();
  LayoutProperty::factory->loadPluginsFromDir(dir + "layout" , "Layout",plug);
  ColorProperty::initFactory();
  ColorProperty::factory->loadPluginsFromDir(dir + "colors" , "Color",plug);
  DoubleProperty::initFactory();
  DoubleProperty::factory->loadPluginsFromDir(dir + "metric" , "Double",plug);
  StringProperty::initFactory();
  StringProperty::factory->loadPluginsFromDir(dir + "string" , "String",plug);
  BooleanProperty::initFactory();
  BooleanProperty::factory->loadPluginsFromDir(dir + "selection" , "Boolean",plug);
  AlgorithmFactory::initFactory();
  AlgorithmFactory::factory->loadPluginsFromDir(dir + "clustering" , "Cluster",plug);
  ImportModuleFactory::initFactory();
  ImportModuleFactory::factory->loadPluginsFromDir(dir + "import" , "Import Module",plug);
  ExportModuleFactory::initFactory();
  ExportModuleFactory::factory->loadPluginsFromDir(dir + "export" , "Export Module",plug);
  // plugins dependencies loop
  bool depsNeedCheck;
  do {
    map<string, TemplateFactoryInterface *>::const_iterator it =
      TemplateFactoryInterface::allFactories->begin();
    depsNeedCheck = false;
    // loop over factories
    for (; it != TemplateFactoryInterface::allFactories->end(); ++it) {
      TemplateFactoryInterface *tfi = (*it).second;
      // loop over plugins
      Iterator<string> *itP = tfi->availablePlugins();
      while(itP->hasNext()) {
	string pluginName = itP->next();
	list<Dependency> dependencies = tfi->getPluginDependencies(pluginName);
	list<Dependency>::const_iterator itD = dependencies.begin();
	// loop over dependencies
	for (; itD != dependencies.end(); ++itD) {
	  string factoryDepName = (*itD).factoryName;
	  string pluginDepName = (*itD).pluginName;
	  bool removed = false;
	  if (!TemplateFactoryInterface::pluginExists(factoryDepName, pluginDepName)) {
	    if (plug)
	      plug->aborted(pluginName, tfi->getPluginsClassName() +
			    " '" + pluginName + "' will be removed, it depends on missing " +
			    factoryDepName + " '" + pluginDepName + "'");
	    tfi->removePlugin(pluginName);
	    depsNeedCheck = true;
	    removed = true;
	  }
	  char** pluginDepParams = (*itD).pluginParams;
	  if (!removed && pluginDepParams) {
	    StructDef params = 
	      (*TemplateFactoryInterface::allFactories)[factoryDepName]->getPluginParameters(pluginDepName);
	    for (unsigned int i = 0; pluginDepParams[i]; ++i) {
	      if (!params.hasField(pluginDepParams[i])) {
		if (plug)
		  plug->aborted(pluginName, tfi->getPluginsClassName() +
				" '" + pluginName + "' will be removed, it depends on the undefined parameter '" +
				pluginDepParams[i] + "' of " + factoryDepName + " '" + pluginDepName + "'");
		tfi->removePlugin(pluginName);
		depsNeedCheck = true;
	      }
	    }
	  }
	}
      } delete itP;
    }
  } while (depsNeedCheck);
}
//=========================================================
void tlp::loadPlugins(PluginLoader *plug) {
  string::const_iterator begin=TulipPluginsPath.begin();
  string::const_iterator end=begin;
  while (end!=TulipPluginsPath.end())
    if ((*end)==PATH_DELIMITER) {
      if (begin!=end) 
	loadPluginsFromDir(string(begin,end)+'/', plug);
      ++end;
      begin=end;
    } else
      ++end;
  if (begin!=end) 
    loadPluginsFromDir(string(begin,end)+'/', plug);
}
//=========================================================
bool tlp::loadPlugin(const std::string & filename, PluginLoader *plug) {
    return PluginLibraryLoader::loadPluginLibrary(filename, plug);
}

//=========================================================
// tlp class names demangler
#if defined(__GNUC__)
#include <cxxabi.h>
char *tlp::demangleTlpClassName(const char* className) {
  static char demangleBuffer[256];
  int status;
  size_t length = 256;
  abi::__cxa_demangle((char *) className, (char *) demangleBuffer,
		      &length, &status);
  // skip tlp::
  return demangleBuffer + 5;
}
#else
#error define symbols demangling function
#endif
