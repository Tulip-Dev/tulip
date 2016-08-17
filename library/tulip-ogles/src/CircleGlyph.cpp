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

#include <tulip/GlUtils.h>
#include <tulip/CircleGlyph.h>

using namespace std;
using namespace tlp;

static const unsigned short nbContourPoints = 30;

CircleGlyph::CircleGlyph(PluginContext *context) : Glyph(context) {
  _vertices.push_back(Coord());
  vector<Coord> contour = computeRegularPolygon(nbContourPoints);
  _vertices.insert(_vertices.end(), contour.begin(), contour.end());
  for (unsigned short i = 0 ; i < nbContourPoints-1 ; ++i) {
    _indices.insert(_indices.end(), {0, ushort_cast(i+1), ushort_cast(i+2)});
  }
  _indices.insert(_indices.end(), {0, nbContourPoints, 1});

  vector<unsigned short> outlineIndices;
  for (unsigned short i = 1 ; i < nbContourPoints + 1 ; ++i) {
    outlineIndices.push_back(i);
  }
  outlineIndices.push_back(1);
  _outlinesIndices.push_back(outlineIndices);
}

void CircleGlyph::getIncludeBoundingBox(BoundingBox &boundingBox) {
  boundingBox[0] = Coord(-0.35f, -0.35f, 0);
  boundingBox[1] = Coord(0.35f, 0.35f, 0);
}

PLUGIN(CircleGlyph)


