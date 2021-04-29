/**
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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
#include <QMap>

#include <tulip/PluginLoader.h>

/**
  @brief a splash screen used when loading plugins.
  Loading plugins in the tulip_app process allows to check for plugins error before loading any
  perspective. It provides the user with some basic preemptive information
  and forbid the use of invalid perspectives.
  */
class TulipSplashScreen : public tlp::PluginLoader, public QSplashScreen {
public:
  TulipSplashScreen();

  void start(const std::string &path) override;
  void loading(const std::string &filename) override;
  void loaded(const tlp::Plugin *info, const std::list<tlp::Dependency> &deps) override;
  void numberOfFiles(int n) override {
    _numberOfFiles = n;
    _fileCounter = 0;
  }
  void aborted(const std::string &filename, const std::string &erreurmsg) override;
  void finished(bool state, const std::string &msg) override;

  const QMap<QString, QString> &errors() {
    return _errors;
  }

protected:
  void drawContents(QPainter *painter) override;

private:
  QString _title;
  QString _message;

  int _fileCounter;
  int _numberOfFiles;

  QMap<QString, QString> _errors;
};

#endif // TULIPSPLASHSCREEN_H
