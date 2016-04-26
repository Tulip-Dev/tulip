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

#include <tulip/glyphs/CircleGlyph.h>

#include <tulip/GlUtils.h>

using namespace std;
using namespace tlp;

static const unsigned int nbContourPoints = 30;

CircleGlyph::CircleGlyph() {
  _vertices.push_back(Coord());
  vector<Coord> contour = computeRegularPolygon(nbContourPoints);
  _vertices.insert(_vertices.end(), contour.begin(), contour.end());
  for (unsigned int i = 0 ; i < nbContourPoints - 1 ; ++i) {
    _indices.push_back(0);
    _indices.push_back(i+1);
    _indices.push_back(i+2);
  }
  _indices.push_back(0);
  _indices.push_back(nbContourPoints);
  _indices.push_back(1);
  for (unsigned int i = 0 ; i < nbContourPoints - 1 ; ++i) {
    _outlineIndices.push_back(i+1);
    _outlineIndices.push_back(i+2);
  }
  _outlineIndices.push_back(nbContourPoints);
  _outlineIndices.push_back(1);
}

void CircleGlyph::getIncludeBoundingBox(BoundingBox &boundingBox) {
  boundingBox[0] = Coord(-0.35f, -0.35f, 0);
  boundingBox[1] = Coord(0.35f, 0.35f, 0);
}
