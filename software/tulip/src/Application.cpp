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
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "Application.h"

#include <PluginInfo.h>
#include <tulip/TlpTools.h>

//**********************************************************************
Application::Application(int& argc, char ** argv): QApplication(argc,argv)
{
#if defined(__APPLE__)
  // allows to load qt imageformats plugin
  QApplication::addLibraryPath(QApplication::applicationDirPath() + "/..");
#endif
  tlp::initTulipLib((char *) QApplication::applicationDirPath().toAscii().data());

  //add local plugins installation path
  tlp::TulipPluginsPath = tlp::PluginInfo::pluginsDirName + tlp::PATH_DELIMITER + tlp::TulipPluginsPath;
}

//**********************************************************************
Application::~Application() {
}
//**********************************************************************










