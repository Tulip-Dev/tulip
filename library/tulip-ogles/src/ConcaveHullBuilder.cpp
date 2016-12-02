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

/**
 *
 * tulip-ogles library : rewrite of old tulip-ogl library using OpenGL ES API
 * Copyright (c) 2016 Antoine Lambert, Thales Services SAS
 * antoine-e.lambert@thalesgroup.com / antoine.lambert33@gmail.com
 *
 */

#include <clipper.hpp>

#ifdef _OPENMP
#include <omp.h>
#endif

#include <tulip/Graph.h>
#include <tulip/LayoutProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/IntegerProperty.h>
#include <tulip/TulipViewSettings.h>
#include <tulip/ParametricCurves.h>
#include <tulip/ConcaveHullBuilder.h>
#include <tulip/GlUtils.h>

using namespace tlp;
using namespace std;
using namespace ClipperLib;

static cInt myRound(float val) {
  if (val < 0)
    return static_cast<cInt>(val - 0.5f);
  else
    return static_cast<cInt>(val + 0.5f);
}

static Paths clipHulls(const Paths &hull1, const Paths &hull2, bool withHoles) {
  Clipper clip;
  Paths solution;
  clip.AddPaths(hull1, ptSubject, true);
  clip.AddPaths(hull2, ptClip, true);
  clip.Execute(ctUnion, solution);

  if (!withHoles) {
    PolyTree sol;
    Path poly;
    clip.Clear();
    clip.AddPaths(solution, ptSubject, true);
    clip.AddPath(poly, ptClip, true);
    clip.Execute(ctUnion, sol);

    solution.clear();
    for (int i = 0; i < sol.ChildCount(); ++i) {
      solution.push_back(sol.Childs[i]->Contour);
    }
  }

  return solution;
}

static Paths buildHull(const vector<Paths> &polygonsToClip, bool withHoles) {
  vector<Paths> polygonsCp(polygonsToClip);
  while (polygonsCp.size() > 1) {
    vector<Paths> tmp(polygonsCp.size() / 2);
#ifdef _OPENMP
#pragma omp parallel for
#endif
    for (size_t i = 0; i < polygonsCp.size(); i += 2) {
      if (i + 1 < polygonsCp.size())
        tmp[i / 2] = clipHulls(polygonsCp[i], polygonsCp[i + 1], withHoles);
    }
    if (polygonsCp.size() % 2 == 1)
      tmp.push_back(polygonsCp.back());
    polygonsCp = tmp;
  }
  return polygonsCp.front();
}

static vector<vector<vector<Coord>>> getFinalHulls(const Paths &hull, float scaling, float z) {
  Clipper clip;
  Paths solution;
  PolyTree sol;

  clip.AddPaths(solution, ptSubject, true);
  clip.AddPaths(hull, ptClip, true);
  clip.Execute(ctUnion, sol);

  vector<vector<vector<Coord>>> ret;
  for (int i = 0; i < sol.ChildCount(); ++i) {
    vector<vector<Coord>> c;
    PolyNode *polyNode = sol.Childs[i];
    while (polyNode) {
      vector<Coord> contour;
      for (size_t j = 0; j < polyNode->Contour.size(); ++j) {
        contour.push_back(Coord(polyNode->Contour[j].X / scaling, polyNode->Contour[j].Y / scaling, z));
      }
      c.push_back(contour);
      polyNode = polyNode->GetNext();
      if (i < sol.ChildCount() - 1 && polyNode == sol.Childs[i + 1]) {
        polyNode = nullptr;
      }
    }
    ret.push_back(c);
  }
  return ret;
}

ConcaveHullBuilder::ConcaveHullBuilder() : _scalingFactor(100.0f), _hullWithHoles(true), _z(-1.0f) {
}

void ConcaveHullBuilder::setScalingFactor(const float scalingFactor) {
  _scalingFactor = scalingFactor;
}

void ConcaveHullBuilder::setHullWithHoles(const bool hullWithHoles) {
  _hullWithHoles = hullWithHoles;
}

void ConcaveHullBuilder::setHullZValue(const float z) {
  _z = z;
}

void ConcaveHullBuilder::addPolygon(const vector<Coord> &polygon) {
  _polygons.push_back(polygon);
}

void ConcaveHullBuilder::computeHulls() {
  std::vector<Paths> clipperPolygons;
  for (size_t i = 0; i < _polygons.size(); ++i) {
    Paths poly(1);
    for (size_t j = 0; j < _polygons[i].size(); ++j) {
      poly[0].push_back(IntPoint(myRound(_polygons[i][j][0] * _scalingFactor), myRound(_polygons[i][j][1] * _scalingFactor)));
    }
    clipperPolygons.push_back(poly);
  }
  Paths hull = buildHull(clipperPolygons, _hullWithHoles);
  _computedHulls = getFinalHulls(hull, _scalingFactor, _z);
}

void ConcaveHullBuilder::clear() {
  _polygons.clear();
  _computedHulls.clear();
}

unsigned int ConcaveHullBuilder::nbComputedHulls() const {
  return _computedHulls.size();
}

const vector<vector<Coord>> &ConcaveHullBuilder::getHullWithHoles(const unsigned int hullId) const {
  return _computedHulls[hullId];
}

const vector<Coord> &ConcaveHullBuilder::getHullOuterContour(const unsigned int hullId) const {
  return _computedHulls[hullId][0];
}

