/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
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

#ifndef TULIPSPLASHSCREEN_H
#define TULIPSPLASHSCREEN_H

#include <QSplashScreen>
#include <tulip/PluginLoader.h>
#include <QMap>

/**
  @brief a splash screen used when loading plugins.
  Loading plugins in the tulip_app process allows to check for plugins error before loading any perspective. It provides the user with some basic preemptive informations
  and forbid the use of invalid perspectives.
  */
class TulipSplashScreen: public tlp::PluginLoader, public QSplashScreen {
public:
  TulipSplashScreen();
  virtual ~TulipSplashScreen() {}

  virtual void start(const std::string &path);
  virtual void loading(const std::string &filename);
  virtual void loaded(const tlp::Plugin* infos, const std::list <tlp::Dependency>& deps);
  virtual void numberOfFiles(int n) {
    _numberOfFiles = n;
    _fileCounter = 0;
  }
  virtual void aborted(const std::string &filename,const  std::string &erreurmsg);
  virtual void finished(bool state,const std::string &msg);

protected:
  virtual void drawContents(QPainter *painter);

private:
  QString _title;
  QString _message;

  int _fileCounter;
  int _numberOfFiles;
};

#endif // TULIPSPLASHSCREEN_H
