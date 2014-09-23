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
#include "OrientableSize.h"
#include "OrientableSizeProxy.h"


//====================================================================
OrientableSize::OrientableSize(OrientableSizeProxy* fatherParam,
                               const float width, const float height,
                               const float depth)
  : father(fatherParam) {
  set(width, height, depth);
}

//====================================================================
OrientableSize::OrientableSize(OrientableSizeProxy* fatherParam,
                               const tlp::Size& size)
  :father(fatherParam) {
  set(size);
}

//====================================================================
void OrientableSize::set(const float width, const float height,
                         const float depth) {
  setW(width);
  setH(height);
  setD(depth);
}

//====================================================================
void OrientableSize::set(const tlp::Size& size) {
  tlp::Size::setW(size.getW());
  tlp::Size::setH(size.getH());
  tlp::Size::setD(size.getD());
}

//====================================================================
void OrientableSize::get(float* width, float* height, float* depth) const {
  *width  = getW();
  *height = getH();
  *depth  = getD();
}

//====================================================================
void OrientableSize::setW(float w) {
  (this->*(father->writeW)) (w);
}

//====================================================================
void OrientableSize::setH(float h) {
  (this->*(father->writeH)) (h);
}

//====================================================================
void OrientableSize::setD(float d) {
  (this->*(father->writeD)) (d);
}

//====================================================================
float OrientableSize::getW() const {
  return (this->*(father->readW)) ();
}

//====================================================================
float OrientableSize:: getH() const {
  return (this->*(father->readH)) ();
}

//====================================================================
float OrientableSize::getD() const {
  return (this->*(father->readD)) ();
}
