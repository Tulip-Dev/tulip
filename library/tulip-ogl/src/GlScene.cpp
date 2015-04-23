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
#include <GL/glew.h>

#include <algorithm>
#include <cstdlib>
#include <climits>
#include <cstdio>

#include <tulip/OpenGlConfigManager.h>
#include <tulip/GlScene.h>
#include <tulip/GlTools.h>
#include <tulip/GlXMLTools.h>
#include <tulip/GlLODSceneVisitor.h>
#include <tulip/GlCPULODCalculator.h>
#include <tulip/GlBoundingBoxSceneVisitor.h>
#include <tulip/GlSelectSceneVisitor.h>
#include <tulip/Camera.h>
#include <tulip/GlSimpleEntity.h>
#include <tulip/GlFeedBackRecorder.h>
#include <tulip/GlSVGFeedBackBuilder.h>
#include <tulip/GlEPSFeedBackBuilder.h>
#include <tulip/GlGraphComposite.h>
#include <tulip/GlSceneObserver.h>

using namespace std;

namespace tlp {

/** \brief Storage class for Z ordering
 * Storage class for Z ordering
 */
struct EntityWithDistance {

  EntityWithDistance(const double &dist,EntityLODUnit *entity)
    :distance(dist),entity(entity),isComplexEntity(false),isNode(true) {
  }
  EntityWithDistance(const double &dist,ComplexEntityLODUnit *entity,bool isNode)
    :distance(dist),entity(entity),isComplexEntity(true),isNode(isNode) {
  }

