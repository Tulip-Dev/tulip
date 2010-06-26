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
#include "tulip/OpenGlConfigManager.h"

#include "tulip/GlScene.h"

#include <iostream>
#include <cstdlib>
#include <climits>

#include "tulip/GlLODSceneVisitor.h"
#include "tulip/TextRenderer.h"
#include "tulip/OcclusionTest.h"
#include "tulip/GlCPULODCalculator.h"
#include "tulip/GlBoundingBoxSceneVisitor.h"
#include "tulip/GlSelectSceneVisitor.h"
#include "tulip/Camera.h"
#include "tulip/GlSimpleEntity.h"
#include "tulip/GlComplexeEntity.h"
#include "tulip/GlNode.h"
#include "tulip/GlEdge.h"
#include "tulip/GlFeedBackRecorder.h"
#include "tulip/GlSVGFeedBackBuilder.h"
#include "tulip/GlEPSFeedBackBuilder.h"
#include "tulip/GlPointManager.h"

using namespace std;

namespace tlp {

    struct entityWithDistanceCompare {
	static GlGraphInputData *inputData;
        bool operator()(const EntityWithDistance &e1, const EntityWithDistance &e2 ) const{
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
  class GreatThanNode {
  public:
    DoubleProperty *metric;
    bool operator() (node n1,node n2)  {
      return (metric->getNodeValue(n1) > metric->getNodeValue(n2));
    }
  };
  //====================================================
  class GreatThanEdge {
  public:
    DoubleProperty *metric;
    Graph *sp;
    bool operator() (edge e1,edge e2) {
      return (metric->getEdgeValue(e1) > metric->getEdgeValue(e2));
    }
  };
  //====================================================
#ifdef _WIN32
#ifdef DLL_EXPORT
  GlGraphInputData *entityWithDistanceCompare::inputData=NULL;
#endif
#else
  GlGraphInputData *entityWithDistanceCompare::inputData=NULL;
#endif

  GlScene::GlScene(GlLODCalculator *calculator):backgroundColor(255, 255, 255, 255),viewLabel(true),viewOrtho(true),glGraphComposite(NULL) {
    Camera camera(this,false);

	if(calculator!=NULL)
      lodCalculator=calculator;
	else
      lodCalculator=new GlCPULODCalculator();
    lodCalculator->setScene(this);
  }

  GlScene::~GlScene(){
    delete lodCalculator;
    for(vector<pair<string,GlLayer *> >::iterator it=layersList.begin();it!=layersList.end();++it) {
      delete (*it).second;
    }
  }

  void GlScene::initGlParameters() {
    OpenGlConfigManager::getInst().checkDrivers();
    if(!OpenGlConfigManager::getInst().glewIsInit())
      OpenGlConfigManager::getInst().initGlew();

	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
	bool antialiased = true;
	if(glGraphComposite) {
      antialiased=glGraphComposite->getInputData()->parameters->isAntialiased();
	}

	if(antialiased) {
      glEnable(GL_LINE_SMOOTH);
      glHint(GL_LINE_SMOOTH_HINT,GL_DONT_CARE);
      glShadeModel(GL_SMOOTH);
	}else{
      glDisable(GL_LINE_SMOOTH);
	}

	glDisable(GL_POINT_SMOOTH);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glLineWidth(1.0);
	glPointSize(1.0);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glEnable(GL_NORMALIZE);
	glDepthFunc(GL_LEQUAL );
	glPolygonMode(GL_FRONT, GL_FILL);
	glColorMask(1, 1, 1, 1);
	glEnable(GL_BLEND);
	glIndexMask(UINT_MAX);
	glClearColor(backgroundColor.getRGL(), backgroundColor.getGGL(), backgroundColor.getBGL(), backgroundColor.getAGL());
	glClearStencil(0xFFFF);
	glStencilOp(GL_KEEP,GL_KEEP,GL_REPLACE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glDisable(GL_TEXTURE_2D);

	GLenum error = glGetError();
	if ( error != GL_NO_ERROR)
      cerr << "[OpenGL Error] => " << gluErrorString(error) << endl << "\tin : " << __PRETTY_FUNCTION__ << endl;
  }

  void GlScene::prerenderMetaNodes(){
	if(!glGraphComposite)
      return;
	//prerender metanodes if need
	set<node> metaNodes=glGraphComposite->getMetaNodes();
	if(!metaNodes.empty() && glGraphComposite->getInputData()->getMetaNodeRenderer()->havePrerender()){

	  initGlParameters();

	  GlLODCalculator *newLodCalculator=lodCalculator->clone();
      newLodCalculator->setRenderingEntitiesFlag(RenderingAll);
	  newLodCalculator->beginNewCamera(getLayer("Main")->getCamera());
	  GlNode glNode(0);
	  for(set<node>::iterator it=metaNodes.begin();it!=metaNodes.end();++it){
	    glNode.id=(*it).id;
	    newLodCalculator->addNodeBoundingBox((*it).id,glNode.getBoundingBox(glGraphComposite->getInputData()));
	  }
      newLodCalculator->compute(viewport,viewport);

      LayersLODVector *layersLODVector=newLodCalculator->getResult();
      LayerLODUnit *layerLODUnit=&(layersLODVector->front());

      for(vector<ComplexEntityLODUnit>::iterator it=layerLODUnit->nodesLODVector.begin();it!=layerLODUnit->nodesLODVector.end();++it) {
        if((*it).lod>=0)
          glGraphComposite->getInputData()->getMetaNodeRenderer()->prerender(node((*it).id),(*it).lod,getLayer("Main")->getCamera());
	  }
	  delete newLodCalculator;
	}
  }

  void drawLabelsForComplexEntities(bool drawSelected,GlGraphComposite *glGraphComposite,TextRenderer *fontRenderer,
                                    OcclusionTest *occlusionTest,LayerLODUnit *layerLODUnit){
    Graph *graph=glGraphComposite->getInputData()->getGraph();
    BooleanProperty *selectionProperty=glGraphComposite->getInputData()->elementSelected;
    DoubleProperty *metric=NULL;
    if(graph->existProperty("viewMetric"))
      metric=graph->getProperty<DoubleProperty>("viewMetric");
    vector<node> nodesMetricOrdered;
    vector<edge> edgesMetricOrdered;
    GlNode glNode(0);
    GlEdge glEdge(0);
    GlMetaNode glMetaNode(0);

    // Draw Labels for Nodes
    if(glGraphComposite->getInputData()->parameters->isViewNodeLabel()) {
      node n;
      for(vector<ComplexEntityLODUnit>::iterator it=layerLODUnit->nodesLODVector.begin();it!=layerLODUnit->nodesLODVector.end();++it) {
        if((*it).lod<0)
          continue;

        n.id=(*it).id;

        if(selectionProperty->getNodeValue(n)==drawSelected){
          if(!glGraphComposite->getInputData()->parameters->isElementOrdered() || !metric){
            // Not metric ordered
            if(!graph->isMetaNode(n)){
              glNode.id=n.id;
              glNode.drawLabel(occlusionTest,fontRenderer,glGraphComposite->getInputData());
            }else{
              glMetaNode.id=n.id;
              glMetaNode.drawLabel(occlusionTest,fontRenderer,glGraphComposite->getInputData());
            }
          }else{
            // Metric ordered
            nodesMetricOrdered.push_back(n);
          }
        }
      }
      // If not Metric ordered : a this point selected nodes are draw
      if(glGraphComposite->getInputData()->parameters->isElementOrdered()){
        // Draw selected nodes label with metric ordering
        GreatThanNode ltn;
        ltn.metric=metric;
        sort(nodesMetricOrdered.begin(),nodesMetricOrdered.end(),ltn);
        for(vector<node>::iterator it=nodesMetricOrdered.begin();it!=nodesMetricOrdered.end();++it){
          if(!graph->isMetaNode(*it)){
            glNode.id=(*it).id;
            glNode.drawLabel(occlusionTest,fontRenderer,glGraphComposite->getInputData());
          }else{
            glMetaNode.id=(*it).id;
            glMetaNode.drawLabel(occlusionTest,fontRenderer,glGraphComposite->getInputData());
          }
        }
      }
    }
    // Draw Labels for Edges
    if(glGraphComposite->getInputData()->parameters->isViewEdgeLabel()) {
      edge e;
      for(vector<ComplexEntityLODUnit>::iterator it=layerLODUnit->edgesLODVector.begin();it!=layerLODUnit->edgesLODVector.end();++it) {
        if((*it).lod<0)
          continue;

        e.id=(*it).id;
        if(selectionProperty->getEdgeValue(e) == drawSelected){
          if(!glGraphComposite->getInputData()->parameters->isElementOrdered() || !metric){
            // Not metric ordered
            glEdge.id=e.id;
            glEdge.drawLabel(occlusionTest,fontRenderer,glGraphComposite->getInputData());
          }else{
            // Metric ordered
            edgesMetricOrdered.push_back(e);
          }
        }
      }
      // If not Metric ordered : a this point selected edges are draw
      if(glGraphComposite->getInputData()->parameters->isElementOrdered()){
        // Draw selected edges label with metric ordering
        GreatThanEdge lte;
        lte.metric=metric;
        sort(edgesMetricOrdered.begin(),edgesMetricOrdered.end(),lte);
        for(vector<edge>::iterator it=edgesMetricOrdered.begin();it!=edgesMetricOrdered.end();++it){
          glEdge.id=(*it).id;
          glEdge.drawLabel(occlusionTest,fontRenderer,glGraphComposite->getInputData());
        }
      }
    }
  }


  void GlScene::draw() {
	initGlParameters();

    lodCalculator->clear();

    lodCalculator->setRenderingEntitiesFlag(RenderingAll);

    /*
      If LOD Calculator need entities to compute LOD, we use visitor system
    */
    if(lodCalculator->needEntities()){
      GlLODSceneVisitor *lodVisitor;
      if(glGraphComposite)
        lodVisitor=new GlLODSceneVisitor(lodCalculator,glGraphComposite->getInputData());
      else
        lodVisitor=new GlLODSceneVisitor(lodCalculator,NULL);

      for(vector<pair<string,GlLayer *> >::iterator it=layersList.begin();it!=layersList.end();++it) {
        (*it).second->acceptVisitor(lodVisitor);
      }
      delete lodVisitor;
    }

    lodCalculator->compute(viewport,viewport);

    LayersLODVector *layersLODVector=lodCalculator->getResult();

	TextRenderer fontRenderer;
	OcclusionTest occlusionTest;

	Camera *camera;
	Graph *graph=NULL;
    if(glGraphComposite){
      graph=glGraphComposite->getInputData()->graph;
    }

	GlNode glNode(0);
	GlMetaNode glMetaNode(0);
	GlEdge glEdge(0);

    // Iterate on Camera
    Camera *oldCamera=NULL;
    for(vector<LayerLODUnit>::iterator itLayer=layersLODVector->begin();itLayer!=layersLODVector->end();++itLayer){
      camera=(Camera*)((*itLayer).camera);
      if(camera!=oldCamera){
        camera->initGl();
        oldCamera=camera;
      }

      // Init GlPointManager for a new rendering pass
      GlPointManager::getInst().beginRendering();

      bool zOrdering=false;
      if(glGraphComposite)
        zOrdering=glGraphComposite->getRenderingParameters().isElementZOrdered();

      if(!zOrdering){
        // If elements are not zOrdered

        // Draw simple entities
        for(vector<SimpleEntityLODUnit>::iterator it=(*itLayer).simpleEntitiesLODVector.begin();it!=(*itLayer).simpleEntitiesLODVector.end();++it) {
          if((*it).lod<0)
            continue;

          glStencilFunc(GL_LEQUAL,((GlSimpleEntity*)((*it).id))->getStencil(),0xFFFF);
          ((GlSimpleEntity*)((*it).id))->draw((*it).lod,camera);
        }

        // Draw complex entities
        if(glGraphComposite){
          for(vector<ComplexEntityLODUnit>::iterator it=(*itLayer).nodesLODVector.begin();it!=(*itLayer).nodesLODVector.end();++it) {
            if((*it).lod<=0)
              continue;

            if(!graph->isMetaNode(node((*it).id))){
              glNode.id=(*it).id;
              glNode.draw((*it).lod,glGraphComposite->getInputData(),camera);
            }else{
              glMetaNode.id=(*it).id;
              glMetaNode.draw((*it).lod,glGraphComposite->getInputData(),camera);
            }
          }
          for(vector<ComplexEntityLODUnit>::iterator it=(*itLayer).edgesLODVector.begin();it!=(*itLayer).edgesLODVector.end();++it) {
            if((*it).lod<=0)
              continue;

            glEdge.id=(*it).id;
            glEdge.draw((*it).lod,glGraphComposite->getInputData(),camera);
          }
        }

      }else{
        // If elements are zOrdered

        entityWithDistanceCompare::inputData=glGraphComposite->getInputData();
        multiset<EntityWithDistance,entityWithDistanceCompare> entitiesSet;
        Coord camPos=camera->getEyes();
        BoundingBox bb;
        double dist;

        // Colect simple entities
        for(vector<SimpleEntityLODUnit>::iterator it=(*itLayer).simpleEntitiesLODVector.begin();it!=(*itLayer).simpleEntitiesLODVector.end();++it) {
          if((*it).lod<0)
            continue;

          bb = (*it).boundingBox;
          Coord middle((bb[1] + bb[0])/2.f);
          dist=(((double)middle[0])-((double)camPos[0]))*(((double)middle[0])-((double)camPos[0]));
          dist+=(((double)middle[1])-((double)camPos[1]))*(((double)middle[1])-((double)camPos[1]));
          dist+=(((double)middle[2])-((double)camPos[2]))*(((double)middle[2])-((double)camPos[2]));
          entitiesSet.insert(EntityWithDistance(dist,&(*it)));
        }

        // Colect complex entities
        if(glGraphComposite){
          for(vector<ComplexEntityLODUnit>::iterator it=(*itLayer).nodesLODVector.begin();it!=(*itLayer).nodesLODVector.end();++it) {
            if((*it).lod<0)
              continue;

            bb=(*it).boundingBox;
            Coord middle((bb[1]+bb[0])/2.f);
            dist=(((double)middle[0])-((double)camPos[0]))*(((double)middle[0])-((double)camPos[0]));
            dist+=(((double)middle[1])-((double)camPos[1]))*(((double)middle[1])-((double)camPos[1]));
            dist+=(((double)middle[2])-((double)camPos[2]))*(((double)middle[2])-((double)camPos[2]));
            entitiesSet.insert(EntityWithDistance(dist,&(*it),true));
          }

          for(vector<ComplexEntityLODUnit>::iterator it=(*itLayer).edgesLODVector.begin();it!=(*itLayer).edgesLODVector.end();++it) {
            if((*it).lod<0)
              continue;

            bb = (*it).boundingBox;
            Coord middle((bb[0] + bb[1])/2.f);
            dist=(((double)middle[0])-((double)camPos[0]))*(((double)middle[0])-((double)camPos[0]));
            dist+=(((double)middle[1])-((double)camPos[1]))*(((double)middle[1])-((double)camPos[1]));
            dist+=(((double)middle[2])-((double)camPos[2]))*(((double)middle[2])-((double)camPos[2]));
            entitiesSet.insert(EntityWithDistance(dist,&(*it),false));
          }
        }

        // Draw
        for(set<EntityWithDistance,entityWithDistanceCompare>::iterator it=entitiesSet.begin();it!=entitiesSet.end();++it){
          if(!(*it).isComplexEntity){
            // Simple entities
            GlSimpleEntity *entity=(GlSimpleEntity*)(((SimpleEntityLODUnit*)((*it).entity))->id);
            glStencilFunc(GL_LEQUAL,entity->getStencil(),0xFFFF);
            entity->draw((*it).entity->lod,camera);
          }else{
            // Complex entities
            ComplexEntityLODUnit *entity=(ComplexEntityLODUnit*)((*it).entity);
            if((*it).isNode){
              if(!graph->isMetaNode(node(entity->id))){
                glNode.id=entity->id;
                glNode.draw(entity->lod,glGraphComposite->getInputData(),camera);
              }else{
                glMetaNode.id=entity->id;
                glMetaNode.draw(entity->lod,glGraphComposite->getInputData(),camera);
              }
            }else{
              glEdge.id=entity->id;
              glEdge.draw(entity->lod,glGraphComposite->getInputData(),camera);
            }
          }
        }
      }

      // End rendering of GlPointManager
      GlPointManager::getInst().endRendering();

      /*
        Label draw
      */
      if(viewLabel && glGraphComposite) {
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glDisable(GL_LIGHTING);
        glDepthFunc(GL_ALWAYS );
        glDisable(GL_CULL_FACE);
        glDisable(GL_COLOR_MATERIAL);

        // Draw Labels for selected entities
        drawLabelsForComplexEntities(true,glGraphComposite,&fontRenderer,&occlusionTest,&(*itLayer));

        // Draw Labels for unselected entities
        drawLabelsForComplexEntities(false,glGraphComposite,&fontRenderer,&occlusionTest,&(*itLayer));

        glPopAttrib();
      }
	}
  }

  void GlScene::addLayer(GlLayer *layer) {
	layersList.push_back(std::pair<std::string,GlLayer*>(layer->getName(),layer));
	layer->setScene(this);
	notifyAddLayer(this,layer->getName(),layer);
  }

  bool GlScene::insertLayerBefore(GlLayer *layer,const string &name) {
    for(vector<pair<string, GlLayer *> >::iterator it=layersList.begin();it!=layersList.end();++it){
      if((*it).first==name){
        layersList.insert(it,pair<string,GlLayer*>(layer->getName(),layer));
        layer->setScene(this);
        notifyAddLayer(this,layer->getName(),layer);
        return true;
      }
    }
    return false;
  }

  bool GlScene::insertLayerAfter(GlLayer *layer,const string &name) {
    for(vector<pair<string, GlLayer *> >::iterator it=layersList.begin();it!=layersList.end();++it){
      if((*it).first==name){
        ++it;
        layersList.insert(it,pair<string,GlLayer*>(layer->getName(),layer));
        layer->setScene(this);
        notifyAddLayer(this,layer->getName(),layer);
        return true;
      }
    }
    return false;
  }

  void GlScene::removeLayer(const std::string& name,bool deleteLayer){
    for(vector<pair<string,GlLayer *> >::iterator it=layersList.begin();it!=layersList.end();++it){
      if((*it).first==name){
        GlLayer *layer=(*it).second;
        layersList.erase(it);
        notifyDelLayer(this,name,layer);
        if(deleteLayer)
          delete layer;
        return;
      }
    }
  }

  void GlScene::removeLayer(GlLayer *layer,bool deleteLayer){
    for(vector<pair<string, GlLayer *> >::iterator it=layersList.begin();it!=layersList.end();++it){
      if((*it).second==layer){
        GlLayer *layer=(*it).second;
        layersList.erase(it);
        notifyDelLayer(this,layer->getName(),layer);
        if(deleteLayer)
          delete layer;
        return;
      }
    }
  }

  void GlScene::centerScene() {
    ajustSceneToSize(viewport[2], viewport[3]);
  }

  void GlScene::computeAjustSceneToSize(int width, int height, Coord *center, Coord *eye, float *sceneRadius, float *xWhiteFactor, float *yWhiteFactor){
	if(xWhiteFactor)
      *xWhiteFactor=0.;
	if(yWhiteFactor)
      *yWhiteFactor=0.;

	GlBoundingBoxSceneVisitor *visitor;
	if(glGraphComposite)
      visitor=new GlBoundingBoxSceneVisitor(glGraphComposite->getInputData());
	else
      visitor=new GlBoundingBoxSceneVisitor(NULL);

	for(vector<pair<string, GlLayer *> >::iterator it=layersList.begin();it!=layersList.end();++it) {
      if((*it).second->getCamera()->is3D() && (!(*it).second->useSharedCamera()))
        (*it).second->acceptVisitor(visitor);
	}

    BoundingBox boundingBox(visitor->getBoundingBox());
    delete visitor;


    if(!boundingBox.isValid()){
      *center=Coord(0,0,0);
      *sceneRadius=sqrt(300);

      *eye=Coord(0, 0, *sceneRadius);
      *eye=*eye + *center;
      return;
    }

    Coord maxC(boundingBox[1]);
    Coord minC(boundingBox[0]);

	double dx = maxC[0] - minC[0];
	double dy = maxC[1] - minC[1];
	double dz = maxC[2] - minC[2];

	Coord centerTmp=(maxC + minC)/2.f;
	if(center)
      *center=centerTmp;

	if ((dx==0) && (dy==0) && (dz==0))
      dx = dy = dz = 10.0;

	double wdx=width/dx;
	double hdy=height/dy;

	float sceneRadiusTmp;
	if(dx<dy){
      if(wdx<hdy){
        sceneRadiusTmp=dx;
        if(yWhiteFactor)
          *yWhiteFactor=(1.-(dy/(sceneRadiusTmp*(height/width))))/2.;
      }else{
        if (width < height)
          sceneRadiusTmp=dx*wdx/hdy;
        else
          sceneRadiusTmp=dy;
        if(xWhiteFactor){
          *xWhiteFactor=(1.-(dx/sceneRadiusTmp))/2.;
        }
      }
	}else{
      if(wdx>hdy){
        sceneRadiusTmp=dy;
        if(xWhiteFactor)
          *xWhiteFactor=(1.-(dx/(sceneRadiusTmp*(width/height))))/2.;
      }else{
        if (height < width)
          sceneRadiusTmp=dy*hdy/wdx;
        else
          sceneRadiusTmp=dx;
        if(yWhiteFactor)
          *yWhiteFactor=(1.-(dy/sceneRadiusTmp))/2.;
      }
	}

	if(sceneRadius)
      *sceneRadius=sceneRadiusTmp;

	if(eye){
      *eye=Coord(0, 0, sceneRadiusTmp);
      *eye=*eye + centerTmp;
	}

  }

  void GlScene::ajustSceneToSize(int width, int height){

	Coord center;
	Coord eye;
	float sceneRadius;

	computeAjustSceneToSize(width,height, &center, &eye, &sceneRadius,NULL,NULL);

	for(vector<pair<string,GlLayer *> >::iterator it=layersList.begin();it!=layersList.end();++it) {
      Camera* camera=(*it).second->getCamera();
      camera->setCenter(center);

      camera->setSceneRadius(sceneRadius);

      camera->setEyes(eye);
      camera->setUp(Coord(0, 1., 0));
      camera->setZoomFactor(1.);
	}
  }

  void GlScene::zoomXY(int step, const int x, const int y) {

	for(vector<pair<string, GlLayer *> >::iterator it=layersList.begin();it!=layersList.end();++it) {
      if((*it).second->getCamera()->is3D() && (!(*it).second->useSharedCamera()))
        (*it).second->getCamera()->setZoomFactor((*it).second->getCamera()->getZoomFactor() * pow(1.1,step));
	}
	if (step < 0) step *= -1;
	int factX = (int)(step*(double(viewport[2])/2.0-x)/ 7.0);
	int factY = (int)(step*(double(viewport[3])/2.0-y)/ 7.0);
	translateCamera(factX,-factY,0);
  }

  void GlScene::zoom(float factor,const Coord& dest) {
	for(vector<pair<string, GlLayer *> >::iterator it=layersList.begin();it!=layersList.end();++it) {
      if((*it).second->getCamera()->is3D() && (!(*it).second->useSharedCamera())) {
        (*it).second->getCamera()->setEyes(dest + ((*it).second->getCamera()->getEyes() - (*it).second->getCamera()->getCenter()));
        (*it).second->getCamera()->setCenter(dest);
      }
	}
  }

  void GlScene::translateCamera(const int x, const int y, const int z) {
	for(vector<pair<string, GlLayer *> >::iterator it=layersList.begin();it!=layersList.end();++it) {
      if((*it).second->getCamera()->is3D() && (!(*it).second->useSharedCamera())) {
        Coord v1(0, 0, 0);
        Coord v2(x, y, z);
        v1 = (*it).second->getCamera()->screenTo3DWorld(v1);
        v2 = (*it).second->getCamera()->screenTo3DWorld(v2);
        Coord move = v2 - v1;
        (*it).second->getCamera()->setEyes((*it).second->getCamera()->getEyes() + move);
        (*it).second->getCamera()->setCenter((*it).second->getCamera()->getCenter() + move);
      }
	}
  }

  void GlScene::zoom(int step) {
	for(vector<pair<string, GlLayer *> >::iterator it=layersList.begin();it!=layersList.end();++it) {
      if((*it).second->getCamera()->is3D() && (!(*it).second->useSharedCamera())) {
        (*it).second->getCamera()->setZoomFactor((*it).second->getCamera()->getZoomFactor() * pow(1.1, step));
      }
	}
  }

  void GlScene::rotateScene(const int x, const int y, const int z) {
	for(vector<pair<string, GlLayer *> >::iterator it=layersList.begin();it!=layersList.end();++it) {
      if((*it).second->getCamera()->is3D() && (!(*it).second->useSharedCamera())) {
        (*it).second->getCamera()->rotate(float(x)/360.0 * M_PI, 1.0, 0, 0);
        (*it).second->getCamera()->rotate(float(y)/360.0 * M_PI, 0, 1.0, 0);
        (*it).second->getCamera()->rotate(float(z)/360.0 * M_PI, 0, 0, 1.0);
      }
	}
  }

  bool GlScene::selectEntities(RenderingEntitiesFlag type,int x, int y, int w, int h, GlLayer* layer, vector<unsigned long>& selectedEntities) {
	if(w==0)
      w=1;
	if(h==0)
      h=1;

    GlLODCalculator *selectLODCalculator;

    //check if the layer is in scene
    bool layerInScene=true;
    if(layer){
      layerInScene=false;
      for(vector<pair<string,GlLayer *> >::iterator it=layersList.begin();it!=layersList.end() && layerInScene;++it){
        if((*it).second==layer)
          layerInScene=true;
      }
    }
    if(layerInScene){
      selectLODCalculator=lodCalculator;
    }else{
      selectLODCalculator=lodCalculator->clone();
    }
    selectLODCalculator->setRenderingEntitiesFlag((RenderingEntitiesFlag)(RenderingAll | RenderingWithoutRemove));

    //Collect entities if need
    GlLODSceneVisitor *lodVisitor;
    if(glGraphComposite)
      lodVisitor=new GlLODSceneVisitor(selectLODCalculator,glGraphComposite->getInputData());
    else
      lodVisitor=new GlLODSceneVisitor(selectLODCalculator,NULL);

    if(layerInScene){
      if(selectLODCalculator->needEntities()){
        for(vector<pair<string,GlLayer *> >::iterator it=layersList.begin();it!=layersList.end();++it) {
          (*it).second->acceptVisitor(lodVisitor);
        }
      }
    }else{
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

    LayersLODVector *layersLODVector=selectLODCalculator->getResult();

    for(vector<LayerLODUnit>::iterator itLayer=layersLODVector->begin();itLayer!=layersLODVector->end();++itLayer){
      Camera *camera=(Camera*)((*itLayer).camera);

      Vector<int, 4> viewport = camera->getViewport();

      unsigned int size;
      size=(*itLayer).simpleEntitiesLODVector.size()+(*itLayer).nodesLODVector.size()+(*itLayer).edgesLODVector.size();

      if(size==0)
        continue;

      glPushAttrib(GL_ALL_ATTRIB_BITS); //save previous attributes
      glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS); //save previous attributes

      //Allocate memory to store the result oh the selection
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
      x += w/2;
      y =  viewport[3] - (y + h/2);
      gluPickMatrix(x, y, w, h, (GLint *)&viewport);


      camera->initProjection(false);

      glMatrixMode(GL_MODELVIEW);
      glPushMatrix(); //save previous model view matrix

      camera->initModelView();

      glPolygonMode(GL_FRONT, GL_FILL);
      glDisable(GL_LIGHTING);
      glDisable(GL_BLEND);
      glDisable(GL_STENCIL_TEST);

      map<unsigned int, unsigned long> idToEntity;
      unsigned int id=1;

      if((type & RenderingSimpleEntities)!=0){
        for(vector<SimpleEntityLODUnit>::iterator it=(*itLayer).simpleEntitiesLODVector.begin();it!=(*itLayer).simpleEntitiesLODVector.end();++it) {
          if((*it).lod<0)
            continue;

          idToEntity[id]=(*it).id;
          glLoadName(id);
          id++;
          ((GlSimpleEntity*)((*it).id))->draw(20.,camera);
        }
      }

      // Draw complex entities
      if(glGraphComposite){
        if((type & RenderingNodes)!=0){
          GlNode glNode(0);
          for(vector<ComplexEntityLODUnit>::iterator it=(*itLayer).nodesLODVector.begin();it!=(*itLayer).nodesLODVector.end();++it) {
            if((*it).lod<0)
              continue;

            idToEntity[id]=(*it).id;
            glLoadName(id);
            id++;

            glNode.id=(*it).id;
            glNode.draw(20.,glGraphComposite->getInputData(),camera);
          }
        }

        if((type & RenderingEdges)!=0){
          GlEdge glEdge(0);
          for(vector<ComplexEntityLODUnit>::iterator it=(*itLayer).edgesLODVector.begin();it!=(*itLayer).edgesLODVector.end();++it) {
            if((*it).lod<0)
              continue;

            idToEntity[id]=(*it).id;
            glLoadName(id);
            id++;

            glEdge.id=(*it).id;
            glEdge.draw(20.,glGraphComposite->getInputData(),camera);
          }
        }
      }

      glPopMatrix();

      glMatrixMode(GL_PROJECTION);
      glPopMatrix();
      //glLoadIdentity();

      glFlush();
      GLint hits = glRenderMode(GL_RENDER);

      while(hits>0) {
        selectedEntities.push_back(idToEntity[selectBuf[hits-1][3]]);
        hits--;
      }

      glPopClientAttrib();
      glPopAttrib();

      delete[] selectBuf;
	}

    selectLODCalculator->clear();
    if(selectLODCalculator!=lodCalculator)
      delete selectLODCalculator;
	glViewport(viewport[0],viewport[1],viewport[2],viewport[3]);
	return (selectedEntities.size()!=0);
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
	recorder.record(false,returned,buffer,layersList[0].second->getCamera()->getViewport());
	string str;
	builder.getResult(&str);
	if(!filename.empty()) {
      /* subgraphs drawing disabled
	 initMapsSVG(_renderingParameters.getGraph(), &ge); */
      FILE* file = fopen(filename.c_str(), "w");
      if (file) {
        fprintf(file, "%s",str.c_str());
        fclose(file);
      } else {
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
	recorder.record(false,returned,buffer,layersList[0].second->getCamera()->getViewport());
	string str;
	builder.getResult(&str);
	if(!filename.empty()) {
      FILE* file = fopen(filename.c_str(), "w");
      if (file) {
        fprintf(file, "%s", str.c_str());
        fclose(file);
      } else {
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
  GlLayer* GlScene::getLayer(const std::string& name) {
	for(vector<pair<string, GlLayer *> >::iterator it=layersList.begin();it!=layersList.end();++it) {
      if((*it).first==name)
        return (*it).second;
	}
	return NULL;
  }
  //====================================================
  void GlScene::getXML(string &out) {
	xmlDocPtr doc = NULL;       /* document pointer */
	xmlNodePtr node = NULL;
	xmlNodePtr rootNode = NULL;
	xmlNodePtr dataNode= NULL;
	xmlNodePtr childrenNode = NULL;/* node pointers */

	doc = xmlNewDoc(BAD_CAST "1.0");
	rootNode = xmlNewNode(NULL, BAD_CAST "scene");
	xmlDocSetRootElement(doc, rootNode);

	GlXMLTools::createDataAndChildrenNodes(rootNode, dataNode, childrenNode);

	GlXMLTools::getXML(dataNode,"viewport",viewport);
	GlXMLTools::getXML(dataNode,"background",backgroundColor);

	for(vector<pair<string, GlLayer *> >::iterator it=layersList.begin();it!=layersList.end();++it) {

      // Don't save working layers
      if((*it).second->isAWorkingLayer())
        continue;

      GlXMLTools::createChild(childrenNode, "GlLayer", node);
      GlXMLTools::createProperty(node, "name", (*it).first);
      (*it).second->getXML(node);
	}

	/*
	 * Dumping document to stdio or file
	 */
	xmlChar *xmlbuff;
	int buffersize;

	xmlDocDumpFormatMemory(doc, &xmlbuff, &buffersize, 1);
	out.append((char *)xmlbuff);

	int lastPos=0;
	int current=out.find("\"",lastPos);
	while(current!=-1){
      out.replace(current,1,"\\\"");
      lastPos=current+2;
      current=out.find("\"",lastPos);
	}

	xmlFree(xmlbuff);

	/*free the document */
	xmlFreeDoc(doc);

	/*
	 *Free the global variables that may
	 *have been allocated by the parser.
	 */
	xmlCleanupParser();

	/*
	 * this is to debug memory for regression tests
	 */
	xmlMemoryDump();
  }
  //====================================================
  void GlScene::setWithXML(string &in, Graph *graph) {
	xmlDocPtr doc;
	xmlNodePtr rootNode = NULL;
	xmlNodePtr dataNode= NULL;
	xmlNodePtr childrenNode = NULL;
	xmlNodePtr node = NULL;/* node pointers */

	glGraphComposite=new GlGraphComposite(graph);

	doc = xmlReadMemory(&in[0], in.length(), "noname.xml", NULL, 0);

	rootNode = xmlDocGetRootElement(doc);

	string name;

	name=((char*)rootNode->name);
	if (rootNode->type == XML_ELEMENT_NODE && name=="scene") {
      GlXMLTools::getDataAndChildrenNodes(rootNode,dataNode,childrenNode);
	}else{
      assert(false);
	}

	// Parse data
	if(dataNode) {
      GlXMLTools::setWithXML(dataNode,"viewport",viewport);
      GlXMLTools::setWithXML(dataNode,"background",backgroundColor);
	}

	// Parse children
	for (node = childrenNode->children; node; node = node->next) {
      if(node->type == XML_ELEMENT_NODE) {
        if(string((char*)node->name)=="GlLayer") {
          //New version
          string propName=(char*)node->properties->name;
          string propValue=(char*)node->properties->children->content;
          if(propName=="name") {
            GlLayer *newLayer=new GlLayer(propValue);
            addLayer(newLayer);
            newLayer->setWithXML(node);
          }else{
            assert(false);
          }
        }else{
          //Previous version compatibility
          string propName=(char*)node->properties->name;
          string propValue=(char*)node->properties->children->content;
          if(propName=="type" && propValue=="GlLayer") {
            GlLayer *newLayer=new GlLayer((char*)node->name);
            addLayer(newLayer);
            newLayer->setWithXML(node);
          }else{
            assert(false);
          }
        }
      }
	}

	getLayer("Main")->addGlEntity(glGraphComposite,"graph");
	addGlGraphCompositeInfo(getLayer("Main"),glGraphComposite);
  }

}
