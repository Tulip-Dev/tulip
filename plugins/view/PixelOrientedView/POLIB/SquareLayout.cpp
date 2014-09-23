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

#include "SquareLayout.h"

using namespace std;
using namespace tlp;

namespace pocore {
//==============================================================
SquareLayout::SquareLayout(unsigned int width):_width(width) {
}
//==============================================================
unsigned int SquareLayout::unproject(const Vector<int, 2> &point) const {

  int x = point[0] + _width/2;
  int y = point[1] + _width/2;

  if (x>int(_width)) return UINT_MAX;

  if (y>int(_width)) return UINT_MAX;

  return (unsigned int)y * _width + (unsigned int)x;
}
//==============================================================
Vector<int, 2> SquareLayout::project(const unsigned int id) const {
  Vector<int, 2> point;
  point[0] = id % _width - _width/2;
  point[1] = id / _width - _width/2;
  return point;
}
}
