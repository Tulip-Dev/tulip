#include "tulip/GlCPULODCalculator.h"

#include <GL/glu.h>
#include <GL/gl.h>

#include <tulip/Matrix.h>

#include "tulip/Camera.h"
#include "tulip/GlEntity.h"
#include "tulip/GlTools.h"
#include "tulip/GlScene.h"

#include <iostream>

using namespace std;

namespace tlp {

  GlCPULODCalculator::~GlCPULODCalculator() {
    for(CameraAndBoundingBoxVector::iterator it=boundingBoxVector.begin();it!=boundingBoxVector.end();++it) {
      delete (*it).second.first;
      delete (*it).second.second;
    }
  }

  void GlCPULODCalculator::beginNewCamera(Camera* camera) {
    actualSEBoundingBoxVector=new BoundingBoxVector();
    actualCEBoundingBoxVector=new BoundingBoxVector();

    pair<BoundingBoxVector*,BoundingBoxVector*> boundingBoxPair=pair<BoundingBoxVector*,BoundingBoxVector*>(actualSEBoundingBoxVector,actualCEBoundingBoxVector);
    boundingBoxVector.push_back(pair<unsigned int ,pair<BoundingBoxVector*,BoundingBoxVector*> >((unsigned int)camera,boundingBoxPair));
  }

  void GlCPULODCalculator::addSimpleEntityBoundingBox(unsigned int entity,const BoundingBox& bb) {
    actualSEBoundingBoxVector->push_back(BoundingBoxUnit(entity,bb));
  }
  void GlCPULODCalculator::addComplexeEntityBoundingBox(unsigned int entity,const BoundingBox& bb) {
    actualCEBoundingBoxVector->push_back(BoundingBoxUnit(entity,bb));
  }
  
  void GlCPULODCalculator::compute(const Vector<int,4>& viewport) {
    for(CameraAndBoundingBoxVector::iterator itM=boundingBoxVector.begin();itM!=boundingBoxVector.end();++itM) {
      		       
      Camera *camera=(Camera*)((*itM).first);

      seResultVector.push_back(LODResultCameraAndEntities((*itM).first,vector<LODResultEntity>()));
      ceResultVector.push_back(LODResultCameraAndEntities((*itM).first,vector<LODResultEntity>()));
      LODResultVector::iterator itSE=--seResultVector.end();
      LODResultVector::iterator itCE=--ceResultVector.end();
      
      Matrix<float,4> modelviewMatrix;
      Matrix<float,4> projectionMatrix;

      camera->getProjAndMVMatrix(viewport,projectionMatrix,modelviewMatrix);

      float lod;

      for(BoundingBoxVector::iterator itV=(*itM).second.first->begin();itV!=(*itM).second.first->end();++itV){
	if(camera->is3D())
	  lod=projectSize((*itV).second, projectionMatrix, modelviewMatrix, viewport);
	else
	  lod=1;
	if(lod>0)
	  (*itSE).second.push_back(pair<unsigned int,float>((*itV).first,lod));
      }
      for(BoundingBoxVector::iterator itV=(*itM).second.second->begin();itV!=(*itM).second.second->end();++itV){
	if(camera->is3D())
	  lod=projectSize((*itV).second, projectionMatrix, modelviewMatrix, viewport);
	else
	  lod=1;
	if(lod>0)
	  (*itCE).second.push_back(pair<unsigned int,float>((*itV).first,lod));
      }
      glMatrixMode(GL_MODELVIEW);
    }
  }
  
  LODResultVector* GlCPULODCalculator::getResultForSimpleEntities() {
    return &seResultVector;
  }

  LODResultVector* GlCPULODCalculator::getResultForComplexeEntities() {
    return &ceResultVector;
  }
 
}

