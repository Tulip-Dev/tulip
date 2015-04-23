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
#include <tulip/GlRect.h>

using namespace std;

namespace tlp {

GlRect::GlRect(const Coord &topLeftPos, const Coord &bottomRightPos, const Color &topLeftCol, const Color &bottomRightCol, bool filled,
               bool outlined) :
  GlPolygon(4u, 4u, 4u, filled, outlined) {

  invertYTexture=false;

  vector<Coord> coords;
  coords.push_back(topLeftPos);
  coords.push_back(topLeftPos);
  coords.push_back(bottomRightPos);
  coords.push_back(bottomRightPos);
  coords[1][0]=bottomRightPos[0];
  coords[3][0]=topLeftPos[0];

  setPoints(coords);

  setFillColor(0, topLeftCol);
  setFillColor(1, topLeftCol);
  setFillColor(2, bottomRightCol);
  setFillColor(3, bottomRightCol);
}

GlRect::GlRect(const Coord &center, const float width, const float height,
               const Color &fillColor, const Color &outlineColor) :
  GlPolygon(4u, 4u, 4u, true, true) {

  invertYTexture=false;

  vector<Coord> coords;
  coords.push_back(center+Coord(width/2.,height/2.,0));
  coords.push_back(center+Coord(width/2.,-height/2.,0));
  coords.push_back(center+Coord(-width/2.,-height/2.,0));
  coords.push_back(center+Coord(-width/2.,height/2.0));
  setPoints(coords);

  setFillColor(fillColor);
  setOutlineColor(outlineColor);
}

//=====================================================
/*
GlRect::GlRect(const Coord &center, const Size &size, const Color &fillColor, const Color &outlineColor):
  GlPolygon(4u, 4u, 4u, true, true) {

  invertYTexture=false;

  vector<Coord> coords;
  coords.push_back(center+Coord(size[0]/2.,size[1]/2.,0));
  coords.push_back(center+Coord(size[0]/2.,-size[1]/2.,0));
  coords.push_back(center+Coord(-size[0]/2.,-size[1]/2.,0));
  coords.push_back(center+Coord(-size[0]/2.,size[1]/2.0));
  setPoints(coords);

  setFillColor(fillColor);

  setOutlineColor(outlineColor);
}
*/
//=====================================================
GlRect::GlRect(bool filled, bool outlined) :
  GlPolygon(4u, 4u, 4u, filled, outlined) {
}
//=====================================================
GlRect::~GlRect() {
}
//=====================================================
Coord GlRect::getCenter() {
  return (point(0) + point(2)) / 2.f;
}
//=====================================================
void GlRect::setCenterAndSize(const Coord &center, const Size &size) {
  points[0]=center+Coord(size[0]/2.,size[1]/2.,0);
  points[1]=center+Coord(size[0]/2.,-size[1]/2.,0);
  points[2]=center+Coord(-size[0]/2.,-size[1]/2.,0);
  points[3]=center+Coord(-size[0]/2.,size[1]/2.,0);
}
//=====================================================
Coord GlRect::getTopLeftPos() {
  return point(0);
}
//=====================================================
Coord GlRect::getBottomRightPos() {
  return point(2);
}
//=====================================================
Color GlRect::getTopLeftColor() {
  return getFillColor(0);
}
//=====================================================
Color GlRect::getBottomRightColor() {
  return getFillColor(2);
}
//=====================================================
void GlRect::setTopLeftPos(const Coord &_topLeftPos) {
  point(0) = _topLeftPos;
  point(1)[1] = _topLeftPos[1];
  point(3)[0] = _topLeftPos[0];

  recomputeBoundingBox();
  clearGenerated();
}
//=====================================================
void GlRect::setBottomRightPos(const Coord &_bottomRightPos) {
  point(2) = _bottomRightPos;
  point(1)[0] = _bottomRightPos[0];
  point(3)[1] = _bottomRightPos[1];

  recomputeBoundingBox();
  clearGenerated();
}
//=====================================================
void GlRect::setTopLeftColor(const Color &_topLeftCol) {
  setFillColor(0, _topLeftCol);
  setFillColor(1, _topLeftCol);
  clearGenerated();
}
//=====================================================
void GlRect::setBottomRightColor(const Color &_bottomRightCol) {
  setFillColor(2, _bottomRightCol);
  setFillColor(3, _bottomRightCol);
  clearGenerated();
}
//=====================================================
bool GlRect::inRect(double x, double y) {
  double minX = std::min(point(0).getX(), point(2).getX());
  double maxX = std::max(point(0).getX(), point(2).getX());
  double minY = std::min(point(0).getY(), point(2).getY());
  double maxY = std::max(point(0).getY(), point(2).getY());
  return ((x >= minX) && (x <= maxX) && (y >= minY) && (y <= maxY));
}
//=====================================================
void GlRect::draw(float lod, Camera *camera) {
  GlPolygon::draw(lod, camera);
}
}

