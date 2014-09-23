/*
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
///@cond DOXYGEN_HIDDEN

#ifndef GLMAINWIDGETGRAPHICSITEM_H
#define GLMAINWIDGETGRAPHICSITEM_H

#include <QGraphicsObject>


#include <tulip/tulipconf.h>

namespace tlp {
class GlMainWidget;

class TLP_QT_SCOPE GlMainWidgetGraphicsItem : public QGraphicsObject {
  Q_OBJECT
public:
  GlMainWidgetGraphicsItem(tlp::GlMainWidget *glMainWidget, int width, int height);
  virtual ~GlMainWidgetGraphicsItem();

  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

  QRectF boundingRect() const;

  void resize(int width, int height);

  void setRedrawNeeded(bool redrawNeeded) {
    this->_redrawNeeded=redrawNeeded;
  }

  tlp::GlMainWidget *getGlMainWidget() {
    return glMainWidget;
  }

  bool eventFilter(QObject *, QEvent *evt);

signals:

  void widgetPainted(bool redraw);

protected :
  void keyPressEvent(QKeyEvent *event);
  void keyReleaseEvent(QKeyEvent *event);
  void wheelEvent(QGraphicsSceneWheelEvent *event);
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
  void mousePressEvent(QGraphicsSceneMouseEvent *event);
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
  void hoverMoveEvent(QGraphicsSceneHoverEvent * event);
  void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
  void contextMenuEvent(QGraphicsSceneContextMenuEvent * event);
  void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
  void dragLeaveEvent(QGraphicsSceneDragDropEvent *event);
  void dragMoveEvent(QGraphicsSceneDragDropEvent *event);
  void dropEvent(QGraphicsSceneDragDropEvent *event);

protected slots:
  void glMainWidgetDraw(GlMainWidget *,bool);
  void glMainWidgetRedraw(GlMainWidget *);

private :
  tlp::GlMainWidget *glMainWidget;
  bool _redrawNeeded;
  bool _graphChanged;
  int width, height;
};

}

#endif // GLMAINWIDGETGRAPHICSITEM_H
///@endcond
