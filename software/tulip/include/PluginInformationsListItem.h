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
#ifndef PLUGININFORMATIONSLISTITEM_H
#define PLUGININFORMATIONSLISTITEM_H

#include <QtGui/QWidget>

class QNetworkReply;
namespace Ui {
class PluginInformationsListItemData;
}
namespace tlp {
class PluginInformations;
}

class PluginInformationsListItem: public QWidget {
  Q_OBJECT

public:
  explicit PluginInformationsListItem(tlp::PluginInformations *, QWidget *parent = NULL);
  tlp::PluginInformations *pluginInformations() const {
    return _pluginInformations;
  }

protected:
  void focusInEvent(QFocusEvent *);

signals:
  void gotFocus();
  void showDetailedInformations();
  void fetch();
  void remove();

public slots:
  void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
  void downloadFinished();
  void expand();
  void collapse();
  void markedForRemoval();
  void DownloadStarted(QNetworkReply*);

private:
  Ui::PluginInformationsListItemData *_ui;
  tlp::PluginInformations *_pluginInformations;

  QString backgroundCode();
};

#endif // PLUGININFORMATIONSLISTITEM_H
