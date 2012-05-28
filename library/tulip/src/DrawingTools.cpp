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
  float cosz = static_cast<float>(cos(zRot));
  float sinz = static_cast<float>(sin(zRot));
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
        points[0].set( nSize[0]/2.f,  nSize[1]/2.f,  nSize[2]/2.f);
        points[1].set(-nSize[0]/2.f, -nSize[1]/2.f, -nSize[2]/2.f);
        points[2].set(+nSize[0]/2.f, -nSize[1]/2.f, -nSize[2]/2.f);
        points[3].set(-nSize[0]/2.f, +nSize[1]/2.f, +nSize[2]/2.f);

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

    if (selection == NULL || selection->getNodeValue(itn)) {
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
        radDir *= static_cast<float>(curRad);
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
  vector<Coord> pointsCp(points);
  pointsCp.push_back(points[0]);
  float A = 0;
  float Cx = 0;
  float Cy = 0;

  for (size_t i = 0 ; i < pointsCp.size() - 1 ; ++i) {
    A += (pointsCp[i][0] * pointsCp[i+1][1] - pointsCp[i+1][0] * pointsCp[i][1]);
    Cx += (pointsCp[i][0] + pointsCp[i+1][0])*(pointsCp[i][0]*pointsCp[i+1][1] - pointsCp[i+1][0]*pointsCp[i][1]);
    Cy += (pointsCp[i][1] + pointsCp[i+1][1])*(pointsCp[i][0]*pointsCp[i+1][1] - pointsCp[i+1][0]*pointsCp[i][1]);
  }

  A *= 0.5f;
  Cx *= 1.0f / (6*A);
  Cy *= 1.0f / (6*A);
  return Coord(Cx, Cy);
}
