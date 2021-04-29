/**
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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
#include <tulip/DrawingTools.h>
#include <tulip/Graph.h>
#include <tulip/LayoutProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/BooleanProperty.h>
#include <tulip/ConvexHull.h>

#include <climits>

using namespace std;
using namespace tlp;

static void rotate(Coord &vec, double alpha) {
  Coord backupVec(vec);
  double zRot = -2.0 * M_PI * alpha / 360.0;
  float cosz = cos(zRot);
  float sinz = sin(zRot);
  vec[0] = backupVec[0] * cosz - backupVec[1] * sinz;
  vec[1] = backupVec[0] * sinz + backupVec[1] * cosz;
}

/**
 * Compute all points of a Tulip layout (with size, rotation, edge bends, and node position)
 * @todo take edge size into account
 * @todo create unit test to automatically check that function
 */
static void computeGraphPoints(const std::vector<node> &nodes, const std::vector<edge> &edges,
                               const LayoutProperty *layout, const SizeProperty *size,
                               const DoubleProperty *rotation, const BooleanProperty *selection,
                               std::vector<Coord> &gPoints) {
  for (auto n : nodes) {
    if ((selection == nullptr) || selection->getNodeValue(n)) {
      Size nSize(size->getNodeValue(n));
      Coord point(layout->getNodeValue(n));
      double rot(rotation->getNodeValue(n));
      vector<Coord> points(4);
      points[0].set(nSize[0] / 2., nSize[1] / 2., nSize[2] / 2.);
      points[1].set(-nSize[0] / 2., -nSize[1] / 2., -nSize[2] / 2.);
      points[2].set(+nSize[0] / 2., -nSize[1] / 2., -nSize[2] / 2.);
      points[3].set(-nSize[0] / 2., +nSize[1] / 2., +nSize[2] / 2.);

      for (unsigned int i = 0; i < 4; ++i) {
        if (rot)
          rotate(points[i], rot);

        gPoints.emplace_back(points[i] + point);
      }
    }
  }

  if (layout->hasNonDefaultValuatedEdges()) {
    for (auto e : edges) {
      if ((selection == nullptr) || selection->getEdgeValue(e)) {
        for (const Coord &coord : layout->getEdgeValue(e))
          gPoints.emplace_back(coord);
      }
    }
  }
}

//===========================================================================
BoundingBox tlp::computeBoundingBox(const Graph *graph, const LayoutProperty *layout,
                                    const SizeProperty *size, const DoubleProperty *rotation,
                                    const BooleanProperty *selection) {
  return computeBoundingBox(graph->nodes(), graph->edges(), layout, size, rotation, selection);
}
//===========================================================================
BoundingBox tlp::computeBoundingBox(const std::vector<node> &nodes, const std::vector<edge> &edges,
                                    const LayoutProperty *layout, const SizeProperty *size,
                                    const DoubleProperty *rotation,
                                    const BooleanProperty *selection) {
  std::vector<Coord> gPoints;
  computeGraphPoints(nodes, edges, layout, size, rotation, selection, gPoints);
  BoundingBox bbox;
  for (const Coord &point : gPoints)
    bbox.expand(point);
  return bbox;
}
//===========================================================================
pair<Coord, Coord> tlp::computeBoundingRadius(const Graph *graph, const LayoutProperty *layout,
                                              const SizeProperty *size,
                                              const DoubleProperty *rotation,
                                              const BooleanProperty *selection) {
  pair<Coord, Coord> result;

  if (graph->isEmpty())
    return result;

  BoundingBox boundingBox(tlp::computeBoundingBox(graph, layout, size, rotation, selection));
  Coord centre(boundingBox.center());
  result.first = result.second = centre;

  double maxRad = 0;
  for (auto n : graph->nodes()) {
    const Coord &curCoord = layout->getNodeValue(n);
    Size curSize(size->getNodeValue(n) / 2.0f);

    if (selection == nullptr || selection->getNodeValue(n)) {
      double nodeRad = sqrt(curSize.getW() * curSize.getW() + curSize.getH() * curSize.getH());
      Coord radDir(curCoord - centre);
      double curRad = nodeRad + radDir.norm();

      if (radDir.norm() < 1e-6) {
        curRad = nodeRad;
        radDir = Coord(1.0, 0.0, 0.0);
      }

      if (curRad > maxRad) {
        maxRad = curRad;
        radDir /= radDir.norm();
        radDir *= curRad;
        result.second = radDir + centre;
      }
    }
  }

  if (layout->hasNonDefaultValuatedEdges()) {
    for (auto e : graph->edges()) {
      if (selection == nullptr || selection->getEdgeValue(e)) {
        for (const auto &coord : layout->getEdgeValue(e)) {
          double curRad = (coord - centre).norm();

          if (curRad > maxRad) {
            maxRad = curRad;
            result.second = coord;
          }
        }
      }
    }
  }

  return result;
}
//======================================================================================
vector<Coord> tlp::computeConvexHull(const std::vector<Coord> &allPoints) {
  vector<unsigned int> hullIndices;
  convexHull(allPoints, hullIndices); // compute the convex hull
  vector<unsigned int>::const_iterator it;
  vector<Coord> finalResult(hullIndices.size());
  vector<Coord>::iterator itR = finalResult.begin();

  for (it = hullIndices.begin(); it != hullIndices.end(); ++it, ++itR) {
    (*itR) = allPoints[*it];
    (*itR)[2] = 0;
  }

  return finalResult;
}
//======================================================================================

