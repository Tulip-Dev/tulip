#include "tulip/GlCPULODCalculator.h"

#include <tulip/Matrix.h>

#include "tulip/Camera.h"
#include "tulip/GlEntity.h"
#include "tulip/GlTools.h"
#include "tulip/GlScene.h"

#include <iostream>

using namespace std;

namespace tlp {

  GlCPULODCalculator::~GlCPULODCalculator() {
  }

  void GlCPULODCalculator::beginNewCamera(Camera* camera) {
    cameraVector.push_back((unsigned long)camera);

    simpleBoundingBoxVector.push_back(SimpleBoundingBoxVector());
    nodesBoundingBoxVector.push_back(ComplexBoundingBoxVector());
    edgesBoundingBoxVector.push_back(ComplexBoundingBoxVector());

    actualSimpleBoundingBoxVector=&(simpleBoundingBoxVector.back());
    actualNodesBoundingBoxVector=&(nodesBoundingBoxVector.back());
    actualEdgesBoundingBoxVector=&(edgesBoundingBoxVector.back());
  }

  void GlCPULODCalculator::addSimpleEntityBoundingBox(unsigned long entity,const BoundingBox& bb) {
    actualSimpleBoundingBoxVector->push_back(SimpleBoundingBoxUnit(entity,bb));
  }
  void GlCPULODCalculator::addNodeBoundingBox(unsigned int id,const BoundingBox& bb) {
    actualNodesBoundingBoxVector->push_back(ComplexBoundingBoxUnit(id,bb));
  }
  void GlCPULODCalculator::addEdgeBoundingBox(unsigned int id,const BoundingBox& bb) {
    actualEdgesBoundingBoxVector->push_back(ComplexBoundingBoxUnit(id,bb));
  }

  void GlCPULODCalculator::compute(const Vector<int,4>& globalViewport,const Vector<int,4>& currentViewport,RenderingEntitiesFlag type) {
    this->type=type;
    assert(cameraVector.size()==simpleBoundingBoxVector.size());
    assert(cameraVector.size()==nodesBoundingBoxVector.size());
    assert(cameraVector.size()==edgesBoundingBoxVector.size());

    int i=0;
    for(VectorOfCamera::iterator it=cameraVector.begin();it!=cameraVector.end();++it){
      Camera *camera=(Camera*)(*it);

      simpleResultVector.push_back(std::vector<LODResultSimpleEntity>());
      nodesResultVector.push_back(std::vector<LODResultComplexEntity>());
      edgesResultVector.push_back(std::vector<LODResultComplexEntity>());

      Matrix<float,4> transformMatrix;
      camera->getTransformMatrix(globalViewport,transformMatrix);

      Coord eye;
      if(camera->is3D()) {
        eye=camera->getEyes() + ( camera->getEyes() -camera->getCenter() ) / camera->getZoomFactor();
        computeFor3DCamera(&simpleBoundingBoxVector[i],&nodesBoundingBoxVector[i],&edgesBoundingBoxVector[i],
            &(simpleResultVector.back()),&(nodesResultVector.back()),&(edgesResultVector.back()),
            eye,transformMatrix,globalViewport,currentViewport);
      }else{
        computeFor2DCamera(&simpleBoundingBoxVector[i],&nodesBoundingBoxVector[i],&edgesBoundingBoxVector[i],
            &(simpleResultVector.back()),&(nodesResultVector.back()),&(edgesResultVector.back()),
            globalViewport,currentViewport);
      }

      glMatrixMode(GL_MODELVIEW);
      ++i;
    }
  }

  void GlCPULODCalculator::computeFor3DCamera(SimpleBoundingBoxVector *inputSimple,ComplexBoundingBoxVector *inputNodes,ComplexBoundingBoxVector *inputEdges,
      SimpleLODResultVector *outputSimple, ComplexLODResultVector *outputNodes, ComplexLODResultVector *outputEdges,
      const Coord &eye,
      const Matrix<float, 4> transformMatrix,
      const Vector<int,4>& globalViewport,
      const Vector<int,4>& currentViewport) {

    float lod;
    if((type & RenderingSimpleEntities)!=0){
      for(SimpleBoundingBoxVector::iterator it=inputSimple->begin();it!=inputSimple->end();++it){
        lod=calculateAABBSize((*it).second,eye,transformMatrix,globalViewport,currentViewport);
        if(lod>=0)
          outputSimple->push_back(pair<unsigned long,float>((*it).first,lod));
      }
    }
    if((type & RenderingNodes)!=0){
      for(ComplexBoundingBoxVector::iterator it=inputNodes->begin();it!=inputNodes->end();++it){
        lod=calculateAABBSize((*it).second,eye,transformMatrix,globalViewport,currentViewport);
        if(lod>=0)
          outputNodes->push_back(pair<unsigned int,float>((*it).first,lod));
      }
    }
    if((type & RenderingEdges)!=0){
      for(ComplexBoundingBoxVector::iterator it=inputEdges->begin();it!=inputEdges->end();++it){
        lod=calculateAABBSize((*it).second,eye,transformMatrix,globalViewport,currentViewport);
        if(lod>=0)
          outputEdges->push_back(pair<unsigned int,float>((*it).first,lod));
      }
    }
  }

  void GlCPULODCalculator::computeFor2DCamera(SimpleBoundingBoxVector *inputSimple,ComplexBoundingBoxVector *inputNodes,ComplexBoundingBoxVector *inputEdges,
      SimpleLODResultVector *outputSimple, ComplexLODResultVector *outputNodes, ComplexLODResultVector *outputEdges,
      const Vector<int,4>& globalViewport,
      const Vector<int,4>& currentViewport) {

    float lod;
    if((type & RenderingSimpleEntities)!=0){
      for(SimpleBoundingBoxVector::iterator it=inputSimple->begin();it!=inputSimple->end();++it){
        lod=calculate2DLod((*it).second,globalViewport,currentViewport);
        if(lod>=0)
          outputSimple->push_back(pair<unsigned long,float>((*it).first,lod));
      }
    }
    if((type & RenderingNodes)!=0){
      for(ComplexBoundingBoxVector::iterator it=inputNodes->begin();it!=inputNodes->end();++it){
        lod=calculate2DLod((*it).second,globalViewport,currentViewport);
        if(lod>=0)
          outputNodes->push_back(pair<unsigned int,float>((*it).first,lod));
      }
    }
    if((type & RenderingEdges)!=0){
      for(ComplexBoundingBoxVector::iterator it=inputEdges->begin();it!=inputEdges->end();++it){
        lod=calculate2DLod((*it).second,globalViewport,currentViewport);
        if(lod>=0)
          outputEdges->push_back(pair<unsigned int,float>((*it).first,lod));
      }
    }
  }

  VectorOfSimpleLODResultVector* GlCPULODCalculator::getResultForSimpleEntities(){
    return &simpleResultVector;
  }
  VectorOfComplexLODResultVector* GlCPULODCalculator::getResultForNodes(){
    return &nodesResultVector;
  }
  VectorOfComplexLODResultVector* GlCPULODCalculator::getResultForEdges(){
    return &edgesResultVector;
  }
  VectorOfCamera* GlCPULODCalculator::getVectorOfCamera(){
    return &cameraVector;
  }

  void GlCPULODCalculator::clear() {
    simpleBoundingBoxVector.clear();
    nodesBoundingBoxVector.clear();
    edgesBoundingBoxVector.clear();
    simpleResultVector.clear();
    nodesResultVector.clear();
    edgesResultVector.clear();
    cameraVector.clear();
  }

}

