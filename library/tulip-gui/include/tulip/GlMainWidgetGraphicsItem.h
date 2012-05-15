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
#ifndef GLMAINWIDGETGRAPHICSITEM_H
#define GLMAINWIDGETGRAPHICSITEM_H

/**
 Author: Antoine Lambert, Morgan Mathiaut and Ludwig Fiolka
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.
 */

#include <QtGui/QGraphicsObject>

namespace tlp {
class GlMainWidget;
}

namespace tlp {
class GlMainWidgetGraphicsItem : public QGraphicsObject {
  Q_OBJECT
public:
  GlMainWidgetGraphicsItem(tlp::GlMainWidget *glMainWidget, int width, int height);
  virtual ~GlMainWidgetGraphicsItem();

  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

  QRectF boundingRect() const;

  void resize(int width, int height);

  void setRedrawNeeded(bool redrawNeeded) {
    this->redrawNeeded=redrawNeeded;
  }

  tlp::GlMainWidget *getGlMainWidget() {
    return glMainWidget;
  }

  bool eventFilter(QObject *, QEvent *evt);

signals:

  void widgetPainted(bool redraw);

protected :
  void wheelEvent(QGraphicsSceneWheelEvent *event);
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
  void mousePressEvent(QGraphicsSceneMouseEvent *event);
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
  void hoverMoveEvent(QGraphicsSceneHoverEvent * event);
  void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
  void contextMenuEvent(QGraphicsSceneContextMenuEvent * event);

protected slots:
  void glMainWidgetDraw(GlMainWidget *,bool);
  void glMainWidgetRedraw(GlMainWidget *);

private :
  tlp::GlMainWidget *glMainWidget;
  bool redrawNeeded;
  int width, height;
  unsigned char *renderingStore;
};

}

#endif // GLMAINWIDGETGRAPHICSITEM_H
