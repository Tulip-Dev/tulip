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

#include <tulip/ConvexHull.h>
#include <tulip/DrawingTools.h>

#include <list>
#include <map>
#include <algorithm>

extern "C" {
#include <libqhull/libqhull.h>
#include <libqhull/qset.h>
#include <libqhull/geom.h>
#include <libqhull/poly.h>
#include <libqhull/io.h>
}

using namespace std;
using namespace tlp;

//================================================================================

static bool runQHull(int dim, vector<double> &points,
                     vector<vector<unsigned int> > &facets,
                     vector<vector<unsigned int> > &neighbors) {

  // Set default options for qhull convex hull
  // - Qt : triangulated output
  string qhullOptions = "Qt";
  // build qhull command
  string qhullCommand = "qhull ";
  qhullCommand += qhullOptions;

  // run qhull convex hull computation
  int qhullKo = qh_new_qhull(dim, points.size()/dim, &points[0], false, const_cast<char *>(qhullCommand.c_str()), NULL, stderr);

  if (!qhullKo) {
    facetT *facet;
    vertexT *vertex, **vertexp;
    // iterate over generated facets
    std::map<unsigned int, unsigned int> faceIds;
    FORALLfacets {
      std::vector<unsigned int> facetV;
      std::vector<unsigned int> neighborsV;
      FOREACHvertex_ (facet->vertices) {
        facetV.push_back(qh_pointid(vertex->point));
      }
      faceIds[getid_(facet)] = facets.size();
      facets.push_back(facetV);
      facetT *neighbor, **neighborp;
      FOREACHneighbor_(facet) {
        neighborsV.push_back(getid_(neighbor));
      }
      neighbors.push_back(neighborsV);
    }

    for (size_t i = 0 ; i < neighbors.size() ; ++i) {
      for (size_t j = 0 ; j < neighbors[i].size() ; ++j) {
        neighbors[i][j] = faceIds[neighbors[i][j]];
      }
    }
  }

  // free memory allocated by qhull
  qh_freeqhull(!qh_ALL);
  int curlong, totlong;
  qh_memfreeshort(&curlong, &totlong);

  return !qhullKo;
}

//================================================================================
void tlp::convexHull(const std::vector<Coord> &points,
                     std::vector<unsigned int> &hull) {
  hull.clear();

  vector<double> pointsQHull;
  vector<vector<unsigned int> > facets;
  vector<vector<unsigned int> > neighbors;

  for(size_t i = 0 ; i < points.size() ; ++i) {
    pointsQHull.push_back(points[i][0]);
    pointsQHull.push_back(points[i][1]);
  }

  // compute the convex hull
  if (runQHull(2, pointsQHull, facets, neighbors)) {

    // compute ordering on points defining the hull polygon
    hull.push_back(facets[0][0]);
    hull.push_back(facets[0][1]);
    unsigned int curFaceId = 0;
    float signedArea = 0;

    while (1) {
      unsigned int ridge = hull.back();
      unsigned int neighFaceAId = neighbors[curFaceId][0];
      unsigned int neighFaceBId = neighbors[curFaceId][1];

      if (facets[neighFaceAId][0] == ridge) {
        hull.push_back(facets[neighFaceAId][1]);
        curFaceId = neighFaceAId;
      }
      else if (facets[neighFaceAId][1] == ridge) {
        hull.push_back(facets[neighFaceAId][0]);
        curFaceId = neighFaceAId;
      }
      else if (facets[neighFaceBId][0] == ridge) {
        hull.push_back(facets[neighFaceBId][1]);
        curFaceId = neighFaceBId;
      }
      else {
        hull.push_back(facets[neighFaceBId][0]);
        curFaceId = neighFaceBId;
      }

      size_t hullSize = hull.size();

      if (hullSize > 1) {
        const Coord &point = points[hull[hullSize-2]];
        const Coord &nextPoint = points[hull[hullSize-1]];
        signedArea += (point[0] * nextPoint[1] - nextPoint[0] * point[1]);
      }

      if (hull.size() == facets.size()) {
        break;
      }
    }

    const Coord &point = points[hull.back()];

    const Coord &nextPoint = points[hull.front()];

    signedArea += (point[0] * nextPoint[1] - nextPoint[0] * point[1]);

    // hull points are in clockwise order, reverse the points vector in that case
    if (signedArea < 0) {
      std::reverse(hull.begin(), hull.end());
    }
  }
}

//================================================================================

void tlp::convexHull(const std::vector<Coord> &points,
                     std::vector<std::vector<unsigned int> > &convexHullFacets,
                     std::vector<std::vector<unsigned int> > &facetNeighbors) {

  convexHullFacets.clear();
  facetNeighbors.clear();
  vector<double> pointsQHull;

  // check if the layout is coplanar
  Mat3f invTransformMatrix;
  bool coPlanarLayout = isLayoutCoPlanar(points, invTransformMatrix);
  int dim = 3;

  if (!coPlanarLayout) {
    for (size_t i = 0 ; i < points.size() ; ++i) {
      pointsQHull.push_back(points[i][0]);
      pointsQHull.push_back(points[i][1]);
      pointsQHull.push_back(points[i][2]);
    }
  }
  else {
    dim = 2;

    for(size_t i = 0 ; i < points.size() ; ++i) {
      Coord p = Coord(invTransformMatrix * points[i]);
      pointsQHull.push_back(p[0]);
      pointsQHull.push_back(p[1]);
    }
  }

  runQHull(dim, pointsQHull, convexHullFacets, facetNeighbors);
}
