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

#include <tulip/Glyph.h>
#include <tulip/GlUtils.h>
#include <tulip/TulipViewSettings.h>


using namespace std;
using namespace tlp;

class CubeGlyph : public Glyph {

  friend class CubeOutlinedGlyph;

public:

  GLYPHINFORMATION("3D - Cube", "3D - Cube extremity", "Antoine Lambert", "20/05/2016", "Cube", "1.0", tlp::NodeShape::Cube)

  CubeGlyph(PluginContext *context=nullptr) : Glyph(context) {

    _vertices = {Coord(0.5f, 0.5f, 0.5f),
                 Coord(-0.5f, 0.5f, 0.5f),
                 Coord(-0.5f, -0.5f, 0.5f),
                 Coord(0.5f, -0.5f, 0.5f),

                 Coord(0.5f, 0.5f, 0.5f),
                 Coord(0.5f, -0.5f, 0.5f),
                 Coord(0.5f, -0.5f, -0.5f),
                 Coord(0.5f, 0.5f, -0.5f),

                 Coord(0.5f, 0.5f, 0.5f),
                 Coord(0.5f, 0.5f, -0.5f),
                 Coord(-0.5f, 0.5f, -0.5f),
                 Coord(-0.5f, 0.5f, 0.5f),

                 Coord(-0.5f, 0.5f, 0.5f),
                 Coord(-0.5f, 0.5f, -0.5f),
                 Coord(-0.5f, -0.5f, -0.5f),
                 Coord(-0.5f, -0.5f, 0.5f),

                 Coord(-0.5f, -0.5f, -0.5f),
                 Coord(0.5f, -0.5f, -0.5f),
                 Coord(0.5f, -0.5f, 0.5f),
                 Coord(-0.5f, -0.5f, 0.5f),

                 Coord(0.5f, -0.5f, -0.5f),
                 Coord(-0.5f, -0.5f, -0.5f),
                 Coord(-0.5f, 0.5f, -0.5f),
                 Coord(0.5f, 0.5f, -0.5f)};

    _texCoords = {Vec2f(1.0f, 1.0f),
                  Vec2f(0.0f, 1.0f),
                  Vec2f(0.0f, 0.0f),
                  Vec2f(1.0f, 0.0f),

                  Vec2f(1.0f, 1.0f),
                  Vec2f(0.0f, 1.0f),
                  Vec2f(0.0f, 0.0f),
                  Vec2f(1.0f, 0.0f),

                  Vec2f(1.0f, 1.0f),
                  Vec2f(0.0f, 1.0f),
                  Vec2f(0.0f, 0.0f),
                  Vec2f(1.0f, 0.0f),

                  Vec2f(1.0f, 1.0f),
                  Vec2f(0.0f, 1.0f),
                  Vec2f(0.0f, 0.0f),
                  Vec2f(1.0f, 0.0f),

                  Vec2f(1.0f, 1.0f),
                  Vec2f(0.0f, 1.0f),
                  Vec2f(0.0f, 0.0f),
                  Vec2f(1.0f, 0.0f),

                  Vec2f(1.0f, 1.0f),
                  Vec2f(0.0f, 1.0f),
                  Vec2f(0.0f, 0.0f),
                  Vec2f(1.0f, 0.0f)};

    _normals = {Coord(0.0f, 0.0f, 1.0f),
                Coord(0.0f, 0.0f, 1.0f),
                Coord(0.0f, 0.0f, 1.0f),
                Coord(0.0f, 0.0f, 1.0f),

                Coord(1.0f, 0.0f, 0.0f),
                Coord(1.0f, 0.0f, 0.0f),
                Coord(1.0f, 0.0f, 0.0f),
                Coord(1.0f, 0.0f, 0.0f),

                Coord(0.0f, 1.0f, 0.0f),
                Coord(0.0f, 1.0f, 0.0f),
                Coord(0.0f, 1.0f, 0.0f),
                Coord(0.0f, 1.0f, 0.0f),

                Coord(-1.0f, 0.0f, 0.0f),
                Coord(-1.0f, 0.0f, 0.0f),
                Coord(-1.0f, 0.0f, 0.0f),
                Coord(-1.0f, 0.0f, 0.0f),

                Coord(0.0f, -1.0f, 0.0f),
                Coord(0.0f, -1.0f, 0.0f),
                Coord(0.0f, -1.0f, 0.0f),
                Coord(0.0f, -1.0f, 0.0f),

                Coord(0.0f, 0.0f, -1.0f),
                Coord(0.0f, 0.0f, -1.0f),
                Coord(0.0f, 0.0f, -1.0f),
                Coord(0.0f, 0.0f, -1.0f)};

    for (unsigned short i = 0 ; i < ushort_cast(_vertices.size()) ; i+=4) {
      _indices.insert(_indices.end(), {i, ushort_cast(i+1), ushort_cast(i+2), i, ushort_cast(i+2), ushort_cast(i+3)});
    }

  }

