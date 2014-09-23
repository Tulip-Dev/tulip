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
#include <tulip/GlRegularPolygon.h>



using namespace std;

namespace tlp {

GlRegularPolygon::GlRegularPolygon(const Coord &position,
                                   const Size &size,
                                   unsigned int numberOfSides,
                                   const Color &fillColor,
                                   const Color &outlineColor,
                                   bool filled,
                                   bool outlined,
                                   const string &textureName,
                                   float outlineSize):
  position(position),
  size(size),
  numberOfSides(numberOfSides),
  startAngle(float(M_PI)/2.0f) {
  computePolygon();

  invertYTexture=false;

  setFillColor(fillColor);
  setOutlineColor(outlineColor);
  setFillMode(filled);
  setOutlineMode(outlined);
  setTextureName(textureName);
  setOutlineSize(outlineSize);
}
//=====================================================
GlRegularPolygon::~GlRegularPolygon() {
}
//=====================================================
void GlRegularPolygon::setStartAngle(float angle) {
  startAngle=angle;
  computePolygon();
}
//=====================================================
unsigned int GlRegularPolygon::getNumberOfSides() {
  return numberOfSides;
}
//=====================================================
void GlRegularPolygon::setNumberOfSides(unsigned int number) {
  numberOfSides=number;
  computePolygon();
}
//=====================================================
void GlRegularPolygon::resizePoints(const unsigned int number) {
  setNumberOfSides(number);
}
//=====================================================
void GlRegularPolygon::computePolygon() {
  boundingBox = BoundingBox();

  BoundingBox box;
  vector<Coord> points;
  float delta = (2.0f * M_PI) / (float)numberOfSides;

  for (unsigned int i=0; i < numberOfSides; ++i) {
    float deltaX = cos(i * delta + startAngle);
    float deltaY = sin(i * delta + startAngle);
    points.push_back(Coord(deltaX,deltaY,position[2]));
    box.expand(points.back());
  }

  for(vector<Coord>::iterator it=points.begin(); it!=points.end(); ++it) {
    (*it)[0]=position[0]+(((*it)[0]-((box[1][0]+box[0][0])/2.))/((box[1][0]-box[0][0])/2.))*size[0];
    (*it)[1]=position[1]+(((*it)[1]-((box[1][1]+box[0][1])/2.))/((box[1][1]-box[0][1])/2.))*size[1];
  }

  boundingBox.expand(position+size/2.f);
  boundingBox.expand(position-size/2.f);

  setPoints(points);

  clearGenerated();
}
}
