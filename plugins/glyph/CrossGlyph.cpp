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

using namespace tlp;

class CrossGlyph : public Glyph {

public:

  GLYPHINFORMATION("2D - Cross", "2D - Cross extremity", "Antoine Lambert", "20/05/2016", "Cross", "1.0", NodeShape::Cross)

  CrossGlyph(PluginContext *context) : Glyph(context) {
    float bWidth = 0.1f;
    _vertices = {Coord(-bWidth, 0.5, 0),
                 Coord(bWidth, 0.5, 0),
                 Coord(bWidth, bWidth, 0),
                 Coord(0.5, bWidth, 0),
                 Coord(0.5, -bWidth, 0),
                 Coord(bWidth, -bWidth, 0),
                 Coord(bWidth, -0.5, 0),
                 Coord(-bWidth, -0.5, 0),
                 Coord(-bWidth, -bWidth, 0),
                 Coord(-0.5, -bWidth, 0),
                 Coord(-0.5, bWidth, 0),
                 Coord(-bWidth, bWidth, 0)};

    _indices = {2, 1, 0, 0, 11, 2,
                10, 9, 3, 9, 4, 3,
                8, 7, 6, 8, 6, 5};

    for (unsigned short i = 0 ; i < ushort_cast(_vertices.size()-1) ; ++i) {
      _outlineIndices.insert(_outlineIndices.end(), {i, ushort_cast(i+1)});
    }
    _outlineIndices.insert(_outlineIndices.end(), {ushort_cast(_vertices.size()-1), 0});
  }

  void getIncludeBoundingBox(BoundingBox &boundingBox) {
    boundingBox[0] = Coord(-0.5, -0.1);
    boundingBox[1] = Coord(0.5, 0.1);
  }

protected:

  Coord getAnchor(const Coord &vector) const {
    Coord v(vector);
    float x, y, z;
    v.get(x, y, z);
    // initialize anchor as the middle of segment points[0], points[1]
    Coord anchor(0, 0.5, 0);
    float distMin = x*x + ((y - 0.5) * (y - 0.5));
    // check with the middle of segment points[3], points[4]
    float dist = ((x - 0.5) * (x - 0.5)) + y*y;

    if (distMin > dist) {
      distMin = dist;
      anchor = Coord(0.5, 0, 0);
    }

    // check with the middle of segment points[7], points[6]
    dist = x*x + ((y + 0.5)*(y + 0.5));

    if (distMin > dist) {
      distMin = dist;
      anchor = Coord(0, -0.5, 0);
    }

    // check with the middle of segment points[9], points[10]
    if (distMin > ((x + 0.5) * (x + 0.5)) + y*y)
      return Coord(-0.5, 0, 0);

    return anchor;
  }

};

PLUGIN(CrossGlyph)
