/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
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
#include <iostream>

#include <stdlib.h>

#include <QtGui/QApplication>
#include <QtCore/QDir>
#include <QtCore/QProcess>
#include <QtCore/QSettings>

using namespace std;

int main( int argc, char **argv ) {
  QApplication app(argc,argv);

  QDir appDir(QApplication::applicationDirPath());
  QProcess proc;
#if defined(_WIN32)
  QString program = appDir.absoluteFilePath("tulip_app.exe");
#else
  QString program = appDir.absoluteFilePath("tulip_app");
#endif
  QStringList arguments;

  proc.setReadChannelMode(QProcess::ForwardedChannels);
  bool needStart=true;
  while(needStart){
    proc.start(program,arguments);
    proc.waitForFinished(-1);
    QSettings settings("TulipSoftware","Tulip");
    settings.beginGroup("PluginsManager");
    needStart=settings.value("needRestart",false).toBool();
    settings.endGroup();
  }

  return 0;
}
