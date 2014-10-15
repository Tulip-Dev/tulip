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

#include <tulip/GlMetaNodeRenderer.h>
#include <tulip/GlGraphInputData.h>
#include <tulip/GlScene.h>
#include <tulip/GlCPULODCalculator.h>
#include <tulip/GlNode.h>
#include <tulip/GlGraphComposite.h>
#include <tulip/Glyph.h>
#include <tulip/Camera.h>

#if defined(_MSC_VER)
#include <Windows.h>
#endif

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

using namespace std;

namespace tlp {

GlMetaNodeRenderer::GlMetaNodeRenderer(GlGraphInputData *inputData) : _inputData(inputData) {}

GlMetaNodeRenderer::~GlMetaNodeRenderer() {
  clearScenes();
}

void GlMetaNodeRenderer::setInputData(GlGraphInputData *inputData) {
  _inputData = inputData;
}

GlGraphInputData *GlMetaNodeRenderer::getInputData() const {
  return _inputData;
}

void GlMetaNodeRenderer::render(node n,float,Camera* camera) {

  bool viewMeta = _inputData->renderingParameters()->isDisplayMetaNodes();//Checks if user wants to see metanode content
  bool viewMetaLabels = _inputData->renderingParameters()->isViewMetaLabel();//Checks if user wants to see metanode content labels

  if (!viewMeta && !viewMetaLabels) {
    return;
  }

  GLint renderMode;
  glGetIntegerv(GL_RENDER_MODE,&renderMode);

  if(renderMode==GL_SELECT)
    return;

  Graph *metaGraph = _inputData->getGraph()->getNodeMetaInfo(n);
  GlScene *scene = NULL;

  if(_metaGraphToSceneMap.count(metaGraph)!=0) {
    scene=_metaGraphToSceneMap[metaGraph];
  }
  else {
    scene = createScene(metaGraph);
    assert(scene!=NULL);
    _metaGraphToSceneMap[metaGraph]=scene;
    metaGraph->addListener(this);
  }

  scene->getGlGraphComposite()->setRenderingParameters(*(_inputData->renderingParameters()));
  int metaStencil = _inputData->renderingParameters()->getMetaNodesStencil();
  int metaSelectedStencil = _inputData->renderingParameters()->getSelectedMetaNodesStencil();
  int metaLabelStencil = _inputData->renderingParameters()->getMetaNodesLabelStencil();
  scene->getGlGraphComposite()->getRenderingParametersPointer()->setDisplayNodes(viewMeta);
  scene->getGlGraphComposite()->getRenderingParametersPointer()->setDisplayEdges(viewMeta);
  scene->getGlGraphComposite()->getRenderingParametersPointer()->setViewEdgeLabel(viewMetaLabels);
  scene->getGlGraphComposite()->getRenderingParametersPointer()->setViewNodeLabel(viewMetaLabels);
  scene->getGlGraphComposite()->getRenderingParametersPointer()->setNodesStencil(metaStencil);
  scene->getGlGraphComposite()->getRenderingParametersPointer()->setEdgesStencil(metaStencil);
  scene->getGlGraphComposite()->getRenderingParametersPointer()->setSelectedNodesStencil(metaSelectedStencil);
  scene->getGlGraphComposite()->getRenderingParametersPointer()->setSelectedEdgesStencil(metaSelectedStencil);
  scene->getGlGraphComposite()->getRenderingParametersPointer()->setNodesLabelStencil(metaLabelStencil);
  scene->getGlGraphComposite()->getRenderingParametersPointer()->setEdgesLabelStencil(metaLabelStencil);

  GlNode glNode(n.id);

  BoundingBox bb=glNode.getBoundingBox(_inputData);
  BoundingBox bbTmp;
  BoundingBox includeBB;
  _inputData->glyphs.get(_inputData->getElementShape()->getNodeValue(n))->getIncludeBoundingBox(includeBB,n);
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

  if(viewport[2]==0 || viewport[3]==0)
    return;

  scene->setViewport(viewport[0],viewport[1],viewport[2],viewport[3]);
  scene->setClearBufferAtDraw(false);
  scene->setClearDepthBufferAtDraw(false);
  scene->setClearStencilBufferAtDraw(false);
  scene->centerScene();

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

  // small hack to avoid z-fighting between the rendering of the metanode content
  // and the rendering of the metanode that occurs afterwards
  glDepthRange(0.1, 1);
  scene->draw();
  // restore default depth range
  glDepthRange(0, 1);

  scene->getGraphLayer()->setCamera(oldCamera);

  camera->getScene()->setClearBufferAtDraw(false);
  camera->getScene()->setClearDepthBufferAtDraw(false);
  camera->getScene()->setClearStencilBufferAtDraw(false);
  camera->getScene()->initGlParameters();
  camera->getScene()->setClearBufferAtDraw(true);
  camera->getScene()->setClearDepthBufferAtDraw(true);
  camera->getScene()->setClearStencilBufferAtDraw(true);
  camera->initGl();
}

GlScene *GlMetaNodeRenderer::createScene(Graph* metaGraph) const {
  GlScene *scene = new GlScene (new GlCPULODCalculator());
  GlLayer* layer=new GlLayer("Main");
  scene->addExistingLayer(layer);
  GlGraphComposite* graphComposite=new GlGraphComposite(metaGraph, scene);
  layer->addGlEntity(graphComposite,"graph");
  return scene;
}

void GlMetaNodeRenderer::treatEvent(const Event &e) {
  if(e.type() == Event::TLP_DELETE) {
    delete _metaGraphToSceneMap[reinterpret_cast<Graph*>(e.sender())];
    _metaGraphToSceneMap.erase(reinterpret_cast<Graph*>(e.sender()));
  }
}

void GlMetaNodeRenderer::clearScenes() {
  for(map<Graph *,GlScene *>::iterator it=_metaGraphToSceneMap.begin(); it!=_metaGraphToSceneMap.end(); ++it) {
    delete it->second;
  }

  _metaGraphToSceneMap.clear();
}

GlScene* GlMetaNodeRenderer::getSceneForMetaGraph(Graph *g) const {
  std::map<Graph *,GlScene *>::const_iterator sceneit(_metaGraphToSceneMap.find(g));
  return (sceneit==_metaGraphToSceneMap.end())?(NULL):(sceneit->second);
}

}
