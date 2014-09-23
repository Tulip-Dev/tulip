/*
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
///@cond DOXYGEN_HIDDEN

#ifndef TLP_GEO_CIRCLE_H
#define TLP_GEO_CIRCLE_H

#include <vector>
#include <tulip/Vector.h>
namespace tlp {
/**
 * @ingroup Structures
 * \brief class for circle
 *
 * Enables to both create and manipulate a circle
 *
 * \author David Auber auber@tulip-software.org
 * \version 0.0.1 24/01/2003
 */
template<typename Obj, typename OTYPE>
struct Circle : public Vector<Obj,2, OTYPE> {
  Circle() {}
  Circle(const Vector<Obj,2, OTYPE> &pos, Obj radius):Vector<Obj,2, OTYPE>(pos),radius(radius) {}
  Circle(const Circle &c):Vector<Obj,2, OTYPE>(c),radius(c.radius) {}
  Circle(Obj x,Obj y,Obj radius):radius(radius) {
    (*this)[0]=x;
    (*this)[1]=y;
  }
  /**
   * Translate "this" by vector v
   */
  void translate(const Vector<Obj,2, OTYPE> &v) {
    (*this)+=v;
  }
  /**
   * Merges this circle with another circle; merging operation
   * consists in computing the smallest enclosing circle of the
   * two circle and to store the result in "this".
   */
  Circle<Obj, OTYPE>& merge(const Circle<Obj, OTYPE> &c);
  /**
   * Radius of the circle
   */
  Obj radius;
  /**
   * Returns true if the circle is include in an other circle, false otherwise.
   */
  bool isIncludeIn(const Circle<Obj, OTYPE> & circle) const;
};

/**
 * Give the instersction of two circles, return false if no intersection exist else put the two points in p1 & p2,
 * if there is only one solution p1 == p2;
 */
template<typename Obj, typename OTYPE>
bool intersection(const tlp::Circle<Obj, OTYPE> &c1, const tlp::Circle<Obj, OTYPE> &c2,  tlp::Vector<Obj,2, OTYPE> &sol1,  tlp::Vector<Obj,2, OTYPE> &sol2) {
  double d =  c1.dist(c2);
  double r1 = c1.radius;
  double r2 = c2.radius;

  if (c1 == c2) return false;

  if (d > (r1 + r2)) return false; //outside

  if (d < fabs(r1 - r2)) return false; //inside

  double a = ((r1*r1) - (r2*r2) + (d*d)) / (2.0 * d);
  Vec2d c1c2(c2 - c1);
  Vec2d p2(c1 + c1c2 * a/d);

  double h = sqrt((r1*r1) - (a*a));
  double rx = -c1c2[1] * (h/d);
  double ry =  c1c2[0] * (h/d);

  sol1[0] = p2[0] + rx;
  sol1[1] = p2[1] + ry;

  sol2[0] = p2[0] - rx;
  sol2[1] = p2[1] - ry;

  return true;
}

/**
 * Compute the optimum enclosing circle of 2 circles.
 */
template<typename Obj, typename OTYPE>
tlp::Circle<Obj, OTYPE> enclosingCircle(const tlp::Circle<Obj, OTYPE> &,const tlp::Circle<Obj, OTYPE> &);

/**
 * Compute the optimum enclosing circle of a set of circles.
 */
template<typename Obj, typename OTYPE>
tlp::Circle<Obj, OTYPE> enclosingCircle(const std::vector< tlp::Circle<Obj, OTYPE> > & circles);
/**
 * Compute an enclosing circle of a set of circles,
 * this algorithm is an aproximation of the smallest
 * enclosing circle.
 */
template<typename Obj, typename OTYPE>
tlp::Circle<Obj, OTYPE> lazyEnclosingCircle(const std::vector< tlp::Circle<Obj, OTYPE> > & circles);
/**
 * Write circle in a stream
 */
template<typename Obj, typename OTYPE>
std::ostream& operator<<(std::ostream &os,const tlp::Circle<Obj, OTYPE> &);


typedef Circle<double, long double> Circled;
typedef Circle<float,  double> Circlef;
typedef Circle<int,  double> Circlei;

}

#include "cxx/Circle.cxx"
#endif
///@endcond
