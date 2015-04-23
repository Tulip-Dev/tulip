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
#ifndef PERSPECTIVEITEMWIDGET_H
#define PERSPECTIVEITEMWIDGET_H

#include <QWidget>

namespace tlp {
struct PluginInformation;
}

namespace Ui {
class PerspectiveItemData;
}

class PerspectiveItemWidget : public QWidget {
  Q_OBJECT

  QString _perspectiveName;
  Ui::PerspectiveItemData *_ui;
public:
  explicit PerspectiveItemWidget(const QString& perspectiveName,QWidget *parent = 0);

public slots:
  void run();

signals:
  void selected();

protected:
  void mouseDoubleClickEvent(QMouseEvent *);
  void focusInEvent(QFocusEvent *);
  void focusOutEvent(QFocusEvent *);
};

#endif // PERSPECTIVEITEMWIDGET_H
