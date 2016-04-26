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

#ifndef CUBEOUTLINEDTRANSPARENTGLYPH_H
#define CUBEOUTLINEDTRANSPARENTGLYPH_H

#include <tulip/glyphs/Glyph.h>

namespace tlp {

class CubeOutlinedGlyph;

class TLP_GLES_SCOPE CubeOutlinedTransparentGlyph : public Glyph {

public:

  CubeOutlinedTransparentGlyph();

  ~CubeOutlinedTransparentGlyph();

  const std::vector<tlp::Coord> &getGlyphVertices() const;

  const std::vector<unsigned short> &getGlyphOutlineIndices() const;

  bool glyph2D() const {
    return false;
  }

protected:

  tlp::Coord getAnchor(const tlp::Coord &vector) const;

private:

  CubeOutlinedGlyph *_cubeOutlinedGlyph;

};

}

#endif // CUBEOUTLINEDTRANSPARENTGLYPH_H
