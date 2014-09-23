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
#include <tulip/GlStar.h>

const float startAngle = float(M_PI)/2.0f;

using namespace std;

namespace tlp {

GlStar::GlStar(const Coord &position,
               const Size &size,
               unsigned int numberOfStarPoints,
               const Color &fillColor,
               const Color &outlineColor,
               bool outlined,
               const string &textureName,
               float outlineSize):
  GlComplexPolygon(vector<Coord>(), fillColor, outlineColor, 0, textureName),
  position(position),
  size(size),
  numberOfStarPoints(numberOfStarPoints) {
  setFillColor(fillColor);
  setOutlineColor(outlineColor);
  setOutlineMode(outlined);
  setTextureName(textureName);
  setOutlineSize(outlineSize);
  computeStar();
}
//=====================================================
GlStar::~GlStar() {
}
//=====================================================
unsigned int GlStar::getNumberOfStarPoints() {
  return numberOfStarPoints;
}
//=====================================================
void GlStar::computeStar() {
  boundingBox = BoundingBox();

  BoundingBox box;
  vector<Coord> points;
  float delta = (2.0f * M_PI) / (float)numberOfStarPoints;

  for (unsigned int i=0; i < numberOfStarPoints; ++i) {
    float deltaX = cos(i * delta + startAngle);
    float deltaY = sin(i * delta + startAngle);
    points.push_back(Coord(deltaX,deltaY,0));
    box.expand(points.back());
    deltaX = 0.5f * cos(i * delta + delta/2.0f + startAngle);
    deltaY = 0.5f * sin(i * delta + delta/2.0f + startAngle);
    points.push_back(Coord(deltaX,deltaY,0));
    box.expand(points.back());
  }

  for(vector<Coord>::iterator it=points.begin(); it!=points.end(); ++it) {
    (*it)[0]=position[0]+(((*it)[0]-((box[1][0]+box[0][0])/2.))/((box[1][0]-box[0][0])/2.))*size[0];
    (*it)[1]=position[1]+(((*it)[1]-((box[1][1]+box[0][1])/2.))/((box[1][1]-box[0][1])/2.))*size[1];
  }

  boundingBox.expand(position+size/2.f);
  boundingBox.expand(position-size/2.f);

  createPolygon(points, 0);
  runTesselation();
}
}
