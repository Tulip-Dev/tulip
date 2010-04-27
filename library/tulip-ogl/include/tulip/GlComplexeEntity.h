//-*-c++-*-
/**
 Author: Morgan Mathiaut
 Email : mathiaut@labri.fr
 Last modification : 23/10/2007
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.
*/
#ifndef Tulip_GLCOMPLEXEENTITY_H
#define Tulip_GLCOMPLEXEENTITY_H

#include <vector>

#include <tulip/BoundingBox.h>

#include "tulip/GlEntity.h"

namespace tlp {

  class OcclusionTest;
  class TextRenderer;
  class GlGraphInputData;
  class Camera;

  /** \brief Virtual class used by GlNode, GlMetaNode and GlEdge
   *
   * This class is a virtual class used by GlNode, GlMetaNode and GlEdge
   * Define draw function for entities who need GlGraphInputData variable
   * Define drawLabel function
   */
  class TLP_GL_SCOPE GlComplexeEntity : public GlEntity {

  public:

    /**
     * return the bounding box of this entity
     */
    virtual BoundingBox getBoundingBox(GlGraphInputData* data) = 0;

    /**
     * Draw this entity with GlGraphInputData and Camera
     */
    virtual void draw(float lod,GlGraphInputData* data,Camera* camera) = 0;

    /**
     * Draw label of this entity
     */
    virtual void drawLabel(OcclusionTest* test,TextRenderer* renderer,GlGraphInputData* data) = 0;
  };

}

#endif // Tulip_GLCOMPLEXEENTITY_H
