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
#include <algorithm>
#include <tulip/TreeTest.h>
#include <tulip/TulipViewSettings.h>

#include "SquarifiedTreeMap.h"
#include "TreeTools.h"

using namespace std;
using namespace tlp;

PLUGIN(SquarifiedTreeMap)
//====================================================================

const double SEPARATION_Z = 10;
const double DEFAULT_RATIO = 1.4;
const int DEFAULT_WIDTH = 1024;
const int DEFAULT_HEIGHT = 1024;

static const char *paramHelp[] = {
    // metric
    "This parameter defines the metric used to estimate the size allocated to each node.",

    // Aspect Ratio
    "This parameter enables to set up the aspect ratio (height/width) for the rectangle "
    "corresponding to the root node.",

    // Treemap Type
    "This parameter indicates to use normal Treemaps (B. Shneiderman) or Squarified Treemaps (J. "
    "J. van Wijk)",

    // Node Size
    "This parameter defines the property used as node sizes.",

    // Node Shape
    "This parameter defines the property used as node shapes."};

//====================================================================
SquarifiedTreeMap::SquarifiedTreeMap(const tlp::PluginContext *context) : LayoutAlgorithm(context) {
  aspectRatio = DEFAULT_RATIO;
  addInParameter<NumericProperty *>("metric", paramHelp[0], "viewMetric", false);
  addInParameter<double>("Aspect Ratio", paramHelp[1], "1.");
  addInParameter<bool>("Treemap Type", paramHelp[2], "false");
  addOutParameter<SizeProperty>("Node Size", paramHelp[3], "viewSize");
  addOutParameter<IntegerProperty>("Node Shape", paramHelp[4], "viewShape");
}

//====================================================================
SquarifiedTreeMap::~SquarifiedTreeMap() {}

//====================================================================
bool SquarifiedTreeMap::check(std::string &errorMsg) {
  if (!TreeTest::isTree(graph)) {
    errorMsg = "The graph must be a tree.";
    return false;
  }

  metric = nullptr;

  if (dataSet != nullptr)
    dataSet->get("metric", metric);

  if (!metric && graph->existProperty("viewMetric")) {
    metric = graph->getProperty<DoubleProperty>("viewMetric");

    if (metric->getNodeDoubleMin() < 0.) {
      errorMsg = "Graph's nodes must have a positive metric.";
      return false;
    }
  }

  errorMsg = "";
  return true;
}

//====================================================================
/**
 *
 *  @todo manage correctly parameters remove texture mode, enable to choose bordersize + header
 * size
 */
