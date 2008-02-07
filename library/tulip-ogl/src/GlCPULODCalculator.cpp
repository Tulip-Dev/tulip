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

  /* static char hullVertexTable[][7] = {
    {0,0,0,0,0,0,0},//0
    {4,0,4,7,3,0,0},//1
    {4,1,2,6,5,0,0},//2
    {0,0,0,0,0,0,0},//3
    {4,0,1,5,4,0,0},//4
    {6,0,1,5,4,7,3},//5
    {6,0,1,2,6,5,4},//6
    {0,0,0,0,0,0,0},//7
    {4,2,3,7,6,0,0},//8
    {6,4,7,6,2,3,0},//9
    {6,2,3,7,6,5,1},//10
    {0,0,0,0,0,0,0},//11
    {0,0,0,0,0,0,0},//12
    {0,0,0,0,0,0,0},//13
    {0,0,0,0,0,0,0},//14
    {0,0,0,0,0,0,0},//15
    {4,0,3,2,1,0,0},//16
    {6,0,4,7,3,2,1},//17
    {6,0,3,2,6,5,1},//18
    {0,0,0,0,0,0,0},//19
    {6,0,3,2,1,5,4},//20
    {6,2,1,5,4,7,3},//21
    {6,0,3,2,6,5,4},//22
    {0,0,0,0,0,0,0},//23
    {6,0,3,7,6,2,1},//24
    {6,0,4,7,6,2,1},//25
    {6,0,3,7,6,5,1},//26
    {0,0,0,0,0,0,0},//27
    {0,0,0,0,0,0,0},//28
    {0,0,0,0,0,0,0},//29
    {0,0,0,0,0,0,0},//30
    {0,0,0,0,0,0,0},//31
    {4,4,5,6,7,0,0},//32
    {6,4,5,6,7,3,0},//33
    {6,1,2,6,7,4,5},//34
    {0,0,0,0,0,0,0},//35
    {6,0,1,5,6,7,4},//36
    {6,0,1,5,6,7,3},//37
    {6,0,1,2,6,7,4},//38
    {0,0,0,0,0,0,0},//39
    {6,2,3,7,4,5,6},//40
    {6,0,4,5,6,2,3},//41
    {6,1,2,3,7,4,5}//42
    };  */

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
      
      Matrix<float,4> tmp;
      camera->getTransformMatrix(viewport,tmp);
      MatrixGL transformMatrix(tmp);
      float lod;

      Coord eye;
      if(camera->is3D())
	eye=camera->getEyes() + ( camera->getEyes() -camera->getCenter() ) / camera->getZoomFactor();
      
      for(BoundingBoxVector::iterator itV=(*itM).second.first->begin();itV!=(*itM).second.first->end();++itV){
	if(camera->is3D()){
	  lod=calculateAABBSize((*itV).second,eye,transformMatrix,viewport);
	}else{
	  lod=1;
	}
	if(lod>=0)
	  (*itSE).second.push_back(pair<unsigned int,float>((*itV).first,lod));
      }
      for(BoundingBoxVector::iterator itV=(*itM).second.second->begin();itV!=(*itM).second.second->end();++itV){
	if(camera->is3D()){
	  lod=calculateAABBSize((*itV).second,eye,transformMatrix,viewport);
	}else{
	  lod=1;
	}
	if(lod>=0)
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

