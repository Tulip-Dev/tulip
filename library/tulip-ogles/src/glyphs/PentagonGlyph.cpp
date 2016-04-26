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

#include <tulip/glyphs/PentagonGlyph.h>

#include <tulip/GlUtils.h>

using namespace std;
using namespace tlp;

PentagonGlyph::PentagonGlyph() {
  _vertices.push_back(Coord(0.f, 0.f));
  vector<Coord> contour = computeRegularPolygon(5, M_PI/2.f);
  _vertices.insert(_vertices.end(), contour.begin(), contour.end());
  for (unsigned int i = 0 ; i < 4 ; ++i) {
    _indices.push_back(0);
    _indices.push_back(i+1);
    _indices.push_back(i+2);
  }
  _indices.push_back(0);
  _indices.push_back(5);
  _indices.push_back(1);
  for (size_t i = 0 ; i < _vertices.size() - 2 ; ++i) {
    _outlineIndices.push_back(i+1);
    _outlineIndices.push_back(i+2);
  }
  _outlineIndices.push_back(_vertices.size()-1);
  _outlineIndices.push_back(1);
}

void PentagonGlyph::getIncludeBoundingBox(BoundingBox &boundingBox) {
  boundingBox[0] = Coord(-0.3f, -0.35f, 0);
  boundingBox[1] = Coord(0.3f, 0.35f, 0);
}

