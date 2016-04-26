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

#include <tulip/glyphs/ConeGlyph.h>
#include <tulip/GlUtils.h>

using namespace std;
using namespace tlp;

static const unsigned int nbContourPoints = 30;

static void genConeGlyph(vector<Coord> &vertices, vector<unsigned short> &indices, vector<Vec2f> &texCoords, bool eeGlyph) {
  vector<Coord> bottomCircle = computeRegularPolygon(nbContourPoints, 0.f, Coord(0.f, 0.f, -0.5f));
  if (eeGlyph) {
    for (size_t i = 0 ; i < bottomCircle.size() ; ++i) {
      swap(bottomCircle[i][1], bottomCircle[i][2]);
    }
  }
  vertices.push_back(Coord(0.f, 0.f, -0.5f));
  if (eeGlyph) {
    swap(vertices.back()[1], vertices.back()[2]);
  }
  vertices.insert(vertices.end(), bottomCircle.begin(), bottomCircle.end());

  for (unsigned int i = 0 ; i < nbContourPoints - 1 ; ++i) {
    indices.push_back(0);
    indices.push_back(i+1);
    indices.push_back(i+2);
  }
  indices.push_back(0);
  indices.push_back(nbContourPoints);
  indices.push_back(1);

  unsigned startIdx = vertices.size();

  vertices.insert(vertices.end(), bottomCircle.begin(), bottomCircle.end());

  vertices.push_back(Coord(0,0,0.5f));
  if (eeGlyph) {
    swap(vertices.back()[1], vertices.back()[2]);
  }

  for (unsigned int i = 0 ; i < nbContourPoints - 1 ; ++i) {
    indices.push_back(vertices.size()-1);
    indices.push_back(startIdx+i);
    indices.push_back(startIdx+i+1);
  }

  indices.push_back(vertices.size()-1);
  indices.push_back(startIdx+nbContourPoints-1);
  indices.push_back(startIdx);


  for (size_t i = 0 ; i < vertices.size() ; ++i) {
    tlp::Vec2f st;
    st[0] = vertices[i][0] + 0.5f;
    st[1] = vertices[i][1] + 0.5f;
    texCoords.push_back(st);
  }

}

ConeGlyph::ConeGlyph(bool eeGlyph) : _eeGlyph(eeGlyph) {
  genConeGlyph(_vertices, _indices, _texCoords, _eeGlyph);
}

void ConeGlyph::getIncludeBoundingBox(tlp::BoundingBox &boundingBox) {
  boundingBox[0] = Coord(-0.25, -0.25, 0);
  boundingBox[1] = Coord(0.25, 0.25, 0.5);
  if (_eeGlyph) {
    swap(boundingBox[0][1], boundingBox[0][2]);
  }
}

tlp::Coord ConeGlyph::getAnchor(const tlp::Coord &vector) const {
  Coord anchor = vector;

  float x, y, z, n;
  anchor.get(x, y, z);
  n = sqrt(x * x + y * y);
  float vx0, vy0, vx1, vy1, x0, y0, x1, y1, px, py;
  x0 = 0;
  y0 = 0.5;
  vx0 = 0.5;
  vy0 = -1.0;
  x1 = 0;
  y1 = 0;
  vx1 = sqrt(x * x + y * y);
  vy1 = z;
  py = -1.0 * (vy1 * (vx0 / vy0 * y0 + x0 - x1) - vx1 * y1) / (vx1 - vy1 / vy0 * vx0);
  px = vx0 * (py - y0) / vy0 + x0;

  if (fabsf(py) > 0.5) {
    n = anchor.norm();
    py = n * 0.5 / fabsf(z);
    anchor.setX(x * py / n);
    anchor.setY(y * py / n);
    anchor.setZ(z * py / n);
  }
  else {
    anchor.setX(x * px / n);
    anchor.setY(y * px / n);
    anchor.setZ(py);
  }

  if (_eeGlyph) {
    swap(anchor[1], anchor[2]);
  }

  return anchor;
}
