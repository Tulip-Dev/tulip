/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: Tulip development Team
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
#include <vector>
#include <tulip/SizeProperty.h>
#include <tulip/LayoutProperty.h>
#include <tulip/StaticProperty.h>

using namespace std;
using namespace tlp;

class OrthoTree : public tlp::LayoutAlgorithm {

  unsigned int nodeSpacing;
  unsigned int layerSpacing;
  tlp::SizeProperty *size;

  void computeVerticalSize(const tlp::node n, tlp::NodeStaticProperty<double> &verticalSize);
  void computeLayout(const tlp::node n, tlp::NodeStaticProperty<double> &verticalSize);

public:
  PLUGININFORMATION("OrthoTree", "Romain Bourqui", "20/02/2012", "Orthogonal Tree", "1.0", "Tree")

  OrthoTree(const tlp::PluginContext *context);

  bool run() override;
};

PLUGIN(OrthoTree)

//================================================================================
static const char *paramHelp[] = {
    // layer spacing
    "Define the spacing between two successive layers",
    // node spacing
    "Define the spacing between two nodes"};

OrthoTree::OrthoTree(const tlp::PluginContext *context)
    : tlp::LayoutAlgorithm(context), nodeSpacing(4), layerSpacing(10), size(nullptr) {
  addInParameter<unsigned int>("Layer spacing", paramHelp[0], "10", true);
  addInParameter<unsigned int>("Node spacing", paramHelp[1], "4", true);
}

void OrthoTree::computeVerticalSize(const node n, NodeStaticProperty<double> &verticalSize) {
  unsigned pos_n = graph->nodePos(n);

  if (graph->outdeg(n) == 0) {
    verticalSize[pos_n] = size->getNodeValue(n)[1];
  } else {
    double s = 0.;
    node u;
    forEach(u, graph->getOutNodes(n)) {
      computeVerticalSize(u, verticalSize);
      s += verticalSize[graph->nodePos(u)];
    }

    if (graph->outdeg(n) > 1) {
      s += nodeSpacing * (graph->outdeg(n) - 1);
    }

    verticalSize[pos_n] = s;
  }
}

void OrthoTree::computeLayout(const node n, NodeStaticProperty<double> &verticalSize) {
  Coord cn = result->getNodeValue(n);
  double prev = 0.;
  edge e;
  forEach(e, graph->getOutEdges(n)) {
    node u = graph->opposite(e, n);
    Coord c(cn);
    c[0] += layerSpacing;
    c[1] -= prev;

    prev += verticalSize[graph->nodePos(u)] + nodeSpacing;
    result->setNodeValue(u, c);

    Coord bend(cn[0], c[1], 0);
    vector<Coord> bends(1);
    bends[0] = bend;
    result->setEdgeValue(e, bends);
    computeLayout(u, verticalSize);
  }
}

bool OrthoTree::run() {
  layerSpacing = 10;
  nodeSpacing = 4;

  if (dataSet != nullptr) {
    dataSet->get("Layer spacing", layerSpacing);
    dataSet->get("Node spacing", nodeSpacing);
  }

  NodeStaticProperty<double> verticalSize(graph);
  size = graph->getProperty<SizeProperty>("viewSize");

  verticalSize.setAll(0);

  node root = graph->getSource();
  assert(root.isValid());

  computeVerticalSize(root, verticalSize);

  result->setAllNodeValue(Coord(0, 0, 0));
  result->setAllEdgeValue(vector<Coord>(0));
  computeLayout(root, verticalSize);

  return true;
}
