//-*-c++-*-
/**
 Author: Morgan Mathiaut
 Email : mathiaut@labri.fr
 Last modification : 25/10/2007
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/
#ifndef Tulip_QTCPULODCALCULATOR_H
#define Tulip_QTCPULODCALCULATOR_H

#include <map>
#include <vector>

#include <QtCore/QThread>
#include <QtCore/QMutex>
#include <GL/gl.h>
#include <GL/glu.h>

#include <tulip/BoundingBox.h>
#include <tulip/GlCPULODCalculator.h>
#include <tulip/Matrix.h>
#include <tulip/Vector.h>

namespace tlp {
  
  class Camera;

  class TLP_GL_SCOPE QtCPULODCalculatorThread : public QThread {
    
    Q_OBJECT

  public:
    
    QtCPULODCalculatorThread(QMutex *mutex,BoundingBox *boundingBoxTab,float *resultTab,unsigned int size,const Coord &eye,const Matrix<float,4> &transformMatrix, const Vector<int,4> &globalViewport, const Vector<int,4> &currentViewport); 

  protected:

    void run();

    QMutex *mutex;
    BoundingBox *boundingBoxTab;
    float *resultTab;
    unsigned int size;
    Coord eye;
    Matrix<float, 4> transformMatrix;
    Vector<int,4> globalViewport;
    Vector<int,4> currentViewport;
  };

  /**
   * Class use to compute bounding boxs of a vector of GlEntity
   */
  class TLP_QT_SCOPE QtCPULODCalculator : public GlCPULODCalculator {
  
  public:

    virtual void computeFor3DCamera(const std::pair<BoundingBoxVector*,BoundingBoxVector*> &entities,
				    const LODResultVector::iterator &itSEOutput, 
				    const LODResultVector::iterator &itCEOutput,
				    const Coord &eye,
				    const Matrix<float, 4> transformMatrix,
				    const Vector<int,4>& globalViewport,
				    const Vector<int,4>& currentViewport);

  };
 
}

#endif // Tulip_GLCPULODCALCULATOR_H
