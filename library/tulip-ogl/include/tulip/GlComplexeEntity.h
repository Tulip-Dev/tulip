/*
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */
///@cond DOXYGEN_HIDDEN

#ifndef Tulip_GLCOMPLEXEENTITY_H
#define Tulip_GLCOMPLEXEENTITY_H

#include <tulip/BoundingBox.h>
#include <tulip/GlEntity.h>

namespace tlp {

struct OcclusionTest;
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
  virtual BoundingBox getBoundingBox(const GlGraphInputData* data) = 0;

  /**
   * Draw this entity with GlGraphInputData and Camera
   */
  virtual void draw(float lod,const GlGraphInputData* data,Camera* camera) = 0;

  /**
   * Draw label of this entity
   */
  virtual void drawLabel(OcclusionTest* test,const GlGraphInputData* data) = 0;
};

}

#endif // Tulip_GLCOMPLEXEENTITY_H
///@endcond
