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

#ifndef FACEBOOKIMPORT_H
#define FACEBOOKIMPORT_H

#include <tulip/ImportModule.h>
#include <tulip/TulipPluginHeaders.h>

#include <QObject>
#include <QUrl>
#include <QString>
#include <QWidget>

namespace Ui {
class FacebookConnectWidget;
}

class FacebookConnectWidget : public QWidget {

  Q_OBJECT

public :

  FacebookConnectWidget(QWidget *parent=0);

  Ui::FacebookConnectWidget *_ui;

  std::string getAvatarsDlPath() const;

public slots:

  void pickDirectory();

  void activateDownloadPath(bool activate);

};

class FacebookImport : public QObject, public tlp::ImportModule {

  Q_OBJECT

public :

  PLUGININFORMATION("Facebook Import","Antoine LAMBERT","19/01/2013","Import a social network from a Facebook account.","1.0","Social network")

  FacebookImport(const tlp::PluginContext*  context);

  ~FacebookImport() {}

  std::string icon() const;

  // import plugins must implement bool importGraph()
  bool importGraph();

public slots:

  void urlChanged(const QUrl & url);

signals:

  void authentificationDone();

private:

  QString accessToken;

};

#endif // FACEBOOKIMPORT_H
