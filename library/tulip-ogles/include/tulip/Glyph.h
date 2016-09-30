/**
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

/**
 *
 * tulip-ogles library : rewrite of old tulip-ogl library using OpenGL ES API
 * Copyright (c) 2016 Antoine Lambert, Thales Services SAS
 * antoine-e.lambert@thalesgroup.com / antoine.lambert33@gmail.com
 *
 */

#ifndef GLYPH_H
#define GLYPH_H

#include <tulip/Size.h>
#include <tulip/Coord.h>
#include <tulip/BoundingBox.h>
#include <tulip/Plugin.h>

#include <vector>

namespace tlp {

static const std::string GLYPH_CATEGORY = "Glyph";

class TLP_GLES_SCOPE Glyph : public Plugin {

public:
  Glyph(PluginContext *) {
  }

  virtual ~Glyph() {
  }

  std::string category() const {
    return GLYPH_CATEGORY;
  }

  std::string icon() const {
    return "shape-plus";
  }

  virtual void getIncludeBoundingBox(tlp::BoundingBox &boundingBox);

  virtual void getTextBoundingBox(tlp::BoundingBox &boundingBox) {
    getIncludeBoundingBox(boundingBox);
  }

  virtual const std::vector<tlp::Coord> &getGlyphVertices() const {
    return _vertices;
  }

  virtual const std::vector<tlp::Vec2f> &getGlyphTexCoords();

  virtual const std::vector<tlp::Coord> &getGlyphNormals();

  virtual const std::vector<unsigned short> &getGlyphVerticesIndices() const {
    return _indices;
  }

  virtual const std::vector<std::vector<unsigned short>> &getGlyphOutlineIndices() const {
    return _outlinesIndices;
  }

  virtual bool glyph2D() const {
    return true;
  }

  /*
   * return a point where an edge coming from "from" can be attached
   * by default, the point will be on the surface of the largest sphere contained
   * inside the unit cube (before scaling).
   */
  virtual tlp::Coord getAnchor(const tlp::Coord &nodeCenter, const tlp::Coord &from, const tlp::Size &scale, const double zRotation) const;

  virtual int id() const = 0;

  virtual std::string nodeGlyphName() const = 0;

  virtual std::string edgeExtremityGlyphName() const = 0;

  virtual bool edgeExtremityGlyph() const = 0;

protected:
  /*
   * called by public method getAnchor to actually compute the anchor point
   * vector is coordinate of the point to anchor to, relative to nodecenter
   * glyph size is (1,1,1)
   * this is the method to redefine for each glyph where the default 'getAnchor' method
   * is inapropriated
   * Returned value is a vector to be applied to 'nodeCenter' in the public method
   */
  virtual tlp::Coord getAnchor(const tlp::Coord &vector) const;

  std::vector<tlp::Coord> _vertices;
  std::vector<unsigned short> _indices;
  std::vector<std::vector<unsigned short>> _outlinesIndices;
  std::vector<tlp::Vec2f> _texCoords;
  std::vector<tlp::Coord> _normals;
};
}

#define GLYPHINFORMATION(NODE_GLYPH_NAME, EDGE_EXTREMITY_GLYPH_NAME, AUTHOR, DATE, INFO, RELEASE, ID)                                                \
  PLUGININFORMATION(NODE_GLYPH_NAME, AUTHOR, DATE, INFO, RELEASE, "")                                                                                \
  int id() const {                                                                                                                                   \
    return ID;                                                                                                                                       \
  }                                                                                                                                                  \
  std::string nodeGlyphName() const {                                                                                                                \
    return NODE_GLYPH_NAME;                                                                                                                          \
  }                                                                                                                                                  \
  std::string edgeExtremityGlyphName() const {                                                                                                       \
    return EDGE_EXTREMITY_GLYPH_NAME;                                                                                                                \
  }                                                                                                                                                  \
  bool edgeExtremityGlyph() const {                                                                                                                  \
    return !std::string(EDGE_EXTREMITY_GLYPH_NAME).empty();                                                                                          \
  }

#define GLYPHINFORMATIONS(NODE_GLYPH_NAME, EDGE_EXTREMITY_GLYPH_NAME, AUTHOR, DATE, INFO, RELEASE, ID)                                               \
  GLYPHINFORMATION(NODE_GLYPH_NAME, EDGE_EXTREMITY_GLYPH_NAME, AUTHOR, DATE, INFO, RELEASE, ID, "")

#endif // GLYPH_H
