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

#ifndef GLOVERVIEWGRAPHICSITEM_H
#define GLOVERVIEWGRAPHICSITEM_H

#include <tulip/tulipconf.h>
#include <tulip/Camera.h>
#include <tulip/Color.h>

#include <QGraphicsPixmapItem>
#include <QGraphicsPathItem>

#include <set>

namespace tlp {

class GlMainView;
class GlScene;

class TLP_QT_SCOPE GlOverviewGraphicsItem : public QObject, public QGraphicsRectItem {

  Q_OBJECT

public:

  GlOverviewGraphicsItem(GlMainView *view,GlScene &scene);
  ~GlOverviewGraphicsItem();

  void setSize(unsigned int width, unsigned int height);
  inline unsigned int getWidth() {
    return width;
  }
  inline unsigned int getHeight() {
    return height;
  }

  void setFrameColor(const Color &color) {
    _frameColor = color;
  }

  void setFrameWidth(int width) {
    _frameWidth = width;
  }

  void setLayerVisible(const std::string &name,bool visible);

public slots :

  void draw(bool generatePixmap);

private :

  void mousePressEvent(QGraphicsSceneMouseEvent* event);
  void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
  void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
  void setScenePosition(QPointF pos);

  GlMainView *view;
  GlScene &baseScene;
  unsigned int width, height;

  QGraphicsPixmapItem overview;
  QGraphicsPathItem overviewBorder;
  QGraphicsLineItem line[8];
  QGraphicsPolygonItem poly[4];

  bool mouseClicked;

  std::set<std::string> _hiddenLayers;

  std::vector<Camera> _oldCameras;

  Color _frameColor;
  int _frameWidth;

};

}

#endif // GLOVERVIEWGRAPHICSITEM_H
///@endcond
