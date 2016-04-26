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

#include <tulip/glyphs/SquareGlyph.h>

#include <tulip/GlUtils.h>

using namespace std;
using namespace tlp;

SquareGlyph::SquareGlyph() {
  _vertices = computeRegularPolygon(4, M_PI/4.f);
  _indices.push_back(0);
  _indices.push_back(1);
  _indices.push_back(3);
  _indices.push_back(1);
  _indices.push_back(2);
  _indices.push_back(3);
  for (size_t i = 0 ; i < _vertices.size() - 1 ; ++i) {
    _outlineIndices.push_back(i);
    _outlineIndices.push_back(i+1);
  }
  _outlineIndices.push_back(_vertices.size()-1);
  _outlineIndices.push_back(0);
}

Coord SquareGlyph::getAnchor(const Coord &vector) const {
  Coord v(vector);
  float x, y, z, fmax;
  v.get(x, y, z);
  v.setZ(0.0f);
  fmax = std::max(fabsf(x), fabsf(y));

  if (fmax > 0.0f)
    return v * (0.5f / fmax);
  else
    return v;
}
