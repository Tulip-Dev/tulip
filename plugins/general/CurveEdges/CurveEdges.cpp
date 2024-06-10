/**
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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

// Original quadratic curves computation code can be found
// in the Vis.js project (http://visjs.org/).
//
// Vis.js
// Copyright 2010-2015 Almende B.V.
// Vis.js is dual licensed under both
//  * The Apache 2.0 License
//    http://www.apache.org/licenses/LICENSE-2.0
//
//    and
//  * The MIT License
//    http://opensource.org/licenses/MIT
//
// Code was ported from Javascript to C++.

#include <tulip/Algorithm.h>
#include <tulip/StringCollection.h>
#include <tulip/LayoutProperty.h>
#include <tulip/IntegerProperty.h>
#include <tulip/TulipViewSettings.h>

#include <climits>

#define CURVE_TYPE_LIST                                                                            \
  "quadratic continuous;quadratic discrete;quadratic diagonal cross;\
quadratic straight cross;quadratic horizontal;quadratic vertical;cubic continuous;cubic vertical;\
cubic diagonal cross;cubic vertical diagonal cross;cubic straight cross source;cubic straight cross target"
#define QUADRATIC_CONTINUOUS 0
#define QUADRATIC_DISCRETE 1
#define QUADRATIC_DIAGONALCROSS 2
#define QUADRATIC_STRAIGHTCROSS 3
#define QUADRATIC_HORIZONTAL 4
#define QUADRATIC_VERTICAL 5
#define CUBIC_CONTINUOUS 6
#define CUBIC_VERTICAL 7
#define CUBIC_DIAGONALCROSS 8
#define CUBIC_VERTICAL_DIAGONALCROSS 9
#define CUBIC_STRAIGHTCROSS_SOURCE 10
#define CUBIC_STRAIGHTCROSS_TARGET 11

static const char *paramHelp[] = {
    // initial layout
    "The input layout of the graph.",

    // curve roundness
    "Parameter for tweaking the curve roundness. The value range is from 0 to 1 with a maximum roundness at 0.5.",

    // curve type
    "The type of curve to compute (12 available: 6 quadratics and 6 cubics).",

    // bezier edges
    "If activated, set all edge shapes to Bezier curves."};

static const char *curveTypeValues = "quadratic continuous<br/>"
                                     "quadratic discrete<br/>"
                                     "quadratic diagonal cross<br/>"
                                     "quadratic straight cross<br/>"
                                     "quadratic horizontal<br/>"
                                     "quadratic vertical<br/>"
                                     "cubic continuous<br/>"
                                     "cubic vertical<br/>"
                                     "cubic diagonal cross<br/>"
                                     "cubic vertical diagonal cross<br/>"
                                     "cubic straight cross source<br/>"
                                     "cubic straight cross target";

class CurveEdges : public tlp::Algorithm {

public:
  PLUGININFORMATION("Curve edges", "Antoine Lambert", "16/01/2015",
                    "Computes quadratic or cubic bezier paths for edges", "1.1", "")

  CurveEdges(tlp::PluginContext *context)
      : tlp::Algorithm(context), curveType(0), curveRoundness(0.5), layout(nullptr),
        bezierEdges(true) {
    addInParameter<tlp::LayoutProperty>("initial layout", paramHelp[0], "viewLayout");
    addInParameter<float>("curve roundness", paramHelp[1], "0.5");
    addInParameter<tlp::StringCollection>("curve type", paramHelp[2], CURVE_TYPE_LIST, true,
                                          curveTypeValues);
    addInParameter<bool>("bezier edges", paramHelp[3], "true");
  }

  std::vector<tlp::Coord> computeCubicBezierControlPoints(tlp::edge e) {
    auto eEnds = graph->ends(e);
    const tlp::Coord &srcCoord = layout->getNodeValue(eEnds.first);
    const tlp::Coord &tgtCoord = layout->getNodeValue(eEnds.second);
    tlp::Coord dir = tgtCoord - srcCoord;
    dir /= dir.norm();
    float length = srcCoord.dist(tgtCoord);
    float factor = curveRoundness * length;
    tlp::Coord normal(dir[1], -dir[0]);
    normal *= factor;

    if (curveType == CUBIC_VERTICAL || curveType == CUBIC_VERTICAL_DIAGONALCROSS ||
        curveType == CUBIC_STRAIGHTCROSS_SOURCE) {
      dir = tlp::Coord(0, 0, 0);
    }

    tlp::Coord p1 = dir;
    p1 *= factor;
    p1 += srcCoord;

    if (curveType != CUBIC_STRAIGHTCROSS_TARGET) {
      p1 += normal;
    }

    if (curveType == CUBIC_STRAIGHTCROSS_TARGET) {
      dir = tlp::Coord(0, 0, 0);
    }

    tlp::Coord p2 = dir;
    p2 *= -factor;
    p2 += tgtCoord;

    if (curveType == CUBIC_DIAGONALCROSS || curveType == CUBIC_VERTICAL_DIAGONALCROSS) {
      p2 -= normal;
    } else if (curveType != CUBIC_STRAIGHTCROSS_SOURCE) {
      p2 += normal;
    }

    return std::vector<tlp::Coord>{{p1, p2}};
  }

  std::vector<tlp::Coord> computeQuadraticBezierControlPoints(tlp::edge e) {
    float x = FLT_MAX, y = FLT_MAX;
    float factor = curveRoundness;
    auto eEnds = graph->ends(e);
    const tlp::Coord &srcCoord = layout->getNodeValue(eEnds.first);
    const tlp::Coord &tgtCoord = layout->getNodeValue(eEnds.second);
    float dx = std::abs(srcCoord[0] - tgtCoord[0]);
    float dy = std::abs(srcCoord[1] - tgtCoord[1]);

    if (curveType == QUADRATIC_DISCRETE || curveType == QUADRATIC_DIAGONALCROSS) {
      if (dx < dy) {
        if (srcCoord[1] > tgtCoord[1]) {
          if (srcCoord[0] < tgtCoord[0]) {
            x = srcCoord[0] + factor * dy;
            y = srcCoord[1] - factor * dy;
          } else if (srcCoord[0] > tgtCoord[0]) {
            x = srcCoord[0] - factor * dy;
            y = srcCoord[1] - factor * dy;
          }
        } else if (srcCoord[1] < tgtCoord[1]) {
          if (srcCoord[0] < tgtCoord[0]) {
            x = srcCoord[0] + factor * dy;
            y = srcCoord[1] + factor * dy;
          } else if (srcCoord[0] > tgtCoord[0]) {
            x = srcCoord[0] - factor * dy;
            y = srcCoord[1] + factor * dy;
          }
        }

        if (curveType == QUADRATIC_DISCRETE && dx < factor * dy) {
          x = srcCoord[0];
        }
      } else if (dx > dy) {
        if (srcCoord[1] > tgtCoord[1]) {
          if (srcCoord[0] < tgtCoord[0]) {
            x = srcCoord[0] + factor * dx;
            y = srcCoord[1] - factor * dx;
          } else if (srcCoord[0] > tgtCoord[0]) {
            x = srcCoord[0] - factor * dx;
            y = srcCoord[1] - factor * dx;
          }
        } else if (srcCoord[1] < tgtCoord[1]) {
          if (srcCoord[0] < tgtCoord[0]) {
            x = srcCoord[0] + factor * dx;
            y = srcCoord[1] + factor * dx;
          } else if (srcCoord[0] > tgtCoord[0]) {
            x = srcCoord[0] - factor * dx;
            y = srcCoord[1] + factor * dx;
          }
        }

        if (curveType == QUADRATIC_DISCRETE && dy < factor * dx) {
          y = srcCoord[1];
        }
      }
    } else if (curveType == QUADRATIC_STRAIGHTCROSS) {
      if (dx < dy) {
        x = srcCoord[0];

        if (srcCoord[1] < tgtCoord[1]) {
          y = tgtCoord[1] - (1 - factor) * dy;
        } else {
          y = tgtCoord[1] + (1 - factor) * dy;
        }
      } else if (dx > dy) {
        if (srcCoord[0] < tgtCoord[0]) {
          x = tgtCoord[0] - (1 - factor) * dx;
        } else {
          x = tgtCoord[0] + (1 - factor) * dx;
        }

        y = srcCoord[1];
      }
    } else if (curveType == QUADRATIC_HORIZONTAL) {
      if (srcCoord[0] < tgtCoord[0]) {
        x = tgtCoord[0] - (1 - factor) * dx;
      } else {
        x = tgtCoord[0] + (1 - factor) * dx;
      }

      y = srcCoord[1];
    } else if (curveType == QUADRATIC_VERTICAL) {
      x = srcCoord[0];

      if (srcCoord[1] < tgtCoord[1]) {
        y = tgtCoord[1] - (1 - factor) * dy;
      } else {
        y = tgtCoord[1] + (1 - factor) * dy;
      }

    } else { // QUADRATIC_CONTINUOUS
      if (dx < dy) {
        if (srcCoord[1] > tgtCoord[1]) {
          if (srcCoord[0] < tgtCoord[0]) {
            x = srcCoord[0] + factor * dy;
            y = srcCoord[1] - factor * dy;

            if (tgtCoord[0] < x)
              x = tgtCoord[0];
          } else if (srcCoord[0] > tgtCoord[0]) {
            x = srcCoord[0] - factor * dy;
            y = srcCoord[1] - factor * dy;

            if (tgtCoord[0] > x)
              x = tgtCoord[0];
          }
        } else if (srcCoord[1] < tgtCoord[1]) {
          if (srcCoord[0] < tgtCoord[0]) {
            x = srcCoord[0] + factor * dy;
            y = srcCoord[1] + factor * dy;

            if (tgtCoord[0] < x)
              x = tgtCoord[0];
          } else if (srcCoord[0] > tgtCoord[0]) {
            x = srcCoord[0] - factor * dy;
            y = srcCoord[1] + factor * dy;

            if (tgtCoord[0] > x)
              x = tgtCoord[0];
          }
        }
      } else if (dx > dy) {
        if (srcCoord[1] > tgtCoord[1]) {
          if (srcCoord[0] < tgtCoord[0]) {
            x = srcCoord[0] + factor * dx;
            y = srcCoord[1] - factor * dx;
            y = tgtCoord[1] > y ? tgtCoord[1] : y;
          } else if (srcCoord[0] > tgtCoord[0]) {
            x = srcCoord[0] - factor * dx;
            y = srcCoord[1] - factor * dx;

            if (tgtCoord[1] > y)
              y = tgtCoord[1];
          }
        } else if (srcCoord[1] < tgtCoord[1]) {
          if (srcCoord[0] < tgtCoord[0]) {
            x = srcCoord[0] + factor * dx;
            y = srcCoord[1] + factor * dx;
            y = tgtCoord[1] < y ? tgtCoord[1] : y;
          } else if (srcCoord[0] > tgtCoord[0]) {
            x = srcCoord[0] - factor * dx;
            y = srcCoord[1] + factor * dx;

            if (tgtCoord[1] < y)
              y = tgtCoord[1];
          }
        }
      }
    }

    if (x != FLT_MAX && y != FLT_MAX)
      return std::vector<tlp::Coord>{{tlp::Coord(x, y)}};
    return std::vector<tlp::Coord>{{(srcCoord + tgtCoord) / 2.f}};
  }

  bool run() override {

    if (dataSet) {
      tlp::StringCollection curveTypeSc;

      if (dataSet->get("curve type", curveTypeSc)) {
        curveType = curveTypeSc.getCurrent();
      }

      dataSet->get("curve roundness", curveRoundness);
      dataSet->get("initial layout", layout);
      dataSet->get("bezier edges", bezierEdges);
    }

    if (layout == nullptr) {
      layout = graph->getProperty<tlp::LayoutProperty>("viewLayout");
    }

    for (auto e : graph->edges()) {
      if (curveType >= CUBIC_CONTINUOUS) {
        layout->setEdgeValue(e, computeCubicBezierControlPoints(e));
      } else {
        layout->setEdgeValue(e, computeQuadraticBezierControlPoints(e));
      }
    }

    if (bezierEdges) {
      tlp::IntegerProperty *viewShape = graph->getProperty<tlp::IntegerProperty>("viewShape");
      viewShape->setAllEdgeValue(tlp::EdgeShape::BezierCurve);
    }

    return true;
  }

private:
  int curveType;
  float curveRoundness;
  tlp::LayoutProperty *layout;
  bool bezierEdges;
};

PLUGIN(CurveEdges)
