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
#include <tulip/Glyph.h>
#include <tulip/TulipViewSettings.h>

using namespace std;
using namespace tlp;

class DiamondGlyph : public Glyph {

public:

  GLYPHINFORMATION("2D - Diamond", "2D - Diamond extremity", "Antoine Lambert", "20/05/2016", "Diamond", "1.0", tlp::NodeShape::Diamond)

  DiamondGlyph(PluginContext *context) : Glyph(context) {
    _vertices = computeRegularPolygon(4);
    _indices = {0, 1, 3, 1, 2, 3};
    for (unsigned short i = 0 ; i < ushort_cast(_vertices.size()-1) ; ++i) {
      _outlineIndices.insert(_outlineIndices.end(), {i, ushort_cast(i+1)});
    }
    _outlineIndices.insert(_outlineIndices.end(), {ushort_cast(_vertices.size()-1), 0});
  }

  void getIncludeBoundingBox(BoundingBox &boundingBox) {
    boundingBox[0] = Coord(-0.35f, -0.35f, 0);
    boundingBox[1] = Coord(0.35f, 0.35f, 0);
  }

protected:

  Coord getAnchor(const Coord &vector) const {
    Coord v(vector);
    float x, y, z;
    v.get(x, y, z);
    // initialize anchor as top corner
    Coord anchor(0, 0.5, 0);
    float distMin = x*x + ((y - 0.5) * (y - 0.5));
    // check with the right corner
    float dist = ((x - 0.5) * (x - 0.5)) + y*y;

    if (distMin > dist) {
      distMin = dist;
      anchor = Coord(0.5, 0, 0);
    }

    // check with the bottom corner
    dist = x*x + ((y + 0.5)*(y + 0.5));

    if (distMin > dist) {
      distMin = dist;
      anchor = Coord(0, -0.5, 0);
    }

    // check with left corner
    if (distMin > ((x + 0.5) * (x + 0.5)) + y*y)
      return Coord(-0.5, 0, 0);

    return anchor;
  }

};

PLUGIN(DiamondGlyph)
