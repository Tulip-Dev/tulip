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

#ifndef GLRECT2D_H
#define GLRECT2D_H

#include <tulip/GlConvexPolygon.h>

namespace tlp {

class TLP_GLES_SCOPE GlRect2D : public GlConvexPolygon {

public:

  GlRect2D(const tlp::Vec2f &bottomLeftCorner, const tlp::Vec2f &topRightCorner, const tlp::Color &fillColor, float z=0);

  GlRect2D(const tlp::Vec2f &bottomLeftCorner, const tlp::Vec2f &topRightCorner, const tlp::Color &fillColor, const tlp::Color &outlineColor, float z=0);

  GlRect2D(float bottom, float left, float width, float height, const tlp::Color &fillColor, float z=0);

  GlRect2D(float bottom, float left, float width, float height, const tlp::Color &fillColor, const tlp::Color &outlineColor, float z=0);

  void setCoordinatesAndSize(float bottom, float left, float width, float height, float z=0);

private:

  void createContour(const tlp::Vec2f &bottomLeftCorner, const tlp::Vec2f &topRightCorner, const float z);

};

}

#endif // GLRECT2D_H
