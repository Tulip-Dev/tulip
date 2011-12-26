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

using namespace std;

namespace tlp {

GlOverviewGraphicsItem::GlOverviewGraphicsItem(GlScene &scene):QGraphicsPixmapItem(),baseScene(scene),vPWidth(128),vPHeight(128),glFrameBuf(NULL){
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

QRectF GlOverviewGraphicsItem::boundingRect() const{
  return QRectF(0,0,128,128);
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

  Camera camera=baseScene.getGraphCamera();
  BoundingBox bbCam=camera.getBoundingBox();

  Vector<int,4> backupViewport=baseScene.getViewport();

  baseScene.setViewport(0,0,vPWidth, vPHeight);

  glPushAttrib(GL_ALL_ATTRIB_BITS);

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();

  vector<Camera> cameras;
  vector<pair<string, GlLayer*> >* layerList=baseScene.getLayersList();
  for(vector<pair<string, GlLayer*> >::iterator it=layerList->begin();it!=layerList->end();++it){
    cameras.push_back((*it).second->getCamera());
  }

  baseScene.centerScene();

  Coord p1=baseScene.getGraphCamera().worldTo2DScreen(Coord(bbCam[0][0],bbCam[0][1],bbCam[0][2]));
  Coord p2=baseScene.getGraphCamera().worldTo2DScreen(Coord(bbCam[1][0],bbCam[1][1],bbCam[1][2]));

  glFrameBuf->bind();

  baseScene.draw();
  glFrameBuf->release();

  unsigned int i=0;
  for(vector<pair<string, GlLayer*> >::iterator it=layerList->begin();it!=layerList->end();++it){
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

  line1->setLine(0,128,p1[0],128-p1[1]);
  line2->setLine(128,128,p2[0],128-p1[1]);
  line3->setLine(128,0,p2[0],128-p2[1]);
  line4->setLine(0,0,p1[0],128-p2[1]);
  QVector<QPointF> tmpVect;
  tmpVect.push_back(QPointF(0,0));
  tmpVect.push_back(QPointF(p1[0],128-p2[1]));
  tmpVect.push_back(QPointF(p1[0],128-p1[1]));
  tmpVect.push_back(QPointF(0,128));
  poly1->setPolygon(QPolygonF(tmpVect));
  tmpVect.clear();
  tmpVect.push_back(QPointF(p1[0],128-p1[1]));
  tmpVect.push_back(QPointF(0,128));
  tmpVect.push_back(QPointF(128,128));
  tmpVect.push_back(QPointF(p2[0],128-p1[1]));
  poly2->setPolygon(QPolygonF(tmpVect));
  tmpVect.clear();
  tmpVect.push_back(QPointF(128,128));
  tmpVect.push_back(QPointF(p2[0],128-p1[1]));
  tmpVect.push_back(QPointF(p2[0],128-p2[1]));
  tmpVect.push_back(QPointF(128,0));
  poly3->setPolygon(QPolygonF(tmpVect));
  tmpVect.clear();
  tmpVect.push_back(QPointF(p2[0],128-p2[1]));
  tmpVect.push_back(QPointF(128,0));
  tmpVect.push_back(QPointF(0,0));
  tmpVect.push_back(QPointF(p1[0],128-p2[1]));
  poly4->setPolygon(QPolygonF(tmpVect));
}

}
