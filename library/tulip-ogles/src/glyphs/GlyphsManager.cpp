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

#include <tulip/TulipViewSettings.h>
#include <tulip/TulipFontAwesome.h>

#include <tulip/glyphs/GlyphsManager.h>
#include <tulip/glyphs/SquareGlyph.h>
#include <tulip/glyphs/TriangleGlyph.h>
#include <tulip/glyphs/CircleGlyph.h>
#include <tulip/glyphs/PentagonGlyph.h>
#include <tulip/glyphs/HexagonGlyph.h>
#include <tulip/glyphs/RoundedBoxGlyph.h>
#include <tulip/glyphs/CrossGlyph.h>
#include <tulip/glyphs/DiamondGlyph.h>
#include <tulip/glyphs/StarGlyph.h>
#include <tulip/glyphs/SphereGlyph.h>
#include <tulip/glyphs/CubeGlyph.h>
#include <tulip/glyphs/CubeOutlinedGlyph.h>
#include <tulip/glyphs/CubeOutlinedTransparentGlyph.h>
#include <tulip/glyphs/RingGlyph.h>
#include <tulip/glyphs/CylinderGlyph.h>
#include <tulip/glyphs/ConeGlyph.h>
#include <tulip/glyphs/FontAwesomeGlyph.h>

using namespace std;
using namespace tlp;

static Glyph* createGlyph(int glyphId) {

  if (glyphId == tlp::NodeShape::Square) {
    return new SquareGlyph();
  } else if (glyphId == tlp::NodeShape::Diamond) {
    return new DiamondGlyph();
  } else if (glyphId == tlp::NodeShape::Triangle) {
    return new TriangleGlyph();
  } else if (glyphId == tlp::NodeShape::Circle) {
    return new CircleGlyph();
  } else if (glyphId == tlp::NodeShape::Pentagon) {
    return new PentagonGlyph();
  } else if (glyphId == tlp::NodeShape::Hexagon) {
    return new HexagonGlyph();
  } else if (glyphId == tlp::NodeShape::RoundedBox) {
    return new RoundedBoxGlyph();
  } else if (glyphId == tlp::NodeShape::Cross) {
    return new CrossGlyph();
  } else if (glyphId == tlp::NodeShape::Star) {
    return new StarGlyph();
  } else if (glyphId == tlp::NodeShape::Sphere) {
    return new SphereGlyph();
  } else if (glyphId == tlp::NodeShape::Cube) {
    return new CubeGlyph();
  } else if (glyphId == tlp::NodeShape::CubeOutlined) {
    return new CubeOutlinedGlyph();
  } else if (glyphId == tlp::NodeShape::CubeOutlinedTransparent) {
    return new CubeOutlinedTransparentGlyph();
  } else if (glyphId == tlp::NodeShape::Ring) {
    return new RingGlyph();
  } else if (glyphId == tlp::NodeShape::Cylinder) {
    return new CylinderGlyph();
  } else if (glyphId == tlp::NodeShape::HalfCylinder) {
    return new HalfCylinderGlyph();
  } else if (glyphId == tlp::NodeShape::Cone) {
    return new ConeGlyph();
  } else if (glyphId == tlp::NodeShape::ChristmasTree) {
    return new FontAwesomeGlyph(tlp::TulipFontAwesome::getFontAwesomeIconCodePoint(tlp::TulipFontAwesome::Tree));
  } else if (glyphId >= 0xf000) {
    return new FontAwesomeGlyph(glyphId);
  } else {
    return nullptr;
  }
}


map<int, Glyph*> GlyphsManager::_glyphs;

Glyph *GlyphsManager::getGlyph(int glyphId) {

  if (glyphId == tlp::EdgeExtremityShape::Square) {
    glyphId = tlp::NodeShape::Square;
  }
  if (glyphId == tlp::EdgeExtremityShape::Diamond) {
    glyphId = tlp::NodeShape::Diamond;
  }
  if (glyphId == tlp::EdgeExtremityShape::Arrow) {
    glyphId = tlp::NodeShape::Triangle;
  }
  if (glyphId == tlp::EdgeExtremityShape::Circle) {
    glyphId = tlp::NodeShape::Circle;
  }
  if (glyphId == tlp::EdgeExtremityShape::Pentagon) {
    glyphId = tlp::NodeShape::Pentagon;
  }
  if (glyphId == tlp::EdgeExtremityShape::Hexagon) {
    glyphId = tlp::NodeShape::Hexagon;
  }
  if (glyphId == tlp::EdgeExtremityShape::Cross) {
    glyphId = tlp::NodeShape::Cross;
  }
  if (glyphId == tlp::EdgeExtremityShape::Star) {
    glyphId = tlp::NodeShape::Star;
  }
  if (glyphId == tlp::EdgeExtremityShape::Sphere) {
    glyphId = tlp::NodeShape::Sphere;
  }
  if (glyphId == tlp::EdgeExtremityShape::Cube) {
    glyphId = tlp::NodeShape::Cube;
  }
  if (glyphId == tlp::EdgeExtremityShape::CubeOutlinedTransparent) {
    glyphId = tlp::NodeShape::CubeOutlinedTransparent;
  }
  if (glyphId == tlp::EdgeExtremityShape::Ring) {
    glyphId = tlp::NodeShape::Ring;
  }
  if (glyphId == tlp::EdgeExtremityShape::Cylinder) {
    glyphId = tlp::NodeShape::Cylinder;
  }

  if (_glyphs.find(glyphId) == _glyphs.end()) {
    Glyph *glyph = createGlyph(glyphId);
    if (!glyph) {
      if (_glyphs.find(tlp::NodeShape::Square) == _glyphs.end()) {
        _glyphs[tlp::NodeShape::Square] = createGlyph(tlp::NodeShape::Square);
      }
      glyphId = tlp::NodeShape::Square;
    } else {
      _glyphs[glyphId] = glyph;
    }
  }
  return _glyphs[glyphId];
}
