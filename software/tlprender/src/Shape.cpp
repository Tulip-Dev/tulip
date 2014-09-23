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
#include "Shape.h"

#include <iostream>

using namespace std;

namespace tlprender {

Shape::Shape() : valid(false), avgZ(0.0) {}
Shape::~Shape() {}

bool Shape::isValid() const {
  return (vertices.size() > 2);
}

void Shape::begin() {
  valid=false;
  vertices=vector<Point>();
}

void Shape::add(const int x, const int y, const GLfloat z) {
  add(Point(x,y,z));
}

bool Shape::end() {
  int count=0;
  avgZ=0.0;

  for (vector<Point>::iterator i = vertices.begin(); i!=vertices.end(); ++i) {

    vector<Point>::const_iterator pre=(i==vertices.begin() ? vertices.end() : i);
    vector<Point>::const_iterator next=i;

    --pre;
    ++next;

    if (next==vertices.end()) next=vertices.begin();

    if ((*i==*pre) || (*i==*next)) {
      i=vertices.erase(i);
      --i; //FIXME: if we erase vertices.begin(), returned value (i) would be the new vertices.begin() and we'll decrement it.
    }
    else {
      ++count;
      avgZ += i->z;
    }
  }

  if (count != 0) avgZ /= count;

  valid = (vertices.size()>2);
  return valid;
}

bool Shape::clip(const Shape &sh) {
#if 0 //commented code, consider it broken    

  if (sh.vertices.size() < 3) return false;

  if (vertices.size() == 0) {
    vertices = sh.vertices;
    return true;
  }

  int size = vertices.size();
  Point prev = vertices.back();
  Point next = vertices[1];
  vector<Point>::iterator it=vertices.begin();

  for (int i=0; i<size; prev=vertices[i++], ++it) {
    next = (i==(size-1)) ? vertices[0] : vertices[i+1];

    for (int j=0; i<3; ++i) {
      if (sh.vertices[j] == vertices[i]) {
        Point p = (j==0) ? sh.vertices[2] : sh.vertices[j-1];
        Point n = (j==2) ? sh.vertices[0] : sh.vertices[j+1];

        if (p==next) {
          cerr << __FUNCTION__ << ": insertion:(" << *this << ")";
          vertices.insert(it, n);
          cerr << " apres: (" << *this << ")" << endl;
          return true;
        }
        else if (n==prev) {
          cerr << __FUNCTION__ << ": insertion:(" << *this << ")";
          vertices.insert(it, p);
          cerr << " apres: (" << *this << ")" << endl;
          return true;
        }
        else {
          return false;
        }
      }
    }
  }

#endif //end of commented code
  return false;
}
ostream& operator<<(ostream &os, const Shape &p) {
  for (vector<Point>::const_iterator i=p.vertices.begin(); i != p.vertices.end();) {
    os << i->x << "," << i->y;

    if (++i != p.vertices.end()) os << ",";
  }

  return os;
}

ostream& operator<<(ostream &os, const Point &p) {
  os << "(" << p.x << "," << p.y << ")";
  return os;
}
}

bool less<tlprender::Shape *>::operator() (tlprender::Shape *&p1, tlprender::Shape *&p2) {
  return (*p2 <= *p1);
}

bool less<tlprender::Shape>::operator()(const tlprender::Shape &p1, const tlprender::Shape &p2) {
  return (p2 <= p1);
}

