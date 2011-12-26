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
#ifndef GLOVERVIEWGRAPHICSITEM_H
#define GLOVERVIEWGRAPHICSITEM_H


#include <QtGui/QGraphicsPixmapItem>
#include <QtOpenGL/QGLFramebufferObject>
#include <tulip/GlScene.h>

namespace tlp {

class GlOverviewGraphicsItem : public QObject, public QGraphicsPixmapItem {

Q_OBJECT

public:
  GlOverviewGraphicsItem(GlScene &scene);

  QRectF boundingRect() const;

public slots :

  void draw();

private :

  GlScene &baseScene;
  unsigned int vPWidth, vPHeight;
  QGLFramebufferObject *glFrameBuf, *glFrameBuf2;

  QGraphicsLineItem *line1;
  QGraphicsLineItem *line2;
  QGraphicsLineItem *line3;
  QGraphicsLineItem *line4;
  QGraphicsPolygonItem *poly1;
  QGraphicsPolygonItem *poly2;
  QGraphicsPolygonItem *poly3;
  QGraphicsPolygonItem *poly4;

};

}

#endif // GLOVERVIEWGRAPHICSITEM_H
