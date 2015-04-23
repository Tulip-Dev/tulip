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
#include <tulip/DrawingTools.h>
#include <tulip/Graph.h>
#include <tulip/LayoutProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/BooleanProperty.h>
#include <tulip/ConvexHull.h>
#include <tulip/ForEach.h>

#include <climits>

using namespace std;
using namespace tlp;
namespace {

void rotate(Coord &vec, double alpha) {
  Coord backupVec(vec);
  double zRot = -2.0 * M_PI * alpha / 360.0;
  float cosz = cos(zRot);
  float sinz = sin(zRot);
  vec[0] = backupVec[0] * cosz - backupVec[1] * sinz;
  vec[1] = backupVec[0] * sinz + backupVec[1] * cosz;
}

class HullBuilder {
public:
  virtual ~HullBuilder() {}
  virtual void addPoint(const Coord &) = 0;
};

/**
 * Compute all points of a Tulip layout (with size, rotation, edge bends, and node position)
 * @todo take edge size into account
 * @todo create unit test to automatically chack that function
 */
void computeGraphPoints(Iterator<node> *itN, Iterator<edge> *itE, const LayoutProperty *layout, const SizeProperty *size, const DoubleProperty *rotation,
                        const BooleanProperty *selection, HullBuilder &builder) {

  if (itN != NULL) {

    node itn;
    forEach(itn, itN) {
      if (!selection || selection->getNodeValue(itn)) {
        Vec3f nSize(size->getNodeValue(itn));
        Vec3f point(layout->getNodeValue(itn));
        double rot(rotation->getNodeValue(itn));
        vector<Coord> points(4);
        points[0].set( nSize[0]/2.,  nSize[1]/2.,  nSize[2]/2.);
        points[1].set(-nSize[0]/2., -nSize[1]/2., -nSize[2]/2.);
        points[2].set(+nSize[0]/2., -nSize[1]/2., -nSize[2]/2.);
        points[3].set(-nSize[0]/2., +nSize[1]/2., +nSize[2]/2.);

        for (unsigned int i = 0; i < 4; ++i) {
          rotate(points[i], rot);
          points[i] += point;
          builder.addPoint(points[i]);
        }
      }
    }

  }

  if (itE != NULL) {

    edge ite;
    forEach(ite, itE) {
      if (!selection || selection->getEdgeValue(ite)) {
        LineType::RealType::const_iterator itCoord;
        const LineType::RealType &bends = layout->getEdgeValue(ite);

        for (itCoord = bends.begin(); itCoord != bends.end(); ++itCoord)
          builder.addPoint(*itCoord);
      }
    }

  }
}
}
//===========================================================================
class BoundingBoxCalculator: public HullBuilder {
public:
  inline virtual void addPoint(const Coord &point) {
    result.expand(point);
  }
  BoundingBox getResult() {
    return result;
  }
private:
  BoundingBox result;
};
//===========================================================================
BoundingBox tlp::computeBoundingBox(const Graph *graph, const LayoutProperty *layout, const SizeProperty *size, const DoubleProperty *rotation, const BooleanProperty *selection) {
  BoundingBoxCalculator calc;
  computeGraphPoints(graph->getNodes(), graph->getEdges(), layout, size, rotation, selection, calc);
  return calc.getResult();
}
//===========================================================================
BoundingBox tlp::computeBoundingBox(Iterator<node> *itN, Iterator<edge> *itE, const LayoutProperty *layout, const SizeProperty *size, const DoubleProperty *rotation, const BooleanProperty *selection) {
  BoundingBoxCalculator calc;
  computeGraphPoints(itN, itE, layout, size, rotation, selection, calc);
  return calc.getResult();
}
//===========================================================================
pair<Coord, Coord> tlp::computeBoundingRadius(const Graph *graph, const LayoutProperty *layout, const SizeProperty *size,
    const DoubleProperty *rotation, const BooleanProperty *selection) {
  pair<Coord, Coord> result;
  result.first.set(0, 0, 0);
  result.second.set(0, 0, 0);

  if (graph->numberOfNodes() == 0)
    return result;

  BoundingBox boundingBox(tlp::computeBoundingBox(graph, layout, size, rotation, selection));
  Coord centre(boundingBox.center());
  result.first = result.second = centre;

  double maxRad = 0;
  node itn;
  forEach(itn, graph->getNodes()) {
    const Coord& curCoord = layout->getNodeValue(itn);
    Size curSize(size->getNodeValue(itn) / 2.0f);

    if (selection == 0 || selection->getNodeValue(itn)) {
      double nodeRad = sqrt(curSize.getW() * curSize.getW() + curSize.getH() * curSize.getH());
      Coord  radDir = curCoord - centre;
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
    }//end if
  }

  edge ite;
  forEach(ite, graph->getEdges()) {
    if (selection == 0 || selection->getEdgeValue(ite)) {
      LineType::RealType::const_iterator itCoord;
      const LineType::RealType &bends = layout->getEdgeValue(ite);

      for (itCoord = bends.begin(); itCoord != bends.end(); ++itCoord) {
        double curRad = (*itCoord - centre).norm();

        if (curRad > maxRad) {
          maxRad = curRad;
          result.second = *itCoord;
        }//end if
      }//end for
    }//end if
  }

  return result;
}
//======================================================================================
class ConvexHullCalculator: public HullBuilder {
public:

  inline virtual void addPoint(const Coord &point) {
    allPoints.push_back(point);
  }

