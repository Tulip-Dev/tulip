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
#include "tulip/DrawingTools.h"
#include "tulip/Graph.h"
#include "tulip/LayoutProperty.h"
#include "tulip/SizeProperty.h"
#include "tulip/DoubleProperty.h"
#include "tulip/BooleanProperty.h"
#include "tulip/ConvexHull.h"
#include "tulip/ForEach.h"

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
        virtual void addPoint(const Coord &) = 0;
    };

    /**
      * Compute all points of a Tulip layout (with size, rotation, edge bends, and node position)
      * @todo take edge size into account
      * @todo create unit test to automatically chack that function
      */
    void computeGraphPoints(const Graph *graph, const LayoutProperty *layout, const SizeProperty *size, const DoubleProperty *rotation,
                            const BooleanProperty *selection, HullBuilder &builder) {
        node itn;
        forEach(itn, graph->getNodes()) {
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

        edge ite;
        forEach(ite, graph->getEdges()) {
            if (!selection || selection->getEdgeValue(ite)) {
                LineType::RealType::const_iterator itCoord;
                const LineType::RealType &bends = layout->getEdgeValue(ite);
                for (itCoord = bends.begin(); itCoord != bends.end(); ++itCoord)
                    builder.addPoint(*itCoord);
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
BoundingBox tlp::computeBoundingBox(const Graph *graph, const LayoutProperty *layout, const SizeProperty *size, const DoubleProperty *rotation,
                                    const BooleanProperty *selection) {
    BoundingBoxCalculator calc;
    computeGraphPoints(graph, layout, size, rotation, selection, calc);
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
    computeGraphPoints(graph, layout, size, rotation, selection, calc);
    return calc.getResult();
}
