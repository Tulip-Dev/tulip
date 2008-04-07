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
#ifndef Tulip_GLLODCALCULATOR_H
#define Tulip_GLLODCALCULATOR_H
#ifndef DOXYGEN_NOTFOR_DEVEL

#include <tulip/tulipconf.h>

#if (__GNUC__ < 3)
#include <hash_map>
#else
#include <ext/hash_map>
#endif

#include <tulip/Matrix.h>
#include <tulip/Vector.h>

namespace tlp {

  class Camera;
  class GlEntity;

  typedef std::pair<unsigned long, float> LODResultEntity;
  typedef std::pair<unsigned long, std::vector<LODResultEntity> > LODResultCameraAndEntities;
  typedef std::vector<LODResultCameraAndEntities> LODResultVector;

  /**
   * Class use to calculate lod of scene entities
   */
  class TLP_GL_SCOPE GlLODCalculator {
  
  public:

    virtual ~GlLODCalculator() {}

    /**
     * Begin a new camera
     */
    virtual void beginNewCamera(Camera* camera)=0;
    /**
     * Record a new simple entity in current camera context 
     */
    virtual void addSimpleEntityBoundingBox(unsigned long entity,const BoundingBox& bb)=0;
    /**
     * Record a new complex entity in current camera context
     */
    virtual void addComplexeEntityBoundingBox(unsigned long entity,const BoundingBox& bb)=0;

    /**
     * Compute all lod
     */
    virtual void compute(const Vector<int,4>& globalViewport,const Vector<int,4>& currentViewport)=0;
    
    /**
     * Return lod result for simple entities
     */
    virtual LODResultVector* getResultForSimpleEntities() = 0;
    /**
     * Return lod result for complexe entities
     */
    virtual LODResultVector* getResultForComplexeEntities() = 0;
    /**
     * Clear class data
     */
    virtual void clear() = 0;
  };
 
}

#endif // DOXYGEN_NOTFOR_DEVEL

#endif // Tulip_GLLODCALCULATOR_H
