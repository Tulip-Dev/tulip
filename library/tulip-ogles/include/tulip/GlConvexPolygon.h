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

#ifndef GLCONVEXPOLYGON_H
#define GLCONVEXPOLYGON_H

#if defined(_MSC_VER)
#include <windows.h>
#endif

#include <GL/gl.h>

#include <vector>

#include <tulip/Coord.h>
#include <tulip/Color.h>
#include <tulip/BoundingBox.h>

#include <tulip/GlEntity.h>

namespace tlp {

class GlBuffer;

class TLP_GLES_SCOPE GlConvexPolygon : public GlEntity {

public:

  GlConvexPolygon();

  GlConvexPolygon(const std::vector<tlp::Coord> &contour, const tlp::Color &fillColor);

  GlConvexPolygon(const std::vector<tlp::Coord> &contour, const tlp::Color &fillColor, const tlp::Color &outlineColor, const float outlineWidth=1.f);

  ~GlConvexPolygon();

  void setPolygonContour(const std::vector<tlp::Coord> &contour);

  const std::vector<tlp::Coord> &polygonContour() const {
    return _contour;
  }

  void setFilled(const bool filled) {
    _filled = filled;
  }

  void setOutlined(const bool outlined) {
    _outlined = outlined;
  }

  void setFillColor(const tlp::Color &fillColor) {
    _fillColor = fillColor;
  }

  void setOutlineColor(const tlp::Color &outlineColor) {
    _outlineColor = outlineColor;
  }

  void setOutlineWidth(const float outlineWidth) {
    _outlineWidth = outlineWidth;
  }

  void setTexture(const std::string &texture) {
    _texture = texture;
  }

  void draw(const Camera &camera, bool pickingMode=false);

  void draw(const Camera &camera, const Light &, bool pickingMode=false);

  void translate(const tlp::Coord &move);

private:

  void uploadRenderingData();

  std::vector<tlp::Coord> _contour;

  bool _filled;
  bool _outlined;
  tlp::Color _fillColor;
  tlp::Color _outlineColor;
  float _outlineWidth;

  GlBuffer *_polygonDataBuffer = nullptr;
  GlBuffer *_polygonIndicesBuffer = nullptr;

  unsigned int _nbVertices = 0;
  unsigned int _nbIndices= 0;

  std::string _texture;

  bool _needRenderingDataUpload = true;

};

}

#endif // GLCONVEXPOLYGON_H
