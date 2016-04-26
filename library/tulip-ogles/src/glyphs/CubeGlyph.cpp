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

#include <tulip/glyphs/CubeGlyph.h>

using namespace tlp;

CubeGlyph::CubeGlyph() {
  _vertices.push_back(Coord(0.5f, 0.5f, 0.5f));
  _vertices.push_back(Coord(-0.5f, 0.5f, 0.5f));
  _vertices.push_back(Coord(-0.5f, -0.5f, 0.5f));
  _vertices.push_back(Coord(0.5f, -0.5f, 0.5f));

  _vertices.push_back(Coord(0.5f, 0.5f, 0.5f));
  _vertices.push_back(Coord(0.5f, -0.5f, 0.5f));
  _vertices.push_back(Coord(0.5f, -0.5f, -0.5f));
  _vertices.push_back(Coord(0.5f, 0.5f, -0.5f));

  _vertices.push_back(Coord(0.5f, 0.5f, 0.5f));
  _vertices.push_back(Coord(0.5f, 0.5f, -0.5f));
  _vertices.push_back(Coord(-0.5f, 0.5f, -0.5f));
  _vertices.push_back(Coord(-0.5f, 0.5f, 0.5f));

  _vertices.push_back(Coord(-0.5f, 0.5f, 0.5f));
  _vertices.push_back(Coord(-0.5f, 0.5f, -0.5f));
  _vertices.push_back(Coord(-0.5f, -0.5f, -0.5f));
  _vertices.push_back(Coord(-0.5f, -0.5f, 0.5f));

  _vertices.push_back(Coord(-0.5f, -0.5f, -0.5f));
  _vertices.push_back(Coord(0.5f, -0.5f, -0.5f));
  _vertices.push_back(Coord(0.5f, -0.5f, 0.5f));
  _vertices.push_back(Coord(-0.5f, -0.5f, 0.5f));

  _vertices.push_back(Coord(0.5f, -0.5f, -0.5f));
  _vertices.push_back(Coord(-0.5f, -0.5f, -0.5f));
  _vertices.push_back(Coord(-0.5f, 0.5f, -0.5f));
  _vertices.push_back(Coord(0.5f, 0.5f, -0.5f));

  _texCoords.push_back(Vec2f(1.0f, 1.0f));
  _texCoords.push_back(Vec2f(0.0f, 1.0f));
  _texCoords.push_back(Vec2f(0.0f, 0.0f));
  _texCoords.push_back(Vec2f(1.0f, 0.0f));

  _texCoords.push_back(Vec2f(1.0f, 1.0f));
  _texCoords.push_back(Vec2f(0.0f, 1.0f));
  _texCoords.push_back(Vec2f(0.0f, 0.0f));
  _texCoords.push_back(Vec2f(1.0f, 0.0f));

  _texCoords.push_back(Vec2f(1.0f, 1.0f));
  _texCoords.push_back(Vec2f(0.0f, 1.0f));
  _texCoords.push_back(Vec2f(0.0f, 0.0f));
  _texCoords.push_back(Vec2f(1.0f, 0.0f));

  _texCoords.push_back(Vec2f(1.0f, 1.0f));
  _texCoords.push_back(Vec2f(0.0f, 1.0f));
  _texCoords.push_back(Vec2f(0.0f, 0.0f));
  _texCoords.push_back(Vec2f(1.0f, 0.0f));

  _texCoords.push_back(Vec2f(1.0f, 1.0f));
  _texCoords.push_back(Vec2f(0.0f, 1.0f));
  _texCoords.push_back(Vec2f(0.0f, 0.0f));
  _texCoords.push_back(Vec2f(1.0f, 0.0f));

  _texCoords.push_back(Vec2f(1.0f, 1.0f));
  _texCoords.push_back(Vec2f(0.0f, 1.0f));
  _texCoords.push_back(Vec2f(0.0f, 0.0f));
  _texCoords.push_back(Vec2f(1.0f, 0.0f));

  _normals.push_back(Coord(0.0f, 0.0f, 1.0f));
  _normals.push_back(Coord(0.0f, 0.0f, 1.0f));
  _normals.push_back(Coord(0.0f, 0.0f, 1.0f));
  _normals.push_back(Coord(0.0f, 0.0f, 1.0f));

  _normals.push_back(Coord(1.0f, 0.0f, 0.0f));
  _normals.push_back(Coord(1.0f, 0.0f, 0.0f));
  _normals.push_back(Coord(1.0f, 0.0f, 0.0f));
  _normals.push_back(Coord(1.0f, 0.0f, 0.0f));

  _normals.push_back(Coord(0.0f, 1.0f, 0.0f));
  _normals.push_back(Coord(0.0f, 1.0f, 0.0f));
  _normals.push_back(Coord(0.0f, 1.0f, 0.0f));
  _normals.push_back(Coord(0.0f, 1.0f, 0.0f));

  _normals.push_back(Coord(-1.0f, 0.0f, 0.0f));
  _normals.push_back(Coord(-1.0f, 0.0f, 0.0f));
  _normals.push_back(Coord(-1.0f, 0.0f, 0.0f));
  _normals.push_back(Coord(-1.0f, 0.0f, 0.0f));

  _normals.push_back(Coord(0.0f, -1.0f, 0.0f));
  _normals.push_back(Coord(0.0f, -1.0f, 0.0f));
  _normals.push_back(Coord(0.0f, -1.0f, 0.0f));
  _normals.push_back(Coord(0.0f, -1.0f, 0.0f));

  _normals.push_back(Coord(0.0f, 0.0f, -1.0f));
  _normals.push_back(Coord(0.0f, 0.0f, -1.0f));
  _normals.push_back(Coord(0.0f, 0.0f, -1.0f));
  _normals.push_back(Coord(0.0f, 0.0f, -1.0f));

  for (size_t i = 0 ; i < _vertices.size() ; i+=4) {
    _indices.push_back(i);
    _indices.push_back(i+1);
    _indices.push_back(i+2);
    _indices.push_back(i);
    _indices.push_back(i+2);
    _indices.push_back(i+3);
  }

}

Coord CubeGlyph::getAnchor(const Coord & vector) const {
  float x, y, z, fmax;
  vector.get(x, y, z);
  fmax = std::max(std::max(fabsf(x), fabsf(y)), fabsf(z));

  if (fmax > 0.0f)
    return vector * (0.5f / fmax);
  else
    return vector;
}


