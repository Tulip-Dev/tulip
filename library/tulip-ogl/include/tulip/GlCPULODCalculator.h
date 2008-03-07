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

  /**
   * Class use to compute bounding boxs of a vector of GlEntity
   */
  class TLP_GL_SCOPE GlCPULODCalculator : public GlLODCalculator {
  
  public:

    virtual ~GlCPULODCalculator();

    /**
     * Begin a new camera (use to render next entities)
     */
    virtual void beginNewCamera(Camera* camera);
    /**
     * Add a bounding box for a simple entity
     */
    virtual void addSimpleEntityBoundingBox(unsigned long entity,const BoundingBox& bb);
    /**
     * Add a bounding box for a complexe entity
     */
    virtual void addComplexeEntityBoundingBox(unsigned long entity,const BoundingBox& bb);

    /**
     * Compute all bounding boxs with the given viewport
     */
    virtual void compute(const Vector<int,4>& viewport);
    
    /**
     * Return the result vector for simpleEntities
     */
    virtual LODResultVector* getResultForSimpleEntities();
    /**
     * Return the result vector for complexeEntities
     */
    virtual LODResultVector* getResultForComplexeEntities();

  protected:
    
    typedef std::pair<unsigned long, BoundingBox> BoundingBoxUnit;
    typedef std::vector<BoundingBoxUnit> BoundingBoxVector;
    typedef std::pair<unsigned long, std::pair<BoundingBoxVector*,BoundingBoxVector*> > CameraAndBoundingBoxUnit;
    typedef std::vector<CameraAndBoundingBoxUnit> CameraAndBoundingBoxVector;
    
    BoundingBoxVector* actualSEBoundingBoxVector;
    BoundingBoxVector* actualCEBoundingBoxVector;
    CameraAndBoundingBoxVector boundingBoxVector;

    LODResultVector seResultVector;
    LODResultVector ceResultVector;

  };
 
}

#endif // Tulip_GLCPULODCALCULATOR_H