static vector<Coord> genNodePolygon(node n, const tlp::LayoutProperty *viewLayout, const tlp::SizeProperty *viewSize,
                                    const tlp::IntegerProperty *viewShape, const float spacing) {

  const Coord &nCoord = viewLayout->getNodeValue(n);

  Size nSize = viewSize->getNodeValue(n) + 2.f * spacing;

  vector<Coord> nodeShape;

  int numberOfSides = 30;
  float startAngle = 0;
  float delta = (2.0f * M_PI) / numberOfSides;

  for (int i = 0; i < numberOfSides; ++i) {
    float deltaX, deltaY;
    if (viewShape->getNodeValue(n) != NodeShape::Circle) {
      deltaX = cos(i * delta + startAngle) * sqrt(nSize[0] * nSize[0] + nSize[1] * nSize[1]) / 2;
      deltaY = sin(i * delta + startAngle) * sqrt(nSize[0] * nSize[0] + nSize[1] * nSize[1]) / 2;
    } else {
      deltaX = cos(i * delta + startAngle) * max(nSize[0] / 2, nSize[1] / 2);
      deltaY = sin(i * delta + startAngle) * max(nSize[0] / 2, nSize[1] / 2);
    }
    nodeShape.push_back(nCoord + Coord(deltaX, deltaY));
  }

  return nodeShape;
}

std::vector<std::vector<tlp::Coord>> ConcaveHullBuilder::computeGraphHullVertices(Graph *graph, const bool withHoles, const float spacing,
                                                                                  const float z) {

  LayoutProperty *viewLayout = graph->getSuperGraph()->getProperty<LayoutProperty>("viewLayout");
  SizeProperty *viewSize = graph->getSuperGraph()->getProperty<SizeProperty>("viewSize");
  IntegerProperty *viewShape = graph->getSuperGraph()->getProperty<IntegerProperty>("viewShape");

  ConcaveHullBuilder chb;
  chb.setHullWithHoles(withHoles);
  chb.setHullZValue(z);

  std::set<tlp::node> visitedNodes;

  if (graph->numberOfEdges() > 0) {
    edge e;
    forEach(e, graph->getEdges()) {
      node src = graph->source(e);
      node tgt = graph->target(e);
      const Coord &srcCoord = viewLayout->getNodeValue(src);
      const Coord &tgtCoord = viewLayout->getNodeValue(tgt);
      const Size &srcSize = viewSize->getNodeValue(src);
      const Size &tgtSize = viewSize->getNodeValue(tgt);

      if (visitedNodes.find(src) == visitedNodes.end()) {
        chb.addPolygon(genNodePolygon(src, viewLayout, viewSize, viewShape, spacing));
        visitedNodes.insert(src);
      }

      const vector<Coord> &bends = viewLayout->getEdgeValue(e);
      vector<Coord> edgePoints;
      edgePoints.push_back(srcCoord);
      edgePoints.insert(edgePoints.end(), bends.begin(), bends.end());
      edgePoints.push_back(tgtCoord);

      const unsigned int nbCurvePoints = 20;

      if (viewShape->getEdgeValue(e) == EdgeShape::BezierCurve) {
        vector<Coord> curvePoints;
        computeBezierPoints(edgePoints, curvePoints, nbCurvePoints);
        edgePoints = curvePoints;
      } else if (viewShape->getEdgeValue(e) == EdgeShape::CatmullRomCurve) {
        vector<Coord> curvePoints;
        computeCatmullRomPoints(edgePoints, curvePoints, false, nbCurvePoints);
        edgePoints = curvePoints;
      } else if (viewShape->getEdgeValue(e) == EdgeShape::CubicBSplineCurve) {
        vector<Coord> curvePoints;
        computeOpenUniformBsplinePoints(edgePoints, curvePoints, 3, nbCurvePoints);
        edgePoints = curvePoints;
      }

      float divisor = 1.f;

      vector<float> sizes;
      bool interpolateEdgeSizes = true;
      if (interpolateEdgeSizes) {
        sizes = getSizes(edgePoints, std::min(srcSize[0] / (2 * divisor) + spacing, srcSize[1] / (2 * divisor) + spacing),
                         std::min(tgtSize[0] / (2 * divisor) + spacing, tgtSize[1] / (2 * divisor) + spacing));
      } else {
        sizes = getSizes(edgePoints, viewSize->getEdgeValue(e)[0] / divisor + spacing, viewSize->getEdgeValue(e)[0] / divisor + spacing);
      }

      vector<Coord> extrusion = buildCurvePoints(edgePoints, sizes, edgePoints[0] - (edgePoints[1] - edgePoints[0]),
                                                 edgePoints.back() + (edgePoints.back() - edgePoints[edgePoints.size() - 2]));

      vector<Coord> extrusionPoly;
      for (size_t i = 0; i < extrusion.size() / 2; ++i) {
        extrusionPoly.push_back(extrusion[2 * i]);
      }
      for (int i = static_cast<int>(extrusion.size() / 2) - 1; i >= 0; --i) {
        extrusionPoly.push_back(extrusion[2 * i + 1]);
      }
      std::reverse(extrusionPoly.begin(), extrusionPoly.end());
      chb.addPolygon(extrusionPoly);

      if (visitedNodes.find(tgt) == visitedNodes.end()) {
        chb.addPolygon(genNodePolygon(tgt, viewLayout, viewSize, viewShape, spacing));
        visitedNodes.insert(tgt);
      }
    }
  }

  node n;
  forEach(n, graph->getNodes()) {
    if (graph->deg(n) == 0) {
      chb.addPolygon(genNodePolygon(n, viewLayout, viewSize, viewShape, spacing));
    }
  }

  chb.computeHulls();

  std::vector<std::vector<tlp::Coord>> contours;

  for (unsigned int i = 0; i < chb.nbComputedHulls(); ++i) {
    if (withHoles) {
      const std::vector<std::vector<tlp::Coord>> &hwo = chb.getHullWithHoles(i);
      contours.insert(contours.end(), hwo.begin(), hwo.end());
    } else {
      contours.push_back(chb.getHullOuterContour(i));
    }
  }

  return contours;
}
