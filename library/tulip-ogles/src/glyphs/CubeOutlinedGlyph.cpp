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

#include <tulip/glyphs/CubeOutlinedGlyph.h>

#include <tulip/glyphs/CubeGlyph.h>

using namespace tlp;
using namespace std;

CubeOutlinedGlyph::CubeOutlinedGlyph() {
  _cubeGlyph = new CubeGlyph();
  _outlineIndices.push_back(0);
  _outlineIndices.push_back(1);
  _outlineIndices.push_back(1);
  _outlineIndices.push_back(2);
  _outlineIndices.push_back(2);
  _outlineIndices.push_back(3);
  _outlineIndices.push_back(3);
  _outlineIndices.push_back(0);
  _outlineIndices.push_back(20);
  _outlineIndices.push_back(21);
  _outlineIndices.push_back(21);
  _outlineIndices.push_back(22);
  _outlineIndices.push_back(22);
  _outlineIndices.push_back(23);
  _outlineIndices.push_back(23);
  _outlineIndices.push_back(20);
  _outlineIndices.push_back(0);
  _outlineIndices.push_back(23);
  _outlineIndices.push_back(1);
  _outlineIndices.push_back(22);
  _outlineIndices.push_back(2);
  _outlineIndices.push_back(21);
  _outlineIndices.push_back(3);
  _outlineIndices.push_back(20);
}

CubeOutlinedGlyph::~CubeOutlinedGlyph() {
  delete _cubeGlyph;
}

void CubeOutlinedGlyph::getIncludeBoundingBox(BoundingBox &boundingBox) {
  _cubeGlyph->getIncludeBoundingBox(boundingBox);
}

void CubeOutlinedGlyph::getTextBoundingBox(BoundingBox &boundingBox) {
  _cubeGlyph->getTextBoundingBox(boundingBox);
}

const vector<Coord> &CubeOutlinedGlyph::getGlyphVertices() const {
  return _cubeGlyph->getGlyphVertices();
}

const vector<Vec2f> &CubeOutlinedGlyph::getGlyphTexCoords() {
  return _cubeGlyph->getGlyphTexCoords();
}

const vector<Coord> &CubeOutlinedGlyph::getGlyphNormals() {
  return _cubeGlyph->getGlyphNormals();
}

const vector<unsigned short> &CubeOutlinedGlyph::getGlyphVerticesIndices() const {
  return _cubeGlyph->getGlyphVerticesIndices();
}

Coord CubeOutlinedGlyph::getAnchor(const Coord &vector) const {
  return _cubeGlyph->getAnchor(vector);
}