bool SquarifiedTreeMap::run() {
  double aspectRatio = DEFAULT_RATIO;
  shneidermanTreeMap = false;
  sizeResult = nullptr;
  glyphResult = nullptr;

  if (dataSet != nullptr) {
    dataSet->get("Aspect Ratio", aspectRatio);
    dataSet->get("Treemap Type", shneidermanTreeMap);
    dataSet->get("Node Size", sizeResult);
    dataSet->get("Node Shape", glyphResult);
  }

  if (sizeResult == nullptr)
    sizeResult = graph->getProperty<SizeProperty>("viewSize");

  if (glyphResult == nullptr)
    glyphResult = graph->getLocalProperty<IntegerProperty>("viewShape");

  {
    // change the glyph of all internal nodes to be a window
    for (auto n : graph->nodes()) {
      if (graph->outdeg(n) != 0)
        glyphResult->setNodeValue(n, NodeShape::Window);
    }
  }

  Rectd initialSpace(0, 0, DEFAULT_WIDTH * aspectRatio, DEFAULT_HEIGHT);

  node root = graph->getSource();
  computeNodesSize(root);

  Vec2d center = initialSpace.center();
  result->setNodeValue(root, Coord(float(center[0]), float(center[1]), 0));
  Size initialSpaceSize(float(initialSpace.width()), float(initialSpace.height()), 0);
  sizeResult->setNodeValue(root, initialSpaceSize);
  vector<node> toTreat(orderedChildren(root));

  if (!toTreat.empty()) {
    Rectd tmp = adjustRectangle(initialSpace);
    squarify(toTreat, tmp, 1);
  }

  return true;
}
//====================================================================
tlp::Rectd SquarifiedTreeMap::adjustRectangle(const tlp::Rectd &r) const {
  assert(r.isValid());
  Rectd result(r);
  Vec2d dist(r[1] - r[0]);
  // header size
  result[1][1] -= dist[1] * 0.1;
  // border size
  result[1][1] -= dist[1] * 0.02;
  result[1][0] -= dist[0] * 0.02;
  result[0][0] += dist[0] * 0.02;
  result[0][1] += dist[1] * 0.02;

  assert(result.isValid());
  return result;
}
//====================================================================
void SquarifiedTreeMap::layoutRow(const std::vector<tlp::node> &row, const int depth,
                                  const tlp::Rectd &rectArea) {
  assert(rectArea.isValid());
  assert(!row.empty());
  double rowArea = 0;

  for (auto n : row)
    rowArea += nodesSize.get(n.id);

  double sum = 0;
  Vec2d dist = rectArea[1] - rectArea[0];

  for (auto n : row) {
    Rectd layoutRec(rectArea);

    if (rectArea.width() > rectArea.height()) {
      layoutRec[0][0] = rectArea[0][0] + (sum / rowArea) * dist[0];
      layoutRec[1][0] = layoutRec[0][0] + (nodesSize.get(n.id) / rowArea) * dist[0];
    } else {
      layoutRec[0][1] = rectArea[0][1] + (sum / rowArea) * dist[1];
      layoutRec[1][1] = layoutRec[0][1] + (nodesSize.get(n.id) / rowArea) * dist[1];
    }

    assert(layoutRec.isValid());
    sum += nodesSize.get(n.id);
    Vec2d center = layoutRec.center();
    result->setNodeValue(n, Coord(float(center[0]), float(center[1]), float(depth * SEPARATION_Z)));
    sizeResult->setNodeValue(n, Size(float(layoutRec.width()), float(layoutRec.height()), 0));

    if (graph->outdeg(n) > 0) {
      vector<node> toTreat(orderedChildren(n));
      Rectd newRec(adjustRectangle(layoutRec));
      squarify(toTreat, newRec, depth + 1);
    }
  }
}
//==================================================================
class IsGreater {
public:
  IsGreater(const tlp::MutableContainer<double> &measure) : measure(measure) {}
  bool operator()(const node a, const node b) const {
    return measure.get(a.id) > measure.get(b.id);
  }
  const tlp::MutableContainer<double> &measure;
};
//======================================
vector<node> SquarifiedTreeMap::orderedChildren(const tlp::node n) const {
  // sort children of n and store it in result
  //======================================
  vector<node> result(graph->outdeg(n));
  // build a list of pair <node, size>
  size_t i = 0;
  for (auto child : graph->getOutNodes(n)) {
    result[i++] = child;
  }
  IsGreater sortFunctor(nodesSize);
  sort(result.begin(), result.end(), sortFunctor);
  return result;
}
//==========================================================
/*
class Row {
    const std::vector<tlp::node> &row;
    double sumOfNodesSurface;

};
*/
double SquarifiedTreeMap::evaluateRow(const std::vector<tlp::node> &row, tlp::node n, double width,
                                      double length, double surface) {

  double sumOfNodesSurface = nodesSize.get(n.id);

  for (auto nr : row) {
    sumOfNodesSurface += nodesSize.get(nr.id);
  }

  //====================
  double size = nodesSize.get(n.id);
  // ratio is the aspect ratio of rectangle of the considered elements
  double nodeRectangleWidth = length * sumOfNodesSurface / surface;
  double nodeRectangleHeight = width * size / sumOfNodesSurface;
  double ratio = std::min(nodeRectangleHeight, nodeRectangleWidth) /
                 std::max(nodeRectangleHeight, nodeRectangleWidth);
  double minratio = ratio;
  double maxratio = ratio;
  double sumratio = ratio;

  for (auto nr : row) {
    double size = nodesSize.get(nr.id);
    double nodeRectangleWidth = length * sumOfNodesSurface / surface;
    double nodeRectangleHeight = width * size / sumOfNodesSurface;
    double ratio = std::min(nodeRectangleHeight, nodeRectangleWidth) /
                   std::max(nodeRectangleHeight, nodeRectangleWidth);
    sumratio += ratio;
    minratio = std::min(minratio, ratio);
    maxratio = std::max(maxratio, ratio);
  }

  // The paper formula does not give the best result
  // return std::max(surface*surface*maxratio/(sumOfNodesSurface*sumOfNodesSurface),
  //                sumOfNodesSurface*sumOfNodesSurface / (surface*surface*minratio));
  return sumratio / (row.size() + 1);
}

