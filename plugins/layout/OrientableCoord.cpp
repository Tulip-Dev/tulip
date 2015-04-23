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

#include "OrientableLayout.h"
#include "OrientableCoord.h"



//====================================================================
OrientableCoord::OrientableCoord(OrientableLayout* fatherParam, const float x,
                                 const float y, const float z)
  :father(fatherParam) {
  set(x, y, z);
}
//====================================================================
OrientableCoord::OrientableCoord(OrientableLayout* fatherParam, const tlp::Coord& v)
  :father(fatherParam) {
  set(v);
}
//====================================================================
void OrientableCoord::set(const float x, const float y, const float z) {
  setX(x);
  setY(y);
  setZ(z);
}
//====================================================================
void OrientableCoord::set(const tlp::Coord& coord) {
  tlp::Coord::setX(coord.getX());
  tlp::Coord::setY(coord.getY());
  tlp::Coord::setZ(coord.getZ());
}
//====================================================================
void OrientableCoord::setX(float x) {
  (this->*(father->writeX))(x);
}
//====================================================================
void OrientableCoord::setY(float y) {
  (this->*(father->writeY))(y);
}
//====================================================================
void OrientableCoord::setZ(float z) {
  (this->*(father->writeZ))(z);
}
//====================================================================
float OrientableCoord::getX() const {
  return (this->*(father->readX))();
}
//====================================================================
float OrientableCoord::getY() const {
  return (this->*(father->readY))();
}
//====================================================================
float OrientableCoord::getZ() const {
  return (this->*(father->readZ))();
}
//====================================================================
void OrientableCoord::setInvertedX(float x) {
  tlp::Coord::setX(-x);
}
//====================================================================
void OrientableCoord::setInvertedY(float y) {
  tlp::Coord::setY(-y);
}
//====================================================================
void OrientableCoord::setInvertedZ(float z) {
  tlp::Coord::setZ(-z);
}
//====================================================================
float OrientableCoord::getInvertedX() const {
  return -tlp::Coord::getX();
}
//====================================================================
float OrientableCoord::getInvertedY() const {
  return -tlp::Coord::getY();
}
//====================================================================
float OrientableCoord::getInvertedZ() const {
  return -tlp::Coord::getZ();
}
//====================================================================
