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

/**
 Author: Antoine Lambert and Morgan Mathiaut
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.
 */
#ifndef Tulip_GRAPHICSITEM_H_
#define Tulip_GRAPHICSITEM_H_

#include <QtGui/QPainter>
#include <QtGui/QGraphicsWidget>
#include <QtGui/QGraphicsScene>


#include <tulip/GlScene.h>
#include <tulip/GlMainWidget.h>
#include <QtOpenGL/QGLFramebufferObject>
#include <QtGui/QGraphicsSceneResizeEvent>
#include <QtGui/QCheckBox>

namespace tlp {


/** \brief Class use to render a GlMainWidget on GraphicsView
 *
 * Use this class if you want to add a GlMainWidget in the form of QGraphicsItem
 *
 */
class TLP_QT_SCOPE GlMainWidgetItem : public QObject, public QGraphicsItem {

  Q_OBJECT
// moc failure on MAC
//#if QT_MINOR_REL > 5
//  Q_INTERFACES(QGraphicsItem)
//#endif

public :

  /**
   * \brief Main constructor
   * \param parent GraphicsView parent class
   * \param glMainWidgte glMainWidget use by this item
   * \param width size on scene
   * \param height size of scene
   * \param decorate true if you want an item with border
   */
  GlMainWidgetItem(GlMainWidget *glMainWidget, int width, int height, bool decorate=false, float borderWidth=20.f);

  /**
   * \brief Default destructor
   */
  ~GlMainWidgetItem();

  /**
   * \brief Function use to render this item
   * Function use to render this item.
   * You don't have to call this function, QGraphicsView do this for you
   */
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

  /**
   * Return the boundingRect of this item
   */
  QRectF boundingRect() const;

  /**
   * Resize the GlMainWidget item
   */
  void resize(int width, int height);

  /**
   * Set if during repaint we have to render GlMainWidget
   */
  void setRedrawNeeded(bool redrawNeeded){
    this->redrawNeeded=redrawNeeded;
  }

  GlMainWidget *getGlMainWidget() { return glMainWidget;}

protected :

  void wheelEvent(QGraphicsSceneWheelEvent *event);
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
  void mousePressEvent(QGraphicsSceneMouseEvent *event);
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
  void hoverMoveEvent(QGraphicsSceneHoverEvent * event);
  void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

protected slots:

  void glMainWidgetDraw(GlMainWidget *,bool);
  void glMainWidgetRedraw(GlMainWidget *);

private :

  GlMainWidget *glMainWidget;
  QCheckBox *lockedCB;

  bool redrawNeeded;
  bool decorate;

  int width, height;
  float borderWidth;

  unsigned char *renderingStore;

};

}

#endif
