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

  typedef std::pair<unsigned int, float> LODResultEntity;
  typedef std::pair<unsigned int, std::vector<LODResultEntity> > LODResultCameraAndEntities;
  typedef std::vector<LODResultCameraAndEntities> LODResultVector;
  
  class TLP_GL_SCOPE GlLODCalculator {
  
  public:

    virtual ~GlLODCalculator() {}

    virtual void beginNewCamera(Camera* camera)=0;
    virtual void addSimpleEntityBoundingBox(unsigned int entity,const BoundingBox& bb)=0;
    virtual void addComplexeEntityBoundingBox(unsigned int entity,const BoundingBox& bb)=0;

    virtual void compute(const Vector<int,4>& viewport)=0;
    
    virtual LODResultVector* getResultForSimpleEntities() = 0;
    virtual LODResultVector* getResultForComplexeEntities() = 0;
  };
 
}

#endif // Tulip_GLLODCALCULATOR_H
