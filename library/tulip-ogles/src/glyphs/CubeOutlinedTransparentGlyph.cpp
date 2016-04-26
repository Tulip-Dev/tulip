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

#include <tulip/glyphs/CubeOutlinedTransparentGlyph.h>

#include <tulip/glyphs/CubeOutlinedGlyph.h>

using namespace tlp;
using namespace std;

CubeOutlinedTransparentGlyph::CubeOutlinedTransparentGlyph() {
  _cubeOutlinedGlyph = new CubeOutlinedGlyph();
}

CubeOutlinedTransparentGlyph::~CubeOutlinedTransparentGlyph() {
  delete _cubeOutlinedGlyph;
}

const vector<Coord> &CubeOutlinedTransparentGlyph::getGlyphVertices() const {
  return _cubeOutlinedGlyph->getGlyphVertices();
}

const vector<unsigned short> &CubeOutlinedTransparentGlyph::getGlyphOutlineIndices() const {
  return _cubeOutlinedGlyph->getGlyphOutlineIndices();
}

Coord CubeOutlinedTransparentGlyph::getAnchor(const Coord &vector) const {
  return _cubeOutlinedGlyph->getAnchor(vector);
}
