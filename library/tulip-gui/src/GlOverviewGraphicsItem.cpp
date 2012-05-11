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
#include "tulip/GlOverviewGraphicsItem.h"
#include "tulip/GlOffscreenRenderer.h"
#include "tulip/GlMainWidget.h"
#include "tulip/GlMainView.h"
#include "tulip/GlGraphComposite.h"
#include <QtGui/QMenu>

#include <QtGui/QGraphicsSceneMouseEvent>

using namespace std;

namespace tlp {

GlOverviewGraphicsItem::GlOverviewGraphicsItem(GlMainView *view,GlScene &scene):QGraphicsRectItem(0,0,128,128),view(view),baseScene(scene),width(128),height(128),glFrameBuffer(NULL),mouseClicked(false) {
  //This flag is needed to don't display overview rectangle outside overview
  setFlag(QGraphicsItem::ItemClipsChildrenToShape);
  overview.setShapeMode(QGraphicsPixmapItem::BoundingRectShape);
  setBrush(QBrush(QColor(255,255,255,255)));
  overview.setParentItem(this);

  //Init lines and polygons item
  for(unsigned int i=0; i<4; ++i) {
    line[i].setParentItem(&overview);
    poly[i].setParentItem(&overview);
    poly[i].setBrush(QBrush(QColor(0,0,0,64)));
  }
}

GlOverviewGraphicsItem::~GlOverviewGraphicsItem() {
  if(glFrameBuffer)
    delete glFrameBuffer;
}

void GlOverviewGraphicsItem::setSize(unsigned int width, unsigned int height) {
  this->width=width;
  this->height=height;

  if(glFrameBuffer) {
    delete glFrameBuffer;
    glFrameBuffer=NULL;
  }

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

  if(baseScene.getLayersList().size()==0)
    return;

  // Initialize the context avoid segfault when trying to render graph without any initialised gl context.
  QGLWidget *firstWidget = GlMainWidget::getFirstQGLWidget();
  firstWidget->makeCurrent();

  if(!glFrameBuffer) {
    // Allocate frame buffer object
    glFrameBuffer=new QGLFramebufferObject(width, height, QGLFramebufferObject::CombinedDepthStencil);
  }

  // Backup initial viewport
  Vector<int,4> backupViewport=baseScene.getViewport();

  // Backup initial cameras
  vector<Camera> cameras;
  const vector<pair<string, GlLayer*> >& layerList=baseScene.getLayersList();

  for(vector<pair<string, GlLayer*> >::const_iterator it=layerList.begin(); it!=layerList.end(); ++it) {
    cameras.push_back((*it).second->getCamera());
  }

  // Compute visible part of the scene
  Camera &baseCamera=baseScene.getGraphCamera();

  vector<Coord> cameraBoundingBox;
  cameraBoundingBox.push_back(baseCamera.screenTo3DWorld(Coord(backupViewport[0],backupViewport[1],0)));
  cameraBoundingBox.push_back(baseCamera.screenTo3DWorld(Coord(backupViewport[0]+backupViewport[2],backupViewport[1],0)));
  cameraBoundingBox.push_back(baseCamera.screenTo3DWorld(Coord(backupViewport[0]+backupViewport[2],backupViewport[1]+backupViewport[3],0)));
  cameraBoundingBox.push_back(baseCamera.screenTo3DWorld(Coord(backupViewport[0],backupViewport[1]+backupViewport[3],0)));

  // This code modify cameraBoundingBox coords to have coords with (x,y,0)
  // If we don't do this we will have invalid polygon when we do worldTo2DScreen transformations
  Coord eyesVector=baseCamera.getEyes()-baseCamera.getCenter();

  eyesVector=eyesVector*(1.f/eyesVector[2]);

  for(unsigned int i=0; i<4; i++)
    cameraBoundingBox[i]=cameraBoundingBox[i]-eyesVector*cameraBoundingBox[i][2];

  // Change viewport of the scene to the overview viewport
  baseScene.setViewport(0,0,width, height);

  // Backup OpenGL matrix
  glPushAttrib(GL_ALL_ATTRIB_BITS);
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();

  // Center the scene
  baseScene.centerScene();

  // Project camera bounding box
  Camera &overviewCamera=baseScene.getGraphCamera();
  Coord p0=overviewCamera.worldTo2DScreen(cameraBoundingBox[0]);
  Coord p1=overviewCamera.worldTo2DScreen(cameraBoundingBox[1]);
  Coord p2=overviewCamera.worldTo2DScreen(cameraBoundingBox[2]);
  Coord p3=overviewCamera.worldTo2DScreen(cameraBoundingBox[3]);

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
    bool edgesLabels=baseScene.getGlGraphComposite()->getRenderingParametersPointer()->isViewEdgeLabel();
    bool nodesLabels=baseScene.getGlGraphComposite()->getRenderingParametersPointer()->isViewNodeLabel();
    bool metaNodesLabels=baseScene.getGlGraphComposite()->getRenderingParametersPointer()->isViewMetaLabel();
    baseScene.getGlGraphComposite()->getRenderingParametersPointer()->setViewEdgeLabel(false);
    baseScene.getGlGraphComposite()->getRenderingParametersPointer()->setViewNodeLabel(false);
    baseScene.getGlGraphComposite()->getRenderingParametersPointer()->setViewMetaLabel(false);

    vector<bool> layersVisibility;

    const vector<pair<string, GlLayer*> > &layersList=baseScene.getLayersList();

    for(vector<pair<string, GlLayer*> >::const_iterator it=layersList.begin(); it!=layersList.end(); ++it) {
      layersVisibility.push_back((*it).second->isVisible());

      if((*it).second->isAWorkingLayer())
        (*it).second->setVisible(false);

      if(_hiddenLayers.count((*it).first)!=0)
        (*it).second->setVisible(false);
    }

    // Draw the scene
    glFrameBuffer->bind();
    baseScene.draw();
    glFrameBuffer->release();

    vector<bool>::iterator itTmp=layersVisibility.begin();

    for(vector<pair<string, GlLayer*> >::const_iterator it=layersList.begin(); it!=layersList.end(); ++it) {
      if((*itTmp)==true)
        (*it).second->setVisible(true);

      ++itTmp;
    }

    baseScene.getGlGraphComposite()->getRenderingParametersPointer()->setViewEdgeLabel(edgesLabels);
    baseScene.getGlGraphComposite()->getRenderingParametersPointer()->setViewNodeLabel(nodesLabels);
    baseScene.getGlGraphComposite()->getRenderingParametersPointer()->setViewMetaLabel(metaNodesLabels);
  }

