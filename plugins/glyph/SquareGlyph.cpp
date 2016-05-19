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

class SquareGlyph : public Glyph {

public:

  GLYPHINFORMATION("2D - Square", "2D - Square extremity", "Antoine Lambert", "20/05/2016", "Square", "1.0", tlp::NodeShape::Square)

  SquareGlyph(PluginContext *context) : Glyph(context) {
    _vertices = computeRegularPolygon(4, M_PI/4.f);
    _indices = {0, 1, 3, 1, 2, 3};
    for (unsigned short i = 0 ; i < ushort_cast(_vertices.size()-1) ; ++i) {
      _outlineIndices.insert(_outlineIndices.end(), {i, ushort_cast(i+1)});
    }
    _outlineIndices.insert(_outlineIndices.end(), {ushort_cast(_vertices.size()-1), 0});
  }

protected:

  Coord getAnchor(const Coord &vector) const {
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

};

PLUGIN(SquareGlyph)
