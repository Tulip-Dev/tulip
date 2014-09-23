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
#include <tulip/GlCircle.h>
#include <tulip/GlXMLTools.h>

using namespace std;

namespace tlp {

//===================================================
GlCircle::GlCircle(const Coord &center, float radius,
                   const Color &outlineColor,
                   const Color &fillColor,
                   bool filled, bool outlined ,
                   float startAngle,
                   unsigned int segments) :
  GlRegularPolygon(center,Size(radius,radius,0),segments,fillColor,outlineColor,filled,outlined) {
  assert(segments<=256);
  setStartAngle(startAngle);
}
//===========================================================
void  GlCircle::set(const Coord &center, float radius, float startAngle) {
  this->position=center;
  this->size=Size(radius,radius,0);
  this->startAngle=startAngle;
  computePolygon();
}
//===========================================================
void GlCircle::getXML(string &outString) {

  GlXMLTools::createProperty(outString,"type","GlCircle","GlEntity");

  GlPolygon::getXMLOnlyData(outString);
}
}
