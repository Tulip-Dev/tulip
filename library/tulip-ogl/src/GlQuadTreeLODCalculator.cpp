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

  GlQuadTreeLODCalculator::GlQuadTreeLODCalculator() : scene(NULL),haveToCompute(true),inputData(NULL),currentGraph(NULL),layoutProperty(NULL),sizeProperty(NULL),selectionProperty(NULL) {
  }

  GlQuadTreeLODCalculator::~GlQuadTreeLODCalculator() {
    removeObservers();
  }

  void GlQuadTreeLODCalculator::setScene(GlScene *scene){
    // If we change scene, whe have to remove observer on the graph and the scene
    // in the next rendering, we have to compute quadtree
    if(this->scene)
      removeObservers();
    this->scene=scene;
    haveToCompute=true;
  }

  bool GlQuadTreeLODCalculator::needEntities(){
    // Check if quadtree need entities
    if(haveToCompute)
      return true;

    // Check if a camera have changed (diff between old backup camera and current camera)
    for(vector<pair<Camera *,Camera> >::iterator it=cameras.begin();it!=cameras.end();++it){
      if(((*it).first)->is3D()){
        Camera *camera=(*it).first;
        Camera *oldCamera=&(*it).second;
        Coord unitCamera=camera->getEyes()-camera->getCenter();
        unitCamera=unitCamera/unitCamera.norm();
        Coord unitOldCamera=oldCamera->getEyes()-oldCamera->getCenter();
        unitOldCamera=unitOldCamera/unitOldCamera.norm();
        if(unitCamera!=unitOldCamera){
          haveToCompute=true;
          return true;
        }
      }
    }

    return false;
  }

  void GlQuadTreeLODCalculator::setNeedEntities(bool){
    haveToCompute=true;
    removeObservers();
  }

  void GlQuadTreeLODCalculator::addSimpleEntityBoundingBox(unsigned long entity,const BoundingBox& bb) {
    GlCPULODCalculator::addSimpleEntityBoundingBox(entity,bb);
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

      // Clear all vectors
      cameras.clear();
      simpleEntities.clear();
      nodesQuadTree.clear();
      edgesQuadTree.clear();
      nodesSelectedQuadTree.clear();
      edgesSelectedQuadTree.clear();
      entitiesQuadTree.clear();

      quadTreesVectorPosition=0;
      for(vector<LayerLODUnit>::iterator it=layersLODVector.begin();it!=layersLODVector.end();++it){
        Camera *camera=(Camera*)((*it).camera);

        cameras.push_back(pair<Camera*,Camera>(camera,*camera));

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

      haveToCompute = false;

    }else{
      // if don't have to compute : use stored quadtree data

      layersLODVector.clear();

      quadTreesVectorPosition=0;
      simpleEntitiesVectorPosition=0;
      for(vector<pair<Camera*,Camera> >::iterator it=cameras.begin();it!=cameras.end();++it){

        Camera *camera=(*it).first;
        layersLODVector.push_back(LayerLODUnit());
        LayerLODUnit *layerLODUnit=&(layersLODVector.back());
        layerLODUnit->camera=(unsigned long)camera;

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
      entitiesQuadTree.push_back(QuadTreeNode<unsigned long>(entitiesGlobalBoundingBox));
      nodesQuadTree.push_back(QuadTreeNode<unsigned int>(nodesGlobalBoundingBox));
      edgesQuadTree.push_back(QuadTreeNode<unsigned int>(edgesGlobalBoundingBox));
      nodesSelectedQuadTree.push_back(QuadTreeNode<unsigned int>(nodesGlobalBoundingBox));
      edgesSelectedQuadTree.push_back(QuadTreeNode<unsigned int>(edgesGlobalBoundingBox));

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
            entitiesQuadTree[quadTreesVectorPosition].insert(layerLODUnit->simpleEntitiesLODVector[i].boundingBox,layerLODUnit->simpleEntitiesLODVector[i].id);
          }
        }
#ifdef _OPENMP
#pragma omp sections nowait
#endif
        {
          for(size_t i=0;i<nbNodes;++i){
            assert(selectedProperty);
            QuadTreeNode<unsigned int> *quadTree;
            if(selectedProperty->getNodeValue(node(layerLODUnit->nodesLODVector[i].id)))
              quadTree=&nodesSelectedQuadTree[quadTreesVectorPosition];
            else
              quadTree=&nodesQuadTree[quadTreesVectorPosition];

            quadTree->insert(layerLODUnit->nodesLODVector[i].boundingBox,layerLODUnit->nodesLODVector[i].id);
          }
        }
#ifdef _OPENMP
#pragma omp sections nowait
#endif
        {
          for(size_t i=0;i<nbEdges;++i){
            assert(selectedProperty);
            QuadTreeNode<unsigned int> *quadTree;
            if(selectedProperty->getEdgeValue(edge(layerLODUnit->edgesLODVector[i].id)))
              quadTree=&edgesSelectedQuadTree[quadTreesVectorPosition];
            else
              quadTree=&edgesQuadTree[quadTreesVectorPosition];

            quadTree->insert(layerLODUnit->edgesLODVector[i].boundingBox,layerLODUnit->edgesLODVector[i].id);
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
    vector<unsigned long> resEntities;

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
              nodesQuadTree[quadTreesVectorPosition].getElementsWithRatio(cameraBoundingBox,resNodes,ratio);
              nodesSelectedQuadTree[quadTreesVectorPosition].getElementsWithRatio(cameraBoundingBox,resNodes,ratio);
            }else{
              nodesQuadTree[quadTreesVectorPosition].getElements(cameraBoundingBox,resNodes);
              nodesSelectedQuadTree[quadTreesVectorPosition].getElements(cameraBoundingBox,resNodes);
            }
          }else{
            nodesQuadTree[quadTreesVectorPosition].getElements(resNodes);
            nodesSelectedQuadTree[quadTreesVectorPosition].getElements(resNodes);
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
              edgesQuadTree[quadTreesVectorPosition].getElementsWithRatio(cameraBoundingBox,resEdges,ratio);
              edgesSelectedQuadTree[quadTreesVectorPosition].getElementsWithRatio(cameraBoundingBox,resEdges,ratio);
            }else{
              edgesQuadTree[quadTreesVectorPosition].getElements(cameraBoundingBox,resEdges);
              edgesSelectedQuadTree[quadTreesVectorPosition].getElements(cameraBoundingBox,resEdges);
            }
          }else{
            edgesQuadTree[quadTreesVectorPosition].getElements(resEdges);
            edgesSelectedQuadTree[quadTreesVectorPosition].getElements(resEdges);
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
        if((renderingEntitiesFlag & RenderingSimpleEntities)!=0){
          if(aX==0 && aY==0){
            if((renderingEntitiesFlag & RenderingWithoutRemove)==0)
              entitiesQuadTree[quadTreesVectorPosition].getElementsWithRatio(cameraBoundingBox,resEntities,ratio);
            else
              entitiesQuadTree[quadTreesVectorPosition].getElements(cameraBoundingBox,resEntities);
          }else{
            entitiesQuadTree[quadTreesVectorPosition].getElements(resEntities);
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
    scene->removeObserver(this);
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
    scene->addObserver(this);
  }

  void GlQuadTreeLODCalculator::setInputData(GlGraphInputData *newInputData){
    if(inputData)
      removeObservers();

    inputData=newInputData;
    if (newInputData == NULL) {
      currentCamera = NULL;
      currentGraph = NULL;
      layoutProperty = NULL;
      sizeProperty = NULL;
      selectionProperty = NULL;
    }
    haveToCompute=true;
  }

  void GlQuadTreeLODCalculator::update(std::set<Observable *>::iterator,std::set<Observable *>::iterator){
    haveToCompute=true;
    removeObservers();
  }

  void GlQuadTreeLODCalculator::addLocalProperty(Graph*, const std::string &name){
    if(name==inputData->getElementLayoutPropName() || name==inputData->getElementSizePropName()){
      haveToCompute=true;
      removeObservers();
      addObservers();
    }
  }
  void GlQuadTreeLODCalculator::delLocalProperty(Graph*, const std::string &name){
    if(name==inputData->getElementLayoutPropName() || name==inputData->getElementSizePropName()){
      haveToCompute=true;
      removeObservers();
      addObservers();
    }
  }

  void GlQuadTreeLODCalculator::destroy(Graph *) {
    clear();
    setInputData( NULL);
  }
}

