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
     * Reserve memory to store nodes LOD
     */
    virtual void reserveMemoryForNodes(unsigned int numberOfNodes);

    /**
     * Reserve memory to store edges LOD
     */
    virtual void reserveMemoryForEdges(unsigned int numberOfEdges);

    /**
     * Compute all bounding boxs with the given viewport
     */
    virtual void compute(const Vector<int,4>& globalViewport,const Vector<int,4>& currentViewport);

    virtual void computeFor3DCamera(LayerLODUnit *layerLODUnit,const Coord &eye,const Matrix<float, 4> transformMatrix,const Vector<int,4>& globalViewport,const Vector<int,4>& currentViewport);

    virtual void computeFor2DCamera(LayerLODUnit *layerLODUnit,const Vector<int,4>& globalViewport,const Vector<int,4>& currentViewport);

  protected:

    LayerLODUnit *currentLayerLODUnit;

  };

}

#endif // DOXYGEN_NOTFOR_DEVEL

#endif // Tulip_GLCPULODCALCULATOR_H
