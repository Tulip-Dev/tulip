//-*-c++-*-
/**
 Author: Morgan Mathiaut
 Email : mathiaut@labri.fr
 Last modification : 09/11/2007
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.
*/
#ifndef Tulip_GLEDGE_H
#define Tulip_GLEDGE_H

#ifndef DOXYGEN_NOTFOR_DEVEL

#include <tulip/Types.h>

#include "tulip/GlComplexeEntity.h"
#include "tulip/EdgeExtremityGlyph.h"

namespace tlp {

  class OcclusionTest;
  class TextRenderer;

  /**
   * Class to represent an edge of a graph
   */
  class TLP_GL_SCOPE GlEdge : public GlComplexeEntity{

  public:

    /**
     * Build an edge with the id : id
     * id must be the id of the edge in graph
     */
    GlEdge(unsigned int id):id(id) {}

    /**
     * Virtual function to accept GlSceneVisitor on this class
     */
    virtual void acceptVisitor(GlSceneVisitor *visitor);

    /**
     * Return the edge bounding box
     */
    virtual BoundingBox getBoundingBox(GlGraphInputData* data);

    /**
     * Draw the edge with level of detail : lod and Camera : camera
     */
    virtual void draw(float lod,GlGraphInputData *data,Camera* camera);

    /**
     * Draw the label of the edge if drawEdgesLabel is true and if label selection is equal to drawSelect
     * Use TextRenderer : renderer to draw the label
     */
    void drawLabel(bool drawSelect,OcclusionTest* test,TextRenderer* renderer,GlGraphInputData* data);

    unsigned int id;

  private :
    /**
     * Draw the Edge : this function is used by draw function
     */
    void drawEdge(const Coord &srcNodePos, const Coord &tgtNodePos,
		  const Coord &startPoint, const Coord &endPoint, const LineType::RealType &bends,
		  const Color &startColor, const Color &endColor, bool colorInterpolate, const Color &borderColor,
		  const Size &size, int shape, bool edge3D, float lod);

    /**
     * Function used to compute bounding box for edge extremity.
     */
    BoundingBox eeGlyphBoundingBox(const Coord& anchor, const Coord& tgt, float glyphNrm,
    		const Matrix<float, 4>& transformation, const Matrix<float, 4>& size);

  };

}

#endif // DOXYGEN_NOTFOR_DEVEL

#endif // Tulip_GLEDGE_H