  bool glyph2D() const {
    return false;
  }

protected:

  Coord getAnchor(const Coord & vector) const {
    float x, y, z, fmax;
    vector.get(x, y, z);
    fmax = std::max(std::max(fabsf(x), fabsf(y)), fabsf(z));

    if (fmax > 0.0f)
      return vector * (0.5f / fmax);
    else
      return vector;
  }

};

class CubeOutlinedGlyph : public Glyph {

  friend class CubeOutlinedTransparentGlyph;

public:

  GLYPHINFORMATION("3D - Cube Outlined", "3D - Cube Outlined extremity", "Antoine Lambert", "20/05/2016", "Cube Outlined", "1.0", tlp::NodeShape::CubeOutlined)

  CubeOutlinedGlyph(PluginContext *context=nullptr) : Glyph(context) {
    _outlineIndices = {0, 1, 1, 2, 2, 3, 3, 0, 20,
                       21, 21, 22, 22, 23, 23, 20,
                       0, 23, 1, 22, 2, 21, 3, 20};
  }

  void getIncludeBoundingBox(BoundingBox &boundingBox) {
    _cubeGlyph.getIncludeBoundingBox(boundingBox);
  }

  void getTextBoundingBox(BoundingBox &boundingBox) {
    _cubeGlyph.getTextBoundingBox(boundingBox);
  }

  const vector<Coord> &getGlyphVertices() const {
    return _cubeGlyph.getGlyphVertices();
  }

  const vector<Vec2f> &getGlyphTexCoords() {
    return _cubeGlyph.getGlyphTexCoords();
  }

  const vector<Coord> &getGlyphNormals() {
    return _cubeGlyph.getGlyphNormals();
  }

  const vector<unsigned short> &getGlyphVerticesIndices() const {
    return _cubeGlyph.getGlyphVerticesIndices();
  }

  bool glyph2D() const {
    return false;
  }

protected:

  Coord getAnchor(const Coord &vector) const {
    return _cubeGlyph.getAnchor(vector);
  }

private:

  CubeGlyph _cubeGlyph;

};

class CubeOutlinedTransparentGlyph : public Glyph {

public:

  GLYPHINFORMATION("3D - Cube Outlined Transparent", "3D - Cube Outlined Transparent extremity", "Antoine Lambert", "20/05/2016", "Cube Outlined Transparent", "1.0", NodeShape::CubeOutlinedTransparent)

  CubeOutlinedTransparentGlyph(PluginContext *context) : Glyph(context) {}

  const vector<Coord> &getGlyphVertices() const {
    return _cubeOutlinedGlyph.getGlyphVertices();
  }

  const vector<unsigned short> &getGlyphOutlineIndices() const {
    return _cubeOutlinedGlyph.getGlyphOutlineIndices();
  }

  bool glyph2D() const {
    return false;
  }

protected:

  Coord getAnchor(const Coord &vector) const {
    return _cubeOutlinedGlyph.getAnchor(vector);
  }

private:

  CubeOutlinedGlyph _cubeOutlinedGlyph;

};

PLUGIN(CubeGlyph)
PLUGIN(CubeOutlinedGlyph)
PLUGIN(CubeOutlinedTransparentGlyph)

