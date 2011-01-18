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

#ifndef Tulip_GLMAINWIDGETGRAPHICSVIEW_H_
#define Tulip_GLMAINWIDGETGRAPHICSVIEW_H_

#include <tulip/AbstractView.h>
#include <tulip/GlMainWidget.h>
#include <tulip/GlScene.h>

#include <QtGui/QGraphicsView>
#include <QtGui/QGraphicsScene>
#include <QtGui/QResizeEvent>
#include <QtGui/QDialog>
#include <QtOpenGL/QGLWidget>

#include <vector>

namespace tlp {

class GlMainWidgetItem;
class GlMainWidgetGraphicsWidget;
class GWOverviewWidget;

/** \brief Class use to create a QGraphicsView with GlMainWidget inside
 *
 * Use this class if you want a view with QGraphicsView system and if you want to render a GlMainWidget inside
 *
 */
class TLP_QT_SCOPE GlMainWidgetGraphicsView : public QGraphicsView {

  Q_OBJECT

public:

  /**
   * Main constructor
   * \param parent widget
   * \param glMainWidget GlMainWidget who will be add on this view
   */
  GlMainWidgetGraphicsView(QWidget *parent, GlMainWidget *glMainWidget) ;

  ~GlMainWidgetGraphicsView();

  /**
   * This function is call when controller need to draw this view
   */
  void draw(bool glSceneChanged = true);

  /**
   * This function is call when controller need to redraw this view
   */
  void redraw();

  QGLWidget * getViewportWidget() const {return viewportWidet;}

  GlMainWidgetItem *getGlMainWidgetItem() const {return glSceneItem;}

protected:

  void resizeEvent(QResizeEvent *event);

  GlMainWidgetItem *glSceneItem;
  GlMainWidget *glMainWidget;
  QGLWidget *viewportWidet;
  bool drawNeeded;
};

}

#endif
