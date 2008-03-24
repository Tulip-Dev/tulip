#include "tulip/QtCPULODCalculator.h"

#include <GL/glu.h>
#include <GL/gl.h>

#include <tulip/Matrix.h>

#include <tulip/Camera.h>
#include <tulip/GlEntity.h>
#include <tulip/GlTools.h>
#include <tulip/GlScene.h>

#include <iostream>

using namespace std;

namespace tlp {
  
  
  void QtCPULODCalculator::computeFor3DCamera(const pair<BoundingBoxVector*,BoundingBoxVector*> &entities,
					      const LODResultVector::iterator &itSEOutput, 
					      const LODResultVector::iterator &itCEOutput,
					      const Coord &eye,
					      const Matrix<float, 4> transformMatrix,
					      const Vector<int,4>& globalViewport,
					      const Vector<int,4>& currentViewport) {
    unsigned int entitiesNumber=entities.first->size()+entities.second->size();
    BoundingBox *boundingBoxTab=new BoundingBox[entitiesNumber];
    float *resultTab=new float[entitiesNumber];
    unsigned int index=0;

    for(BoundingBoxVector::iterator itV=entities.first->begin();itV!=entities.first->end();++itV){
      boundingBoxTab[index]=(*itV).second;
      ++index;
    }
    for(BoundingBoxVector::iterator itV=entities.second->begin();itV!=entities.second->end();++itV){
      boundingBoxTab[index]=(*itV).second;
      ++index;
    }

    QtCPULODCalculatorThread thread1(boundingBoxTab,resultTab,index/2,eye,transformMatrix,globalViewport,currentViewport);
    QtCPULODCalculatorThread thread2(boundingBoxTab+index/2,resultTab+index/2,index-(index/2),eye,transformMatrix,globalViewport,currentViewport);
	  
    thread1.start();
    thread2.start();
    
    thread1.wait();
    thread2.wait();
	
    index=0;
    for(BoundingBoxVector::iterator itV=entities.first->begin();itV!=entities.first->end();++itV){
      if(resultTab[index]>=0)
	(*itSEOutput).second.push_back(pair<unsigned long,float>((*itV).first,resultTab[index]));
      index++;
    }
    for(BoundingBoxVector::iterator itV=entities.second->begin();itV!=entities.second->end();++itV){
      if(resultTab[index]>=0)
	(*itCEOutput).second.push_back(pair<unsigned long,float>((*itV).first,resultTab[index]));
      index++;
    }
    
    delete[] boundingBoxTab;
    delete[] resultTab;
  }
 
  void QtCPULODCalculatorThread::run() {
    for(unsigned int i=0;i<size;++i){
      resultTab[i]=calculateAABBSize(boundingBoxTab[i],eye,transformMatrix,globalViewport,currentViewport);
    }
  }

}

