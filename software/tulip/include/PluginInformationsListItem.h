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
#include <tulip/PluginManager.h>

class QNetworkReply;
namespace Ui {
class PluginInformationsListItemData;
}

class PluginInformationsListItem: public QWidget {
  Q_OBJECT
  Ui::PluginInformationsListItemData *_ui;

  tlp::PluginInformations _infos;

public:
  explicit PluginInformationsListItem(tlp::PluginInformations, QWidget *parent = NULL);
  virtual ~PluginInformationsListItem();

public slots:
  void focusOut();
  void focusIn();
  void install();

signals:
  void focused();

protected slots:
  void downloadProgress(qint64,qint64);

protected:
  void focusInEvent(QFocusEvent *);
};

#endif // PLUGININFORMATIONSLISTITEM_H
