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

#ifndef GLGLYPHENTITY_H
#define GLGLYPHENTITY_H

#include <tulip/Coord.h>
#include <tulip/Size.h>
#include <tulip/Color.h>

#include <string>

#include <tulip/GlEntity.h>

namespace tlp {

class TLP_GLES_SCOPE GlGlyphEntity : public GlEntity {

public:

  GlGlyphEntity(int glyphId);

  GlGlyphEntity(int glyphId, const tlp::Coord &center, const tlp::Size &size, const tlp::Color &color,
                const std::string &texture="", const float borderWidth=0, const tlp::Color &borderColor=tlp::Color::Black,
                const tlp::Vec4f &rotationAxisAndAngle=tlp::Vec4f(0.0f,0.0f,1.0f,0.0f));

  void draw(const Camera &camera, const Light &light, bool pickingMode=false);

  void draw(const Camera &camera, bool pickingMode=false);

  void setCenter(const tlp::Coord &center);

  const tlp::Coord &center() const {
    return _center;
  }

  void setSize(const tlp::Size &size);

  const tlp::Size &size() const {
    return _size;
  }

  void setColor(const tlp::Color &color) {
    _color = color;
  }

  const tlp::Color &color() const {
    return _color;
  }

  void setTexture(const std::string &texture) {
    _texture = texture;
  }

  const std::string &texture() const {
    return _texture;
  }

  void setBorderWidth(const float borderWidth) {
    _borderWidth = borderWidth;
  }

  float borderWidth() const {
    return _borderWidth;
  }

  void setBorderColor(const tlp::Color &borderColor) {
    _borderColor = borderColor;
  }

  const tlp::Color &borderColor() const {
    return _borderColor;
  }

  void setRotationAxisAndAngle(const tlp::Vec4f &rotationAxisAndAngle) {
    _rotationAxisAndAngle = rotationAxisAndAngle;
  }

  const tlp::Vec4f &rotationAxisAndAngle() const {
    return _rotationAxisAndAngle;
  }


private:

  void updateBoundingBox();

  int _glyphId;
  tlp::Coord _center;
  tlp::Size _size;
  tlp::Color _color;
  std::string _texture;
  float _borderWidth;
  tlp::Color _borderColor;
  tlp::Vec4f _rotationAxisAndAngle;


};

}

#endif // GLGLYPHENTITY_H
