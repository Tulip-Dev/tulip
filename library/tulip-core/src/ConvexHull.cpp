/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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
#include <algorithm>
#include <cfloat>
#include <tulip/ConvexHull.h>

using namespace std;
using namespace tlp;

//==============================================================
//A private struct for vectors about a point used in convex hull computation
#ifndef DOXYGEN_NOTFOR_DEVEL
struct p0Vectors {
  Vec2d pos;
  unsigned int index;
};
#endif // DOXYGEN_NOTFOR_DEVEL


bool operator<(const p0Vectors &p1, const p0Vectors &p2) {
    Vec2d a(p1.pos);
    Vec2d b(p2.pos);
    a.normalize();
    b.normalize();
    double cosa = a.x();
    double cosb = b.x();
    double sina = a.y();
    double sinb = b.y();
    double deltacos = fabs(a.x() - b.x());
    double deltasin = fabs(a.y() - b.y());
    // aligned points so check norm of vectors
    if (deltacos < 1E-10 && deltasin < 1E-10) {
            return  (p1.pos.x()*p1.pos.x()) + (p1.pos.y() *p1.pos.y())
                    >
                    (p2.pos.x()*p2.pos.x()) + (p2.pos.y() *p2.pos.y());
    }
    if (sina > 0. && sinb < 0.) return true;
    if (sina < 0. && sinb > 0.) return false;
    if (sina > 0. && sinb > 0.)
        return cosa > cosb;
    else
        return cosa < cosb;
}
//==============================================================
//A function that returns a point inside the convex hull
//(the point of minimum y-coordinate).  The index returned is
//an index of the vector points.
inline Vec2d findP0(const std::vector<Coord> &points,
                    Vec2d &l, Vec2d &r, Vec2d &t, Vec2d &b) {
    Vec2d pmin(points[0].x(), points[0].y());
    Vec2d pmax(points[0].x(), points[0].y());
    l.set(points[0].x(), points[0].y());
    r.set(points[0].x(), points[0].y());
    t.set(points[0].x(), points[0].y());
    b.set(points[0].x(), points[0].y());

    for (unsigned int i = 1; i < points.size(); ++i) {
        if (points[i].x() > pmax.x()) {
            pmax.x() = points[i].x();
            r.set(points[i].x(), points[i].y());
        }
        if (points[i].y() > pmax.y()) {
            pmax.y() = points[i].y();
            t.set(points[i].x(), points[i].y());
        }
        if (points[i].x() < pmin.x()) {
            pmin.x() = points[i].x();
            l.set(points[i].x(), points[i].y());
        }
        if (points[i].y() < pmin.y()) {
            pmin.y() = points[i].y();
            b.set(points[i].x(), points[i].y());
        }
    }
    return (pmin + pmax)/2.;
}//end findP0

double ccw(const Vec2d &p1, const Vec2d &p2, const Vec2d &p3) {
    double d1 = p1.dist(p2);
    double d2 = p2.dist(p3);
    double d3 = p1.dist(p3);
    if (fabs(d1 + d2 - d3) < 1E-5) return 0.; //aligned
    //cerr << p1 << " | " << p2 << " | " << p3 << endl;
    Vec2d a((double)p2.x() - (double)p1.x(), (double)p2.y() - (double)p1.y());
    a.normalize();
    //cerr << a << " ^ ";
    Vec2d b((double)p3.x() - (double)p1.x(), (double)p3.y() - (double)p1.y());
    b.normalize();
    //cerr << b << " = ";
    //cerr << a.x()*b.y() - a.y()*b.x() << endl;
    if (a.dotProduct(b) < -0.9999) return -1;
    return a.x()*b.y() - a.y()*b.x();
}
bool inside(const Vec2d &p,
            Vec2d &l, Vec2d &r, Vec2d &t, Vec2d &b) {
    if (ccw(l, p, b) <= 0.) return false;
    if (ccw(b, p, r) <= 0.) return false;
    if (ccw(r, p, t) <= 0.) return false;
    if (ccw(t, p, l) <= 0.) return false;
    return true;
}
//==============================================================
void tlp::convexHull (const std::vector<Coord> &points,
                      std::vector<unsigned int> &convexHull) {

  //clear the input vector
  convexHull.clear();
  //There is no meaning to compute the convex hull of less than three points;
  assert(points.size() > 2);

  //get the starting point of the convex hull computation
  Vec2d lP, rP, tP, bP;
  Vec2d p0 = findP0(points, lP, rP, tP, bP);
  //translate all points to have p0 in (0, 0);
  list<p0Vectors> vectors;
  for (unsigned int i = 0; i < points.size(); ++i) {
    p0Vectors curPoint;
    curPoint.pos.x() = (double)points[i].x();
    curPoint.pos.y() = (double)points[i].y();
    if (inside(curPoint.pos, lP, rP, tP, bP)) continue;
    curPoint.pos -= p0;
    if (curPoint.pos.length() < 1E-5) continue;
    curPoint.index = i;
    vectors.push_back(curPoint);
  }//end for

  vectors.sort();
  bool stopG = false;
  while (!stopG) {
      stopG = true;
      list<p0Vectors>::iterator it = vectors.begin();
      while(it != vectors.end()) {
          p0Vectors a,b,c;
          if (it == vectors.begin())
              a = vectors.back();
          else {
              list<p0Vectors>::iterator it2 = it;
              --it2;
              a = *it2;
          }
          b = *it;
          list<p0Vectors>::iterator it3 = it;
          ++it3;
          if (it3 == vectors.end())
              c = vectors.front();
          else {
              list<p0Vectors>::iterator it2 = it;
              ++it2;
              c = *it2;
          }
          if (ccw(a.pos, b.pos, c.pos) < 0.) {
              list<p0Vectors>::iterator it2 = it;
              if (it != vectors.begin())
                  --it2;
              else {
                  ++it2;
              }
              stopG = false;
              vectors.erase(it);
              it = it2;
          }
          else {
              ++it;
          }
      }
  }

  list<p0Vectors>::iterator it = vectors.begin();
  for (; it != vectors.end(); ++it)
      convexHull.push_back((*it).index);
/*
  for (int i=1; i<vectors.size(); ++i) {
      int m = convexHull.size() - 1;
      while(m > 1 && (ccw(points[convexHull[m-1]], points[convexHull[m]], points[vectors[i].index]) <= 1E-2)) {
      }
      convexHull.push_back(vectors[i].index);
  }
*/
}//end ConvexHull