  vector<Coord> getResult() {
    vector<unsigned int> hullIndices;
    convexHull(allPoints, hullIndices); //compute the convex hull
    vector<unsigned int>::const_iterator it;
    vector<Coord> finalResult(hullIndices.size());
    vector<Coord>::iterator itR = finalResult.begin();

    for(it = hullIndices.begin(); it != hullIndices.end(); ++it, ++itR) {
      (*itR) = allPoints[*it];
      (*itR)[2] = 0;
    }

    return finalResult;
  }

private:
  vector<Coord> allPoints;
};
//======================================================================================

std::vector<Coord> tlp::computeConvexHull(const Graph *graph, const LayoutProperty *layout, const SizeProperty *size, const DoubleProperty *rotation,
    const BooleanProperty *selection) {

  ConvexHullCalculator calc;
  computeGraphPoints(graph->getNodes(), graph->getEdges(), layout, size, rotation, selection, calc);
  return calc.getResult();
}

//======================================================================================

std::vector<Coord> tlp::computeConvexHull(const std::vector<tlp::Coord> &points) {
  ConvexHullCalculator calc;

  for (size_t i = 0 ; i < points.size() ; ++i) {
    calc.addPoint(points[i]);
  }

  return calc.getResult();
}

//======================================================================================

// implementation based on http://mathworld.wolfram.com/Line-LineIntersection.html
// reference : Hill, F. S. Jr. "The Pleasures of 'Perp Dot' Products." Ch. II.5 in Graphics Gems IV (Ed. P. S. Heckbert). San Diego: Academic Press, pp. 138-148, 1994.
bool tlp::computeLinesIntersection(const std::pair<tlp::Coord, tlp::Coord> &line1,
                                   const std::pair<tlp::Coord, tlp::Coord> &line2,
                                   tlp::Coord &intersectionPoint) {

  Coord a = line1.second - line1.first;
  Coord b = line2.second - line2.first;
  Coord c = line2.first - line1.first;
  Coord axb = a^b;
  float axbnorm = axb.norm();

  // lines are parallel, no intersection
  if (axbnorm == 0)
    return false;

  // skew lines, no intersection
  if (c.dotProduct(axb) != 0)
    return false;

  // lines intersects, compute the point
  float s = (c^b).dotProduct(axb) / (axbnorm*axbnorm);
  intersectionPoint = line1.first + a * s;

  return true;

}

//======================================================================================================

Coord tlp::computePolygonCentroid(const vector<Coord> &points) {
  vector<Vec3d> pointsCp;

  for (size_t i = 0 ; i < points.size() ; ++i) {
    pointsCp.push_back(Vec3d(points[i][0], points[i][1], 0.0));
  }

  pointsCp.push_back(Vec3d(points[0][0], points[0][1], 0.0));
  double A = 0.0;
  double Cx = 0.0;
  double Cy = 0.0;

  for (size_t i = 0 ; i < pointsCp.size() - 1 ; ++i) {
    A += (pointsCp[i][0] * pointsCp[i+1][1] - pointsCp[i+1][0] * pointsCp[i][1]);
    Cx += (pointsCp[i][0] + pointsCp[i+1][0])*(pointsCp[i][0]*pointsCp[i+1][1] - pointsCp[i+1][0]*pointsCp[i][1]);
    Cy += (pointsCp[i][1] + pointsCp[i+1][1])*(pointsCp[i][0]*pointsCp[i+1][1] - pointsCp[i+1][0]*pointsCp[i][1]);
  }

  A *= 0.5;
  Cx *= 1.0 / (6.0*A);
  Cy *= 1.0 / (6.0*A);
  return Coord(static_cast<float>(Cx), static_cast<float>(Cy));
}

//======================================================================================================

static void normalize(Vec3f &v) {
  if (v.norm() != 0)
    v /= v.norm();
}

//======================================================================================================

bool tlp::isLayoutCoPlanar(const vector<Coord> &points, Mat3f &invTransformMatrix) {
  Coord A, B, C;
  bool BSet = false;

  // pick three points to define a plane
  for (size_t i = 0 ; i < points.size() ; ++i) {
    if (i == 0) {
      A = points[i];
    }
    else if (!BSet && points[i] != A) {
      B = points[i];
      BSet = true;
    }
    else {
      // pick a third point non aligned with the two others
      C = points[i];

      if (((C-A)^(B-A)).norm() > 1e-3) {
        break;
      }
    }

    ++i;
  }

  Coord a = B -A;
  Coord b = C -A;
  normalize(a);
  normalize(b);
  Coord c = a^b;
  normalize(c);
  b = c^a;
  normalize(b);

  // compute the distance of each point to the plane
  for (size_t i = 0 ; i < points.size() ; ++i) {
    const Coord &D = points[i];

    // if the point is too far from the plane, the layout is not coplanar
    if (abs(c.dotProduct(D-A)) > 1e-3) {
      return false;
    }
  }

  // compute the inverse transform matrix for projecting the points in the z = 0 plane
  invTransformMatrix[0][0]=a[0];
  invTransformMatrix[1][0]=a[1];
  invTransformMatrix[2][0]=a[2];
  invTransformMatrix[0][1]=b[0];
  invTransformMatrix[1][1]=b[1];
  invTransformMatrix[2][1]=b[2];
  invTransformMatrix[0][2]=c[0];
  invTransformMatrix[1][2]=c[1];
  invTransformMatrix[2][2]=c[2];
  invTransformMatrix.inverse();

  return true;
}
