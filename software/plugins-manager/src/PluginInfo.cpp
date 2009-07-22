#include "PluginInfo.h"

#include <QtCore/QDir>

using namespace std;

//====================================================
#ifdef _WIN32
#ifdef DLL_EXPORT
const string tlp::PluginInfo::pluginsDirName=(QDir::homePath()+"/Application Data/Tulip-" + QString(TULIP_RELEASE) +"/plugins/").toStdString();
#endif
#else
const string tlp::PluginInfo::pluginsDirName=(QDir::homePath()+"/.Tulip-" + QString(TULIP_RELEASE) +"/plugins/").toStdString();
#endif

namespace tlp {

  string PluginInfo::getInstallationSubDir(const string &type) {
    if(type=="Glyph")
      return "glyphs/";
    if(type=="Interactor")
      return "interactors/" ;
    if(type=="View")
      return "view/" ;
    if(type=="Controller")
      return "controller/" ;
    return "";
  }

  bool LocalPluginInfo::isInstalledInHome() {
    string pluginFilePath=pluginsDirName+getInstallationSubDir(type);
    string fileNameWithExt=fileName+
#if defined(_WIN32)
  ".dll";
#elif defined(__APPLE__)
  ".dylib";
#else
  ".so";
#endif
    QDir dir(pluginFilePath.c_str());
    return dir.exists(fileNameWithExt.c_str());
  }
}
