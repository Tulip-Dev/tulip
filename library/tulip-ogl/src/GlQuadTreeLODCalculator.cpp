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
#include "tulip/GlQuadTreeLODCalculator.h"

#ifdef _OPENMP
#include <omp.h>
#endif

#include <tulip/Matrix.h>
#include <tulip/QuadTree.h>

#include "tulip/Camera.h"
#include "tulip/GlEntity.h"
#include "tulip/GlTools.h"
#include "tulip/GlScene.h"
#include <iostream>

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

    GlQuadTreeLODCalculator::GlQuadTreeLODCalculator() : haveToCompute(true),currentGraph(NULL),layoutProperty(NULL),sizeProperty(NULL),selectionProperty(NULL) {
    }

    GlQuadTreeLODCalculator::~GlQuadTreeLODCalculator() {
        setHaveToCompute();
        clearCamerasObservers();
    }

    void GlQuadTreeLODCalculator::setScene(GlScene &scene){
        // If we change scene, whe have to remove observer on the graph and the scene
        // in the next rendering, we have to compute quadtree
        setHaveToCompute();
        GlLODCalculator::setScene(scene);
    }

    void GlQuadTreeLODCalculator::setInputData(GlGraphInputData *newInputData){
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

    bool GlQuadTreeLODCalculator::needEntities(){
        // Check if quadtree need entities
        if(haveToCompute)
            return true;

        // Check if a camera have changed (diff between old backup camera and current camera)
        for(map<GlLayer *,pair<Camera*, Camera> >::iterator it=layerToCamera.begin(); it!=layerToCamera.end(); ++it){
            if(((*it).first->getCamera()).is3D()) {
                Camera camera      = (*it).first->getCamera();
                Camera oldCamera   = (*it).second.second;
                Coord unitCamera    = camera.getEyes() - camera.getCenter();
                unitCamera          = unitCamera/unitCamera.norm();
                Coord unitOldCamera = oldCamera.getEyes()-oldCamera.getCenter();
                unitOldCamera  = unitOldCamera/unitOldCamera.norm();
                if(unitCamera != unitOldCamera){
                    haveToCompute = true;
                    return true;
                }
            }
        }

        return false;
    }

    void GlQuadTreeLODCalculator::setNeedEntities(bool){
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

        if(haveToCompute){
            // if have to compute : rebuild quadtree

            addObservers();

            clearCamerasObservers();

            // Clear all vectors
            cameras.clear();
            layerToCamera.clear();
            simpleEntities.clear();

            for(vector<QuadTreeNode<unsigned int> *>::iterator it=nodesQuadTree.begin();it!=nodesQuadTree.end();++it)
                delete (*it);
            nodesQuadTree.clear();
            for(vector<QuadTreeNode<unsigned int> *>::iterator it=edgesQuadTree.begin();it!=edgesQuadTree.end();++it)
                delete (*it);
            edgesQuadTree.clear();
            for(vector<QuadTreeNode<GlSimpleEntity *> *>::iterator it=entitiesQuadTree.begin();it!=entitiesQuadTree.end();++it)
                delete (*it);
            entitiesQuadTree.clear();

            quadTreesVectorPosition=0;
            vector<pair<std::string, GlLayer*> > *layersVector=glScene->getLayersList();
            for(vector<LayerLODUnit>::iterator it=layersLODVector.begin();it!=layersLODVector.end();++it){
                Camera *camera= ((*it).camera);

                GlLayer *currentLayer=NULL;
                for(vector<pair<std::string, GlLayer*> >::iterator itL=layersVector->begin();itL!=layersVector->end();++itL){
                  if(&(*itL).second->getCamera()==camera){
                    currentLayer=(*itL).second;
                    break;
                  }
                }

                cameras.push_back(camera);
                if(currentLayer!=NULL)
                  layerToCamera.insert(pair<GlLayer*,pair<Camera*,Camera> >(currentLayer,pair<Camera *,Camera>(camera,*camera)));

                Matrix<float,4> transformMatrix;
                camera->getTransformMatrix(globalViewport,transformMatrix);

                Coord eye;
                if(camera->is3D()) {
                    currentCamera=camera;
                    eye=camera->getEyes() + ( camera->getEyes() -camera->getCenter() ) / (float)camera->getZoomFactor();
                    computeFor3DCamera(&(*it),eye,transformMatrix,globalViewport,currentViewport);
                    quadTreesVectorPosition++;
                }else{
                    simpleEntities.push_back((*it).simpleEntitiesLODVector);
                    computeFor2DCamera(&(*it),globalViewport,currentViewport);
                }

                glMatrixMode(GL_MODELVIEW);
            }

            initCamerasObservers();

            haveToCompute = false;

        }else{
            // if don't have to compute : use stored quadtree data

            layersLODVector.clear();

            quadTreesVectorPosition=0;
            simpleEntitiesVectorPosition=0;
            for(vector<Camera*>::iterator it=cameras.begin();it!=cameras.end();++it){

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
                }else{
                    layerLODUnit->simpleEntitiesLODVector=simpleEntities[simpleEntitiesVectorPosition];
                    computeFor2DCamera(layerLODUnit,globalViewport,currentViewport);
                    simpleEntitiesVectorPosition++;
                }
            }

        }
    }

    void GlQuadTreeLODCalculator::computeFor3DCamera(LayerLODUnit *layerLODUnit,
                                                     const Coord &eye,
                                                     const Matrix<float, 4> transformMatrix,
                                                     const Vector<int,4>& globalViewport,
                                                     const Vector<int,4>& currentViewport) {

        BooleanProperty *selectedProperty=NULL;
        if(inputData)
            selectedProperty=inputData->getGraph()->getProperty<BooleanProperty>(inputData->getElementSelectedPropName());

        // aX,aY : rotation on the camera in x and y
        Coord eyeCenter=currentCamera->getCenter()-currentCamera->getEyes();
        double aX=atan(eyeCenter[1]/eyeCenter[2]);
        double aY=atan(eyeCenter[0]/eyeCenter[2]);

        if(haveToCompute){
            // Create quadtrees
            if(entitiesGlobalBoundingBox.isValid())
                entitiesQuadTree.push_back(new QuadTreeNode<GlSimpleEntity *>(entitiesGlobalBoundingBox));
            else
                entitiesQuadTree.push_back(NULL);

            if(nodesGlobalBoundingBox.isValid()){
                nodesQuadTree.push_back(new QuadTreeNode<unsigned int>(nodesGlobalBoundingBox));
            }else{
                nodesQuadTree.push_back(NULL);
            }

            if(edgesGlobalBoundingBox.isValid()){
                edgesQuadTree.push_back(new QuadTreeNode<unsigned int>(edgesGlobalBoundingBox));
            }else{
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
                    for(size_t i=0;i<nbSimples;++i){
                        entitiesQuadTree[quadTreesVectorPosition]->insert(layerLODUnit->simpleEntitiesLODVector[i].boundingBox,layerLODUnit->simpleEntitiesLODVector[i].entity);
                    }
                }
#ifdef _OPENMP
#pragma omp sections nowait
#endif
                {
                    for(size_t i=0;i<nbNodes;++i){
                        nodesQuadTree[quadTreesVectorPosition]->insert(layerLODUnit->nodesLODVector[i].boundingBox,layerLODUnit->nodesLODVector[i].id);
                    }
                }
#ifdef _OPENMP
#pragma omp sections nowait
#endif
                {
                    for(size_t i=0;i<nbEdges;++i){
                        edgesQuadTree[quadTreesVectorPosition]->insert(layerLODUnit->edgesLODVector[i].boundingBox,layerLODUnit->edgesLODVector[i].id);
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
        pSrc[1] = transformedViewport[1];
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
                if((renderingEntitiesFlag & RenderingNodes)!=0){
                    if(aX==0 && aY==0){
                        if((renderingEntitiesFlag & RenderingWithoutRemove)==0){
                            if(nodesQuadTree[quadTreesVectorPosition])
                                nodesQuadTree[quadTreesVectorPosition]->getElementsWithRatio(cameraBoundingBox,resNodes,ratio);
                        }else{
                            if(nodesQuadTree[quadTreesVectorPosition])
                                nodesQuadTree[quadTreesVectorPosition]->getElements(cameraBoundingBox,resNodes);
                        }
                    }else{
                        if(nodesQuadTree[quadTreesVectorPosition])
                            nodesQuadTree[quadTreesVectorPosition]->getElements(resNodes);
                    }

                    GlNode glNode(0);
                    layerLODUnit->nodesLODVector.reserve(resNodes.size());
                    for(size_t i=0;i<resNodes.size();++i){
                        glNode.id=resNodes[i];
                        layerLODUnit->nodesLODVector.push_back(ComplexEntityLODUnit(resNodes[i],glNode.getBoundingBox(inputData)));
                    }
                }
            }
#ifdef _OPENMP
#pragma omp sections nowait
#endif
            {
                if((renderingEntitiesFlag & RenderingEdges)!=0){
                    if(aX==0 && aY==0){
                        if((renderingEntitiesFlag & RenderingWithoutRemove)==0){
                            if(edgesQuadTree[quadTreesVectorPosition])
                                edgesQuadTree[quadTreesVectorPosition]->getElementsWithRatio(cameraBoundingBox,resEdges,ratio);
                        }else{
                            if(edgesQuadTree[quadTreesVectorPosition])
                                edgesQuadTree[quadTreesVectorPosition]->getElements(cameraBoundingBox,resEdges);
                        }
                    }else{
                        if(edgesQuadTree[quadTreesVectorPosition])
                            edgesQuadTree[quadTreesVectorPosition]->getElements(resEdges);
                    }

                    GlEdge glEdge(0);
                    layerLODUnit->edgesLODVector.reserve(resEdges.size());
                    for(size_t i=0;i<resEdges.size();++i){
                        glEdge.id=resEdges[i];
                        layerLODUnit->edgesLODVector.push_back(ComplexEntityLODUnit(resEdges[i],glEdge.getBoundingBox(inputData)));
                    }
                }
            }
#ifdef _OPENMP
#pragma omp sections nowait
#endif
            {
                if((renderingEntitiesFlag & RenderingSimpleEntities)!=0 && entitiesQuadTree[quadTreesVectorPosition]!=NULL){
                    if(aX==0 && aY==0){
                        if((renderingEntitiesFlag & RenderingWithoutRemove)==0)
                            entitiesQuadTree[quadTreesVectorPosition]->getElementsWithRatio(cameraBoundingBox, resEntities, ratio);
                        else
                            entitiesQuadTree[quadTreesVectorPosition]->getElements(cameraBoundingBox,resEntities);
                    }else{
                        entitiesQuadTree[quadTreesVectorPosition]->getElements(resEntities);
                    }
                    for(size_t i=0;i<resEntities.size();++i){
                        layerLODUnit->simpleEntitiesLODVector.push_back(SimpleEntityLODUnit(resEntities[i],((GlSimpleEntity*)(resEntities[i]))->getBoundingBox()));
                    }
                }
            }
        }

        GlCPULODCalculator::computeFor3DCamera(layerLODUnit,eye,transformMatrix,globalViewport,currentViewport);

    }

    void GlQuadTreeLODCalculator::removeObservers() {
        if(inputData){
            if(currentGraph)
                currentGraph->removeGraphObserver(this);
            if(layoutProperty)
                layoutProperty->removeObserver(this);
            if(sizeProperty)
                sizeProperty->removeObserver(this);
            if(selectionProperty)
                selectionProperty->removeObserver(this);
        }
        glScene->removeObserver(this);
    }

    void GlQuadTreeLODCalculator::addObservers() {
        if(inputData){
            currentGraph=inputData->getGraph();
            currentGraph->addGraphObserver(this);
            layoutProperty=currentGraph->getProperty(inputData->getElementLayoutPropName());
            layoutProperty->addObserver(this);
            sizeProperty=currentGraph->getProperty(inputData->getElementSizePropName());
            sizeProperty->addObserver(this);
            selectionProperty=currentGraph->getProperty(inputData->getElementSelectedPropName());
            selectionProperty->addObserver(this);
        }
        glScene->addObserver(this);
    }

    void GlQuadTreeLODCalculator::update(std::set<Observable *>::iterator it,std::set<Observable *>::iterator itEnd){
        bool needCompute=false;
        while(it!=itEnd){
            if((*it)==inputData->elementLayout || (*it)==inputData->elementSize){
                needCompute=true;
                break;
            }
            ++it;
        }

        if(needCompute)
            setHaveToCompute();
    }

    void GlQuadTreeLODCalculator::addLocalProperty(Graph*, const std::string &name){
        if(name==inputData->getElementLayoutPropName() || name==inputData->getElementSizePropName()){
            setHaveToCompute();
            addObservers();
        }
    }
    void GlQuadTreeLODCalculator::delLocalProperty(Graph*, const std::string &name){
        if(name==inputData->getElementLayoutPropName() || name==inputData->getElementSizePropName()){
            setHaveToCompute();
            addObservers();
        }
    }

    void GlQuadTreeLODCalculator::destroy(Graph *) {
      clear();
      setInputData(NULL);
    }

    void GlQuadTreeLODCalculator::destroy(Camera *camera) {
      std::vector<std::pair<std::string, GlLayer*> > *layerList=glScene->getLayersList();

      clearCamerasObservers();

      cameras.clear();

      for(std::vector<std::pair<std::string, GlLayer*> >::iterator it=layerList->begin();it!=layerList->end();++it){
        if(layerToCamera.find((*it).second)==layerToCamera.end())
          continue;
        (*layerToCamera.find((*it).second)).second.first=&(*it).second->getCamera();
        cameras.push_back(&(*it).second->getCamera());
      }

      initCamerasObservers();
    }

    void GlQuadTreeLODCalculator::initCamerasObservers() {
      for(vector<Camera *>::iterator it=cameras.begin();it!=cameras.end();++it){
        (*it)->addObserver(this);
      }
    }

    void GlQuadTreeLODCalculator::clearCamerasObservers(){
      for(vector<Camera *>::iterator it=cameras.begin();it!=cameras.end();++it){
        (*it)->removeObserver(this);
      }
    }

    void GlQuadTreeLODCalculator::setHaveToCompute(){
        if(haveToCompute)
            return;

        haveToCompute=true;

        if(!inputData)
            return;
        if(!glScene)
            return;

        removeObservers();
    }
}