  // invert applied camera transformations
  unsigned int i=0;

  for(vector<pair<string, GlLayer*> >::const_iterator it=layerList.begin(); it!=layerList.end(); ++it) {
    (*it).second->getCamera()=cameras[i];
    ++i;
  }

  // invert applied OpenGl transformations
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glPopAttrib();

  // invert applied viewport
  baseScene.setViewport(backupViewport);

  if(generatePixmap) {
    // Load scene pixmap to the item
    QPixmap pixmap;
    QImage img(glFrameBuffer->toImage());
    img = QImage(img.bits(),img.width(),img.height(),QImage::Format_ARGB32);
    pixmap.convertFromImage(img);
    overview.setPixmap(pixmap);
  }

  // set lines and polygons coordinates
  line[0].setLine(width,0,p0[0],height-p0[1]);
  line[1].setLine(0,0,p1[0],height-p1[1]);
  line[2].setLine(0,height,p2[0],height-p2[1]);
  line[3].setLine(width,height,p3[0],height-p3[1]);
  QVector<QPointF> tmpVect;
  tmpVect.push_back(QPointF(width,0));
  tmpVect.push_back(QPointF(p0[0],height-p0[1]));
  tmpVect.push_back(QPointF(p1[0],height-p1[1]));
  tmpVect.push_back(QPointF(0,0));
  poly[0].setPolygon(QPolygonF(tmpVect));
  tmpVect.clear();
  tmpVect.push_back(QPointF(0,0));
  tmpVect.push_back(QPointF(p1[0],height-p1[1]));
  tmpVect.push_back(QPointF(p2[0],height-p2[1]));
  tmpVect.push_back(QPointF(0,height));
  poly[1].setPolygon(QPolygonF(tmpVect));
  tmpVect.clear();
  tmpVect.push_back(QPointF(0,height));
  tmpVect.push_back(QPointF(p2[0],height-p2[1]));
  tmpVect.push_back(QPointF(p3[0],height-p3[1]));
  tmpVect.push_back(QPointF(width,height));
  poly[2].setPolygon(QPolygonF(tmpVect));
  tmpVect.clear();
  tmpVect.push_back(QPointF(width,height));
  tmpVect.push_back(QPointF(p3[0],height-p3[1]));
  tmpVect.push_back(QPointF(p0[0],height-p0[1]));
  tmpVect.push_back(QPointF(width,0));
  poly[3].setPolygon(QPolygonF(tmpVect));
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
  Coord position(width-pos.x(),pos.y(),0);

  Vector<int,4> backupViewport=baseScene.getViewport();

  baseScene.setViewport(0,0,width, height);

  glPushAttrib(GL_ALL_ATTRIB_BITS);
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();

  vector<Camera> cameras;
  const vector<pair<string, GlLayer*> >& layerList=baseScene.getLayersList();

  for(vector<pair<string, GlLayer*> >::const_iterator it=layerList.begin(); it!=layerList.end(); ++it) {
    cameras.push_back((*it).second->getCamera());
  }

  baseScene.centerScene();

  vector<Coord> centerPos;

  for(vector<pair<string, GlLayer*> >::const_iterator it=layerList.begin(); it!=layerList.end(); ++it) {
    centerPos.push_back((*it).second->getCamera().screenTo3DWorld(position));
  }

  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glPopAttrib();

  unsigned int i=0;

  for(vector<pair<string, GlLayer*> >::const_iterator it=layerList.begin(); it!=layerList.end(); ++it) {
    Coord eyesVector=cameras[i].getEyes()-cameras[i].getCenter();
    Camera &camera=(*it).second->getCamera();
    camera=cameras[i];
    camera.setCenter(centerPos[i]);
    camera.setEyes(centerPos[i]+eyesVector);
    ++i;
  }

  baseScene.setViewport(backupViewport);

  view->getGlMainWidget()->draw(false);
}

}