  double distance;
  EntityLODUnit *entity;
  bool isComplexEntity;
  bool isNode;
};

/** \brief Comparator to order entities with Z
 * Comparator to order entities with Z
 */
struct entityWithDistanceCompare {
  static const GlGraphInputData *inputData;
  bool operator()(const EntityWithDistance &e1, const EntityWithDistance &e2 ) const {
    if(e1.distance>e2.distance)
      return true;

    if(e1.distance<e2.distance)
      return false;

    BoundingBox &bb1 = e1.entity->boundingBox;
    BoundingBox &bb2 = e2.entity->boundingBox;

    if(bb1[1][0] - bb1[0][0] > bb2[1][0] - bb2[0][0])
      return false;
    else
      return true;

  }
};

//====================================================

GlScene::GlScene(GlLODCalculator *calculator)
  : backgroundColor(255, 255, 255, 255),viewOrtho(true),glGraphComposite(NULL),
    graphLayer(NULL), clearBufferAtDraw(true), inDraw(false), clearDepthBufferAtDraw(true), clearStencilBufferAtDraw(true) {

  if(calculator!=NULL)
    lodCalculator=calculator;
  else
    lodCalculator=new GlCPULODCalculator();

  lodCalculator->setScene(*this);
}

GlScene::~GlScene() {
  delete lodCalculator;

  for(vector<pair<string,GlLayer *> >::iterator it=layersList.begin(); it!=layersList.end(); ++it) {
    delete it->second;
  }
}

void GlScene::initGlParameters() {
  OpenGlConfigManager::getInst().initExtensions();

  glViewport(viewport[0],viewport[1],viewport[2],viewport[3]);
  glScissor(viewport[0],viewport[1],viewport[2],viewport[3]);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glLineWidth(1.0);
  glPointSize(1.0);
  glEnable(GL_CULL_FACE);
  glEnable(GL_SCISSOR_TEST);
  glDepthFunc(GL_LEQUAL);
  glEnable(GL_DEPTH_TEST);
  glClearStencil(0xFFFF);
  glStencilOp(GL_KEEP,GL_KEEP,GL_REPLACE);
  glEnable(GL_STENCIL_TEST);
  glEnable(GL_NORMALIZE);
  glShadeModel(GL_SMOOTH);
  glPolygonMode(GL_FRONT, GL_FILL);
  glColorMask(1, 1, 1, 1);
  glIndexMask(UINT_MAX);


  if (OpenGlConfigManager::getInst().antiAliasing()) {
    OpenGlConfigManager::getInst().activateAntiAliasing();
  }
  else {
    OpenGlConfigManager::getInst().desactivateAntiAliasing();
  }

  if (clearBufferAtDraw) {
    glClearColor(backgroundColor.getRGL(), backgroundColor.getGGL(), backgroundColor.getBGL(), backgroundColor.getAGL());
    glClear(GL_COLOR_BUFFER_BIT);
  }

  if (clearDepthBufferAtDraw) {
    glClear(GL_DEPTH_BUFFER_BIT);
  }

  if (clearStencilBufferAtDraw) {
    glClear(GL_STENCIL_BUFFER_BIT);
  }

  glDisable(GL_TEXTURE_2D);

#ifndef NDEBUG
  GLenum error = glGetError();

  if ( error != GL_NO_ERROR)
    tlp::warning() << "[OpenGL Error] => " << glGetErrorDescription(error).c_str() << endl << "\tin : " << __PRETTY_FUNCTION__ << endl;

#endif
}

void GlScene::draw() {

  assert(inDraw == false);

  inDraw=true;

  initGlParameters();

  /**********************************************************************
  LOD Compute
  **********************************************************************/
  lodCalculator->clear();
  lodCalculator->setRenderingEntitiesFlag(RenderingAll);

  /**
  * If LOD Calculator need entities to compute LOD, we use visitor system
  */
  if(lodCalculator->needEntities()) {
    GlLODSceneVisitor *lodVisitor;

    lodVisitor = new GlLODSceneVisitor(lodCalculator, NULL);

    for(vector<pair<string,GlLayer *> >::iterator it=layersList.begin(); it!=layersList.end(); ++it) {
      it->second->acceptVisitor(lodVisitor);
    }

    delete lodVisitor;
  }

  lodCalculator->compute(viewport, viewport);
  LayersLODVector &layersLODVector = lodCalculator->getResult();
  BoundingBox sceneBoundingBox = lodCalculator->getSceneBoundingBox();

  Camera *camera;
  // Iterate on Camera
  Camera *oldCamera = NULL;

  vector<LayerLODUnit>::iterator itLayer;

  for(itLayer = layersLODVector.begin(); itLayer!=layersLODVector.end(); ++itLayer) {
    camera = itLayer->camera;
    camera->setSceneRadius(camera->getSceneRadius(), sceneBoundingBox);

    if(camera != oldCamera) {
      camera->initGl();
      oldCamera = camera;
    }

    // Draw simple entities
    vector<SimpleEntityLODUnit>::iterator it;

    if(getGlGraphComposite() && !getGlGraphComposite()->getInputData()->parameters->isElementZOrdered()) {
      for( it = itLayer->simpleEntitiesLODVector.begin(); it!= itLayer->simpleEntitiesLODVector.end(); ++it) {
        if(it->lod<0)
          continue;

        glStencilFunc(GL_LEQUAL,((it->entity))->getStencil(), 0xFFFF);
        (it->entity)->draw(it->lod,camera);
      }
    }
    else {

      entityWithDistanceCompare::inputData=glGraphComposite->getInputData();
      multiset<EntityWithDistance,entityWithDistanceCompare> entitiesSet;
      Coord camPos=camera->getEyes();
      BoundingBox bb;
      double dist;

      for( it = itLayer->simpleEntitiesLODVector.begin(); it!=itLayer->simpleEntitiesLODVector.end(); ++it) {
        if(it->lod<0)
          continue;

        bb = it->boundingBox;
        Coord middle((bb[1] + bb[0])/2.f);
        dist=(((double)middle[0])-((double)camPos[0]))*(((double)middle[0])-((double)camPos[0]));
        dist+=(((double)middle[1])-((double)camPos[1]))*(((double)middle[1])-((double)camPos[1]));
        dist+=(((double)middle[2])-((double)camPos[2]))*(((double)middle[2])-((double)camPos[2]));
        entitiesSet.insert(EntityWithDistance(dist,&(*it)));
      }

      for(multiset<EntityWithDistance,entityWithDistanceCompare>::iterator it=entitiesSet.begin(); it!=entitiesSet.end(); ++it) {
        // Simple entities
        GlSimpleEntity *entity = (((SimpleEntityLODUnit*)(it->entity))->entity);
        glStencilFunc(GL_LEQUAL,entity->getStencil(),0xFFFF);
        entity->draw(it->entity->lod,camera);
      }
    }
  }

  inDraw=false;

  OpenGlConfigManager::getInst().desactivateAntiAliasing();

}

/******************************************************************************
 * GlLayer management functions
*******************************************************************************/

GlLayer *GlScene::createLayer(const std::string &name) {
  GlLayer *oldLayer=getLayer(name);

  if(oldLayer!=NULL) {
    tlp::warning() << "Warning : You have a layer in the scene with same name : old layer will be deleted" << endl;
    removeLayer(oldLayer);
  }

  GlLayer *newLayer=new GlLayer(name);
  layersList.push_back(std::pair<std::string,GlLayer*>(name,newLayer));
  newLayer->setScene(this);

  if (hasOnlookers())
    sendEvent(GlSceneEvent(*this,GlSceneEvent::TLP_ADDLAYER,name,newLayer));

  return newLayer;
}

GlLayer *GlScene::createLayerBefore(const std::string &layerName,const std::string &beforeLayerWithName) {
  GlLayer *newLayer=NULL;
  GlLayer *oldLayer=getLayer(layerName);

  for(vector<pair<string, GlLayer *> >::iterator it=layersList.begin(); it!=layersList.end(); ++it) {
    if(it->first==beforeLayerWithName) {
      newLayer=new GlLayer(layerName);
      layersList.insert(it,pair<string,GlLayer*>(layerName,newLayer));
      newLayer->setScene(this);

      if (hasOnlookers())
        sendEvent(GlSceneEvent(*this,GlSceneEvent::TLP_ADDLAYER,layerName,newLayer));

      if(oldLayer!=NULL) {
        removeLayer(oldLayer);
        tlp::warning() << "Warning : You have a layer in the scene with same name : old layer will be deleted" << endl;
      }

      break;
    }
  }

  return newLayer;
}

GlLayer *GlScene::createLayerAfter(const std::string &layerName,const std::string &afterLayerWithName) {
  GlLayer *newLayer=NULL;
  GlLayer *oldLayer=getLayer(layerName);

  for(vector<pair<string, GlLayer *> >::iterator it=layersList.begin(); it!=layersList.end(); ++it) {
    if(it->first==afterLayerWithName) {
      ++it;
      newLayer=new GlLayer(layerName);
      layersList.insert(it,pair<string,GlLayer*>(layerName,newLayer));
      newLayer->setScene(this);

      if (hasOnlookers())
        sendEvent(GlSceneEvent(*this,GlSceneEvent::TLP_ADDLAYER,layerName,newLayer));

      if(oldLayer!=NULL) {
        tlp::warning() << "Warning : You have a layer in the scene with same name : old layer will be deleted" << endl;
        removeLayer(oldLayer);
      }

      break;
    }
  }

  return newLayer;
}

void GlScene::addExistingLayer(GlLayer *layer) {
  GlLayer *oldLayer=getLayer(layer->getName());

  if(oldLayer!=NULL) {
    tlp::warning() << "Warning : You have a layer in the scene with same name : old layer will be deleted" << endl;
    removeLayer(oldLayer);
  }

  layersList.push_back(std::pair<std::string,GlLayer*>(layer->getName(),layer));
  layer->setScene(this);

  if (hasOnlookers())
    sendEvent(GlSceneEvent(*this,GlSceneEvent::TLP_ADDLAYER,layer->getName(),layer));
}

bool GlScene::addExistingLayerBefore(GlLayer *layer, const std::string &beforeLayerWithName) {
  bool insertionOk=false;
  GlLayer *oldLayer=getLayer(layer->getName());

  for(vector<pair<string, GlLayer *> >::iterator it=layersList.begin(); it!=layersList.end(); ++it) {
    if(it->first==beforeLayerWithName) {
      layersList.insert(it,pair<string,GlLayer*>(layer->getName(),layer));
      layer->setScene(this);

      if (hasOnlookers())
        sendEvent(GlSceneEvent(*this,GlSceneEvent::TLP_ADDLAYER,layer->getName(),layer));

      if(oldLayer!=NULL) {
        tlp::warning() << "Warning : You have a layer in the scene with same name : old layer will be deleted" << endl;
        removeLayer(oldLayer);
      }

      insertionOk=true;
      break;
    }
  }

  return insertionOk;
}

bool GlScene::addExistingLayerAfter(GlLayer *layer, const std::string &afterLayerWithName) {
  bool insertionOk=false;
  GlLayer *oldLayer=getLayer(layer->getName());

  for(vector<pair<string, GlLayer *> >::iterator it=layersList.begin(); it!=layersList.end(); ++it) {
    if(it->first==afterLayerWithName) {
      ++it;
      layersList.insert(it,pair<string,GlLayer*>(layer->getName(),layer));
      layer->setScene(this);

      if (hasOnlookers())
        sendEvent(GlSceneEvent(*this,GlSceneEvent::TLP_ADDLAYER,layer->getName(),layer));

      if(oldLayer!=NULL) {
        tlp::warning() << "Warning : You have a layer in the scene with same name : old layer will be deleted" << endl;
        removeLayer(oldLayer);
      }

      insertionOk=true;
      break;
    }
  }

  return insertionOk;
}

GlLayer *GlScene::getLayer(const std::string& name) {
  for(vector<pair<string, GlLayer *> >::iterator it=layersList.begin(); it!=layersList.end(); ++it) {
    if(it->first==name) {
      return it->second;
    }
  }

  return NULL;
}

void GlScene::removeLayer(const std::string& name,bool deleteLayer) {
  for(vector<pair<string, GlLayer *> >::iterator it=layersList.begin(); it!=layersList.end(); ++it) {
    if(it->first==name) {
      if (hasOnlookers())
        sendEvent(GlSceneEvent(*this,GlSceneEvent::TLP_DELLAYER,name,it->second));

      if(deleteLayer)
        delete it->second;

      layersList.erase(it);
      return;
    }
  }
}

void GlScene::removeLayer(GlLayer *layer,bool deleteLayer) {
  for(vector<pair<string, GlLayer *> >::iterator it=layersList.begin(); it!=layersList.end(); ++it) {
    if(it->second==layer) {
      if (hasOnlookers())
        sendEvent(GlSceneEvent(*this,GlSceneEvent::TLP_DELLAYER,layer->getName(),layer));

      if(deleteLayer)
        delete it->second;

      layersList.erase(it);
      return;
    }
  }
}

void GlScene::notifyModifyLayer(const std::string &name,GlLayer *layer) {
  if (hasOnlookers())
    sendEvent(GlSceneEvent(*this,GlSceneEvent::TLP_MODIFYLAYER,name,layer));
}

void GlScene::notifyModifyEntity(GlSimpleEntity *entity) {
  if (hasOnlookers())
    sendEvent(GlSceneEvent(*this,GlSceneEvent::TLP_MODIFYENTITY, entity));
}

void GlScene::notifyDeletedEntity(GlSimpleEntity *entity) {
  if (hasOnlookers())
    sendEvent(GlSceneEvent(*this,GlSceneEvent::TLP_DELENTITY, entity));
}

void GlScene::centerScene() {
  ajustSceneToSize(viewport[2], viewport[3]);
}

void GlScene::computeAjustSceneToSize(int width, int height, Coord *center, Coord *eye, float *sceneRadius, float *xWhiteFactor, float *yWhiteFactor,BoundingBox *sceneBoundingBox,float *zoomFactor) {
  if(xWhiteFactor)
    *xWhiteFactor=0.;

  if(yWhiteFactor)
    *yWhiteFactor=0.;

  GlBoundingBoxSceneVisitor *visitor;

  if(glGraphComposite)
    visitor=new GlBoundingBoxSceneVisitor(glGraphComposite->getInputData());
  else
    visitor=new GlBoundingBoxSceneVisitor(NULL);

  for(vector<pair<string, GlLayer *> >::iterator it=layersList.begin(); it!=layersList.end(); ++it) {
    if(it->second->getCamera().is3D() && (!it->second->useSharedCamera()))
      it->second->acceptVisitor(visitor);
  }

  BoundingBox boundingBox(visitor->getBoundingBox());
  delete visitor;


  if(!boundingBox.isValid()) {
    if(center)
      *center=Coord(0,0,0);

    if(sceneRadius)
      *sceneRadius=static_cast<float>(sqrt(300.0));

    if(eye && center && sceneRadius) {
      *eye=Coord(0, 0, *sceneRadius);
      *eye=*eye + *center;
    }

    if(zoomFactor) {
      *zoomFactor=1.;
    }


    return;
  }

  Coord maxC(boundingBox[1]);
  Coord minC(boundingBox[0]);

  double dx = maxC[0] - minC[0];
  double dy = maxC[1] - minC[1];
  double dz = maxC[2] - minC[2];

  double dxZoomed=(maxC[0] - minC[0]);
  double dyZoomed=(maxC[1] - minC[1]);

  if(center) {
    *center=(maxC + minC) / 2.f;
  }

  if ((dx==0) && (dy==0) && (dz==0))
    dx = dy = /*dz =*/ 10.0;

  double wdx=width/dxZoomed;
  double hdy=height/dyZoomed;

  float sceneRadiusTmp;

  if(dx<dy) {
    if(wdx<hdy) {
      sceneRadiusTmp=static_cast<float>(dx);

      if(yWhiteFactor)
        *yWhiteFactor=static_cast<float>((1.-(dy/(sceneRadiusTmp*(height/width))))/2.);
    }
    else {
      if (width < height)
        sceneRadiusTmp=static_cast<float>(dx*wdx/hdy);
      else
        sceneRadiusTmp=static_cast<float>(dy);

      if(xWhiteFactor) {
        *xWhiteFactor=static_cast<float>((1.-(dx/sceneRadiusTmp))/2.);
      }
    }
  }
  else {
    if(wdx>hdy) {
      sceneRadiusTmp=static_cast<float>(dy);

      if(xWhiteFactor)
        *xWhiteFactor=static_cast<float>((1.-(dx/(sceneRadiusTmp*(width/height))))/2.);
    }
    else {
      if (height < width)
        sceneRadiusTmp=static_cast<float>(dy*hdy/wdx);
      else
        sceneRadiusTmp=static_cast<float>(dx);

      if(yWhiteFactor)
        *yWhiteFactor=static_cast<float>((1.-(dy/sceneRadiusTmp))/2.);
    }
  }

  if(sceneRadius) {
    *sceneRadius=sceneRadiusTmp;
  }

  if(eye) {
    *eye=Coord(0, 0, sceneRadiusTmp);
    Coord centerTmp=(maxC + minC) / 2.f;
    *eye=*eye + centerTmp;
  }

  if(sceneBoundingBox) {
    *sceneBoundingBox=boundingBox;
  }

  if(zoomFactor) {
    *zoomFactor=1.;
  }

}

void GlScene::ajustSceneToSize(int width, int height) {

  Coord center;
  Coord eye;
  float sceneRadius;
  float zoomFactor;
  BoundingBox sceneBoundingBox;

  computeAjustSceneToSize(width,height, &center, &eye, &sceneRadius,NULL,NULL,&sceneBoundingBox,&zoomFactor);

  for(vector<pair<string,GlLayer *> >::iterator it=layersList.begin(); it!=layersList.end(); ++it) {
    Camera &camera=it->second->getCamera();
    camera.setCenter(center);

    camera.setSceneRadius(sceneRadius,sceneBoundingBox);

    camera.setEyes(eye);
    camera.setUp(Coord(0, 1., 0));
    camera.setZoomFactor(zoomFactor);
  }
}

void GlScene::zoomXY(int step, const int x, const int y) {

  for(vector<pair<string, GlLayer *> >::iterator it=layersList.begin(); it!=layersList.end(); ++it) {
    if(it->second->getCamera().is3D() && (!it->second->useSharedCamera()))
      it->second->getCamera().setZoomFactor(it->second->getCamera().getZoomFactor() * pow(1.1,step));
  }

  if (step < 0) step *= -1;

  int factX = (int)(step*(double(viewport[2])/2.0-x)/ 7.0);
  int factY = (int)(step*(double(viewport[3])/2.0-y)/ 7.0);
  translateCamera(factX,-factY,0);
}

void GlScene::zoom(float,const Coord& dest) {
  for(vector<pair<string, GlLayer *> >::iterator it=layersList.begin(); it!=layersList.end(); ++it) {
    if(it->second->getCamera().is3D() && (!it->second->useSharedCamera())) {
      it->second->getCamera().setEyes(dest + (it->second->getCamera().getEyes() - it->second->getCamera().getCenter()));
      it->second->getCamera().setCenter(dest);
    }
  }
}

void GlScene::translateCamera(const int x, const int y, const int z) {
  for(vector<pair<string, GlLayer *> >::iterator it=layersList.begin(); it!=layersList.end(); ++it) {
    if(it->second->getCamera().is3D() && (!it->second->useSharedCamera())) {
      Coord v1(0, 0, 0);
      Coord v2(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z));
      v1 = it->second->getCamera().screenTo3DWorld(v1);
      v2 = it->second->getCamera().screenTo3DWorld(v2);
      Coord move = v2 - v1;
      it->second->getCamera().setEyes(it->second->getCamera().getEyes() + move);
      it->second->getCamera().setCenter(it->second->getCamera().getCenter() + move);
    }
  }
}

