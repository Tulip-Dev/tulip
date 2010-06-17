/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
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
#ifndef TULIP_SIZE_H
#define TULIP_SIZE_H
#include <tulip/Vector.h>
#include <tulip/VectorCast.h>
///

namespace tlp {

  class TLP_SCOPE Size : public tlp::VectorCast<float,3, tlp::Size> {
public:
  inline explicit Size(const float width=0,const float height=0,const float depth=0);
  inline explicit Size(const tlp::Vector<float,3> &);

  inline void set(const float width=0,const float height=0,const float depth=0);
  inline void set(const Size& size);
  inline void setW(const float width);
  inline void setH(const float height);
  inline void setD(const float depth);
  inline float getW() const;
  inline float getH() const;
  inline float getD() const;
  inline void get(float &width, float &height, float &depth) const;
};

  Size::Size(const tlp::Vector<float,3> &v) : tlp::VectorCast<float,3, tlp::Size>(v) {}

Size::Size(const float width,const float height,const float depth) {
  set(width, height, depth);
}

void Size::set(const float width,const float height,const float depth) {
  array[0]=width;
  array[1]=height;
  array[2]=depth;
}
void Size::set(const Size &size) { (*this) = size; }

float Size::getW() const {return array[0];}
float Size::getH() const {return array[1];}
float Size::getD() const {return array[2];}

void Size::get(float &width,float &height,float &depth) const {
  width=array[0];
  height=array[1];
  depth=array[2];
}
void Size::setW(const float width) {array[0]=width;}
void Size::setH(const float height) {array[1]=height;}
void Size::setD(const float depth) {array[2]=depth;}

}
#endif
