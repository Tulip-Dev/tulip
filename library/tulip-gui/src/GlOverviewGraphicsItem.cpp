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

#include <QtGui/QGraphicsSceneMouseEvent>

using namespace std;

namespace tlp {

map<pair<unsigned int,unsigned int>, QGLFramebufferObject *> GlOverviewGraphicsItem::glFrameBufferMap = map<pair<unsigned int,unsigned int>, QGLFramebufferObject *>();

GlOverviewGraphicsItem::GlOverviewGraphicsItem(GlMainView *view,GlScene &scene):QGraphicsPixmapItem(),view(view),baseScene(scene),width(128),height(128),mouseClicked(false) {
  //This flag is needed to don't display overview rectangle outside overview
  setFlag(QGraphicsItem::ItemClipsChildrenToShape);
  setShapeMode(QGraphicsPixmapItem::BoundingRectShape);

  //Init lines and polygons item
  for(unsigned int i=0; i<4; ++i) {
    line[i].setParentItem(this);
    poly[i].setParentItem(this);
    poly[i].setBrush(QBrush(QColor(0,0,0,64)));
  }
}

void GlOverviewGraphicsItem::draw() {

  if(baseScene.getLayersList()->size()==0)
    return;

  // Initialize the context avoid segfault when trying to render graph without any initialised gl context.
  QGLWidget *firstWidget = GlMainWidget::getFirstQGLWidget();
  firstWidget->makeCurrent();

  pair<unsigned int,unsigned int> pairWidthHeight(width,height);
  if(glFrameBufferMap.count(pairWidthHeight)==0){
  // Allocate frame buffer object
#if (QT_VERSION >= QT_VERSION_CHECK(4, 6, 0))
    QGLFramebufferObjectFormat fboFmt;
    fboFmt.setAttachment(QGLFramebufferObject::CombinedDepthStencil);

    glFrameBufferMap[pairWidthHeight]=new QGLFramebufferObject(width, height, fboFmt);

#else
    glFrameBufferMap[pairWidthHeight]=new QGLFramebufferObject(width, height, QGLFramebufferObject::CombinedDepthStencil);
#endif
  }

  QGLFramebufferObject *glFrameBuffer = glFrameBufferMap[pairWidthHeight];

  // Backup initial viewport
  Vector<int,4> backupViewport=baseScene.getViewport();

  // Backup initial cameras
  vector<Camera> cameras;
  vector<pair<string, GlLayer*> >* layerList=baseScene.getLayersList();

  for(vector<pair<string, GlLayer*> >::iterator it=layerList->begin(); it!=layerList->end(); ++it) {
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
  eyesVector=eyesVector*(1./eyesVector[2]);

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


  // Draw the scene
  glFrameBuffer->bind();
  baseScene.draw();
  glFrameBuffer->release();

  // invert applied camera transformations
  unsigned int i=0;

  for(vector<pair<string, GlLayer*> >::iterator it=layerList->begin(); it!=layerList->end(); ++it) {
    (*it).second->setCamera(cameras[i]);
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

  // Load scene pixmap to the item
  QPixmap pixmap;
  pixmap.convertFromImage(glFrameBuffer->toImage().convertToFormat(QImage::Format_RGB32));
  setPixmap(pixmap);

  // set lines and polygons coordinates
  line[0].setLine(128,0,p0[0],128-p0[1]);
  line[1].setLine(0,0,p1[0],128-p1[1]);
  line[2].setLine(0,128,p2[0],128-p2[1]);
  line[3].setLine(128,128,p3[0],128-p3[1]);
  QVector<QPointF> tmpVect;
  tmpVect.push_back(QPointF(128,0));
  tmpVect.push_back(QPointF(p0[0],128-p0[1]));
  tmpVect.push_back(QPointF(p1[0],128-p1[1]));
  tmpVect.push_back(QPointF(0,0));
  poly[0].setPolygon(QPolygonF(tmpVect));
  tmpVect.clear();
  tmpVect.push_back(QPointF(0,0));
  tmpVect.push_back(QPointF(p1[0],128-p1[1]));
  tmpVect.push_back(QPointF(p2[0],128-p2[1]));
  tmpVect.push_back(QPointF(0,128));
  poly[1].setPolygon(QPolygonF(tmpVect));
  tmpVect.clear();
  tmpVect.push_back(QPointF(0,128));
  tmpVect.push_back(QPointF(p2[0],128-p2[1]));
  tmpVect.push_back(QPointF(p3[0],128-p3[1]));
  tmpVect.push_back(QPointF(128,128));
  poly[2].setPolygon(QPolygonF(tmpVect));
  tmpVect.clear();
  tmpVect.push_back(QPointF(128,128));
  tmpVect.push_back(QPointF(p3[0],128-p3[1]));
  tmpVect.push_back(QPointF(p0[0],128-p0[1]));
  tmpVect.push_back(QPointF(128,0));
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
  Coord position(128-pos.x(),pos.y(),0);

  Vector<int,4> backupViewport=baseScene.getViewport();

  baseScene.setViewport(0,0,width, height);

  glPushAttrib(GL_ALL_ATTRIB_BITS);
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();

  vector<Camera> cameras;
  vector<pair<string, GlLayer*> >* layerList=baseScene.getLayersList();

  for(vector<pair<string, GlLayer*> >::iterator it=layerList->begin(); it!=layerList->end(); ++it) {
    cameras.push_back((*it).second->getCamera());
  }

  baseScene.centerScene();

  vector<Coord> centerPos;

  for(vector<pair<string, GlLayer*> >::iterator it=layerList->begin(); it!=layerList->end(); ++it) {
    centerPos.push_back((*it).second->getCamera().screenTo3DWorld(position));
  }

  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glPopAttrib();

  unsigned int i=0;

  for(vector<pair<string, GlLayer*> >::iterator it=layerList->begin(); it!=layerList->end(); ++it) {
    Coord eyesVector=cameras[i].getEyes()-cameras[i].getCenter();
    cameras[i].setCenter(centerPos[i]);
    cameras[i].setEyes(cameras[i].getCenter()+eyesVector);
    (*it).second->setCamera(cameras[i]);
    ++i;
  }

  baseScene.setViewport(backupViewport);

  view->emitDrawNeededSignal();
}

}
