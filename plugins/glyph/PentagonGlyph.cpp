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

class PentagonGlyph : public Glyph {

public:

  GLYPHINFORMATION("2D - Pentagon", "2D - Pentagon extremity", "Antoine Lambert", "20/05/2016", "Pentagon", "1.0", tlp::NodeShape::Pentagon)

  PentagonGlyph(PluginContext *context) : Glyph(context) {
    _vertices.push_back(Coord(0.f, 0.f));
    vector<Coord> contour = computeRegularPolygon(5, M_PI/2.f);
    _vertices.insert(_vertices.end(), contour.begin(), contour.end());
    for (unsigned short i = 0 ; i < 4 ; ++i) {
      _indices.insert(_indices.end(), {0, ushort_cast(i+1), ushort_cast(i+2)});
    }
    _indices.insert(_indices.end(), {0, 5, 1});

    vector<unsigned short> outlineIndices;
    for (unsigned short i = 1 ; i < ushort_cast(_vertices.size()) ; ++i) {
      outlineIndices.push_back(ushort_cast(i));
    }
    outlineIndices.push_back(1);
    _outlinesIndices.push_back(outlineIndices);
  }

  void getIncludeBoundingBox(BoundingBox &boundingBox) {
    boundingBox[0] = Coord(-0.3f, -0.35f, 0);
    boundingBox[1] = Coord(0.3f, 0.35f, 0);
  }

};

PLUGIN(PentagonGlyph)

