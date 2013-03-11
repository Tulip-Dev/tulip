/*
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

#include <tulip/ConvexHull.h>

#include <list>

using namespace std;
using namespace tlp;

//================================================================================
static double distanceToSeg(const tlp::Coord& p,
                            const std::pair<tlp::Coord, tlp::Coord>& seg) {
  double segx = seg.second.getX()-seg.first.getX();
  double segy = seg.second.getY()-seg.first.getY();
  return segx*(seg.first.getY()-p.getY())-segy*(seg.first.getX()-p.getX());
}
//================================================================================
static bool findMostDistantPointFromBaseLine(const vector<Coord>& points,
    const std::pair<unsigned int, unsigned int>& baseLine,
    const std::list<unsigned int> &subset,
    unsigned int &mostDistantPointIdx,
    list<unsigned int>& newSubset) {
  bool pointsOutside = false;
  double maxD = 0;

  for (std::list<unsigned int>::const_iterator it = subset.begin() ; it != subset.end() ; ++it) {
    double d = distanceToSeg(points[*it], make_pair(points[baseLine.first], points[baseLine.second]));

    if (d > 0) {
      newSubset.push_back(*it);
    }
    else {
      continue;
    }

    if (d > maxD) {
      maxD = d;
      mostDistantPointIdx = *it;
      pointsOutside = true;
    }
  }

  return pointsOutside;
}
//================================================================================
static void buildConvexHull(const std::vector<Coord> &points,
                            const std::pair<unsigned int, unsigned int>& baseLine,
                            const std::list<unsigned int> &subset,
                            std::vector<unsigned int> &hull) {
  unsigned int mostDistantPointIdx;
  std::list<unsigned int> newSubset;
  // compute the most distant point from the base line if any and the subset of points on the left of the base line
  bool pointsOutside = findMostDistantPointFromBaseLine(points, baseLine, subset, mostDistantPointIdx, newSubset);

  // case when there is points on the left of the base line
  // this function is then called recusively for two new base lines
  if (pointsOutside) {
    buildConvexHull(points, make_pair(baseLine.first, mostDistantPointIdx), newSubset, hull);
    buildConvexHull(points, make_pair(mostDistantPointIdx, baseLine.second), newSubset, hull);
  }
  // no points on the left of the base line
  // the base line is then part of the convex hull
  else {
    if (hull.empty())
      hull.push_back(baseLine.first);

    if (baseLine.second != hull.front())
      hull.push_back(baseLine.second);
  }
}
//================================================================================
void tlp::convexHull(const std::vector<Coord> &points,
                     std::vector<unsigned int> &hull) {
  //clear hull
  hull.clear();

  //dummy test
  if(points.size()<=3) {
    for(unsigned int i=0; i<points.size(); ++i)
      hull.push_back(i);

    return;
  }

  //get first base line
  Coord pMinX = points[0];
  Coord pMaxX = pMinX;
  unsigned int iMinX = 0;
  unsigned int iMaxX = 0;

  for(unsigned int i=0; i<points.size(); ++i) {
    if(points[i].getX()<pMinX.getX())
      pMinX=points[iMinX = i];
    else if(points[i].getX()>pMaxX.getX())
      pMaxX=points[iMaxX = i];
  }

  // all points need to be considered for the first iterations
  std::list<unsigned int> idxs;

  for (unsigned int i = 0 ; i < points.size() ; ++i) {
    idxs.push_back(i);
  }

  buildConvexHull(points, make_pair(iMinX, iMaxX), idxs, hull);
  buildConvexHull(points, make_pair(iMaxX, iMinX), idxs, hull);
}
//================================================================================
