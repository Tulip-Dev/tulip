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
 * Copyright (c) 2016 Antoine Lambert, Thales Services SAS
 * antoine-e.lambert@thalesgroup.com / antoine.lambert33@gmail.com
 *
 */

#include <tulip/Glyph.h>
#include <tulip/GlUtils.h>
#include <tulip/TulipViewSettings.h>

using namespace std;
using namespace tlp;

static const unsigned short nbContourPoints = 30;

static void genCylinderGlyph(const float zBottom, const float zTop, vector<Coord> &vertices, vector<unsigned short> &indices,
                             vector<Vec2f> &texCoords) {
  vector<Coord> bottomCircle = computeRegularPolygon(nbContourPoints, 0.f, Coord(0.f, 0.f, zBottom));
  vector<Coord> topCircle = computeRegularPolygon(nbContourPoints, 0.f, Coord(0.f, 0.f, zTop));
  vertices.push_back(Coord(0.f, 0.f, zBottom));
  vertices.insert(vertices.end(), bottomCircle.begin(), bottomCircle.end());
  vertices.push_back(Coord(0.f, 0.f, zTop));
  vertices.insert(vertices.end(), topCircle.begin(), topCircle.end());

  for (unsigned short i = 0; i < nbContourPoints - 1; ++i) {
    indices.insert(indices.end(), {0, ushort_cast(i + 1), ushort_cast(i + 2)});
  }
  indices.insert(indices.end(), {0, nbContourPoints, 1});

  for (unsigned short i = 0; i < nbContourPoints - 1; ++i) {
    indices.insert(indices.end(), {ushort_cast(nbContourPoints + 1), ushort_cast(nbContourPoints + i + 2), ushort_cast(nbContourPoints + i + 3)});
  }
  indices.insert(indices.end(), {ushort_cast(nbContourPoints + 1), ushort_cast(2 * nbContourPoints + 1), ushort_cast(nbContourPoints + 2)});

  for (size_t i = 0; i < vertices.size(); ++i) {
    tlp::Vec2f st;
    st[0] = vertices[i][0] + 0.5f;
    st[1] = vertices[i][1] + 0.5f;
    texCoords.push_back(st);
  }

  unsigned short startIdx = ushort_cast(vertices.size());

  vertices.insert(vertices.end(), bottomCircle.begin(), bottomCircle.end());
  vertices.insert(vertices.end(), topCircle.begin(), topCircle.end());

  for (unsigned short i = 0; i < nbContourPoints - 1; ++i) {
    indices.insert(indices.end(),
                   {ushort_cast(startIdx + i), ushort_cast(startIdx + i + 1), ushort_cast(startIdx + i + nbContourPoints),
                    ushort_cast(startIdx + i + 1), ushort_cast(startIdx + i + nbContourPoints + 1), ushort_cast(startIdx + i + nbContourPoints)});
  }

  indices.insert(indices.end(), {ushort_cast(startIdx + nbContourPoints - 1), startIdx, ushort_cast(startIdx + 2 * nbContourPoints - 1), startIdx,
                                 ushort_cast(startIdx + nbContourPoints), ushort_cast(startIdx + 2 * nbContourPoints - 1)});

  for (size_t i = startIdx; i < vertices.size(); ++i) {
    tlp::Vec2f st;
    st[0] = vertices[i][0] + 0.5f;
    st[1] = vertices[i][2] + 0.5f;
    texCoords.push_back(st);
  }
}

class CylinderGlyph : public Glyph {

public:
  GLYPHINFORMATION("3D - Cylinder", "3D - Cylinder extremity", "Antoine Lambert", "20/05/2016", "Cylinder", "1.0", tlp::NodeShape::Cylinder)

  CylinderGlyph(PluginContext *context) : Glyph(context) {
    genCylinderGlyph(-0.5f, 0.5f, _vertices, _indices, _texCoords);
  }

  bool glyph2D() const {
    return false;
  }
};

PLUGIN(CylinderGlyph)

class HalfCylinderGlyph : public Glyph {

public:
  GLYPHINFORMATION("3D - Half Cylinder", "3D - Half Cylinder extremity", "Antoine Lambert", "20/05/2016", "Half Cylinder", "1.0",
                   tlp::NodeShape::HalfCylinder)

  HalfCylinderGlyph(PluginContext *context) : Glyph(context) {
    genCylinderGlyph(0.f, 0.5f, _vertices, _indices, _texCoords);
  }

  bool glyph2D() const {
    return false;
  }
};

PLUGIN(HalfCylinderGlyph)
