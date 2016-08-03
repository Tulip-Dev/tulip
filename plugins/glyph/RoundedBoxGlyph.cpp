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

static vector<Coord> createRoundedRect(const Size &size) {
  float radius = min(size[0] / 4, size[1] / 4);
  float radiusL = radius / size[0];
  float radiusH = radius / size[1];

  float wi = 1.0 - 2*radiusL;
  float hi = 1.0 - 2*radiusH;

  Coord P1 = Coord(-0.5, 0.5) + Coord(radiusL, -radiusH);
  Coord P2 = P1 + Coord(wi, 0);
  Coord P3 = P2 + Coord(0, -hi);
  Coord P4 = P1 + Coord(0, -hi);

  int steps = 20;
  float delta = (M_PI/2) / steps;

  vector<Coord> boxPoints;
  boxPoints.resize(steps * 4);

  for (int i = 0 ; i < steps; ++i ) {
    float w = delta + i * delta;
    float x = -cos(w);
    float y = sin(w);
    Coord p = P1 + Coord(x, y) * Coord(radiusL, radiusH);
    boxPoints[i] = p;

    w = delta + (steps - i - 1) * delta;
    x = cos(w);
    y = sin(w);
    p = P2 + Coord(x, y) * Coord(radiusL, radiusH);
    boxPoints[steps + i] = p;

    w = delta +  i * delta;
    x = cos(w);
    y = -sin(w);
    p = P3 + Coord(x, y) * Coord(radiusL, radiusH);
    boxPoints[2 * steps + i] = p;

    w = delta + (steps - i - 1) * delta;
    x = -cos(w);
    y = -sin(w);
    p = P4 + Coord(x, y) * Coord(radiusL, radiusH);
    boxPoints[3 * steps + i] = p;
  }

  return boxPoints;
}

static Coord computeCircleArcMidPoint(const Coord &start, const Coord &end, const Coord &center) {
  float radius = start.dist(center);
  float c = atan2(start[1]+end[1], start[0]+end[0]);
  return Coord(center.x() + radius*cos(c), center.y() + radius*sin(c));
}

static Coord minIncludeBBSquare = computeCircleArcMidPoint(Coord(-0.25, -0.5), Coord(-0.5, -0.25), Coord(-0.25, -0.25));
static Coord maxIncludeBBSquare = -minIncludeBBSquare;

class RoundedBoxGlyph : public Glyph {

public:

  GLYPHINFORMATION("2D - Rounded Box", "2D - Rounded Box extremity", "Antoine Lambert", "20/05/2016", "Rounded Box", "1.0", tlp::NodeShape::RoundedBox)

  RoundedBoxGlyph(PluginContext *context) : Glyph(context) {
    _vertices.push_back(Coord());
    vector<Coord> contour = createRoundedRect(Size(1,1,1));
    _vertices.insert(_vertices.end(), contour.begin(), contour.end());
    for (unsigned short i = 0 ; i < ushort_cast(contour.size()-1) ; ++i) {
      _indices.insert(_indices.end(), {0, ushort_cast(i+1), ushort_cast(i+2)});
    }
    _indices.insert(_indices.end(), {0, ushort_cast(contour.size()), 1});

    vector<unsigned short> outlineIndices;
    for (unsigned short i = 1 ; i < ushort_cast(_vertices.size()) ; ++i) {
      outlineIndices.push_back(i);
    }
    outlineIndices.push_back(1);
    _outlinesIndices.push_back(outlineIndices);
  }

  void getIncludeBoundingBox(BoundingBox &boundingBox) {
    boundingBox[0] = minIncludeBBSquare;
    boundingBox[1] = maxIncludeBBSquare;
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

PLUGIN(RoundedBoxGlyph)
