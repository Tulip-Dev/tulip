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

#include <tulip/glyphs/RingGlyph.h>

#include <tulip/GlUtils.h>

using namespace std;
using namespace tlp;

static const unsigned int nbContourPoints = 30;

RingGlyph::RingGlyph() {
  vector<Coord> outerContour = computeRegularPolygon(nbContourPoints);
  vector<Coord> innerContour = computeRegularPolygon(nbContourPoints, 0.f, Coord(0.f, 0.f, 0.f), Size(0.25f, 0.25f));
  _vertices.insert(_vertices.end(), innerContour.begin(), innerContour.end());
  _vertices.insert(_vertices.end(), outerContour.begin(), outerContour.end());
  for (unsigned int i = 0 ; i < nbContourPoints - 1 ; ++i) {
    _indices.push_back(i);
    _indices.push_back(i+1);
    _indices.push_back(i + nbContourPoints);

    _indices.push_back(i+1);
    _indices.push_back(i + nbContourPoints + 1);
    _indices.push_back(i + nbContourPoints);
  }

  _indices.push_back(nbContourPoints-1);
  _indices.push_back(0);
  _indices.push_back(2*nbContourPoints-1);

  _indices.push_back(0);
  _indices.push_back(nbContourPoints);
  _indices.push_back(2*nbContourPoints-1);

  for (size_t i = 0 ; i < outerContour.size()-1 ; ++i) {
    _outlineIndices.push_back(i);
    _outlineIndices.push_back(i+1);
    _outlineIndices.push_back(nbContourPoints+i);
    _outlineIndices.push_back(nbContourPoints+i+1);
  }
  _outlineIndices.push_back(nbContourPoints-1);
  _outlineIndices.push_back(0);
  _outlineIndices.push_back(nbContourPoints+nbContourPoints-1);
  _outlineIndices.push_back(nbContourPoints);


}

void RingGlyph::getIncludeBoundingBox(BoundingBox &boundingBox) {
  boundingBox[0] = Coord(-0.35f, -0.35f, 0);
  boundingBox[1] = Coord(0.35f, 0.35f, 0);
}
