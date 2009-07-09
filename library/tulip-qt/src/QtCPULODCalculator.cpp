#include "tulip/QtCPULODCalculator.h"

#include <tulip/Matrix.h>

#include <tulip/Camera.h>
#include <tulip/GlEntity.h>
#include <tulip/GlTools.h>
#include <tulip/GlScene.h>

#include <iostream>

using namespace std;

namespace tlp {


  void QtCPULODCalculator::computeFor3DCamera(SimpleBoundingBoxVector *inputSimple,ComplexBoundingBoxVector *inputNodes,ComplexBoundingBoxVector *inputEdges,
      SimpleLODResultVector *outputSimple, ComplexLODResultVector *outputNodes, ComplexLODResultVector *outputEdges,
      const Coord &eye,
      const Matrix<float, 4> transformMatrix,
      const Vector<int,4>& globalViewport,
      const Vector<int,4>& currentViewport) {
    unsigned int entitiesNumber=inputSimple->size()+inputNodes->size()+inputEdges->size();
    BoundingBox *boundingBoxTab=new BoundingBox[entitiesNumber];
    float *resultTab=new float[entitiesNumber];
    unsigned int index=0;

    for(SimpleBoundingBoxVector::iterator it=inputSimple->begin();it!=inputSimple->end();++it){
      boundingBoxTab[index]=(*it).second;
      ++index;
    }
    for(ComplexBoundingBoxVector::iterator it=inputNodes->begin();it!=inputNodes->end();++it){
      boundingBoxTab[index]=(*it).second;
      ++index;
    }
    for(ComplexBoundingBoxVector::iterator it=inputEdges->begin();it!=inputEdges->end();++it){
      boundingBoxTab[index]=(*it).second;
      ++index;
    }

    if(index>1000){
      QtCPULODCalculatorThread thread1(boundingBoxTab,resultTab,index/2,eye,transformMatrix,globalViewport,currentViewport);
      QtCPULODCalculatorThread thread2(boundingBoxTab+index/2,resultTab+index/2,index-(index/2),eye,transformMatrix,globalViewport,currentViewport);

      thread1.start();
      thread2.start();

      thread1.wait();
      thread2.wait();
    }else{
      QtCPULODCalculatorThread thread1(boundingBoxTab,resultTab,index,eye,transformMatrix,globalViewport,currentViewport);
      thread1.run();
    }

    index=0;
    for(SimpleBoundingBoxVector::iterator it=inputSimple->begin();it!=inputSimple->end();++it){
      if(resultTab[index]>=0)
        outputSimple->push_back(pair<unsigned long,float>((*it).first,resultTab[index]));
      index++;
    }
    for(ComplexBoundingBoxVector::iterator it=inputNodes->begin();it!=inputNodes->end();++it){
      if(resultTab[index]>=0)
        outputNodes->push_back(pair<unsigned int,float>((*it).first,resultTab[index]));
      index++;
    }
    for(ComplexBoundingBoxVector::iterator it=inputEdges->begin();it!=inputEdges->end();++it){
      if(resultTab[index]>=0)
        outputEdges->push_back(pair<unsigned int,float>((*it).first,resultTab[index]));
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

