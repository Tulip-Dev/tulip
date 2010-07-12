/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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
    
    /**
     * Draw the label of the edge if drawEdgesLabel is true
     * Use TextRenderer : renderer to draw the label
     */
    void drawLabel(OcclusionTest* test,TextRenderer* renderer,GlGraphInputData* data);

    unsigned int id;

  private :
    /**
     * Draw the Edge : this function is used by draw function
     */
    void drawEdge(const Coord &srcNodePos, const Coord &tgtNodePos,
		  const Coord &startPoint, const Coord &endPoint, const LineType::RealType &bends,
          const Color &startColor, const Color &endColor, const Coord &lookDir, bool colorInterpolate, const Color &borderColor,
          const Size &size, int shape, bool edge3D, float lod,const std::string &textureName);

    /**
     * Function used to compute bounding box for edge extremity.
     */
    BoundingBox eeGlyphBoundingBox(const Coord& anchor, const Coord& tgt, float glyphNrm,
    		const Matrix<float, 4>& transformation, const Matrix<float, 4>& size);

    void getEdgeColor(GlGraphInputData *data,edge e,node source, node target, bool selected,Color &srcCol, Color &tgtCol);

    void getEdgeSize(GlGraphInputData *data,edge e,const Size &srcSize, const Size &tgtSize,Size &edgeSize, float &maxSrcSize, float &maxTgtSize);

    float getEdgeWidthLod(const Coord &edgeCoord,const Size &edgeSize,Camera *camera);

    void displayArrow(GlGraphInputData *data,
                      edge e,
                      node source,
                      float edgeSize,
                      const Color &color,
                      float maxSize,
                      bool selected,
                      int srcEdgeGlyph,
                      int tgtEdgeGlyph,
                      size_t numberOfBends,
                      const Coord &anchor,
                      const Coord &tgtCoord,
                      const Coord &srcAnchor,
                      const Coord &tgtAnchor,
                      Coord &lineAnchor);

  };

}

#endif // DOXYGEN_NOTFOR_DEVEL

#endif // Tulip_GLEDGE_H
