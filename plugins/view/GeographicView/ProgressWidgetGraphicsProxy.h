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

#ifndef PROGRESSWIDGETGRAPHICSPROXY_H_
#define PROGRESSWIDGETGRAPHICSPROXY_H_

#include <QGraphicsProxyWidget>
#include <QWidget>
#include <QColor>

class QPainter;
class QStyleOptionGraphicsItem;

namespace Ui {
class ProgressWidgetData;
}

namespace tlp {

class ProgressWidget : public QWidget {

  Q_OBJECT

  Ui::ProgressWidgetData* _ui;

public :

  ProgressWidget(QWidget *parent = 0);
  ~ProgressWidget();

  void setComment(const QString &comment);

  void setProgress(int value, int max);

  bool cancelRequested() const {
    return cancelClicked;
  }

protected :

  void showEvent(QShowEvent * event);

private slots :

  void cancelButtonClicked();

private :

  bool cancelClicked;

};

class ProgressWidgetGraphicsProxy : public QGraphicsProxyWidget {

public :

  ProgressWidgetGraphicsProxy();

  void setComment(const QString &comment);

  void setProgress(int value, int max);

  inline void setFrameColor(const QColor &frameColor) {
    this->frameColor = frameColor;
  }

  bool cancelRequested() const;

protected :

  void paintWindowFrame (QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);

private :

  ProgressWidget *progressWidget;
  QColor frameColor;
};
}
#endif /* PROGRESSWIDGETGRAPHICSPROXY_H_ */
