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

#include <tulip/BoundingBox.h>
#include <tulip/GlCPULODCalculator.h>
#include <tulip/Matrix.h>
#include <tulip/Vector.h>

namespace tlp {

  class Camera;

  class TLP_QT_SCOPE QtCPULODCalculatorThread : public QThread {

    Q_OBJECT

  public :

    void run();

  protected:

    BoundingBox *boundingBoxTab;
    float *resultTab;
    unsigned int size;
    Coord eye;
    Matrix<float, 4> transformMatrix;
    Vector<int,4> globalViewport;
    Vector<int,4> currentViewport;

  public:

    QtCPULODCalculatorThread(BoundingBox *bbTab, float *rTab,unsigned int sz,const Coord &e,const Matrix<float,4> &tfMatrix, const Vector<int,4> &global, const Vector<int,4> &current)
    :boundingBoxTab(bbTab), resultTab(rTab), size(sz), eye(e),transformMatrix(tfMatrix), globalViewport(global),currentViewport(current){}
  };

  /**
   * Class use to compute bounding boxs of a vector of GlEntity
   */
  class TLP_QT_SCOPE QtCPULODCalculator : public GlCPULODCalculator {

  public:

    virtual GlLODCalculator *clone(){return new QtCPULODCalculator;}
    virtual void computeFor3DCamera(SimpleBoundingBoxVector *inputSimple,ComplexBoundingBoxVector *inputNodes,ComplexBoundingBoxVector *inputEdges,
        SimpleLODResultVector *outputSimple, ComplexLODResultVector *outputNodes, ComplexLODResultVector *outputEdges,
        const Coord &eye,
        const Matrix<float, 4> transformMatrix,
        const Vector<int,4>& globalViewport,
        const Vector<int,4>& currentViewport);

  };

}

#endif // Tulip_GLCPULODCALCULATOR_H