//====================================================================
void SquarifiedTreeMap::squarify(const std::vector<tlp::node> &toTreat, const tlp::Rectd &rectArea,
                                 const int depth) {
  assert(rectArea.isValid());
  assert(!toTreat.empty());

  vector<node> rowNodes;
  vector<node> unTreated;
  double unTreatedSurface = 0;

  vector<node>::const_iterator it;
  double surface = 0;

  for (it = toTreat.begin(); it != toTreat.end(); ++it)
    surface += nodesSize.get(it->id);

  it = toTreat.begin();
  double length = std::max(rectArea.width(), rectArea.height());
  double width = std::min(rectArea.width(), rectArea.height());
  double ratio = evaluateRow(rowNodes, *it, width, length, surface);
  rowNodes.push_back(*it);
  ++it;

  // build the new row
  while (it != toTreat.end()) { // add node in the current row while condition is ok
    if (shneidermanTreeMap) {
      rowNodes.push_back(*it);
    } else {
      double newRatio = evaluateRow(rowNodes, *it, width, length, surface);

      if (newRatio < ratio) { // we finish to build that row
        break;
        /*unTreated.push_back(*it);
        unTreatedSurface += nodesSize.get(it->id);*/
      } else {
        ratio = newRatio;
        rowNodes.push_back(*it); // add the node to the current row
      }
    }

    ++it;
  }

  // Compute measure on unTreated nodes to do a recursive call
  while (it != toTreat.end()) {
    unTreated.push_back(*it);
    unTreatedSurface += nodesSize.get(it->id);
    ++it;
  }

  assert(unTreated.size() + rowNodes.size() == toTreat.size());

  Vec2d dist = rectArea[1] - rectArea[0];
  assert(!rowNodes.empty());

  Rectd rowRec(rectArea); // The rectangle for that row

  if (rectArea.width() > rectArea.height())
    rowRec[1][0] -= (unTreatedSurface / surface) * dist[0];
  else
    rowRec[0][1] += (unTreatedSurface / surface) * dist[1];

  assert(rowRec.isValid());

  layoutRow(rowNodes, depth, rowRec);

  if (!unTreated.empty()) {
    Rectd subRec(rectArea); // the rectangle of unTreated nodes

    if (rectArea.width() > rectArea.height())
      subRec[0][0] = rowRec[1][0];
    else
      subRec[1][1] = rowRec[0][1];

    assert(subRec.isValid());
    squarify(unTreated, subRec, depth);
  }
}
//====================================================================
void SquarifiedTreeMap::computeNodesSize(const tlp::node n) {

  if (graph->outdeg(n) == 0) { // the node is a leaf of the tree
    double leafValue = metric ? metric->getNodeDoubleValue(n) : 1.;

    nodesSize.set(n.id, (leafValue > 0) ? leafValue : 1.);
    return;
  }

  double internalNodeValue = 0.;
  for (auto child : graph->getOutNodes(n)) {
    computeNodesSize(child);
    internalNodeValue += nodesSize.get(child.id);
  }

  nodesSize.set(n.id, internalNodeValue);
}
//====================================================================
