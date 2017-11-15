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
 *  This plugin is an implementation of the connected component packing
 *  algorithm published as:
 *
 *  Freivalds Karlis, Dogrusoz Ugur and Kikusts Paulis, \n
 *  "Disconnected Graph Layout and the Polyomino Packing Approach", \n
 *  In Proceeding GD '01 Revised Papers from the 9th International Symposium on Graph Drawing, \n
 *
 *  The implementation is freely inspired from the one that can be found in the GraphViz project (http://www.graphviz.org/).
 *
 *  \author Antoine LAMBERT, University of Bordeaux 1, FR: Email: antoine.lambert@labri.fr
 *  Version 1.0: May 2011
 */
#include <vector>
#include <set>
#include <tulip/TulipPluginHeaders.h>
#include <tulip/Vector.h>
#include <tulip/BoundingBox.h>
#include <tulip/DrawingTools.h>
#include <tulip/ConnectedTest.h>
#include <tulip/ForEach.h>
#include <tulip/ParametricCurves.h>
#include <tulip/TulipViewSettings.h>

#include "DatasetTools.h"

using namespace std;
using namespace tlp;

static const char *paramHelp[] = {
  // coordinates
  "Input layout of nodes and edges.",

  // rotation
  "Input rotation of nodes on z-axis",

  // margin
  "The minimum margin between each pair of nodes in the resulting packed layout.",

  // increment
  "The polyomino packing tries to find a place where the next polyomino will fit by following a square."
  "If there is no place where the polyomino fits, the square gets bigger and every place gets tried again."
};

typedef struct {
  std::vector<node> ccNodes; // the connected nodes associated to that polyomino
  int perim; // the perimeter value of the polyomino
  std::vector<tlp::Vec2i> cells; // the cells of the grid representing the polyomino
  tlp::BoundingBox ccBB; // the bounding box of the connected nodes
  tlp::Vec2i newPlace;
} Polyomino;

class PolyominoPacking : public tlp::LayoutAlgorithm {

public :
  PLUGININFORMATION("Connected Component Packing (Polyomino)",
                    "Antoine Lambert", "05/05/11",
                    "Implements the connected component packing algorithm published as:<br/>"
                    "<b>Disconnected Graph Layout and the Polyomino Packing Approach</b>, Freivalds Karlis, Dogrusoz Ugur and Kikusts Paulis, "
                    "Graph Drawing '01 Revised Papers from the 9th International Symposium on Graph Drawing.", "1.0", "Misc")
  PolyominoPacking(const tlp::PluginContext* context);

  ~PolyominoPacking();

  bool run();

private :

  int computeGridStep();
  void genPolyomino(Polyomino &poly, LayoutProperty* layout, SizeProperty* size);
  void fillEdge(tlp::edge e, tlp::Vec2i p, std::vector<tlp::Vec2i> &cells, int dx, int dy, LayoutProperty* layout);
  void fillLine(const tlp::Coord &p, const tlp::Coord &q, std::vector<tlp::Vec2i> &cells);
  bool polyominoFits(Polyomino& info, int x, int y);
  void placePolyomino(int i, Polyomino &info);

  unsigned int margin;
  unsigned int bndIncrement;

  std::vector<Polyomino> polyominos;

  int gridStepSize;

  TLP_HASH_MAP<tlp::Vec2i, bool> pointsSet;

  tlp::IntegerProperty *shape;
};

#define C 100

class polyPerimOrdering : public binary_function<Polyomino, Polyomino, bool> {

public :

  polyPerimOrdering() {}

  bool operator()(Polyomino ci1, Polyomino ci2) const {
    return ci1.perim > ci2.perim;
  }

};

PLUGIN(PolyominoPacking)

PolyominoPacking::PolyominoPacking( const PluginContext* context ) : LayoutAlgorithm(context) {
  addInParameter<LayoutProperty> ("coordinates", paramHelp[0], "viewLayout");
  addNodeSizePropertyParameter(this);
  addInParameter<DoubleProperty> ("rotation", paramHelp[1], "viewRotation");
  addInParameter<unsigned int>("margin", paramHelp[2], "1");
  addInParameter<unsigned int>("increment", paramHelp[3], "1");
}

PolyominoPacking::~PolyominoPacking() {}

bool PolyominoPacking::run() {

  LayoutProperty *layout = NULL;
  SizeProperty *size = NULL;
  DoubleProperty *rotation = NULL;
  margin = 1;
  bndIncrement = 1;

  if(dataSet != NULL) {
    dataSet->get("coordinates", layout);
    getNodeSizePropertyParameter(dataSet, size);
    dataSet->get("rotation", rotation);
    dataSet->get("margin", margin);
    dataSet->get("increment", bndIncrement);
  }

  if (pluginProgress) {
    pluginProgress->setComment("Computing connected components ...");
  }

  vector< vector<node> > connectedComponents;
  tlp::ConnectedTest::computeConnectedComponents(graph, connectedComponents);

  if (connectedComponents.size() <= 1) {
    node n;
    forEach(n, graph->getNodes()) {
      result->setNodeValue(n, layout->getNodeValue(n));
    }
    edge e;
    forEach(e, graph->getEdges()) {
      result->setEdgeValue(e, layout->getEdgeValue(e));
    }
    return true;
  }

  shape = graph->getProperty<IntegerProperty>("viewShape");

  polyominos.reserve(connectedComponents.size());

  for (size_t i = 0 ; i < connectedComponents.size() ; ++i) {
    std::vector<node>& ccNodes = connectedComponents[i];
    unsigned int nbNodes = ccNodes.size();
    std::vector<edge> ccEdges;

    // get edges of current connected component
    for (unsigned int j = 0; j < nbNodes; ++j) {
      edge e;
      forEach(e, graph->getOutEdges(ccNodes[j])) {
        ccEdges.push_back(e);
      }
    }

    BoundingBox ccBB = tlp::computeBoundingBox(ccNodes, ccEdges, layout,
                       size, rotation);
    Polyomino info;
    info.ccNodes = ccNodes;
    info.ccBB = ccBB;
    polyominos.push_back(info);

    if (pluginProgress) {
      pluginProgress->progress(i+1, connectedComponents.size());
    }
  }

  gridStepSize = computeGridStep();

  if (gridStepSize <= 0)
    return true;

  if (pluginProgress) {
    pluginProgress->setComment("Generating polyominos ...");
    pluginProgress->progress(0, polyominos.size());
  }

  for (size_t i = 0; i < polyominos.size() ; ++i) {
    genPolyomino(polyominos[i], layout, size);

    if (pluginProgress) {
      pluginProgress->progress(i+1, polyominos.size());
    }
  }

  std::sort(polyominos.begin(), polyominos.end(), polyPerimOrdering());

  if (pluginProgress) {
    pluginProgress->setComment("Packing polyominos ...");
    pluginProgress->progress(0, polyominos.size());
  }

  for (size_t i = 0 ; i < polyominos.size() ; ++i) {
    placePolyomino(i, polyominos[i]);

    if (pluginProgress) {
      pluginProgress->progress(i+1, polyominos.size());
    }
  }

  for (size_t i = 0 ; i < polyominos.size() ; ++i) {
    Polyomino& poly = polyominos[i];
    Coord move = Coord(poly.newPlace[0], poly.newPlace[1]);
    const std::vector<node>& ccNodes = poly.ccNodes;
    unsigned int nbNodes = ccNodes.size();

    for (unsigned int j = 0; j < nbNodes; ++j) {
      node n = ccNodes[j];
      result->setNodeValue(n, layout->getNodeValue(n) + move);
      edge e;
      forEach(e, graph->getOutEdges(n)) {
        vector<Coord> bends = layout->getEdgeValue(e);

        if (bends.size()) {
          for (size_t k = 0 ; k < bends.size() ; ++k) {
            bends[j] += move;
          }

          result->setEdgeValue(e, bends);
        }
      }
    }
  }

  return true;
}

int PolyominoPacking::computeGridStep() {
  double a = C * polyominos.size() - 1.0;
  double b = 0.0;
  double c = 0.0;

  for (size_t i = 0; i < polyominos.size() ; ++i) {
    const BoundingBox &ccBB = polyominos[i].ccBB;
    double W = ccBB[1][0] - ccBB[0][0] + 2 * margin;
    double H = ccBB[1][1] - ccBB[0][1] + 2 * margin;
    b -= (W + H);
    c -= (W * H);
  }

  double d = b * b - 4.0 * a * c;

  if (d < 0) {
    return -1;
  }

  double r = sqrt(d);
  double l1 = (-b + r) / (2 * a);
  int root = int(l1);

  if (root == 0) root = 1;

  return root;
}

static int grid(float x, int s) {
  return int(ceil(x/s));
}

template <typename T>
static T cval(T val, int size) {
  if (val >= 0) {
    return (val / size);
  }
  else {
    return (((val+1)/size)-1);
  }
}

template <typename T>
static T cell(const T &p, int gridStep) {
  T ret;
  ret[0] = cval(p[0], gridStep);
  ret[1] = cval(p[1], gridStep);
  return ret;
}

static Vec2i vec3fToVec2i(const Vec3f &c) {
  return Vec2i(int(rint(c[0])), int(rint(c[1])));
}

void PolyominoPacking::genPolyomino(Polyomino &poly, LayoutProperty* layout,
                                    SizeProperty* size) {

  const BoundingBox &ccBB = poly.ccBB;
  const std::vector<node>& ccNodes = poly.ccNodes;

  int dx = - rint(ccBB[0][0]);
  int dy = - rint(ccBB[0][1]);

  unsigned int nbNodes = ccNodes.size();

  for (unsigned int i = 0; i < nbNodes; ++i) {
    node n = ccNodes[i];
    const Coord &nodeCoord = layout->getNodeValue(n);
    const Size &nodeSize = size->getNodeValue(n);
    Vec2i point = vec3fToVec2i(nodeCoord);
    point[0] += dx;
    point[1] += dy;
    Vec2i s2;
    s2[0] = margin + nodeSize[0] / 2;
    s2[1] = margin + nodeSize[1] / 2;
    Vec2i LL = point - s2;
    Vec2i UR = point + s2;
    LL = cell(LL, gridStepSize);
    UR = cell(UR, gridStepSize);

    for (int x = LL[0] ; x <= UR[0] ; ++x) {
      for (int y = LL[1]; y <= UR[1] ; ++y) {
        Vec2i cellCoord;
        cellCoord[0] = x;
        cellCoord[1] = y;
        poly.cells.push_back(cellCoord);
      }
    }

    point = cell(point, gridStepSize);
    edge e;
    forEach(e, graph->getOutEdges(n)) {
      fillEdge(e, point, poly.cells, dx, dy, layout);
    }
  }

  int W = grid(ccBB[1][0] - ccBB[0][0] + 2 * margin, gridStepSize);
  int H = grid(ccBB[1][1] - ccBB[0][1] + 2 * margin, gridStepSize);
  poly.perim = W + H;
}

void PolyominoPacking::fillEdge(edge e, Vec2i p, std::vector<Vec2i> &cells, int dx, int dy, LayoutProperty* layout) {

  Coord pf(p[0], p[1]);
  std::pair<node, node> ends = graph->ends(e);
  const Coord &srcCoord = layout->getNodeValue(ends.first);
  Coord tgtCoord = layout->getNodeValue(ends.second);
  std::vector<Coord> bends = layout->getEdgeValue(e);

  if (bends.empty()) {
    tgtCoord += Coord(dx, dy);
    tgtCoord = cell(tgtCoord, gridStepSize);
    fillLine(pf, tgtCoord, cells);
    return;
  }

  if (shape->getEdgeValue(e) == EdgeShape::BezierCurve) {
    vector<Coord> controlPoints;
    controlPoints.push_back(srcCoord);
    controlPoints.insert(controlPoints.end(), bends.begin(), bends.end());
    controlPoints.push_back(tgtCoord);
    bends.clear();
    computeBezierPoints(controlPoints, bends, 20);
    bends.erase(bends.begin());
    bends.pop_back();
  }
  else if (shape->getEdgeValue(e) == EdgeShape::CubicBSplineCurve) {
    vector<Coord> controlPoints;
    controlPoints.push_back(srcCoord);
    controlPoints.insert(controlPoints.end(), bends.begin(), bends.end());
    controlPoints.push_back(tgtCoord);
    bends.clear();
    computeOpenUniformBsplinePoints(controlPoints, bends, 3, 20);
    bends.erase(bends.begin());
    bends.pop_back();
  }
  else if (shape->getEdgeValue(e) == EdgeShape::CatmullRomCurve) {
    vector<Coord> controlPoints;
    controlPoints.push_back(srcCoord);
    controlPoints.insert(controlPoints.end(), bends.begin(), bends.end());
    controlPoints.push_back(tgtCoord);
    bends.clear();
    computeCatmullRomPoints(controlPoints, bends, false, 20);
    bends.erase(bends.begin());
    bends.pop_back();
  }

  Coord curSrc = pf;

  for (size_t i = 0; i < bends.size(); ++i) {
    bends[i] += Coord(dx, dy);
    bends[i] = cell(bends[i], gridStepSize);
    fillLine(curSrc, bends[i], cells);
    curSrc = bends[i];
  }

  tgtCoord += Coord(dx, dy);
  tgtCoord = cell(tgtCoord, gridStepSize);
  fillLine(curSrc, tgtCoord, cells);
}

void PolyominoPacking::fillLine(const Coord &p, const Coord &q, std::vector<Vec2i> &cells) {
  int x1 = rint(p[0]);
  int y1 = rint(p[1]);
  int x2 = rint(q[0]);
  int y2 = rint(q[1]);

  int dx = x2 - x1;
  int ax = abs(dx) << 1;
  int sx = dx < 0 ? -1 : 1;
  int dy = y2 - y1;
  int ay = abs(dy) << 1;
  int sy = dy < 0 ? -1 : 1;

  int x = x1;
  int y = y1;

  if (ax > ay) {
    int d = ay - (ax >> 1);

    while (true) {
      Vec2i cell;
      cell[0] = x;
      cell[1] = y;
      cells.push_back(cell);

      if (x == x2)
        return;

      if (d >= 0) {
        y += sy;
        d -= ax;
      }

      x += sx;
      d += ay;
    }
  }
  else {
    int d = ax - (ay >> 1);

    while (true) {
      Vec2i cell;
      cell[0] = x;
      cell[1] = y;
      cells.push_back(cell);

      if (y == y2)
        return;

      if (d >= 0) {
        x += sx;
        d -= ay;
      }

      y += sy;
      d += ax;
    }
  }
}

bool PolyominoPacking::polyominoFits(Polyomino& poly, int x, int y) {
  std::vector<Vec2i> &cells = poly.cells;
  const BoundingBox &ccBB = poly.ccBB;

  for (size_t i = 0; i < cells.size() ; ++i) {
    Vec2i cell = cells[i];
    cell[0] += x;
    cell[1] += y;

    if (pointsSet.find(cell) != pointsSet.end())
      return false;
  }

  Vec2i LL = vec3fToVec2i(ccBB[0]);
  poly.newPlace[0] = gridStepSize * x - LL[0];
  poly.newPlace[1] = gridStepSize * y - LL[1];

  for (size_t i = 0; i < cells.size() ; ++i) {
    Vec2i cell = cells[i];
    cell[0] += x;
    cell[1] += y;
    pointsSet[cell] = true;
  }

  return true;
}

void PolyominoPacking::placePolyomino(int i, Polyomino &poly) {
  int x = 0, y = 0;
  int W = 0, H = 0;

  const BoundingBox &ccBB = poly.ccBB;

  if (i == 0) {
    W = grid(ccBB[1][0] - ccBB[0][0] + 2 * margin, gridStepSize);
    H = grid(ccBB[1][1] - ccBB[0][1] + 2 * margin, gridStepSize);

    if (polyominoFits(poly, -W / 2, -H / 2))
      return;
  }

  if (polyominoFits(poly, 0, 0))
    return;

  W = ceil(ccBB[1][0] - ccBB[0][0]);
  H = ceil(ccBB[1][1] - ccBB[0][1]);

  if (W >= H) {
    for (int bnd = 1 ; ; bnd += bndIncrement) {
      x = 0;
      y = -bnd;

      for (; x < bnd; ++x)
        if (polyominoFits(poly, x, y))
          return;

      for (; y < bnd; ++y)
        if (polyominoFits(poly, x, y))
          return;

      for (; x > -bnd; --x)
        if (polyominoFits(poly, x, y))
          return;

      for (; y > -bnd; --y)
        if (polyominoFits(poly, x, y))
          return;

      for (; x < 0 ; ++x)
        if (polyominoFits(poly, x, y))
          return;
    }
  }
  else {
    for (int bnd = 1;; bnd += bndIncrement) {
      y = 0;
      x = -bnd;

      for (; y > -bnd; --y)
        if (polyominoFits(poly, x, y))
          return;

      for (; x < bnd; ++x)
        if (polyominoFits(poly, x, y))
          return;

      for (; y < bnd; ++y)
        if (polyominoFits(poly, x, y))
          return;

      for (; x > -bnd; --x)
        if (polyominoFits(poly, x, y))
          return;

      for (; y > 0; --y)
        if (polyominoFits(poly, x, y))
          return;
    }
  }
}
