/*
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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

#ifndef Tulip_GLEDGE_H
#define Tulip_GLEDGE_H

#ifndef DOXYGEN_NOTFOR_DEVEL

#include <tulip/PropertyTypes.h>
#include <tulip/Size.h>
#include <tulip/GlEntity.h>
#include <tulip/Matrix.h>
#include <tulip/GlLabel.h>
#include <tulip/GlSceneVisitor.h>

namespace tlp {

struct OcclusionTest;
class EdgeExtremityGlyph;
class TextRenderer;
class GlGraphInputData;
class Camera;

/**
 * Class to represent an edge of a graph
 */
class TLP_GL_SCOPE GlEdge final : public GlEntity {

public:
  /**
   * Build an edge with the id : id
   * id must be the id of the edge in graph
   */
  GlEdge(unsigned int eId = UINT_MAX, unsigned int ePos = UINT_MAX, bool sel = false)
      : id(eId), pos(ePos), selectionDraw(sel) {}

  /**
   * Virtual function to accept GlSceneVisitor on this class
   */
  void acceptVisitor(GlSceneVisitor *visitor) override {
    visitor->visit(this);
  }

  /**
   * Return the edge bounding box
   */
  BoundingBox getBoundingBox(const GlGraphInputData *data);

  /**
   * Return the edge bounding box
   */
  BoundingBox getBoundingBox(const GlGraphInputData *data, const edge e, const node src,
                             const node tgt, const Coord &srcCoord, const Coord &tgtCoord,
                             const Size &srcSize, const Size &tgtSize,
                             const LineType::RealType &bends);

  /**
   * Draw the edge with level of detail : lod and Camera : camera
   */
  void draw(float lod, const GlGraphInputData *data, Camera *camera);

  /**
   * Draw the label of the edge if drawEdgesLabel is true
   * Use TextRenderer : renderer to draw the label
   */
  void drawLabel(GlLabel &label, OcclusionTest *test, const GlGraphInputData *data, float lod,
                 Camera *camera = nullptr);

  /**
   * This function is used by the engine to get line coordinates of the edge
   */
  size_t getVertices(const GlGraphInputData *data, const edge e, const node src, const node tgt,
                     Coord &srcCoord, Coord &tgtCoord, Size &srcSize, Size &tgtSize,
                     std::vector<Coord> &vertices, float lengthRatio = 1);

  /**
   * This function is used by the engine to get line colors of the edge
   */
  void getColors(const GlGraphInputData *data, const node src, const node tgt, const Color &eColor,
                 Color &srcCol, Color &tgtCol, const Coord *vertices, unsigned int numberOfVertices,
                 std::vector<Color> &colors);

  /**
   * Compute the edge size
   */
  void getEdgeSize(const GlGraphInputData *data, edge e, const Size &srcSize, const Size &tgtSize,
                   const float maxSrcSize, const float maxTgtSize, Size &edgeSize);

  /**
   * Compute edge anchor
   */
  void getEdgeAnchor(const GlGraphInputData *data, const node src, const node tgt,
                     const LineType::RealType &bends, const Coord &srcCoord, const Coord &tgtCoord,
                     const Size &srcSize, const Size &tgtSize, Coord &srcAnchor, Coord &tgtAnchor,
                     float lengthRatio = 1);

  void setSelectionDraw(bool selectDraw) {
    selectionDraw = selectDraw;
  }

  // edge id and edge position in graph->edges()
  unsigned int id, pos;

private:
  bool selectionDraw;

  /**
   * Draw the Edge : this function is used by draw function
   */
  void drawEdge(const Coord &srcNodePos, const Coord &tgtNodePos, const Coord &startPoint,
                const Coord &endPoint, const LineType::RealType &bends, const Color &startColor,
                const Color &endColor, const Coord &lookDir, bool colorInterpolate,
                const Color &borderColor, const Size &size, int shape, bool edge3D, float lod,
                const std::string &textureName, const float outlineWidth);

  /**
   * Function used to compute bounding box for edge extremity.
   */
  BoundingBox eeGlyphBoundingBox(const Coord &anchor, const Coord &tgt, float glyphNrm,
                                 const Matrix<float, 4> &transformation,
                                 const Matrix<float, 4> &size);

  /**
   * Compute the edge colors and store these colors in srcCol and tgtCol
   * \param data : input data used to compute edge colors
   */
  void getEdgeColor(const GlGraphInputData *data, const edge e, const node src, const node tgt,
                    bool selected, Color &srcCol, Color &tgtCol);

  /**
   * Compute width lod of edge
   * This lod is used to know if the edge is render in polygon mode or line mode
   */
  float getEdgeWidthLod(const Coord &edgeCoord, const Size &edgeSize, Camera *camera);

  /**
   * Thgis function is used to render edge arrows
   */
  void displayArrowAndAdjustAnchor(const GlGraphInputData *data, const edge e, const node src,
                                   const Size &sizeRatio, float edgeSize, const Color &color,
                                   float maxSize, bool selected, float selectionOutlineSize,
                                   int tgtEdgeGlyph, bool hasBends, const Coord &anchor,
                                   const Coord &tgtCoord, const Coord &srcAnchor,
                                   const Coord &tgtAnchor, Coord &lineAnchor,
                                   EdgeExtremityGlyph *srcEdgeGlyph = nullptr,
                                   Camera *camera = nullptr);
};
} // namespace tlp

#endif // DOXYGEN_NOTFOR_DEVEL

#endif // Tulip_GLEDGE_H
///@endcond
