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
#include <QtCore/QTimer>
#include <QtCore/QLocale>
#include <QtGui/QDesktopWidget>

#include <tulip/PluginLister.h>

#include <PluginInfo.h>
#include "TulipApp.h"

using namespace std;
int main( int argc, char **argv ) {
  QApplication tulip(argc, argv);
  QLocale::setDefault(QLocale(QLocale::English));

  #if defined(__APPLE__)
  // allows to load qt imageformats plugin
  QApplication::addLibraryPath(QApplication::applicationDirPath() + "/..");
  #endif
  tlp::initTulipLib((char *) QApplication::applicationDirPath().toAscii().data());
  
  //add local plugins installation path. This is an application behavior, not a library one, and should stay here.
  tlp::TulipPluginsPath = tlp::PluginInfo::pluginsDirName + tlp::PATH_DELIMITER + tlp::TulipPluginsPath;
  
  TulipApp *mainWindow = new TulipApp();
  QRect screenRect = tulip.desktop()->availableGeometry();
  if (screenRect.height() > 890) {
    QRect wRect = mainWindow->geometry();
    int delta = (870 - wRect.height())/2;
    wRect.setTop(wRect.top() - delta);
    wRect.setBottom(wRect.bottom() + delta);
    mainWindow->setGeometry(wRect);
  }
  mainWindow->startTulip();

  return tulip.exec();
}
