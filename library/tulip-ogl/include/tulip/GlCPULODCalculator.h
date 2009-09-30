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
#ifndef DOXYGEN_NOTFOR_DEVEL

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

  protected:

    typedef std::pair<unsigned long, BoundingBox> SimpleBoundingBoxUnit;
    typedef std::pair<unsigned int, BoundingBox> ComplexBoundingBoxUnit;
    typedef std::vector<SimpleBoundingBoxUnit> SimpleBoundingBoxVector;
    typedef std::vector<ComplexBoundingBoxUnit> ComplexBoundingBoxVector;
    typedef std::vector<SimpleBoundingBoxVector> VectorOfSimpleBoundingBoxVector;
    typedef std::vector<ComplexBoundingBoxVector> VectorOfComplexBoundingBoxVector;

  public:

    virtual ~GlCPULODCalculator();
    virtual GlLODCalculator *clone(){return new GlCPULODCalculator;}

    /**
     * Begin a new camera (use to render next entities)
     */
    virtual void beginNewCamera(Camera* camera);
    /**
     * Add a bounding box for a simple entity
     */
    virtual void addSimpleEntityBoundingBox(unsigned long entity,const BoundingBox& bb);
    /**
     * Add a bounding box for a node
     */
    virtual void addNodeBoundingBox(unsigned int id,const BoundingBox& bb);
    /**
     * Add a bounding box for an edge
     */
    virtual void addEdgeBoundingBox(unsigned int id,const BoundingBox& bb);

    /**
     * Compute all bounding boxs with the given viewport
     */
    virtual void compute(const Vector<int,4>& globalViewport,const Vector<int,4>& currentViewport,RenderingEntitiesFlag type);

    virtual void computeFor3DCamera(SimpleBoundingBoxVector *inputSimple,ComplexBoundingBoxVector *inputNodes,ComplexBoundingBoxVector *inputEdges,
        SimpleLODResultVector *outputSimple, ComplexLODResultVector *outputNodes, ComplexLODResultVector *outputEdges,
        const Coord &eye,
        const Matrix<float, 4> transformMatrix,
        const Vector<int,4>& globalViewport,
        const Vector<int,4>& currentViewport);

    void computeFor2DCamera(SimpleBoundingBoxVector *inputSimple,ComplexBoundingBoxVector *inputNodes,ComplexBoundingBoxVector *inputEdges,
        SimpleLODResultVector *outputSimple, ComplexLODResultVector *outputNodes, ComplexLODResultVector *outputEdges,
        const Vector<int,4>& globalViewport,
        const Vector<int,4>& currentViewport);

    /**
     * Return the result vector for simpleEntities
     */
    virtual VectorOfSimpleLODResultVector* getResultForSimpleEntities();
    /**
     * Return the result vector for nodes
     */
    virtual VectorOfComplexLODResultVector* getResultForNodes();
    /**
     * Return the result vector for edges
     */
    virtual VectorOfComplexLODResultVector* getResultForEdges();
    /**
     * Return cameras vector
     */
    virtual VectorOfCamera* getVectorOfCamera();

    /**
     * Clear the class data
     */
    virtual void clear();

  protected:

    VectorOfCamera cameraVector;

    SimpleBoundingBoxVector* actualSimpleBoundingBoxVector;
    ComplexBoundingBoxVector* actualNodesBoundingBoxVector;
    ComplexBoundingBoxVector* actualEdgesBoundingBoxVector;
    VectorOfSimpleBoundingBoxVector simpleBoundingBoxVector;
    VectorOfComplexBoundingBoxVector nodesBoundingBoxVector;
    VectorOfComplexBoundingBoxVector edgesBoundingBoxVector;

    VectorOfSimpleLODResultVector simpleResultVector;
    VectorOfComplexLODResultVector nodesResultVector;
    VectorOfComplexLODResultVector edgesResultVector;

    RenderingEntitiesFlag type;

  };

}

#endif // DOXYGEN_NOTFOR_DEVEL

#endif // Tulip_GLCPULODCALCULATOR_H
