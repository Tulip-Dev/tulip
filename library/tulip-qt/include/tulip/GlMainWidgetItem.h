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

#include <QtGui/QGraphicsWidget>

#include "tulip/GlMainWidgetGraphicsView.h"

#include <tulip/GlScene.h>
#include <QtOpenGL/QGLFramebufferObject>
#include <QtGui/QGraphicsSceneResizeEvent>

namespace tlp {


/** \brief Class use to render a GlMainWidget on GraphicsView
 *
 * Use this class if you want to add a GlMainWidget in the form of QGraphicsItem
 *
 */
class TLP_QT_SCOPE GlMainWidgetItem : public QObject, public QGraphicsItem {

  Q_OBJECT
  Q_INTERFACES(QGraphicsItem)
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
   * \param parentItem use this if you want to construct this item with a parent item
   * \param decorate true if you want an item with border
   */
  GlMainWidgetItem(GlMainWidgetGraphicsView *parent,GlMainWidget *glMainWidget, int width, int height,QGraphicsItem *parentItem=NULL,bool decorate=false);

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
   * Set width of this item
   */
  void setWidth(int width){
    this->width=width;
  }

  /**
   * Set height of this item
   */
  void setHeight(int height){
    this->height=height;
  }

  /**
   * Set if during repaint we have to render GlMainWidget
   */
  void setRedrawNeed(bool redrawNeed){
    this->redrawNeed=redrawNeed;
  }

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

  GlMainWidgetGraphicsView *parent;
  GlMainWidget *glMainWidget;

  bool redrawNeed;

  bool decorate;
  QGLFramebufferObject *fbo1, *fbo2;

  int width, height;
  int vpWidth, vpHeight;
  int x, y;
};

/** \brief Class use to render a GlMainWidget in a QGraphicsItemWidget
 *
 * Use this class if you want to add a GlMainWidget in the form of QGraphicsItemWidget
 *
 */
class TLP_QT_SCOPE GlMainWidgetGraphicsWidget : public QGraphicsWidget {

protected:
  GlMainWidgetGraphicsView *parent;
  GlMainWidgetItem glMainWidgetItem;

public :
  /**
     * \brief Main constructor
     * \param parent GraphicsView parent class
     * \param glMainWidgte glMainWidget use by this item
     * \param width size on scene
     * \param height size of scene
     * \param parentItem use this if you want to construct this item with a parent item
     */
  GlMainWidgetGraphicsWidget(GlMainWidgetGraphicsView *parent,GlMainWidget *glMainWidget, int width, int height,QGraphicsItem * =NULL,Qt::WindowFlags flags=Qt::Widget):
    QGraphicsWidget(NULL,flags),parent(parent),glMainWidgetItem(parent,glMainWidget,width,height,this){
    glMainWidgetItem.setPos(width/2,height/2);
    resize(width,height);
  }

  /**
   * This fuction is use when the size of the widget modified
   * Change size of the GlMainWidgetItem
   */
  void resizeEvent ( QGraphicsSceneResizeEvent * event ){
    glMainWidgetItem.setWidth(event->newSize().width());
    glMainWidgetItem.setHeight(event->newSize().height());
    glMainWidgetItem.setPos(event->newSize().width()/2,event->newSize().height()/2);
    glMainWidgetItem.setRedrawNeed(true);
    parent->scene()->update();

  }

  /**
   * Return the GlMainWidgetItem use
   */
  GlMainWidgetItem *getGlMainWidgetItem() {
    return &glMainWidgetItem;
  }

};

}

#endif
