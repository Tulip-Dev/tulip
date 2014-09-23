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
#include <tulip/Plane.h>

using namespace std;
using namespace tlp;

Plane::Plane() {
  a = 1.0;
  b = 1.0;
  c = 1.0;
  d = 1.0;
}

Plane::Plane(float a, float b, float c, float d) {
  this->a = a;
  this->b = b;
  this->c = c;
  this->d = d;
}

Plane::~Plane() {
}

float Plane::planeValue(float a, float b, float c, float d, float x, float y) {
  return - (a * x + b * y + c) / d;
}

bool Plane::computePlane(Coord &p1, Coord &p2, Coord &p3, Coord &p4) {
  // This is not a plane !
  if (a == 0 && b == 0 && c == 0)
    return false;

  if (a != 0) {
    p2    = p1;
    p2[2] = p3[2];

    p4    = p3;
    p4[2] = p1[2];

    p1[0] = planeValue(b, c, d, a, p1[1], p1[2]);
    p2[0] = planeValue(b, c, d, a, p2[1], p2[2]);
    p3[0] = planeValue(b, c, d, a, p3[1], p3[2]);
    p4[0] = planeValue(b, c, d, a, p4[1], p4[2]);

  }
  else if (b != 0) {
    p2    = p1;
    p2[2] = p3[2];

    p4    = p3;
    p4[2] = p1[2];

    p1[1] = planeValue(a, c, d, b, p1[0], p1[2]);
    p2[1] = planeValue(a, c, d, b, p2[0], p2[2]);
    p3[1] = planeValue(a, c, d, b, p3[0], p3[2]);
    p4[1] = planeValue(a, c, d, b, p4[0], p4[2]);
  }
  else if (c != 0) {
    // The same for z, a = b = 0 => z = -d/c
    p2    = p1;
    p2[0] = p3[0];

    p4    = p3;
    p4[0] = p1[0];

    p1[2] = planeValue(a, b, d, c, p1[0], p1[1]);
    p2[2] = planeValue(a, b, d, c, p2[0], p2[1]);
    p3[2] = planeValue(a, b, d, c, p3[0], p3[1]);
    p4[2] = planeValue(a, b, d, c, p4[0], p4[1]);
  }

  return true;
}

float Plane::planePointValue(const Coord &pos) {
  return (a * pos[0] + b * pos[1] + c * pos[2] + d);
}
