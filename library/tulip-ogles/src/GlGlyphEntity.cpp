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

#include <tulip/GlGlyphEntity.h>
#include <tulip/GlyphsRenderer.h>

using namespace tlp;
using namespace std;

GlGlyphEntity::GlGlyphEntity(int glyphId) : _glyphId(glyphId), _center(Coord(0.f,0.f,0.f)), _size(1.f,1.f,1.f), _color(Color::Red),
  _texture(""), _borderWidth(1.f), _borderColor(Color::Black),
  _rotationAxisAndAngle(Vec4f(0.f,0.f,1.f,0.f)), _filled(true), _outlined(true) {
  updateBoundingBox();
}

GlGlyphEntity::GlGlyphEntity(int glyphId, const Coord &center, const Size &size, const Color &color,
                             const string &texture, const float borderWidth, const Color &borderColor,
                             const Vec4f &rotationAxisAndAngle) : _glyphId(glyphId), _center(center), _size(size), _color(color),
  _texture(texture), _borderWidth(borderWidth), _borderColor(borderColor),
  _rotationAxisAndAngle(rotationAxisAndAngle), _filled(true), _outlined(true) {
  updateBoundingBox();
}

void GlGlyphEntity::setCenter(const tlp::Coord &center) {
  _center = center;
  updateBoundingBox();
}

void GlGlyphEntity::setSize(const tlp::Size &size) {
  _size = size;
  updateBoundingBox();
}

void GlGlyphEntity::updateBoundingBox() {
  _boundingBox[0] = _center - _size / 2.f;
  _boundingBox[1] = _center + _size / 2.f;
}

void GlGlyphEntity::draw(const Camera &camera, const Light &light, bool pickingMode) {
  if (!pickingMode) {
    Color fillColor = _color;
    float borderWidth_ = _borderWidth;
    if (!_filled) {
      fillColor = Color(0,0,0,0);
    }
    if (!_outlined) {
      borderWidth_ = 0;
    }
    GlyphsRenderer::instance()->renderGlyph(camera, light, _glyphId, _center, _size, fillColor, _texture, borderWidth_, _borderColor, _rotationAxisAndAngle);
  } else {
    GlyphsRenderer::instance()->renderGlyph(camera, light, _glyphId, _center, _size, _pickingColor, _texture, _borderWidth, _pickingColor, _rotationAxisAndAngle, true);
  }

}

void GlGlyphEntity::draw(const Camera &camera, bool pickingMode) {
  GlEntity::draw(camera, pickingMode);
}
