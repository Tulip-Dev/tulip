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

GlOverviewGraphicsItem::GlOverviewGraphicsItem(GlMainView *view,GlScene &scene):QGraphicsPixmapItem(),view(view),baseScene(scene),vPWidth(128),vPHeight(128),glFrameBuf(NULL),mouseClicked(false) {
  setFlag(QGraphicsItem::ItemClipsChildrenToShape);
  line1=new QGraphicsLineItem(this);
  line2=new QGraphicsLineItem(this);
  line3=new QGraphicsLineItem(this);
  line4=new QGraphicsLineItem(this);
  poly1=new QGraphicsPolygonItem(this);
  poly2=new QGraphicsPolygonItem(this);
  poly3=new QGraphicsPolygonItem(this);
  poly4=new QGraphicsPolygonItem(this);
  poly1->setBrush(QBrush(QColor(0,0,0,64)));
  poly2->setBrush(QBrush(QColor(0,0,0,64)));
  poly3->setBrush(QBrush(QColor(0,0,0,64)));
  poly4->setBrush(QBrush(QColor(0,0,0,64)));
}

void GlOverviewGraphicsItem::draw() {

  if(baseScene.getLayersList()->size()==0)
    return;

  //Initialize the context avoid segfault when trying to render graph without any initialised gl context.
  QGLWidget *firstWidget = GlMainWidget::getFirstQGLWidget();
  firstWidget->makeCurrent();

  if (glFrameBuf != NULL && (vPWidth != static_cast<unsigned int>(glFrameBuf->width()) || vPHeight != static_cast<unsigned int>(glFrameBuf->height()))) {
    delete glFrameBuf;
    glFrameBuf = NULL;
  }

  if (glFrameBuf == NULL) {
#if (QT_VERSION >= QT_VERSION_CHECK(4, 6, 0))
    QGLFramebufferObjectFormat fboFmt;
    fboFmt.setAttachment(QGLFramebufferObject::CombinedDepthStencil);

    glFrameBuf = new QGLFramebufferObject(vPWidth, vPHeight, fboFmt);
  }

#else
    glFrameBuf = new QGLFramebufferObject(vPWidth, vPHeight, QGLFramebufferObject::CombinedDepthStencil);
  }
#endif

  Vector<int,4> backupViewport=baseScene.getViewport();

  vector<Camera> cameras;
  vector<pair<string, GlLayer*> >* layerList=baseScene.getLayersList();
  for(vector<pair<string, GlLayer*> >::iterator it=layerList->begin();it!=layerList->end();++it){
    cameras.push_back((*it).second->getCamera());
  }

  Camera baseCamera=baseScene.getGraphCamera();
  vector<Coord> cameraBoundingBox;

  cameraBoundingBox.push_back(baseCamera.screenTo3DWorld(Coord(backupViewport[0],backupViewport[1],0)));
  cameraBoundingBox.push_back(baseCamera.screenTo3DWorld(Coord(backupViewport[0]+backupViewport[2],backupViewport[1],0)));
  cameraBoundingBox.push_back(baseCamera.screenTo3DWorld(Coord(backupViewport[0]+backupViewport[2],backupViewport[1]+backupViewport[3],0)));
  cameraBoundingBox.push_back(baseCamera.screenTo3DWorld(Coord(backupViewport[0],backupViewport[1]+backupViewport[3],0)));

  baseScene.setViewport(0,0,vPWidth, vPHeight);

  glPushAttrib(GL_ALL_ATTRIB_BITS);

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();

  baseScene.centerScene();

  baseCamera=baseScene.getGraphCamera();
  Coord p0=baseCamera.worldTo2DScreen(cameraBoundingBox[0]);
  Coord p1=baseCamera.worldTo2DScreen(cameraBoundingBox[1]);
  Coord p2=baseCamera.worldTo2DScreen(cameraBoundingBox[2]);
  Coord p3=baseCamera.worldTo2DScreen(cameraBoundingBox[3]);

  while(p1[0]>p3[0]){
    Coord tmp(p0);
    p0=p1;
    p1=p2;
    p2=p3;
    p3=tmp;
  }

  while(p1[1]<p3[1]){
    Coord tmp(p0);
    p0=p3;
    p3=p2;
    p2=p1;
    p1=tmp;
  }


  glFrameBuf->bind();

  baseScene.draw();
  glFrameBuf->release();

  unsigned int i=0;

  for(vector<pair<string, GlLayer*> >::iterator it=layerList->begin(); it!=layerList->end(); ++it) {
    (*it).second->setCamera(cameras[i]);
    ++i;
  }

  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();

  glMatrixMode(GL_PROJECTION);
  glPopMatrix();

  glPopAttrib();

  baseScene.setViewport(backupViewport);

  QPixmap pixmap;
  pixmap.convertFromImage(glFrameBuf->toImage());
  setPixmap(pixmap);

  line1->setLine(128,0,p0[0],128-p0[1]);
  line2->setLine(0,0,p1[0],128-p1[1]);
  line3->setLine(0,128,p2[0],128-p2[1]);
  line4->setLine(128,128,p3[0],128-p3[1]);
  QVector<QPointF> tmpVect;
  tmpVect.push_back(QPointF(128,0));
  tmpVect.push_back(QPointF(p0[0],128-p0[1]));
  tmpVect.push_back(QPointF(p1[0],128-p1[1]));
  tmpVect.push_back(QPointF(0,0));
  poly1->setPolygon(QPolygonF(tmpVect));
  tmpVect.clear();
  tmpVect.push_back(QPointF(0,0));
  tmpVect.push_back(QPointF(p1[0],128-p1[1]));
  tmpVect.push_back(QPointF(p2[0],128-p2[1]));
  tmpVect.push_back(QPointF(0,128));
  poly2->setPolygon(QPolygonF(tmpVect));
  tmpVect.clear();
  tmpVect.push_back(QPointF(0,128));
  tmpVect.push_back(QPointF(p2[0],128-p2[1]));
  tmpVect.push_back(QPointF(p3[0],128-p3[1]));
  tmpVect.push_back(QPointF(128,128));
  poly3->setPolygon(QPolygonF(tmpVect));
  tmpVect.clear();
  tmpVect.push_back(QPointF(128,128));
  tmpVect.push_back(QPointF(p3[0],128-p3[1]));
  tmpVect.push_back(QPointF(p0[0],128-p0[1]));
  tmpVect.push_back(QPointF(128,0));
  poly4->setPolygon(QPolygonF(tmpVect));
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

  baseScene.setViewport(0,0,vPWidth, vPHeight);

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
