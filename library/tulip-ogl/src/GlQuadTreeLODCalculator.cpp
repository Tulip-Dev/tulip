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
#include <tulip/GlQuadTreeLODCalculator.h>

#ifdef _OPENMP
#include <omp.h>
#endif

#include <tulip/Matrix.h>
#include <tulip/QuadTree.h>
#include <tulip/GlGraphInputData.h>
#include <tulip/Camera.h>
#include <tulip/GlEntity.h>
#include <tulip/GlTools.h>
#include <tulip/GlScene.h>
#include <tulip/GlNode.h>
#include <tulip/GlEdge.h>
#include <tulip/GlSceneObserver.h>

using namespace std;

namespace tlp {

BoundingBox computeNewBoundingBox(const BoundingBox &box,const Coord &centerScene,double aX,double aY) {
  // compute a new bouding box : this bounding box is the rotation of the old bounding box
  Coord size( (box[1] - box[0])/2.f);
  Coord center(box[0] + size);
  //size = Coord(size.norm(),size.norm(),size.norm());
  size.fill(size.norm());
  center[0]=centerScene[0]+(cos(aY)*(center[0]-centerScene[0]));
  center[1]=centerScene[1]+(cos(aX)*(center[1]-centerScene[1]));

  return BoundingBox(center-size,center+size);
}

GlQuadTreeLODCalculator::GlQuadTreeLODCalculator() : haveToCompute(true),haveToInitObservers(true),currentGraph(NULL),layoutProperty(NULL),sizeProperty(NULL),selectionProperty(NULL) {
}

GlQuadTreeLODCalculator::~GlQuadTreeLODCalculator() {
  setHaveToCompute();
  clearCamerasObservers();

  for(vector<QuadTreeNode<unsigned int> *>::iterator it=nodesQuadTree.begin(); it!=nodesQuadTree.end(); ++it)
    delete (*it);

  for(vector<QuadTreeNode<unsigned int> *>::iterator it=edgesQuadTree.begin(); it!=edgesQuadTree.end(); ++it)
    delete (*it);

  for(vector<QuadTreeNode<GlSimpleEntity *> *>::iterator it=entitiesQuadTree.begin(); it!=entitiesQuadTree.end(); ++it)
    delete (*it);
}

void GlQuadTreeLODCalculator::setScene(GlScene &scene) {
  // If we change scene, whe have to remove observer on the graph and the scene
  // in the next rendering, we have to compute quadtree
  setHaveToCompute();
  GlLODCalculator::setScene(scene);
}

void GlQuadTreeLODCalculator::setInputData(const GlGraphInputData *newInputData) {
  setHaveToCompute();

  if (newInputData == NULL) {
    currentCamera = NULL;
    currentGraph = NULL;
    layoutProperty = NULL;
    sizeProperty = NULL;
    selectionProperty = NULL;
  }

  GlLODCalculator::setInputData(newInputData);
}

bool GlQuadTreeLODCalculator::needEntities() {
  if(inputData != NULL) {
    //Checks if the properties in the GlGraphInputData have changed
    if(layoutProperty != inputData->getElementLayout() || sizeProperty != inputData->getElementSize() || selectionProperty != inputData->getElementSelected()) {
      //Remove observers on old properties
      removeObservers();
      //Reinit properties and listen them
      addObservers();
      //Need to recompute
      haveToCompute = true;
      haveToInitObservers=false;
    }
  }

  // Check if quadtree need entities
  if(haveToCompute) {
    if(inputData)
      oldParameters=*inputData->parameters;

    return true;
  }

  // Check if a camera have changed (diff between old backup camera and current camera)
  for(map<GlLayer *, Camera>::iterator it=layerToCamera.begin(); it!=layerToCamera.end(); ++it) {
    if(((*it).first->getCamera()).is3D()) {
      Camera camera      = (*it).first->getCamera();
      Camera oldCamera   = (*it).second;
      Coord unitCamera    = camera.getEyes() - camera.getCenter();
      unitCamera          = unitCamera/unitCamera.norm();
      Coord unitOldCamera = oldCamera.getEyes()-oldCamera.getCenter();
      unitOldCamera  = unitOldCamera/unitOldCamera.norm();

      if(unitCamera != unitOldCamera) {
        haveToCompute = true;

        if(inputData)
          oldParameters=*inputData->parameters;

        return true;
      }
    }
  }

  if(inputData) {
    // Check visibility flags
    GlGraphRenderingParameters *newParameters=inputData->parameters;

    if(oldParameters.isDisplayEdges()!=newParameters->isDisplayEdges() ||
        oldParameters.isDisplayMetaNodes()!=newParameters->isDisplayMetaNodes() ||
        oldParameters.isDisplayNodes()!=newParameters->isDisplayNodes() ||
        oldParameters.isViewNodeLabel() != newParameters->isViewNodeLabel() ||
        oldParameters.isViewEdgeLabel() != newParameters->isViewEdgeLabel() ||
        oldParameters.isViewMetaLabel() != newParameters->isViewMetaLabel()) {
      oldParameters=*inputData->parameters;
      haveToCompute = true;
      return true;
    }
  }

  return false;
}

void GlQuadTreeLODCalculator::setNeedEntities(bool) {
  setHaveToCompute();
}

void GlQuadTreeLODCalculator::addSimpleEntityBoundingBox(GlSimpleEntity *entity,const BoundingBox& bb) {
  GlCPULODCalculator::addSimpleEntityBoundingBox(entity, bb);
  entitiesGlobalBoundingBox.expand(bb[0]);
  entitiesGlobalBoundingBox.expand(bb[1]);
}
void GlQuadTreeLODCalculator::addNodeBoundingBox(unsigned int id,const BoundingBox& bb) {
  GlCPULODCalculator::addNodeBoundingBox(id,bb);
  nodesGlobalBoundingBox.expand(bb[0]);
  nodesGlobalBoundingBox.expand(bb[1]);
}
void GlQuadTreeLODCalculator::addEdgeBoundingBox(unsigned int id,const BoundingBox& bb) {
  GlCPULODCalculator::addEdgeBoundingBox(id,bb);
  edgesGlobalBoundingBox.expand(bb[0]);
  edgesGlobalBoundingBox.expand(bb[1]);
}

void GlQuadTreeLODCalculator::compute(const Vector<int,4>& globalViewport,const Vector<int,4>& currentViewport) {

  if(haveToCompute) {
    // if have to compute : rebuild quadtree

    if(haveToInitObservers) {
      addObservers();
      haveToInitObservers=false;
    }

    clearCamerasObservers();

    // Clear all vectors
    cameras.clear();
    layerToCamera.clear();
    simpleEntities.clear();

    for(vector<QuadTreeNode<unsigned int> *>::iterator it=nodesQuadTree.begin(); it!=nodesQuadTree.end(); ++it)
      delete (*it);

    nodesQuadTree.clear();

    for(vector<QuadTreeNode<unsigned int> *>::iterator it=edgesQuadTree.begin(); it!=edgesQuadTree.end(); ++it)
      delete (*it);

    edgesQuadTree.clear();

    for(vector<QuadTreeNode<GlSimpleEntity *> *>::iterator it=entitiesQuadTree.begin(); it!=entitiesQuadTree.end(); ++it)
      delete (*it);

    entitiesQuadTree.clear();

    quadTreesVectorPosition=0;
    const vector<pair<std::string, GlLayer*> > &layersVector=glScene->getLayersList();

    for(vector<LayerLODUnit>::iterator it=layersLODVector.begin(); it!=layersLODVector.end(); ++it) {
      Camera *camera= ((*it).camera);

      GlLayer *currentLayer=NULL;

      for(vector<pair<std::string, GlLayer*> >::const_iterator itL=layersVector.begin(); itL!=layersVector.end(); ++itL) {
        if(&(*itL).second->getCamera()==camera) {
          currentLayer=(*itL).second;
          break;
        }
      }

      cameras.push_back(camera);

      if(currentLayer!=NULL) {
        layerToCamera.insert(pair<GlLayer*, Camera>(currentLayer, *camera));
      }

      Matrix<float,4> transformMatrix;
      camera->getTransformMatrix(globalViewport,transformMatrix);

      Coord eye;

      if(camera->is3D()) {
        currentCamera=camera;
        eye=camera->getEyes() + ( camera->getEyes() -camera->getCenter() ) / (float)camera->getZoomFactor();
        computeFor3DCamera(&(*it),eye,transformMatrix,globalViewport,currentViewport);
        quadTreesVectorPosition++;
      }
      else {
        simpleEntities.push_back((*it).simpleEntitiesLODVector);
        computeFor2DCamera(&(*it),globalViewport,currentViewport);
      }

      glMatrixMode(GL_MODELVIEW);
    }

    initCamerasObservers();

    haveToCompute = false;

  }
  else {
    // if don't have to compute : use stored quadtree data

    layersLODVector.clear();

    quadTreesVectorPosition=0;
    simpleEntitiesVectorPosition=0;

    for(vector<Camera*>::iterator it=cameras.begin(); it!=cameras.end(); ++it) {

      Camera *camera=*it;
      layersLODVector.push_back(LayerLODUnit());
      LayerLODUnit *layerLODUnit=&(layersLODVector.back());
      layerLODUnit->camera = camera;

      Matrix<float,4> transformMatrix;
      camera->getTransformMatrix(globalViewport,transformMatrix);

      Coord eye;

      if(camera->is3D()) {
        currentCamera=camera;
        eye=camera->getEyes() + ( camera->getEyes() -camera->getCenter() ) / (float)camera->getZoomFactor();
        computeFor3DCamera(layerLODUnit,eye,transformMatrix,globalViewport,currentViewport);
        quadTreesVectorPosition++;
      }
      else {
        layerLODUnit->simpleEntitiesLODVector=simpleEntities[simpleEntitiesVectorPosition];
        computeFor2DCamera(layerLODUnit,globalViewport,currentViewport);
        simpleEntitiesVectorPosition++;
      }
    }

  }
}

void GlQuadTreeLODCalculator::computeFor3DCamera(LayerLODUnit *layerLODUnit,
    const Coord &eye,
    const Matrix<float, 4>& transformMatrix,
    const Vector<int,4>& globalViewport,
    const Vector<int,4>& currentViewport) {

  // aX,aY : rotation on the camera in x and y
  Coord eyeCenter=currentCamera->getCenter()-currentCamera->getEyes();
  double aX=atan(eyeCenter[1]/eyeCenter[2]);
  double aY=atan(eyeCenter[0]/eyeCenter[2]);

  if(haveToCompute) {
    // Create quadtrees
    if(entitiesGlobalBoundingBox.isValid())
      entitiesQuadTree.push_back(new QuadTreeNode<GlSimpleEntity *>(entitiesGlobalBoundingBox));
    else
      entitiesQuadTree.push_back(NULL);

    if(nodesGlobalBoundingBox.isValid()) {
      nodesQuadTree.push_back(new QuadTreeNode<unsigned int>(nodesGlobalBoundingBox));
    }
    else {
      nodesQuadTree.push_back(NULL);
    }

    if(edgesGlobalBoundingBox.isValid()) {
      edgesQuadTree.push_back(new QuadTreeNode<unsigned int>(edgesGlobalBoundingBox));
    }
    else {
      edgesQuadTree.push_back(NULL);
    }

    // Add entities in quadtrees
    size_t nbSimples=layerLODUnit->simpleEntitiesLODVector.size();
    size_t nbNodes=layerLODUnit->nodesLODVector.size();
    size_t nbEdges=layerLODUnit->edgesLODVector.size();
#ifdef _OPENMP
    #pragma omp parallel
#endif
    {
#ifdef _OPENMP
      #pragma omp sections nowait
#endif
      {
#ifdef _OPENMP
        #pragma omp section
#endif
        {
          for(size_t i=0; i<nbSimples; ++i) {
            entitiesQuadTree[quadTreesVectorPosition]->insert(layerLODUnit->simpleEntitiesLODVector[i].boundingBox,layerLODUnit->simpleEntitiesLODVector[i].entity);
          }
        }
#ifdef _OPENMP
        #pragma omp section
#endif
        {
          for(size_t i=0; i<nbNodes; ++i) {
            nodesQuadTree[quadTreesVectorPosition]->insert(layerLODUnit->nodesLODVector[i].boundingBox,layerLODUnit->nodesLODVector[i].id);
          }
        }
#ifdef _OPENMP
        #pragma omp section
#endif
        {
          for(size_t i=0; i<nbEdges; ++i) {
            // This code is here to expand edge bonding box when we have an edge with direction (0,0,x)
            if(layerLODUnit->edgesLODVector[i].boundingBox[0][0] == layerLODUnit->edgesLODVector[i].boundingBox[1][0] &&
            layerLODUnit->edgesLODVector[i].boundingBox[0][1] == layerLODUnit->edgesLODVector[i].boundingBox[1][1]) {
              layerLODUnit->edgesLODVector[i].boundingBox.expand(layerLODUnit->edgesLODVector[i].boundingBox[1]+Coord(0.01,0.01,0));
            }

            edgesQuadTree[quadTreesVectorPosition]->insert(layerLODUnit->edgesLODVector[i].boundingBox,layerLODUnit->edgesLODVector[i].id);
          }
        }
      }
    }

    layerLODUnit->simpleEntitiesLODVector.clear();
    layerLODUnit->nodesLODVector.clear();
    layerLODUnit->edgesLODVector.clear();
  }


  MatrixGL invTransformMatrix(transformMatrix);
  invTransformMatrix.inverse();
  Coord pSrc = projectPoint(Coord(0,0,0), transformMatrix, globalViewport);

  Vector<int,4> transformedViewport=currentViewport;
  transformedViewport[1]=globalViewport[3]-(currentViewport[1]+currentViewport[3]);
  BoundingBox cameraBoundingBox;

  // Project camera bondinx box to know visible part of the quadtree
  pSrc[0] = transformedViewport[0];
  pSrc[1] = (globalViewport[1] + globalViewport[3]) - (transformedViewport[1] + transformedViewport[3]);
  cameraBoundingBox.expand(unprojectPoint(pSrc, invTransformMatrix, globalViewport));
  pSrc[1] = transformedViewport[1]+transformedViewport[3];
  cameraBoundingBox.expand(unprojectPoint(pSrc, invTransformMatrix, globalViewport));
  pSrc[0] = transformedViewport[0]+transformedViewport[2];
  cameraBoundingBox.expand(unprojectPoint(pSrc, invTransformMatrix, globalViewport));
  pSrc[1] = transformedViewport[1];
  cameraBoundingBox.expand(unprojectPoint(pSrc, invTransformMatrix, globalViewport));

  int ratio;

  if(currentViewport[2]>currentViewport[3])
    ratio=currentViewport[2];
  else
    ratio=currentViewport[3];

  vector<unsigned int> resNodes;
  vector<unsigned int> resEdges;
  vector<GlSimpleEntity *> resEntities;

  // Get result of quadtrees
#ifdef _OPENMP
  #pragma omp parallel
#endif
  {
#ifdef _OPENMP
    #pragma omp sections nowait
#endif
    {
#ifdef _OPENMP
      #pragma omp section
#endif
      {
        if((renderingEntitiesFlag & RenderingNodes)!=0) {
          if(aX==0 && aY==0) {
            if((renderingEntitiesFlag & RenderingWithoutRemove)==0) {
              if(nodesQuadTree[quadTreesVectorPosition])
                nodesQuadTree[quadTreesVectorPosition]->getElementsWithRatio(cameraBoundingBox,resNodes,ratio);
            }
            else {
              if(nodesQuadTree[quadTreesVectorPosition])
                nodesQuadTree[quadTreesVectorPosition]->getElements(cameraBoundingBox,resNodes);
            }
          }
          else {
            if(nodesQuadTree[quadTreesVectorPosition])
              nodesQuadTree[quadTreesVectorPosition]->getElements(resNodes);
          }

          GlNode glNode(0);
          layerLODUnit->nodesLODVector.reserve(resNodes.size());

          for(size_t i=0; i<resNodes.size(); ++i) {
            glNode.id=resNodes[i];
            layerLODUnit->nodesLODVector.push_back(ComplexEntityLODUnit(resNodes[i],glNode.getBoundingBox(inputData)));
          }
        }
      }
#ifdef _OPENMP
      #pragma omp section
#endif
      {
        if((renderingEntitiesFlag & RenderingEdges)!=0) {
          if(aX==0 && aY==0) {
            if((renderingEntitiesFlag & RenderingWithoutRemove)==0) {
              if(edgesQuadTree[quadTreesVectorPosition])
                edgesQuadTree[quadTreesVectorPosition]->getElementsWithRatio(cameraBoundingBox,resEdges,ratio);
            }
            else {
              if(edgesQuadTree[quadTreesVectorPosition])
                edgesQuadTree[quadTreesVectorPosition]->getElements(cameraBoundingBox,resEdges);
            }
          }
          else {
            if(edgesQuadTree[quadTreesVectorPosition])
              edgesQuadTree[quadTreesVectorPosition]->getElements(resEdges);
          }

          GlEdge glEdge(0);
          layerLODUnit->edgesLODVector.reserve(resEdges.size());

          for(size_t i=0; i<resEdges.size(); ++i) {
            glEdge.id=resEdges[i];
            layerLODUnit->edgesLODVector.push_back(ComplexEntityLODUnit(resEdges[i],glEdge.getBoundingBox(inputData)));
          }
        }
      }
    }
#ifdef _OPENMP
    #pragma omp master
#endif
    {
      if((renderingEntitiesFlag & RenderingSimpleEntities)!=0 && entitiesQuadTree[quadTreesVectorPosition]!=NULL) {
        if(aX==0 && aY==0) {
          if((renderingEntitiesFlag & RenderingWithoutRemove)==0)
            entitiesQuadTree[quadTreesVectorPosition]->getElementsWithRatio(cameraBoundingBox, resEntities, ratio);
          else
            entitiesQuadTree[quadTreesVectorPosition]->getElements(cameraBoundingBox,resEntities);
        }
        else {
          entitiesQuadTree[quadTreesVectorPosition]->getElements(resEntities);
        }
      }

      for(size_t i=0; i<resEntities.size(); ++i) {
        layerLODUnit->simpleEntitiesLODVector.push_back(SimpleEntityLODUnit(resEntities[i],resEntities[i]->getBoundingBox()));
      }
    }
  }

  GlCPULODCalculator::computeFor3DCamera(layerLODUnit,eye,transformMatrix,globalViewport,currentViewport);
}

void GlQuadTreeLODCalculator::removeObservers() {
  if(inputData) {
    if(currentGraph)
      currentGraph->removeListener(this);

    if(layoutProperty) {
      layoutProperty->removeListener(this);
      layoutProperty = NULL;
    }

    if(sizeProperty) {
      sizeProperty->removeListener(this);
      sizeProperty = NULL;
    }

    if(selectionProperty) {
      selectionProperty->removeListener(this);
      selectionProperty = NULL;
    }
  }

  if(glScene)
    glScene->removeListener(this);
}

void GlQuadTreeLODCalculator::addObservers() {
  if(inputData) {
    currentGraph=inputData->getGraph();
    currentGraph->addListener(this);

    layoutProperty = inputData->getElementLayout();

    if(layoutProperty != NULL) {
      layoutProperty->addListener(this);
    }

    sizeProperty = inputData->getElementSize();

    if(sizeProperty != NULL) {
      sizeProperty->addListener(this);
    }

    selectionProperty=inputData->getElementSelected();

    if(selectionProperty != NULL) {
      selectionProperty->addListener(this);
    }
  }

  if(glScene)
    glScene->addListener(this);
}

void GlQuadTreeLODCalculator::update(PropertyInterface *property) {
  if(property==inputData->getElementLayout() ||
      property==inputData->getElementSize() ||
      property==inputData->getElementSelected())
    setHaveToCompute();
}

void GlQuadTreeLODCalculator::treatEvent(const Event &ev) {
  const GlSceneEvent *sceneEv = dynamic_cast<const GlSceneEvent *>(&ev);

  if(sceneEv) {
    setHaveToCompute();
  }
  else if (typeid(ev) == typeid(GraphEvent)) {
    const GraphEvent* graphEvent = dynamic_cast<const GraphEvent*>(&ev);

    switch(graphEvent->getType()) {
    case GraphEvent::TLP_ADD_NODE:
    case GraphEvent::TLP_ADD_EDGE:
    case GraphEvent::TLP_DEL_NODE:
    case GraphEvent::TLP_DEL_EDGE:
      setHaveToCompute();
      break;

    case GraphEvent::TLP_ADD_LOCAL_PROPERTY:
    case GraphEvent::TLP_BEFORE_DEL_LOCAL_PROPERTY: {
      const PropertyInterface *property = inputData->getGraph()->getProperty(graphEvent->getPropertyName());

      if(property==inputData->getElementLayout() || property==inputData->getElementSize()) {
        setHaveToCompute();
        removeObservers();
        addObservers();
      }

      break;
    }

    default:
      break;
    }
  }
  else if(typeid(ev) == typeid(PropertyEvent)) {
    const PropertyEvent* propertyEvent = dynamic_cast<const PropertyEvent*>(&ev);
    PropertyInterface* property = propertyEvent->getProperty();

    switch(propertyEvent->getType()) {
    case PropertyEvent::TLP_BEFORE_SET_ALL_NODE_VALUE:
    case PropertyEvent::TLP_BEFORE_SET_NODE_VALUE:
    case PropertyEvent::TLP_BEFORE_SET_ALL_EDGE_VALUE:
    case PropertyEvent::TLP_BEFORE_SET_EDGE_VALUE:
      update(property);
      break;

    default:
      break;
    }
  }
  else if (ev.type()==Event::TLP_DELETE) {
    if (dynamic_cast<Camera*>(ev.sender())) {

      for(vector<Camera *>::iterator it=cameras.begin(); it!=cameras.end(); ++it) {
        if(*it == dynamic_cast<Camera*>(ev.sender())) {
          (*it)->removeListener(this);
          cameras.erase(it);
          break;
        }
      }

      haveToCompute=true;
    }

    if (dynamic_cast<tlp::Graph*>(ev.sender())) {
      clear();
      setInputData(NULL);
    }

    PropertyInterface* property;

    if ((property = dynamic_cast<PropertyInterface*>(ev.sender()))) {
      if(property == layoutProperty) {
        layoutProperty=NULL;
      }
      else if(property == sizeProperty) {
        sizeProperty = NULL;
      }
      else if(property == selectionProperty) {
        selectionProperty = NULL;
      }
    }
  }
}

void GlQuadTreeLODCalculator::initCamerasObservers() {
  set<Camera*> treatedCameras;

  for(vector<Camera *>::iterator it=cameras.begin(); it!=cameras.end(); ++it) {
    if(treatedCameras.find(*it)==treatedCameras.end()) {
      treatedCameras.insert(*it);
      (*it)->addListener(this);
    }
  }
}

void GlQuadTreeLODCalculator::clearCamerasObservers() {
  set<Camera*> treatedCameras;

  for(vector<Camera *>::iterator it=cameras.begin(); it!=cameras.end(); ++it) {
    if(treatedCameras.find(*it)==treatedCameras.end()) {
      treatedCameras.insert(*it);
      (*it)->removeListener(this);
    }
  }
}

void GlQuadTreeLODCalculator::setHaveToCompute() {
  if(haveToCompute)
    return;

  GlQuadTreeLODCalculator *attachedQuadTreeLODCalculator=dynamic_cast<GlQuadTreeLODCalculator*>(attachedLODCalculator);

  if(attachedQuadTreeLODCalculator)
    attachedQuadTreeLODCalculator->setHaveToCompute();

  haveToCompute=true;
  haveToInitObservers=true;
  removeObservers();
}
}

