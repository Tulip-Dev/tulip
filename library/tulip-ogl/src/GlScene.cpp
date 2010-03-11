#include "tulip/GlewManager.h"

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
      BoundingBox bb1,bb2;
      if(e1.isComplexEntity)
        if(e1.isNode){
        GlNode glNode(e1.complexEntity->first);
        bb1=glNode.getBoundingBox(inputData);
      }else{
        GlEdge glEdge(e1.complexEntity->first);
        bb1=glEdge.getBoundingBox(inputData);
      }
      else
        bb1=((GlSimpleEntity*)(e1.simpleEntity->first))->getBoundingBox();

      if(e2.isComplexEntity)
        if(e2.isNode){
        GlNode glNode(e2.complexEntity->first);
        bb2=glNode.getBoundingBox(inputData);
      }else{
        GlEdge glEdge(e2.complexEntity->first);
        bb2=glEdge.getBoundingBox(inputData);
      }
      else
        bb2=((GlSimpleEntity*)(e2.simpleEntity->first))->getBoundingBox();

      if(bb1.second[0]-bb1.first[0] > bb2.second[0]-bb2.first[0])
        return false;
      else
        return true;

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
	selectionLayer= new GlLayer("Selection");
	selectionLayer->setCamera(camera);
	selectionLayer->setScene(this);
    selectionLayer->getComposite()->setDeleteComponentsInDestructor(false);

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
    delete selectionLayer;
  }

  void GlScene::initGlParameters() {
	if(!GlewManager::getInst().glewIsInit())
      GlewManager::getInst().initGlew();
	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
	bool antialiased = true;
	if(glGraphComposite) {
      antialiased=glGraphComposite->getInputData()->parameters->isAntialiased();
	}

	if(antialiased) {
      glEnable(GL_LINE_SMOOTH);
      glEnable(GL_POLYGON_SMOOTH);
      glHint(GL_LINE_SMOOTH_HINT,GL_DONT_CARE);
      glHint(GL_POLYGON_SMOOTH_HINT,GL_DONT_CARE);
      glShadeModel(GL_SMOOTH);
	}else{
      glDisable(GL_LINE_SMOOTH);
      glDisable(GL_POLYGON_SMOOTH);
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
	  newLodCalculator->beginNewCamera(getLayer("Main")->getCamera());
	  GlNode glNode(0);
	  for(set<node>::iterator it=metaNodes.begin();it!=metaNodes.end();++it){
	    glNode.id=(*it).id;
	    newLodCalculator->addNodeBoundingBox((*it).id,glNode.getBoundingBox(glGraphComposite->getInputData()));
	  }
	  newLodCalculator->compute(viewport,viewport,RenderingAll);
	  VectorOfComplexLODResultVector* nodesVector=newLodCalculator->getResultForNodes();

	  assert(nodesVector->size()!=0);
	  for(vector<LODResultComplexEntity>::iterator it=(*nodesVector)[0].begin();it!=(*nodesVector)[0].end();++it) {
	    glGraphComposite->getInputData()->getMetaNodeRenderer()->prerender(node((*it).first),(*it).second,getLayer("Main")->getCamera());
	  }
	  delete newLodCalculator;
	}
  }

  void GlScene::draw() {
	initGlParameters();

    if(lodCalculator->needEntities()){
      GlLODSceneVisitor *lodVisitor;
      if(glGraphComposite)
        lodVisitor=new GlLODSceneVisitor(lodCalculator,glGraphComposite->getInputData());
      else
        lodVisitor=new GlLODSceneVisitor(lodCalculator,NULL);

      for(vector<pair<string,GlLayer *> >::iterator it=layersList.begin();it!=layersList.end();++it) {
        (*it).second->acceptVisitor(lodVisitor);
        if((*it).first=="Main")
          selectionLayer->acceptVisitor(lodVisitor);
      }
      delete lodVisitor;
    }

    lodCalculator->compute(viewport,viewport,RenderingAll);

	TextRenderer fontRenderer;
	OcclusionTest occlusionTest;

	VectorOfCamera* cameraVector=lodCalculator->getVectorOfCamera();
	VectorOfSimpleLODResultVector* simpleVector=lodCalculator->getResultForSimpleEntities();
	VectorOfComplexLODResultVector* nodesVector=lodCalculator->getResultForNodes();
	VectorOfComplexLODResultVector* edgesVector=lodCalculator->getResultForEdges();
	VectorOfSimpleLODResultVector::iterator itSimple=simpleVector->begin();
	VectorOfComplexLODResultVector::iterator itNodes=nodesVector->begin();
	VectorOfComplexLODResultVector::iterator itEdges=edgesVector->begin();

	Camera *camera;
	Graph *graph=NULL;
	if(glGraphComposite)
      graph=glGraphComposite->getInputData()->graph;
	GlNode glNode(0);
	GlMetaNode glMetaNode(0);
	GlEdge glEdge(0);

	for(VectorOfCamera::iterator itCamera=cameraVector->begin();itCamera!=cameraVector->end();++itCamera){
      camera=(Camera*)(*itCamera);
      camera->initGl();

      GlPointManager::getInst().beginRendering();

      bool zOrdering=false;
      if(glGraphComposite)
        zOrdering=glGraphComposite->getRenderingParameters().isElementZOrdered();

      if(!zOrdering){
        for(vector<LODResultSimpleEntity>::iterator it=(*itSimple).begin();it!=(*itSimple).end();++it) {
          glStencilFunc(GL_LEQUAL,((GlSimpleEntity*)((*it).first))->getStencil(),0xFFFF);
          ((GlSimpleEntity*)((*it).first))->draw((*it).second,camera);
        }

        if(glGraphComposite){
          for(vector<LODResultComplexEntity>::iterator it=(*itNodes).begin();it!=(*itNodes).end();++it) {
            assert(graph);
            if(!graph->isMetaNode(node((*it).first))){
              glNode.id=(*it).first;
              glNode.draw((*it).second,glGraphComposite->getInputData(),camera);
            }else{
              glMetaNode.id=(*it).first;
              glMetaNode.draw((*it).second,glGraphComposite->getInputData(),camera);
            }
          }
          for(vector<LODResultComplexEntity>::iterator it=(*itEdges).begin();it!=(*itEdges).end();++it) {
            glEdge.id=(*it).first;
            glEdge.draw((*it).second,glGraphComposite->getInputData(),camera);
          }
        }
      }else{
        entityWithDistanceCompare::inputData=glGraphComposite->getInputData();
        multiset<EntityWithDistance,entityWithDistanceCompare> entitiesSet;
        Coord camPos=camera->getEyes();
        BoundingBox bb;
        Coord middle;
        double dist;

        for(vector<LODResultSimpleEntity>::iterator it=(*itSimple).begin();it!=(*itSimple).end();++it) {
          bb=((GlSimpleEntity*)((*it).first))->getBoundingBox();
          middle=bb.first+(bb.second-bb.first)/2.f;
          dist=(((double)middle[0])-((double)camPos[0]))*(((double)middle[0])-((double)camPos[0]));
          dist+=(((double)middle[1])-((double)camPos[1]))*(((double)middle[1])-((double)camPos[1]));
          dist+=(((double)middle[2])-((double)camPos[2]))*(((double)middle[2])-((double)camPos[2]));
          entitiesSet.insert(EntityWithDistance(dist,&(*it)));
        }

        if(glGraphComposite){
          GlNode glNode(0);
          for(vector<LODResultComplexEntity>::iterator it=(*itNodes).begin();it!=(*itNodes).end();++it) {
            glNode.id=(*it).first;
            bb=glNode.getBoundingBox(glGraphComposite->getInputData());
            middle=bb.first+(bb.second-bb.first)/2.f;
            dist=(((double)middle[0])-((double)camPos[0]))*(((double)middle[0])-((double)camPos[0]));
            dist+=(((double)middle[1])-((double)camPos[1]))*(((double)middle[1])-((double)camPos[1]));
            dist+=(((double)middle[2])-((double)camPos[2]))*(((double)middle[2])-((double)camPos[2]));
            entitiesSet.insert(EntityWithDistance(dist,&(*it),true));
          }
          GlEdge glEdge(0);
          for(vector<LODResultComplexEntity>::iterator it=(*itEdges).begin();it!=(*itEdges).end();++it) {
            glEdge.id=(*it).first;
            bb=glEdge.getBoundingBox(glGraphComposite->getInputData());
            middle=bb.first+(bb.second-bb.first)/2.f;
            dist=(((double)middle[0])-((double)camPos[0]))*(((double)middle[0])-((double)camPos[0]));
            dist+=(((double)middle[1])-((double)camPos[1]))*(((double)middle[1])-((double)camPos[1]));
            dist+=(((double)middle[2])-((double)camPos[2]))*(((double)middle[2])-((double)camPos[2]));
            entitiesSet.insert(EntityWithDistance(dist,&(*it),false));
          }
        }

        //entitiesVector.sort(entityWithDistanceCompare::compare);

        for(set<EntityWithDistance,entityWithDistanceCompare>::iterator it=entitiesSet.begin();it!=entitiesSet.end();++it){
          if(!(*it).isComplexEntity){
            glStencilFunc(GL_LEQUAL,((GlSimpleEntity*)((*it).simpleEntity->first))->getStencil(),0xFFFF);
            ((GlSimpleEntity*)((*it).simpleEntity->first))->draw((*it).simpleEntity->second,camera);
          }else{
            if((*it).isNode){
              if(!graph->isMetaNode(node((*it).complexEntity->first))){
                glNode.id=(*it).complexEntity->first;
                glNode.draw((*it).complexEntity->second,glGraphComposite->getInputData(),camera);
              }else{
                glMetaNode.id=(*it).complexEntity->first;
                glMetaNode.draw((*it).complexEntity->second,glGraphComposite->getInputData(),camera);
              }
            }else{
              glEdge.id=(*it).complexEntity->first;
              glEdge.draw((*it).complexEntity->second,glGraphComposite->getInputData(),camera);
            }
          }
        }
      }

      GlPointManager::getInst().endRendering();

      if(viewLabel && glGraphComposite) {
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glDisable(GL_LIGHTING);
        glDepthFunc(GL_ALWAYS );
        glDisable(GL_CULL_FACE);
        glDisable(GL_COLOR_MATERIAL);

        // Draw Nodes and MetaNodes Label
        if(glGraphComposite->getInputData()->parameters->isViewNodeLabel()) {
          // Draw Label for selected Nodes
          for(vector<LODResultComplexEntity>::iterator it=(*itNodes).begin();it!=(*itNodes).end();++it) {
            glNode.id=(*it).first;
            glNode.drawLabel(true,&occlusionTest,&fontRenderer,glGraphComposite->getInputData());
          }
          // Draw Label for others Nodes
          for(vector<LODResultComplexEntity>::iterator it=(*itNodes).begin();it!=(*itNodes).end();++it) {
            glNode.id=(*it).first;
            glNode.drawLabel(false,&occlusionTest,&fontRenderer,glGraphComposite->getInputData());
          }
        }
        // Draw Edges Label
        if(glGraphComposite->getInputData()->parameters->isViewEdgeLabel()) {
          glStencilFunc(GL_LEQUAL,glGraphComposite->getEdgesLabelStencil(),0xFFFF);
          // Draw Label for selected Edges
          for(vector<LODResultComplexEntity>::iterator it=(*itEdges).begin();it!=(*itEdges).end();++it) {
            glEdge.id=(*it).first;
            glEdge.drawLabel(true,&occlusionTest,&fontRenderer,glGraphComposite->getInputData());
          }
          // Draw Label for others Edges
          for(vector<LODResultComplexEntity>::iterator it=(*itEdges).begin();it!=(*itEdges).end();++it) {
            glEdge.id=(*it).first;
            glEdge.drawLabel(false,&occlusionTest,&fontRenderer,glGraphComposite->getInputData());
          }
        }

        glPopAttrib();
      }

      ++itSimple;
      ++itNodes;
      ++itEdges;
	}

	lodCalculator->clear();
  }

  void GlScene::addLayer(GlLayer *layer) {
	layersList.push_back(std::pair<std::string,GlLayer*>(layer->getName(),layer));
	layer->setScene(this);
	notifyAddLayer(this,layer->getName(),layer);
  }

  void GlScene::centerScene() {
	GlBoundingBoxSceneVisitor *visitor;

	if(glGraphComposite)
      visitor=new GlBoundingBoxSceneVisitor(glGraphComposite->getInputData());
	else
      visitor=new GlBoundingBoxSceneVisitor(NULL);

	for(vector<pair<string, GlLayer *> >::iterator it=layersList.begin();it!=layersList.end();++it) {
      if((*it).second->getCamera()->is3D())
        (*it).second->acceptVisitor(visitor);
	}

	Coord maxC = visitor->getBoundingBox().second;
	Coord minC = visitor->getBoundingBox().first;

	delete visitor;

	double dx = maxC[0] - minC[0];
	double dy = maxC[1] - minC[1];
	double dz = maxC[2] - minC[2];

	for(vector<pair<string,GlLayer *> >::iterator it=layersList.begin();it!=layersList.end();++it) {
      Camera* camera=(*it).second->getCamera();
      camera->setCenter((maxC + minC) / 2.f);

      if ((dx==0) && (dy==0) && (dz==0))
        dx = dy = dz = 10.0;

      camera->setSceneRadius(sqrt(dx*dx+dy*dy+dz*dz)/2.);

      camera->setEyes(Coord(0, 0, camera->getSceneRadius()));
      camera->setEyes(camera->getEyes() + camera->getCenter());
      camera->setUp(Coord(0, 1., 0));
      camera->setZoomFactor(0.5);
	}
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
      if((*it).second->getCamera()->is3D())
        (*it).second->acceptVisitor(visitor);
	}

	Coord maxC = visitor->getBoundingBox().second;
	Coord minC = visitor->getBoundingBox().first;

	delete visitor;

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
      if((*it).second->getCamera()->is3D())
        (*it).second->getCamera()->setZoomFactor((*it).second->getCamera()->getZoomFactor() * pow(1.1,step));
	}
	if (step < 0) step *= -1;
	int factX = (int)(step*(double(viewport[2])/2.0-x)/ 7.0);
	int factY = (int)(step*(double(viewport[3])/2.0-y)/ 7.0);
	translateCamera(factX,-factY,0);
  }

  void GlScene::zoom(float factor,const Coord& dest) {
	for(vector<pair<string, GlLayer *> >::iterator it=layersList.begin();it!=layersList.end();++it) {
      if((*it).second->getCamera()->is3D()) {
        (*it).second->getCamera()->setEyes(dest + ((*it).second->getCamera()->getEyes() - (*it).second->getCamera()->getCenter()));
        (*it).second->getCamera()->setCenter(dest);
      }
	}
  }

  void GlScene::translateCamera(const int x, const int y, const int z) {
	for(vector<pair<string, GlLayer *> >::iterator it=layersList.begin();it!=layersList.end();++it) {
      if((*it).second->getCamera()->is3D()) {
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
      if((*it).second->getCamera()->is3D()) {
        (*it).second->getCamera()->setZoomFactor((*it).second->getCamera()->getZoomFactor() * pow(1.1, step));
      }
	}
  }

  void GlScene::rotateScene(const int x, const int y, const int z) {
	for(vector<pair<string, GlLayer *> >::iterator it=layersList.begin();it!=layersList.end();++it) {
      if((*it).second->getCamera()->is3D()) {
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
          if((*it).first=="Main")
            selectionLayer->acceptVisitor(lodVisitor);
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

    selectLODCalculator->compute(viewport,selectionViewport,type);
	//lodCalculator->compute();

    VectorOfCamera* cameraVector=selectLODCalculator->getVectorOfCamera();
    VectorOfSimpleLODResultVector* simpleVector=selectLODCalculator->getResultForSimpleEntities();
    VectorOfComplexLODResultVector* nodesVector=selectLODCalculator->getResultForNodes();
    VectorOfComplexLODResultVector* edgesVector=selectLODCalculator->getResultForEdges();
	VectorOfSimpleLODResultVector::iterator itSimple=simpleVector->begin();
	VectorOfComplexLODResultVector::iterator itNodes=nodesVector->begin();
	VectorOfComplexLODResultVector::iterator itEdges=edgesVector->begin();

	for(VectorOfCamera::iterator it=cameraVector->begin();it!=cameraVector->end();++it) {

      Camera *camera=(Camera*)(*it);

      Vector<int, 4> viewport = camera->getViewport();

      unsigned int size;
      if((type & RenderingSimpleEntities)!=0) {
        size=(*itSimple).size();
      }else if((type & RenderingNodes)!=0){
        size=(*itNodes).size();
      }else{
        size=(*itEdges).size();
      }

      if(size==0){
        itSimple++;
        itNodes++;
        itEdges++;
        continue;
      }

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

      if((type&RenderingSimpleEntities)!=0) {
        for(vector<LODResultSimpleEntity>::iterator itE=(*itSimple).begin();itE!=(*itSimple).end();++itE){
          idToEntity[id]=(*itE).first;
          glLoadName(id);
          id++;
          ((GlSimpleEntity*)((*itE).first))->draw(20.,camera);
        }
      }else if((type&RenderingNodes)!=0){
        if(glGraphComposite){
          GlNode glNode(0);
          for(vector<LODResultComplexEntity>::iterator itE=(*itNodes).begin();itE!=(*itNodes).end();++itE){
            idToEntity[id]=(*itE).first;
            glLoadName(id);
            id++;
            glNode.id=(*itE).first;
            glNode.draw(20.,glGraphComposite->getInputData(),camera);
          }
        }
      }else{
        if(glGraphComposite){
          GlEdge glEdge(0);
          for(vector<LODResultComplexEntity>::iterator itE=(*itEdges).begin();itE!=(*itEdges).end();++itE){
            idToEntity[id]=(*itE).first;
            glLoadName(id);
            id++;
            glEdge.id=(*itE).first;
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
      itSimple++;
      itNodes++;
      itEdges++;
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
