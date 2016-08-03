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

static const unsigned short nbContourPoints = 30;

class RingGlyph : public Glyph {

public:

  GLYPHINFORMATION("2D - Ring", "2D - Ring extremity", "Antoine Lambert", "20/05/2016", "Ring", "1.0", tlp::NodeShape::Ring)

  RingGlyph(PluginContext *context) : Glyph(context) {
    vector<Coord> outerContour = computeRegularPolygon(nbContourPoints);
    vector<Coord> innerContour = computeRegularPolygon(nbContourPoints, 0.f, Coord(0.f, 0.f, 0.f), Size(0.25f, 0.25f));
    _vertices.insert(_vertices.end(), innerContour.begin(), innerContour.end());
    _vertices.insert(_vertices.end(), outerContour.begin(), outerContour.end());
    for (unsigned short i = 0 ; i < nbContourPoints - 1 ; ++i) {
      _indices.insert(_indices.end(), {i, ushort_cast(i+1), ushort_cast(i+nbContourPoints),
                                       ushort_cast(i+1), ushort_cast(i+nbContourPoints+1), ushort_cast(i+nbContourPoints)});
    }
    _indices.insert(_indices.end(), {nbContourPoints-1, 0, 2*nbContourPoints-1,
                                     0, nbContourPoints, 2*nbContourPoints-1});

    vector<unsigned short> firstOutlineIndices;
    vector<unsigned short> secondOutlineIndices;
    for (unsigned short i = 0 ; i < ushort_cast(outerContour.size()) ; ++i) {
      firstOutlineIndices.push_back(ushort_cast(i));
      secondOutlineIndices.push_back(ushort_cast(nbContourPoints+i));
    }
    firstOutlineIndices.push_back(0);
    secondOutlineIndices.push_back(nbContourPoints);
    _outlinesIndices.push_back(firstOutlineIndices);
    _outlinesIndices.push_back(secondOutlineIndices);
  }

  void getIncludeBoundingBox(BoundingBox &boundingBox) {
    boundingBox[0] = Coord(-0.35f, -0.35f, 0);
    boundingBox[1] = Coord(0.35f, 0.35f, 0);
  }

};

PLUGIN(RingGlyph)

