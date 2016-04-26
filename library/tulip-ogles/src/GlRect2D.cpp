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

#include <tulip/GlRect2D.h>

using namespace std;
using namespace tlp;

GlRect2D::GlRect2D(const Vec2f &bottomLeftCorner, const Vec2f &topRightCorner, const Color &fillColor, float z) {
  createContour(bottomLeftCorner, topRightCorner, z);
  setFilled(true);
  setFillColor(fillColor);
  setOutlined(false);
}

GlRect2D::GlRect2D(const Vec2f &bottomLeftCorner, const Vec2f &topRightCorner, const Color &fillColor, const Color &outlineColor, float z) {
  createContour(bottomLeftCorner, topRightCorner, z);
  setFilled(true);
  setFillColor(fillColor);
  setOutlined(true);
  setOutlineColor(outlineColor);
}

GlRect2D::GlRect2D(float bottom, float left, float width, float height, const Color &fillColor, float z) {
  createContour(Vec2f(left, bottom), Vec2f(left+width, bottom+height), z);
  setFilled(true);
  setFillColor(fillColor);
  setOutlined(false);
}

GlRect2D::GlRect2D(float bottom, float left, float width, float height, const Color &fillColor, const Color &outlineColor, float z) {
  createContour(Vec2f(left, bottom), Vec2f(left+width, bottom+height), z);
  setFilled(true);
  setFillColor(fillColor);
  setOutlined(true);
  setOutlineColor(outlineColor);
}

void GlRect2D::setCoordinatesAndSize(float bottom, float left, float width, float height, float z) {
  createContour(Vec2f(left, bottom), Vec2f(left+width, bottom+height), z);
}

void GlRect2D::createContour(const Vec2f &bottomLeftCorner, const Vec2f &topRightCorner, const float z) {
  vector<Coord> contour;
  contour.push_back(Coord(bottomLeftCorner, z));
  contour.push_back(Coord(topRightCorner[0], bottomLeftCorner[1], z));
  contour.push_back(Coord(topRightCorner, z));
  contour.push_back(Coord(bottomLeftCorner[0], topRightCorner[1], z));
  setPolygonContour(contour);
}