std::vector<Coord> tlp::computeConvexHull(const Graph *graph, const LayoutProperty *layout,
                                          const SizeProperty *size, const DoubleProperty *rotation,
                                          const BooleanProperty *selection) {

  std::vector<Coord> gPoints;
  computeGraphPoints(graph->nodes(), graph->edges(), layout, size, rotation, selection, gPoints);
  return computeConvexHull(gPoints);
}

//======================================================================================

// implementation based on http://mathworld.wolfram.com/Line-LineIntersection.html
// reference : Hill, F. S. Jr. "The Pleasures of 'Perp Dot' Products." Ch. II.5 in Graphics Gems IV
// (Ed. P. S. Heckbert). San Diego: Academic Press, pp. 138-148, 1994.
bool tlp::computeLinesIntersection(const std::pair<tlp::Coord, tlp::Coord> &line1,
                                   const std::pair<tlp::Coord, tlp::Coord> &line2,
                                   tlp::Coord &intersectionPoint) {

  Coord a(line1.second - line1.first);
  Coord b(line2.second - line2.first);
  Coord axb(a ^ b);
  float axbnorm = axb.norm();

  // lines are parallel, no intersection
  if (axbnorm == 0)
    return false;

  Coord c(line2.first - line1.first);
  // skew lines, no intersection
  if (c.dotProduct(axb) != 0)
    return false;

  // lines intersects, compute the point
  float s = (c ^ b).dotProduct(axb) / (axbnorm * axbnorm);
  intersectionPoint = line1.first + a * s;

  return true;
}

//======================================================================================================

Coord tlp::computePolygonCentroid(const vector<Coord> &points) {
  vector<Vec3d> pointsCp;
  pointsCp.reserve(points.size() + 1);

  for (size_t i = 0; i < points.size(); ++i) {
    pointsCp.emplace_back(points[i][0], points[i][1], 0.0);
  }

  pointsCp.emplace_back(points[0][0], points[0][1], 0.0);
  double A = 0.0;
  double Cx = 0.0;
  double Cy = 0.0;

  for (size_t i = 0; i < pointsCp.size() - 1; ++i) {
    A += (pointsCp[i][0] * pointsCp[i + 1][1] - pointsCp[i + 1][0] * pointsCp[i][1]);
    Cx += (pointsCp[i][0] + pointsCp[i + 1][0]) *
          (pointsCp[i][0] * pointsCp[i + 1][1] - pointsCp[i + 1][0] * pointsCp[i][1]);
    Cy += (pointsCp[i][1] + pointsCp[i + 1][1]) *
          (pointsCp[i][0] * pointsCp[i + 1][1] - pointsCp[i + 1][0] * pointsCp[i][1]);
  }

  A *= 0.5;
  Cx *= 1.0 / (6.0 * A);
  Cy *= 1.0 / (6.0 * A);
  return Coord(float(Cx), float(Cy));
}

//======================================================================================================

static inline void normalize(Vec3f &v) {
  if (v.norm() != 0)
    v /= v.norm();
}

//======================================================================================================

bool tlp::isLayoutCoPlanar(const vector<Coord> &points, Mat3f &invTransformMatrix) {
  Coord A(points[0]), B(0), C(0);
  bool BSet = false;

  // pick three points to define a plane
  for (size_t i = 1; i < points.size(); ++i) {
    if (!BSet && points[i] != A) {
      B = points[i];
      BSet = true;
    } else if (BSet) {
      // pick a third point non aligned with the two others
      C = points[i];

      if (((C - A) ^ (B - A)).norm() > 1e-3) {
        break;
      }
    }
  }

  Coord a(B - A);
  Coord b(C - A);
  normalize(a);
  normalize(b);
  Coord c(a ^ b);
  normalize(c);
  b = c ^ a;
  normalize(b);

  // compute the distance of each point to the plane
  for (const Coord &D : points) {
    // if the point is too far from the plane, the layout is not coplanar
    if (abs(c.dotProduct(D - A)) > 1e-3) {
      return false;
    }
  }

  // compute the inverse transform matrix for projecting the points in the z = 0 plane
  invTransformMatrix[0][0] = a[0];
  invTransformMatrix[1][0] = a[1];
  invTransformMatrix[2][0] = a[2];
  invTransformMatrix[0][1] = b[0];
  invTransformMatrix[1][1] = b[1];
  invTransformMatrix[2][1] = b[2];
  invTransformMatrix[0][2] = c[0];
  invTransformMatrix[1][2] = c[1];
  invTransformMatrix[2][2] = c[2];
  invTransformMatrix.inverse();

  return true;
}

//======================================================================================================

std::vector<tlp::Coord> tlp::computeRegularPolygon(unsigned int numberOfSides,
                                                   const tlp::Coord &center, const tlp::Size &size,
                                                   float startAngle) {

  assert(numberOfSides > 2);

  BoundingBox box;
  vector<Coord> points;
  float delta = (2.0f * M_PI) / float(numberOfSides);

  for (unsigned int i = 0; i < numberOfSides; ++i) {
    float deltaX = cos(i * delta + startAngle);
    float deltaY = sin(i * delta + startAngle);
    points.emplace_back(deltaX, deltaY, center[2]);
    box.expand(points.back());
  }

  for (auto &point : points) {
    point.set(
        center[0] + ((point[0] - ((box[1][0] + box[0][0]) / 2.)) / ((box[1][0] - box[0][0]) / 2.)) *
                        size[0],
        center[1] + ((point[1] - ((box[1][1] + box[0][1]) / 2.)) / ((box[1][1] - box[0][1]) / 2.)) *
                        size[1]);
  }

  return points;
}
