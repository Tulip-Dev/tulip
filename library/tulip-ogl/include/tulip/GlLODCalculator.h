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

  typedef std::pair<unsigned long, float> LODResultSimpleEntity;
  typedef std::pair<unsigned int, float> LODResultComplexEntity;
  typedef std::vector<LODResultSimpleEntity> SimpleLODResultVector;
  typedef std::vector<LODResultComplexEntity> ComplexLODResultVector;
  typedef std::vector<SimpleLODResultVector> VectorOfSimpleLODResultVector;
  typedef std::vector<ComplexLODResultVector> VectorOfComplexLODResultVector;
  typedef std::vector<unsigned long> VectorOfCamera;

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
     * Record a new node in current camera context
     */
    virtual void addNodeBoundingBox(unsigned int id,const BoundingBox& bb)=0;
    /**
     * Record a new edge in current camera context
     */
    virtual void addEdgeBoundingBox(unsigned int id,const BoundingBox& bb)=0;

    /**
     * Compute all lod
     */
    virtual void compute(const Vector<int,4>& globalViewport,const Vector<int,4>& currentViewport)=0;

    /**
     * Return lod result for simple entities
     */
    virtual VectorOfSimpleLODResultVector* getResultForSimpleEntities() = 0;
    /**
     * Return lod result for nodes
     */
    virtual VectorOfComplexLODResultVector* getResultForNodes() = 0;
    /**
     * Return lod result for edges
     */
    virtual VectorOfComplexLODResultVector* getResultForEdges() = 0;
    /**
     * Return cameras vector
     */
    virtual VectorOfCamera* getVectorOfCamera() = 0;
    /**
     * Clear class data
     */
    virtual void clear() = 0;
  };

}

#endif // DOXYGEN_NOTFOR_DEVEL

#endif // Tulip_GLLODCALCULATOR_H
