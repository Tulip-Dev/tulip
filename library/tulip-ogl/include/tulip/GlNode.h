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

#ifndef Tulip_GLNODE_H
#define Tulip_GLNODE_H

#ifndef DOXYGEN_NOTFOR_DEVEL

#include <tulip/Color.h>
#include <tulip/Coord.h>
#include <tulip/GlComplexeEntity.h>

#include <vector>

namespace tlp {

struct OcclusionTest;
class TextRenderer;
class GlSceneVisitor;
class GlLabel;
class GlBox;

/**
 * Class to represent a node of a graph
 */
class TLP_GL_SCOPE GlNode : public GlComplexeEntity {

public:

  /**
   * Default constructor with id
   * id must be the id of the node in graph
   */
  GlNode(unsigned int id);

  /**
   * Virtual function to accept GlSceneVisitor on this class
   */
  virtual void acceptVisitor(GlSceneVisitor *visitor);

  /**
   * Return the node bounding box
   */
  virtual BoundingBox getBoundingBox(const GlGraphInputData* data);

  /**
   * Draw the node with level of detail : lod and Camera : camera
   */
  virtual void draw(float lod,const GlGraphInputData *data,Camera* camera);

  /**
   * Draw the label of the node if drawNodesLabel is true and if label selection is equal to drawSelect
   * Use TextRenderer : renderer to draw the label
   */
  virtual void drawLabel(bool drawSelect,OcclusionTest* test,const GlGraphInputData* data,float lod);

  /**
   * Draw the label of the node if drawEdgesLabel is true
   * Use TextRenderer : renderer to draw the label
   */
  virtual void drawLabel(OcclusionTest* test,const GlGraphInputData* data);

  /**
   * Draw the label of the node if drawEdgesLabel is true
   * Use TextRenderer : renderer to draw the label
   */
  virtual void drawLabel(OcclusionTest* test,const GlGraphInputData* data,float lod,Camera *camera=NULL);

  unsigned int id;

  /**
   * This function is used by the engine to get point coordinate and color of the node
   */
  void getPointAndColor(GlGraphInputData *inputData,std::vector<Coord> &pointsCoordsArray,std::vector<Color> &pointsColorsArray);

  /**
   * This function is used by the engine to get color of the node
   */
  void getColor(GlGraphInputData *inputData,std::vector<Color> &pointsColorsArray);

protected :

  static GlLabel *label;
  static GlBox* selectionBox;

};

}

#endif // DOXYGEN_NOTFOR_DEVEL

#endif // Tulip_GLNODE_H
///@endcond
