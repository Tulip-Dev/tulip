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
#include <tulip/TreeTest.h>

using namespace std;
using namespace tlp;

class OrthoTree : public tlp::LayoutAlgorithm {

  unsigned int nodeSpacing;
  unsigned int layerSpacing;
  tlp::SizeProperty *size;
  Graph *tree;

  void computeVerticalSize(const tlp::node n, tlp::NodeStaticProperty<double> &verticalSize);
  void computeLayout(const tlp::node n, tlp::NodeStaticProperty<double> &verticalSize);

public:
  PLUGININFORMATION("OrthoTree", "Romain Bourqui", "20/02/2012", "Orthogonal Tree layout", "1.0",
                    "Tree")

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
  addInParameter<unsigned int>("layer spacing", paramHelp[0], "10", true);
  addInParameter<unsigned int>("node spacing", paramHelp[1], "4", true);
}

void OrthoTree::computeVerticalSize(const node n, NodeStaticProperty<double> &verticalSize) {
  auto d = tree->outdeg(n);
  if (d == 0) {
    verticalSize[n] = size->getNodeValue(n)[1];
  } else {
    double s = 0.;
    for (auto u : tree->getOutNodes(n)) {
      computeVerticalSize(u, verticalSize);
      s += verticalSize[u];
    }

    if (d > 1) {
      s += nodeSpacing * (d - 1);
    }

    verticalSize[n] = s;
  }
}

void OrthoTree::computeLayout(const node n, NodeStaticProperty<double> &verticalSize) {
  const Coord &cn = result->getNodeValue(n);
  double prev = 0.;
  for (auto e : tree->getOutEdges(n)) {
    node u = tree->opposite(e, n);
    Coord c(cn[0] + layerSpacing, cn[1] - prev, cn[2]);

    prev += verticalSize[u] + nodeSpacing;
    result->setNodeValue(u, c);

    vector<Coord> bends{Coord(cn[0], c[1], 0)};
    result->setEdgeValue(e, bends);
    computeLayout(u, verticalSize);
  }
}

bool OrthoTree::run() {
  layerSpacing = 10;
  nodeSpacing = 4;

  if (dataSet != nullptr) {
    dataSet->get("layer spacing", layerSpacing);
    dataSet->get("node spacing", nodeSpacing);
  }

  NodeStaticProperty<double> verticalSize(graph, 0);
  size = graph->getProperty<SizeProperty>("viewSize");

  if (pluginProgress)
    pluginProgress->showPreview(false);

  // push a temporary graph state (not redoable)
  // preserving layout updates if any
  std::vector<PropertyInterface *> propsToPreserve;

  // preserve result if it not a temporary property
  if (!result->getName().empty())
    propsToPreserve.push_back(result);

  graph->push(false, &propsToPreserve);

  tree = TreeTest::computeTree(graph, pluginProgress);

  if (pluginProgress && pluginProgress->state() != TLP_CONTINUE) {
    graph->pop();
    return pluginProgress->state() != TLP_CANCEL;
  }

  node root = tree->getSource();
  assert(root.isValid());

  computeVerticalSize(root, verticalSize);

  result->setAllNodeValue(Coord(0, 0, 0));
  result->setAllEdgeValue(vector<Coord>(0));
  computeLayout(root, verticalSize);

  // forget last temporary graph state
  graph->pop();

  return true;
}
