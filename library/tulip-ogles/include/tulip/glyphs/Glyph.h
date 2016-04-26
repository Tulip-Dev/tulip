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
 * Copyright (c) 2016 Antoine Lambert, Thales Services
 * antoine-e.lambert@thalesgroup.com / antoine.lambert33@gmail.com
 *
 */

#ifndef GLYPH_H
#define GLYPH_H

#include <tulip/Size.h>
#include <tulip/Coord.h>
#include <tulip/BoundingBox.h>

#include <vector>

namespace tlp {

class TLP_GLES_SCOPE Glyph {
public:

  Glyph() {}

  virtual ~Glyph() {}

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

  virtual const std::vector<unsigned short> &getGlyphOutlineIndices() const {
    return _outlineIndices;
  }

  virtual bool glyph2D() const {
    return true;
  }

  /*
   * return a point where an edge coming from "from" can be attached
   * by default, the point will be on the surface of the largest sphere contained
   * inside the unit cube (before scaling).
   */
  virtual tlp::Coord getAnchor(const tlp::Coord &nodeCenter, const tlp::Coord &from,
                               const tlp::Size &scale, const double zRotation) const;


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
  std::vector<unsigned short> _outlineIndices;
  std::vector<tlp::Vec2f> _texCoords;
  std::vector<tlp::Coord> _normals;

};

}

#endif // GLYPH_H

