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

#include <iostream>
#include <cmath>
#include <climits>

#include "ZOrderLayout.h"

using namespace std;
using namespace pocore;

//===============================================================
namespace {
inline Vec2i zorderPoint(const unsigned int key, const unsigned char order) {
  //  cerr << "========" << endl;
  Vec2i point;
  point.fill(0);

  for (char i = order - 1; i >= 0; --i) {
    //read two bits;
    unsigned char bits = (key >> (i << 1))  & 3;
    point[1] += (bits & 1) << i;
    point[0] += ((bits >> 1) ) << i;
  }

  return point;
}

inline unsigned int zorderKey(const Vec2i &p, const unsigned char order) {
  //  cerr << "========" << endl;
  unsigned int key = 0;

  for (char i = order - 1; i >= 0; --i) {
    //read two bits;
    unsigned char bits = (p[1] >> i)  & 1;
    bits += ((p[0] >> i)  & 1) << 1;
    key += bits << (i << 1);
  }

  return key;
}
}
//===============================================================
namespace pocore {
ZorderLayout::ZorderLayout(unsigned char order):order(order) {
  shift = (int)rint(sqrt(pow(4., order))/2.);
}
//==============================================================
unsigned int ZorderLayout::unproject(const Vec2i &point) const {
  //    cerr << point << " ==> " << shift << " order=" << (int)order << endl;
  Vec2i p;

  if (point[0] <= -shift || point[0] >= shift)
    return UINT_MAX;

  if (point[1] <= -shift || point[1] >= shift)
    return UINT_MAX;

  p[0] = point[0] + shift;
  p[1] = point[1] + shift;
  return zorderKey(p, order);
}
//==============================================================
Vec2i ZorderLayout::project(const unsigned int id) const {
  return zorderPoint(id, order)-=shift;
}
}
//==============================================================================
