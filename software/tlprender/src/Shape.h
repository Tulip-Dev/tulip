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
#ifndef SHAPE_H // -*-c++-*-
#define SHAPE_H

#if (__GNUC__ < 3)
#include <ostream.h>
#else
#include <ostream>
#endif

#include <functional>
#include <vector>

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

namespace tlprender {

class Point {
  friend std::ostream& operator<<(std::ostream &,const Point &);
public:
  int x, y;
  GLfloat z;

  inline Point(const int x=0, const int y=0, const GLfloat z=0.0): x(x), y(y), z(z) {}
  ~Point() {}

  inline bool operator ==(const Point &p) const {
    return (x==p.x) && (y==p.y) && (z==p.z);
  }
  inline bool operator !=(const Point &p) const {
    return !(*this==p);
  }
};

class Shape {
  friend std::ostream& operator<<(std::ostream &,const Shape &);
private:
  bool valid;
  std::vector<Point> vertices;
  GLfloat avgZ;
public:
  Shape();
  ~Shape();
  bool isValid() const;
  void begin();
  void add(const int x, const int y, const GLfloat z);
  inline void add(const Point& p) {
    vertices.push_back(p);
  }

  bool end();

  //extend one Shape with another
  bool clip(const Shape& p);

  inline bool operator<=(const Shape &p) const {
    return (avgZ <= p.avgZ);
  }
};

std::ostream& operator<<(std::ostream &os, const Shape &p);
std::ostream& operator<<(std::ostream &os, const Point &p);
} //namespace tlprender

namespace std {
template<>
struct less<tlprender::Shape *> {
  bool operator()(tlprender::Shape *&p1, tlprender::Shape *&p2);
};
template<>
struct less<tlprender::Shape> {
  bool operator()(const tlprender::Shape &p1, const tlprender::Shape &p2);
};
}
#endif //SHAPE_H
