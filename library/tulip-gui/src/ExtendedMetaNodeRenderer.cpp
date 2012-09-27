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
#include <tulip/ExtendedMetaNodeRenderer.h>

#include <tulip/OpenGlConfigManager.h>

#include <tulip/DrawingTools.h>
#include <tulip/BoundingBox.h>

#include <tulip/Camera.h>
#include <tulip/GlGraphInputData.h>
#include <tulip/GlCPULODCalculator.h>
#include <tulip/Glyph.h>
#include <tulip/GlNode.h>
#include <tulip/GlEdge.h>
#include <tulip/GlVertexArrayManager.h>
#include <tulip/GlMainWidget.h>
#include <tulip/NodeLinkDiagramComponent.h>

using namespace std;

namespace tlp {

ExtendedMetaNodeRenderer::ExtendedMetaNodeRenderer(GlGraphInputData *inputData):inputData(inputData) {
}

ExtendedMetaNodeRenderer::~ExtendedMetaNodeRenderer() {
  for(map<Graph *,GlMainView *>::iterator it=metaGraphToViewMap.begin(); it!=metaGraphToViewMap.end(); ++it) {
    delete (*it).second;
  }
}

void ExtendedMetaNodeRenderer::render(node n,float,Camera* camera) {

  GLint renderMode;
  glGetIntegerv(GL_RENDER_MODE,&renderMode);

  if(renderMode==GL_SELECT)
    return;

  GlMainView *view;
  Graph *metaGraph = inputData->getGraph()->getNodeMetaInfo(n);

  if(metaGraphToViewMap.count(metaGraph)!=0) {
    view=metaGraphToViewMap[metaGraph];
  }
  else {
    view=new NodeLinkDiagramComponent;
    //view->setOverviewVisible(false);
    view->setupUi();
    view->setGraph(metaGraph);
    view->setOverviewVisible(false);
    DataSet dataSet;
    dataSet.set("overviewVisible",false);
    view->getGlMainWidget()->getScene()->setCalculator(new GlCPULODCalculator());
    view->setState(dataSet);
    metaGraphToViewMap[metaGraph]=view;

    metaGraph->addListener(this);
  }

  GlScene *scene=view->getGlMainWidget()->getScene();

  GlNode glNode(n.id);

  BoundingBox bb=glNode.getBoundingBox(inputData);
  BoundingBox bbTmp;
  BoundingBox includeBB;
  inputData->glyphs.get(inputData->getElementShape()->getNodeValue(n))->getIncludeBoundingBox(includeBB,n);
  bbTmp[0]=bb.center()-Coord((bb.width()/2.f)*(includeBB[0][0]*-2.f),(bb.height()/2.f)*(includeBB[0][1]*-2.f),(bb.depth()/2.f)*(includeBB[0][2]*-2.f));
  bbTmp[1]=bb.center()+Coord((bb.width()/2.f)*(includeBB[1][0]*2.f),(bb.height()/2.f)*(includeBB[1][1]*2.f),(bb.depth()/2.f)*(includeBB[1][2]*2.f));
  bb=bbTmp;

  Coord eyeDirection=camera->getEyes()-camera->getCenter();
  eyeDirection = eyeDirection/eyeDirection.norm();

  Camera newCamera2=*camera;
  newCamera2.setEyes(newCamera2.getCenter()+Coord(0,0,1)*(newCamera2.getEyes()-newCamera2.getCenter()).norm());
  newCamera2.setUp(Coord(0,1,0));

  Coord first=newCamera2.worldTo2DScreen((Coord)(bb[0]));
  Coord second=newCamera2.worldTo2DScreen((Coord)(bb[1]));

  Coord center=camera->worldTo2DScreen((Coord)(bb[0]+bb[1])/2.f);
  Coord size=second-first;

  Vector<int,4> viewport;
  viewport[0]=center[0]-size[0]/2;
  viewport[1]=center[1]-size[1]/2;
  viewport[2]=size[0];
  viewport[3]=size[1];

  viewport[0]=camera->getViewport()[0]+viewport[0]-viewport[2]/2;
  viewport[1]=camera->getViewport()[1]+viewport[1]-viewport[3]/2;
  viewport[2]*=2;
  viewport[3]*=2;

  if(viewport[2]==0)
    viewport[2]=1;

  if(viewport[3]==0)
    viewport[3]=1;

  view->getGlMainWidget()->resizeGL(camera->getViewport()[2],camera->getViewport()[3]);
  scene->setViewport(viewport[0],viewport[1],viewport[2],viewport[3]);

  scene->setClearBufferAtDraw(false);
  view->getGlMainWidget()->centerScene();

  float baseNorm=(scene->getGraphLayer()->getCamera().getEyes()-scene->getGraphLayer()->getCamera().getCenter()).norm();
  Camera newCamera=scene->getGraphLayer()->getCamera();
  Camera *oldCamera=new Camera(scene,true);
  newCamera.setScene(scene);
  *oldCamera=newCamera;
  newCamera.setScene(scene);
  newCamera.setUp(camera->getUp());
  newCamera.setEyes(newCamera.getCenter()+(eyeDirection*baseNorm));
  newCamera.setZoomFactor(newCamera.getZoomFactor()*0.5);
  scene->getGraphLayer()->setSharedCamera(&newCamera);

  scene->draw();

  scene->getGraphLayer()->setCamera(oldCamera);

  camera->getScene()->setClearBufferAtDraw(false);
  camera->getScene()->initGlParameters();
  camera->getScene()->setClearBufferAtDraw(true);
  camera->initGl();
}

void ExtendedMetaNodeRenderer::treatEvent(const Event &e) {
  if(e.type() == Event::TLP_DELETE) {
    delete metaGraphToViewMap[(Graph*)(e.sender())];
    metaGraphToViewMap.erase((Graph*)(e.sender()));
  }
}

}
