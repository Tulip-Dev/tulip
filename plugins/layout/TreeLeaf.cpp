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
#include <tulip/GraphTools.h>
#include "TreeLeaf.h"
#include "DatasetTools.h"
#include "Orientation.h"

PLUGIN(TreeLeaf)

using namespace std;
using namespace tlp;

const char* paramHelp[] = {
  //Orientation
  HTML_HELP_OPEN() \
  HTML_HELP_DEF("Type", "bool") \
  HTML_HELP_DEF("Default", "true") \
  HTML_HELP_BODY() \
  "If the layer spacing is uniform, the spacing between to consecutive layers will be the same." \
  HTML_HELP_CLOSE(),
};

void TreeLeaf::computeLevelHeights(tlp::Graph *tree, tlp::node n, unsigned int depth,
                                   OrientableSizeProxy *oriSize) {
  if (levelHeights.size() == depth)
    levelHeights.push_back(0);

  float nodeHeight = oriSize->getNodeValue(n).getH();

  if (nodeHeight > levelHeights[depth])
    levelHeights[depth] = nodeHeight;

  node on;
  forEach(on, tree->getOutNodes(n))
  computeLevelHeights(tree, on, depth + 1, oriSize);
}

float TreeLeaf::dfsPlacement(tlp::Graph* tree, tlp::node n, float x, float y, unsigned int depth,
                             OrientableLayout *oriLayout, OrientableSizeProxy *oriSize) {
  float minX = 0;
  float maxX = 0;
  float nodeWidth = oriSize->getNodeValue(n).getW();

  if (tree->outdeg(n) == 0) {
    oriLayout->setNodeValue(n, OrientableCoord(oriLayout, x + nodeWidth/2, y, 0));
    return x + nodeWidth;
  }

  Iterator<node> *itN=tree->getOutNodes(n);

  float layerSpacing = minLayerSpacing;

  if (uniformLayerDistance == false) {
    if (depth < levelHeights.size()-1) {
      layerSpacing += nodeSpacing;
      layerSpacing = max(minLayerSpacing, (levelHeights[depth] + levelHeights[depth + 1]) / 2);
    }
  }

  if (itN->hasNext()) {
    node itn = itN->next();
    minX = x;
    maxX = x = dfsPlacement(tree, itn, x, y + layerSpacing, depth + 1, oriLayout, oriSize);

    if (minX + nodeWidth > maxX)
      maxX = minX + nodeWidth;
  }

  for (; itN->hasNext();) {
    node itn = itN->next();
    x += nodeSpacing;
    x = dfsPlacement(tree, itn, x, y + layerSpacing, depth + 1, oriLayout, oriSize);

    if (x > maxX)
      maxX = x;

    if (x < minX)
      minX = x;
  }

  delete itN;
  x = (minX + maxX)/2;
  oriLayout->setNodeValue(n, OrientableCoord(oriLayout, x, y, 0));
  return maxX;
}

TreeLeaf::TreeLeaf(const tlp::PluginContext* context):LayoutAlgorithm(context) {
  addNodeSizePropertyParameter(this);
  addOrientationParameters(this);
  addInParameter<bool>("uniform layer spacing", paramHelp[0], "true");
  addSpacingParameters(this);
}

TreeLeaf::~TreeLeaf() {}

bool TreeLeaf::run() {
  orientationType mask = getMask(dataSet);
  OrientableLayout oriLayout(result, mask);
  SizeProperty* size;

  if (!getNodeSizePropertyParameter(dataSet, size))
    size = graph->getProperty<SizeProperty>("viewSize");

  uniformLayerDistance = true;

  if (dataSet != NULL)
    dataSet->get("uniform layer spacing", uniformLayerDistance);

  OrientableSizeProxy oriSize(size, mask);
  getSpacingParameters(dataSet, nodeSpacing, minLayerSpacing);

  if (pluginProgress)
    pluginProgress->showPreview(false);

  // push a temporary graph state (not redoable)
  // preserving layout updates
  std::vector<PropertyInterface*> propsToPreserve;

  if (result->getName() != "")
    propsToPreserve.push_back(result);

  graph->push(false, &propsToPreserve);

  Graph *tree = TreeTest::computeTree(graph, pluginProgress);

  if (pluginProgress && pluginProgress->state() != TLP_CONTINUE) {
    graph->pop();
    return false;
  }

  node root = tree->getSource();

  if (!root.isValid())
    // graph is empty
    return true;

  computeLevelHeights(tree, root, 0, &oriSize);

  // check if the specified layer spacing is greater
  // than the max of the minimum layer spacing of the tree
  if (uniformLayerDistance == true) {
    for (unsigned int i = 0; i < levelHeights.size() - 1;  ++i) {
      float layerSpacing = (levelHeights[i] + levelHeights[i + 1]) / 2 + nodeSpacing;

      if (layerSpacing > minLayerSpacing)
        minLayerSpacing = layerSpacing;
    }
  }

  dfsPlacement(tree, root, 0, 0, 0, &oriLayout, &oriSize);

  // forget last temporary graph state
  graph->pop();

  return true;
}
