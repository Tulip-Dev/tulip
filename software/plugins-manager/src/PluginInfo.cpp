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
#include "tulip/TulipRelease.h"
#include "PluginInfo.h"

#include <QtCore/QDir>

using namespace std;

const QString TulipSimpleVersion=QString(TULIP_MM_RELEASE);

//====================================================
const string tlp::PluginInfo::localTulipDirectory=(QDir::homePath()+"/.Tulip-"+TulipSimpleVersion).toStdString();
const string tlp::PluginInfo::pluginsDirName=localTulipDirectory+"/plugins/";

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
