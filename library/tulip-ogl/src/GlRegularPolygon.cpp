/**
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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
#include <tulip/GlRegularPolygon.h>
#include <tulip/DrawingTools.h>

using namespace std;

namespace tlp {

GlRegularPolygon::GlRegularPolygon(const Coord &position, const Size &size,
                                   unsigned int numberOfSides, const Color &fillColor,
                                   const Color &outlineColor, bool filled, bool outlined,
                                   const string &textureName, float outlineSize)
    : position(position), size(size), numberOfSides(numberOfSides), startAngle(float(M_PI) / 2.0f) {
  computePolygon();

  invertYTexture = false;

  setFillColor(fillColor);
  setOutlineColor(outlineColor);
  setFillMode(filled);
  setOutlineMode(outlined);
  setTextureName(textureName);
  setOutlineSize(outlineSize);
}
//=====================================================
GlRegularPolygon::~GlRegularPolygon() {}
//=====================================================
void GlRegularPolygon::setStartAngle(float angle) {
  startAngle = angle;
  computePolygon();
}
//=====================================================
unsigned int GlRegularPolygon::getNumberOfSides() {
  return numberOfSides;
}
//=====================================================
void GlRegularPolygon::setNumberOfSides(unsigned int number) {
  numberOfSides = number;
  computePolygon();
}
//=====================================================
void GlRegularPolygon::resizePoints(const unsigned int number) {
  setNumberOfSides(number);
}
//=====================================================
void GlRegularPolygon::computePolygon() {
  boundingBox = BoundingBox();
  boundingBox.expand(position + size / 2.f);
  boundingBox.expand(position - size / 2.f);

  setPoints(computeRegularPolygon(numberOfSides, position, size, startAngle));

  clearGenerated();
}
}