void GlScene::zoom(int step) {
  for(vector<pair<string, GlLayer *> >::iterator it=layersList.begin(); it!=layersList.end(); ++it) {
    if(it->second->getCamera().is3D() && (!it->second->useSharedCamera())) {
      it->second->getCamera().setZoomFactor(it->second->getCamera().getZoomFactor() * pow(1.1, step));
    }
  }
}

void GlScene::zoomFactor(float factor) {
  for(vector<pair<string, GlLayer *> >::iterator it=layersList.begin(); it!=layersList.end(); ++it) {
    if(it->second->getCamera().is3D() && (!it->second->useSharedCamera())) {
      it->second->getCamera().setZoomFactor(it->second->getCamera().getZoomFactor() * factor);
    }
  }
}

void GlScene::rotateScene(const int x, const int y, const int z) {
  for(vector<pair<string, GlLayer *> >::iterator it=layersList.begin(); it!=layersList.end(); ++it) {
    if(it->second->getCamera().is3D() && (!it->second->useSharedCamera())) {
      it->second->getCamera().rotate(static_cast<float>(x/360.0 * M_PI), 1.0f, 0, 0);
      it->second->getCamera().rotate(static_cast<float>(y/360.0 * M_PI), 0, 1.0f, 0);
      it->second->getCamera().rotate(static_cast<float>(z/360.0 * M_PI), 0, 0, 1.0f);
    }
  }
}
//========================================================================================================
void GlScene::glGraphCompositeAdded(GlLayer* layer,GlGraphComposite *glGraphComposite) {
  this->graphLayer=layer;
  this->glGraphComposite=glGraphComposite;
}
//========================================================================================================
void GlScene::glGraphCompositeRemoved(GlLayer* layer,GlGraphComposite *glGraphComposite) {
  if(this->glGraphComposite==glGraphComposite) {
    //fixes unused warning in release
    (void)layer;
    assert(graphLayer==layer);
    graphLayer=NULL;
    this->glGraphComposite=NULL;
  }
}
//========================================================================================================
bool GlScene::selectEntities(RenderingEntitiesFlag type,int x, int y, int w, int h, GlLayer* layer,
                             vector<SelectedEntity>& selectedEntities) {
  if(w==0)
    w=1;

  if(h==0)
    h=1;

  //check if the layer is in the scene
  bool layerInScene=true;

  if(layer) {
    layerInScene=false;

    for(vector<pair<string,GlLayer *> >::const_iterator it=layersList.begin(); it!=layersList.end() && layerInScene; ++it) {
      if(it->second==layer)
        layerInScene=true;
    }
  }

  GlLODCalculator *selectLODCalculator= layerInScene?lodCalculator:lodCalculator->clone();

  selectLODCalculator->setRenderingEntitiesFlag((RenderingEntitiesFlag)(RenderingAll | RenderingWithoutRemove));
  selectLODCalculator->clear();

  //Collect entities if needed
  GlLODSceneVisitor *lodVisitor=new GlLODSceneVisitor(selectLODCalculator,NULL);

  if(layerInScene) {
    if(selectLODCalculator->needEntities()) {
      for(vector<pair<string,GlLayer *> >::iterator it=layersList.begin(); it!=layersList.end(); ++it) {
        it->second->acceptVisitor(lodVisitor);
      }
    }
  }
  else {
    layer->acceptVisitor(lodVisitor);
  }

  delete lodVisitor;

  Vector<int,4> selectionViewport;
  selectionViewport[0]=x;
  selectionViewport[1]=y;
  selectionViewport[2]=w;
  selectionViewport[3]=h;

  glViewport(selectionViewport[0],selectionViewport[1],selectionViewport[2],selectionViewport[3]);

  selectLODCalculator->compute(viewport,selectionViewport);

  LayersLODVector &layersLODVector=selectLODCalculator->getResult();

  for(vector<LayerLODUnit>::const_iterator itLayer=layersLODVector.begin(); itLayer!=layersLODVector.end(); ++itLayer) {
    Camera *camera=itLayer->camera;

    vector<GlGraphComposite*> compositesToRender;

    Vector<int, 4> viewport = camera->getViewport();

    unsigned int size=itLayer->simpleEntitiesLODVector.size();

    if(size==0)
      continue;

    glPushAttrib(GL_ALL_ATTRIB_BITS); //save previous attributes
    glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS); //save previous attributes

    //Allocate memory to store the result of the selection
    GLuint (*selectBuf)[4] = new GLuint[size][4];
    glSelectBuffer(size*4 , (GLuint *)selectBuf);
    //Activate Open Gl Selection mode
    glRenderMode(GL_SELECT);
    glInitNames();
    glPushName(0);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix(); //save previous projection matrix

    //initialize picking matrix
    glLoadIdentity();
    int newX = x + w/2;
    int newY = viewport[3] - (y + h/2);
    gluPickMatrix(newX, newY, w, h, (GLint *)&viewport);


    camera->initProjection(false);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix(); //save previous model view matrix

    camera->initModelView();

    glPolygonMode(GL_FRONT, GL_FILL);
    glDisable(GL_LIGHTING);
    glDisable(GL_BLEND);
    glDisable(GL_STENCIL_TEST);

    map<unsigned int, SelectedEntity> idToEntity;

    if(type & RenderingSimpleEntities) {
      unsigned int id=1;

      for(vector<SimpleEntityLODUnit>::const_iterator it = itLayer->simpleEntitiesLODVector.begin(); it!=itLayer->simpleEntitiesLODVector.end(); ++it) {
        if(it->lod<0)
          continue;

        idToEntity[id]=SelectedEntity(it->entity);
        glLoadName(id);
        ++id;
        it->entity->draw(20.,camera);
      }
    }

    if((type & RenderingNodes) || (type & RenderingEdges)) {
      for(vector<SimpleEntityLODUnit>::const_iterator it = itLayer->simpleEntitiesLODVector.begin(); it!=itLayer->simpleEntitiesLODVector.end(); ++it) {
        if(it->lod<0)
          continue;

        GlGraphComposite *composite=dynamic_cast<GlGraphComposite*>(it->entity);

        if(composite) {
          compositesToRender.push_back(composite);
        }
      }
    }

    glFlush();
    GLint hits = glRenderMode(GL_RENDER);

    while(hits>0) {
      selectedEntities.push_back(idToEntity[selectBuf[hits-1][3]]);
      hits--;
    }

    delete[] selectBuf;

    for(vector<GlGraphComposite*>::const_iterator it=compositesToRender.begin(); it!=compositesToRender.end(); ++it) {
      (*it)->selectEntities(camera,type,x,y,w,h,selectedEntities);
    }

    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glPopClientAttrib();
    glPopAttrib();
  }

  selectLODCalculator->clear();

  if(selectLODCalculator!=lodCalculator)
    delete selectLODCalculator;

  glViewport(viewport[0],viewport[1],viewport[2],viewport[3]);
  return (!selectedEntities.empty());
}
//====================================================
void GlScene::outputSVG(unsigned int size,const string& filename) {
  if(!glGraphComposite)
    return;

  GLint returned;
  GLfloat clearColor[4];
  GLfloat lineWidth;
  GLfloat pointSize;
  GLfloat* buffer = (GLfloat *)calloc(size, sizeof(GLfloat));
  glFeedbackBuffer(size, GL_3D_COLOR, buffer);
  glRenderMode(GL_FEEDBACK);
  glGraphComposite->getInputData()->parameters->setFeedbackRender(true);
  draw();
  glGraphComposite->getInputData()->parameters->setFeedbackRender(false);
  glGetFloatv(GL_COLOR_CLEAR_VALUE, clearColor);
  glGetFloatv(GL_LINE_WIDTH, &lineWidth);
  glGetFloatv(GL_POINT_SIZE, &pointSize);

  glFlush();
  glFinish();
  returned = glRenderMode(GL_RENDER);
  GlSVGFeedBackBuilder builder;
  GlFeedBackRecorder recorder(&builder);
  builder.begin(viewport,clearColor,pointSize,lineWidth);
  recorder.record(false,returned,buffer,layersList[0].second->getCamera().getViewport());
  string str;
  builder.getResult(&str);

  if(!filename.empty()) {
    /* subgraphs drawing disabled
       initMapsSVG(_renderingParameters.getGraph(), &ge); */
    FILE* file ;
#ifndef _MSC_VER
    file = fopen(filename.c_str(), "w");
#else
    fopen_s(&file, filename.c_str(), "w");
#endif

    if (file) {
      fprintf(file, "%s",str.c_str());
      fclose(file);
    }
    else {
      perror(filename.c_str());
    }
  }
}
//====================================================
void GlScene::outputEPS(unsigned int size,const string& filename) {
  if(!glGraphComposite)
    return;

  GLint returned;
  GLfloat clearColor[4];
  GLfloat lineWidth;
  GLfloat pointSize;
  GLfloat* buffer = (GLfloat *)calloc(size, sizeof(GLfloat));
  glFeedbackBuffer(size, GL_3D_COLOR, buffer);
  glRenderMode(GL_FEEDBACK);
  glGraphComposite->getInputData()->parameters->setFeedbackRender(true);
  draw();
  glGraphComposite->getInputData()->parameters->setFeedbackRender(false);

  glGetFloatv(GL_COLOR_CLEAR_VALUE, clearColor);
  glGetFloatv(GL_LINE_WIDTH, &lineWidth);
  glGetFloatv(GL_POINT_SIZE, &pointSize);

  glFlush();
  glFinish();
  returned = glRenderMode(GL_RENDER);
  GlEPSFeedBackBuilder builder;
  GlFeedBackRecorder recorder(&builder);
  builder.begin(viewport,clearColor,pointSize,lineWidth);
  recorder.record(false,returned,buffer,layersList[0].second->getCamera().getViewport());
  string str;
  builder.getResult(&str);

  if(!filename.empty()) {
    FILE* file;
#ifndef _MSC_VER
    file = fopen(filename.c_str(), "w");
#else
    fopen_s(&file, filename.c_str(), "w");
#endif


    if (file) {
      fprintf(file, "%s", str.c_str());
      fclose(file);
    }
    else {
      perror(filename.c_str());
    }
  }
}
//====================================================
unsigned char * GlScene::getImage() {
  unsigned char *image = (unsigned char *)malloc(viewport[2]*viewport[3]*3*sizeof(unsigned char));
  draw();
  glFlush();
  glFinish();
  glPixelStorei(GL_PACK_ALIGNMENT,1);
  glReadPixels(viewport[0],viewport[1],viewport[2],viewport[3],GL_RGB,GL_UNSIGNED_BYTE,image);
  return image;
}
//====================================================
void GlScene::getXML(string &out) {

  out.append("<scene>");

  GlXMLTools::beginDataNode(out);

  GlXMLTools::getXML(out,"viewport",viewport);
  GlXMLTools::getXML(out,"background",backgroundColor);

  GlXMLTools::endDataNode(out);

  GlXMLTools::beginChildNode(out);

  for(vector<pair<string, GlLayer *> >::iterator it=layersList.begin(); it!=layersList.end(); ++it) {

    // Don't save working layers
    if(it->second->isAWorkingLayer())
      continue;

    GlXMLTools::beginChildNode(out,"GlLayer");

    GlXMLTools::createProperty(out, "name", it->first);

    it->second->getXML(out);

    GlXMLTools::endChildNode(out,"GlLayer");
  }

  GlXMLTools::endChildNode(out);

  out.append("</scene>");
}
//====================================================
void GlScene::getXMLOnlyForCameras(string &out) {

  out.append("<scene>");

  GlXMLTools::beginDataNode(out);

  GlXMLTools::getXML(out,"viewport",viewport);
  GlXMLTools::getXML(out,"background",backgroundColor);

  GlXMLTools::endDataNode(out);

  GlXMLTools::beginChildNode(out);

  for(vector<pair<string, GlLayer *> >::iterator it=layersList.begin(); it!=layersList.end(); ++it) {

    // Don't save working layers
    if(it->second->isAWorkingLayer())
      continue;

    GlXMLTools::beginChildNode(out,"GlLayer");

    GlXMLTools::createProperty(out, "name", it->first);

    it->second->getXMLOnlyForCameras(out);

    GlXMLTools::endChildNode(out,"GlLayer");
  }

  GlXMLTools::endChildNode(out);

  out.append("</scene>");
}
//====================================================
void GlScene::setWithXML(string &in, Graph *graph) {

  if(graph)
    glGraphComposite=new GlGraphComposite(graph);

  assert(in.substr(0,7)=="<scene>");
  unsigned int currentPosition=7;
  GlXMLTools::enterDataNode(in,currentPosition);
  GlXMLTools::setWithXML(in,currentPosition,"viewport",viewport);
  GlXMLTools::setWithXML(in,currentPosition,"background",backgroundColor);
  GlXMLTools::leaveDataNode(in,currentPosition);

  GlXMLTools::enterChildNode(in,currentPosition);
  string childName=GlXMLTools::enterChildNode(in,currentPosition);

  while(childName!="") {
    assert(childName=="GlLayer");

    map<string,string> properties=GlXMLTools::getProperties(in,currentPosition);

    assert(properties.count("name")!=0);

    GlLayer *newLayer=getLayer(properties["name"]);

    if(!newLayer) {
      newLayer=createLayer(properties["name"]);
    }

    newLayer->setWithXML(in,currentPosition);

    GlXMLTools::leaveChildNode(in,currentPosition,"GlLayer");

    childName=GlXMLTools::enterChildNode(in,currentPosition);
  }

  if(graph)
    getLayer("Main")->addGlEntity(glGraphComposite,"graph");
}

BoundingBox GlScene::getBoundingBox() {
  return lodCalculator->getSceneBoundingBox();
}

}
