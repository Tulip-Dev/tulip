/*
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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

#ifndef Tulip_GLNODE_H
#define Tulip_GLNODE_H

#ifndef DOXYGEN_NOTFOR_DEVEL

#include <climits>
#include <tulip/Color.h>
#include <tulip/Coord.h>
#include <tulip/Size.h>
#include <tulip/GlBox.h>
#include <tulip/GlComplexeEntity.h>
#include <tulip/GlLabel.h>
#include <tulip/GlSceneVisitor.h>
#include <tulip/GlGraphInputData.h>

#include <vector>

namespace tlp {

struct OcclusionTest;

/**
 * Class to represent a node of a graph
 */
class TLP_GL_SCOPE GlNode final : public GlComplexeEntity {

public:
  /**
   * Default constructor with id
   * id must be the id of the node in graph
   */
  GlNode(unsigned int _nid = UINT_MAX, unsigned int _npos = UINT_MAX)
      : id(_nid), pos(_npos), oldId(UINT_MAX),
        selectionBox(Coord(0, 0, 0), Size(1, 1, 1), Color(0, 0, 255, 255), Color(0, 255, 0, 255),
                     false, true, "", 3) {}

  /**
   * Virtual function to accept GlSceneVisitor on this class
   */
  void acceptVisitor(GlSceneVisitor *visitor) override {
    visitor->visit(this);
  }

  /**
   * Return the node bounding box
   */
  BoundingBox getBoundingBox(const GlGraphInputData *data) override;

  /**
   * Draw the node with level of detail : lod and Camera : camera
   */
  void draw(float lod, const GlGraphInputData *data, Camera *camera) override;

  /**
   * Draw the label of the node if drawNodesLabel is true and if label selection is equal to
   * drawSelect
   */
  void drawLabel(bool drawSelect, OcclusionTest *test, const GlGraphInputData *data, float lod);

  /**
   * Draw the label of the node if drawEdgesLabel is true
   */
  void drawLabel(OcclusionTest *test, const GlGraphInputData *data) override;

  /**
   * Draw the label of the node if drawEdgesLabel is true
   */
  void drawLabel(OcclusionTest *test, const GlGraphInputData *data, float lod,
                 Camera *camera = nullptr);

  // node id and node position in graph->nodes()
  unsigned int id, pos;

  /**
   * This function is used by the engine to get point coordinates of the node
   */
  Coord getPoint(GlGraphInputData *inputData) {
    init(inputData);
    return coord;
  }

  /**
   * This function is used by the engine to get color of the node
   */
  Color getColor(GlGraphInputData *inputData) {
    node n(id);

    return (inputData->getElementBorderWidth()->getNodeValue(n) > 0)
               ? inputData->getElementBorderColor()->getNodeValue(n)
               : inputData->getElementColor()->getNodeValue(n);
  }

protected:
  unsigned int oldId;
  GlBox selectionBox;
  GlLabel label;

  // initialize the data member below
  void init(const GlGraphInputData *data);

  tlp::Coord coord;
  int glyph;
  tlp::Size size;
  float rot;
  bool selected;
};
} // namespace tlp

#endif // DOXYGEN_NOTFOR_DEVEL

#endif // Tulip_GLNODE_H
///@endcond
