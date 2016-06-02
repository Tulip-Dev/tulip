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

#include "tulip/GlOverviewGraphicsItem.h"

#include <QMenu>
#include <QMessageBox>
#include <QGraphicsSceneMouseEvent>

#include <tulip/GlOffscreenRenderer.h>
#include <tulip/GlMainWidget.h>
#include <tulip/GlMainView.h>
#include <tulip/GlGraph.h>
#include <tulip/GlOffscreenRenderer.h>
#include <tulip/GlLayer.h>

using namespace std;

namespace tlp {

GlOverviewGraphicsItem::GlOverviewGraphicsItem(GlMainView *view,GlScene &scene):
  QGraphicsRectItem(0,0,128,128),view(view),baseScene(scene),width(128),height(128), mouseClicked(false),
  _frameColor(Color::Gray), _frameWidth(2), _generatePixmap(true) {
}

GlOverviewGraphicsItem::~GlOverviewGraphicsItem() {
  overview.setParentItem(nullptr);
}

void GlOverviewGraphicsItem::setSize(unsigned int width, unsigned int height) {
  this->width=width;
  this->height=height;
  draw(true);
}

void GlOverviewGraphicsItem::setLayerVisible(const string &name, bool visible) {
  if(visible==false) {
    _hiddenLayers.insert(name);
  }
  else {
    _hiddenLayers.erase(name);
  }
}

void GlOverviewGraphicsItem::draw(bool generatePixmap) {

  if(baseScene.getLayersList().empty())
    return;

  if (_frameWidth%2==1) {
    ++_frameWidth;
  }

  // Initialize the context avoid segfault when trying to render graph without any initialised gl context.
  QGLWidget *firstWidget = GlMainWidget::getFirstQGLWidget();
  firstWidget->makeCurrent();

  if(!overviewBorder.parentItem()) {

    //This flag is needed to don't display overview rectangle outside overview
    setFlag(QGraphicsItem::ItemClipsChildrenToShape);
    overview.setFlag(QGraphicsItem::ItemClipsChildrenToShape);
    overview.setShapeMode(QGraphicsPixmapItem::BoundingRectShape);
    setBrush(QBrush(QColor(255,255,255,255)));


    QPainterPath path;
    path.addRect(_frameWidth/2, _frameWidth/2, width-_frameWidth, height-_frameWidth);
    overviewBorder.setPath(path);
    overviewBorder.setParentItem(this);

    overview.setParentItem(&overviewBorder);

    //Init lines and polygons item
    for(unsigned int i=0; i<8; ++i) {
      line[i].setParentItem(&overview);

      if (i < 4) {
        poly[i].setParentItem(&overview);
        poly[i].setBrush(QBrush(QColor(0,0,0,64)));
        poly[i].setPen(Qt::NoPen);
      }
    }

  }

  Color backgroundColor = baseScene.getBackgroundColor();
  int bgV = backgroundColor.getV();

  for(unsigned int i=0; i<8; ++i) {
    if (bgV < 128) {
      line[i].setPen(QColor(255, 255, 255));
    }
    else {
      line[i].setPen(QColor(0, 0, 0));
    }

    if (i < 4) {
      if (bgV < 128) {
        poly[i].setBrush(QBrush(QColor(255,255,255,64)));
      }
      else {
        poly[i].setBrush(QBrush(QColor(0,0,0,64)));
      }
    }
  }

  // Backup initial viewport
  Vector<int,4> backupViewport=baseScene.getViewport();

  // Backup initial cameras
  vector<Camera> cameras;
  const vector<GlLayer*>& layersList=baseScene.getLayersList();

  for(GlLayer *layer : layersList) {
    cameras.push_back(*(layer->getCamera()));
  }

  // Compute visible part of the scene
  Camera &baseCamera= *(baseScene.getMainLayer()->getCamera());

  vector<Coord> cameraBoundingBox;
  cameraBoundingBox.push_back(baseCamera.viewportTo3DWorld(Coord(backupViewport[0],backupViewport[1],0)));
  cameraBoundingBox.push_back(baseCamera.viewportTo3DWorld(Coord(backupViewport[0]+backupViewport[2],backupViewport[1],0)));
  cameraBoundingBox.push_back(baseCamera.viewportTo3DWorld(Coord(backupViewport[0]+backupViewport[2],backupViewport[1]+backupViewport[3],0)));
  cameraBoundingBox.push_back(baseCamera.viewportTo3DWorld(Coord(backupViewport[0],backupViewport[1]+backupViewport[3],0)));

  // This code modify cameraBoundingBox coords to have coords with (x,y,0)
  // If we don't do this we will have invalid polygon when we do worldTo2DViewport transformations
  Coord eyesVector=baseCamera.getEyes()-baseCamera.getCenter();

  eyesVector=eyesVector*(1.f/eyesVector[2]);

  for(unsigned int i=0; i<4; i++)
    cameraBoundingBox[i]=cameraBoundingBox[i]-eyesVector*cameraBoundingBox[i][2];

  // Change viewport of the scene to the overview viewport
  baseScene.setBackupBackBuffer(false);
  baseScene.setViewport(0, 0, width, height);

  if(generatePixmap || _oldCameras.size() != layersList.size()) {
    // Center the scene
    baseScene.centerScene();
    _oldCameras.clear();

    for(GlLayer *layer : layersList) {
      _oldCameras.push_back(*(layer->getCamera()));
    }
  }
  else {
    unsigned int i=0;

    for(GlLayer *layer : layersList) {
      *(layer->getCamera()) = _oldCameras[i];
      ++i;
    }
  }

  // Project camera bounding box
  Camera &overviewCamera=*(baseScene.getMainLayer()->getCamera());
  Coord p0=overviewCamera.worldTo2DViewport(cameraBoundingBox[0]);
  Coord p1=overviewCamera.worldTo2DViewport(cameraBoundingBox[1]);
  Coord p2=overviewCamera.worldTo2DViewport(cameraBoundingBox[2]);
  Coord p3=overviewCamera.worldTo2DViewport(cameraBoundingBox[3]);

  // Rotation of the coordinates to have no crossing lines
  while(p1[0]>p3[0]) {
    Coord tmp(p0);
    p0=p1;
    p1=p2;
    p2=p3;
    p3=tmp;
  }

  while(p1[1]<p3[1]) {
    Coord tmp(p0);
    p0=p3;
    p3=p2;
    p2=p1;
    p1=tmp;
  }


  if(generatePixmap) {

    bool edgesLabels=baseScene.getMainGlGraph()->getRenderingParameters().displayEdgesLabels();
    bool nodesLabels=baseScene.getMainGlGraph()->getRenderingParameters().displayNodesLabels();
    bool metaNodesLabels=baseScene.getMainGlGraph()->getRenderingParameters().displayMetaNodesLabels();
    baseScene.getMainGlGraph()->getRenderingParameters().setDisplayEdgesLabels(false);
    baseScene.getMainGlGraph()->getRenderingParameters().setDisplayNodesLabels(false);
    baseScene.getMainGlGraph()->getRenderingParameters().setDisplayMetaNodesLabels(false);

    vector<bool> layersVisibility;

    const vector<GlLayer*>&layersList=baseScene.getLayersList();

    for(GlLayer *layer : layersList) {
      layersVisibility.push_back(layer->isVisible());

      if(_hiddenLayers.count(layer->getName())!=0)
        layer->setVisible(false);
    }

    // Draw the scene
    GlOffscreenRenderer::getInstance()->setViewPortSize(width-2*_frameWidth, height-2*_frameWidth);
    GlOffscreenRenderer::getInstance()->renderExternalScene(&baseScene, true);

    vector<bool>::iterator itTmp=layersVisibility.begin();

    for(GlLayer *layer : layersList) {
      if(*itTmp)
        layer->setVisible(true);

      ++itTmp;
    }

    GlGraphRenderingParameters &param = baseScene.getMainGlGraph()->getRenderingParameters();
    param.setDisplayEdgesLabels(edgesLabels);
    param.setDisplayNodesLabels(nodesLabels);
    param.setDisplayMetaNodesLabels(metaNodesLabels);
  }

  // invert applied camera transformations
  unsigned int i=0;

  for(GlLayer *layer : layersList) {
    *(layer->getCamera())=cameras[i];
    ++i;
  }

  // invert applied viewport
  baseScene.setViewport(backupViewport);
  baseScene.setBackupBackBuffer(true);
  baseScene.setSceneNeedRedraw(false);


  if(generatePixmap) {
    // Load scene pixmap to the item
    QPixmap pixmap;
    QImage img = GlOffscreenRenderer::getInstance()->getImage();
    pixmap.convertFromImage(img);
    overview.setPos(_frameWidth, _frameWidth);
    overview.setPixmap(pixmap);
  }

  // set lines and polygons coordinates
  line[0].setLine(width-2*_frameWidth,0,p0[0],height-p0[1]);
  line[1].setLine(0,0,p1[0],height-p1[1]);
  line[2].setLine(0,height-2*_frameWidth,p2[0],height-p2[1]);
  line[3].setLine(width-2*_frameWidth,height-2*_frameWidth,p3[0],height-p3[1]);
  line[4].setLine(p0[0],height-p0[1], p1[0],height-p1[1]);
  line[5].setLine(p1[0],height-p1[1], p2[0],height-p2[1]);
  line[6].setLine(p2[0],height-p2[1], p3[0],height-p3[1]);
  line[7].setLine(p3[0],height-p3[1], p0[0],height-p0[1]);

  QVector<QPointF> tmpVect;
  tmpVect.push_back(QPointF(width-2*_frameWidth,0));
  tmpVect.push_back(QPointF(p0[0],height-p0[1]));
  tmpVect.push_back(QPointF(p1[0],height-p1[1]));
  tmpVect.push_back(QPointF(0,0));
  poly[0].setPolygon(QPolygonF(tmpVect));
  tmpVect.clear();
  tmpVect.push_back(QPointF(0,0));
  tmpVect.push_back(QPointF(p1[0],height-p1[1]));
  tmpVect.push_back(QPointF(p2[0],height-p2[1]));
  tmpVect.push_back(QPointF(0,height-2*_frameWidth));
  poly[1].setPolygon(QPolygonF(tmpVect));
  tmpVect.clear();
  tmpVect.push_back(QPointF(0,height-2*_frameWidth));
  tmpVect.push_back(QPointF(p2[0],height-p2[1]));
  tmpVect.push_back(QPointF(p3[0],height-p3[1]));
  tmpVect.push_back(QPointF(width-2*_frameWidth,height-2*_frameWidth));
  poly[2].setPolygon(QPolygonF(tmpVect));
  tmpVect.clear();
  tmpVect.push_back(QPointF(width-2*_frameWidth,height-2*_frameWidth));
  tmpVect.push_back(QPointF(p3[0],height-p3[1]));
  tmpVect.push_back(QPointF(p0[0],height-p0[1]));
  tmpVect.push_back(QPointF(width-2*_frameWidth,0));
  poly[3].setPolygon(QPolygonF(tmpVect));

  QPen pen(QColor(_frameColor[0], _frameColor[1], _frameColor[2], _frameColor[3]));
  pen.setWidth(_frameWidth);
  pen.setJoinStyle(Qt::MiterJoin);

  overviewBorder.setPen(pen);

}

void GlOverviewGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
  draw(_generatePixmap);
  QGraphicsRectItem::paint(painter, option, widget);
  _generatePixmap = false;
}

void GlOverviewGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent* event) {
  if(event->button()==Qt::LeftButton) {
    mouseClicked=true;
    setScenePosition(event->pos());
  }
}

void GlOverviewGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
  if(event->button()==Qt::LeftButton) {
    mouseClicked=false;
  }
}

void GlOverviewGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
  if(mouseClicked)
    setScenePosition(event->pos());
}

void GlOverviewGraphicsItem::setScenePosition(QPointF pos) {

  Coord position(width+_frameWidth-pos.x(),pos.y()-_frameWidth,0);

  Vector<int,4> backupViewport=baseScene.getViewport();

  baseScene.setViewport(0,0,width, height);

  vector<Camera> cameras;
  const vector<GlLayer*>& layersList=baseScene.getLayersList();

  for(GlLayer* layer : layersList) {
    cameras.push_back(*(layer->getCamera()));
  }

  baseScene.centerScene();

  vector<Coord> centerPos;

  for(GlLayer* layer : layersList) {
    centerPos.push_back(layer->getCamera()->viewportTo3DWorld(position));
  }

  unsigned int i=0;

  for(GlLayer* layer : layersList) {
    Coord eyesVector=cameras[i].getEyes()-cameras[i].getCenter();
    Camera &camera=*(layer->getCamera());
    camera=cameras[i];
    camera.setCenter(centerPos[i]);
    camera.setEyes(centerPos[i]+eyesVector);
    ++i;
  }

  baseScene.setViewport(backupViewport);

  view->getGlMainWidget()->draw(false);
}

}
