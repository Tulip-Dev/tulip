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
#ifndef Tulip_GLCPULODCALCULATOR_H
#define Tulip_GLCPULODCALCULATOR_H

#include <map>
#include <vector>

#include <tulip/BoundingBox.h>

#include "tulip/GlLODCalculator.h"

namespace tlp {
  
  class Camera;

  class TLP_GL_SCOPE GlCPULODCalculator : public GlLODCalculator {
  
  public:

    virtual ~GlCPULODCalculator();

    virtual void beginNewCamera(Camera* camera);
    virtual void addSimpleEntityBoundingBox(unsigned int entity,const BoundingBox& bb);
    virtual void addComplexeEntityBoundingBox(unsigned int entity,const BoundingBox& bb);

    virtual void compute(const Vector<int,4>& viewport);
    
    virtual LODResultVector* getResultForSimpleEntities();
    virtual LODResultVector* getResultForComplexeEntities();

  protected:
    
    typedef std::pair<unsigned int, BoundingBox> BoundingBoxUnit;
    typedef std::vector<BoundingBoxUnit> BoundingBoxVector;
    typedef std::pair<unsigned int, std::pair<BoundingBoxVector*,BoundingBoxVector*> > CameraAndBoundingBoxUnit;
    typedef std::vector<CameraAndBoundingBoxUnit> CameraAndBoundingBoxVector;
    
    BoundingBoxVector* actualSEBoundingBoxVector;
    BoundingBoxVector* actualCEBoundingBoxVector;
    CameraAndBoundingBoxVector boundingBoxVector;

    LODResultVector seResultVector;
    LODResultVector ceResultVector;

  };
 
}

#endif // Tulip_GLCPULODCALCULATOR_H
