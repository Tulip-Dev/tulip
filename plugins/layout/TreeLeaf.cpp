/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
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


LAYOUTPLUGINOFGROUP(TreeLeaf,"Tree Leaf","David Auber","01/12/1999","ok","1.0","Tree");

using namespace std;
using namespace tlp;


void TreeLeaf::computeLevelHeights(Graph *tree, node n, unsigned int depth,
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
  
float TreeLeaf::dfsPlacement(Graph* tree, node n, float x, float y, unsigned int depth,
			     OrientableLayout *oriLayout, OrientableSizeProxy *oriSize) {
  float minX = 0;
  float maxX = 0;
  float nodeWidth = oriSize->getNodeValue(n).getW();
  if (tree->outdeg(n) == 0) {
    oriLayout->setNodeValue(n, OrientableCoord(oriLayout, x + nodeWidth/2, y, 0));
    return x + nodeWidth;
  }
  Iterator<node> *itN=tree->getOutNodes(n);
  if (itN->hasNext()) {
    node itn = itN->next();
    minX = x;
    maxX = x = dfsPlacement(tree, itn, x, y + spacing, depth + 1, oriLayout, oriSize);
    if (minX + nodeWidth > maxX)
      maxX = minX + nodeWidth;
  }
  for (;itN->hasNext();) {
    node itn = itN->next();
    x += nodeSpacing;
    x = dfsPlacement(tree, itn, x, y + spacing, depth + 1, oriLayout, oriSize);
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

TreeLeaf::TreeLeaf(const PropertyContext &context):LayoutAlgorithm(context){
  addNodeSizePropertyParameter(this);
  addOrientationParameters(this);
  addSpacingParameters(this);
}

TreeLeaf::~TreeLeaf() {}

bool TreeLeaf::run() {
  orientationType mask = getMask(dataSet);
  OrientableLayout oriLayout(layoutResult, mask);
  SizeProperty* size;
  if (!getNodeSizePropertyParameter(dataSet, size))
    size = graph->getProperty<SizeProperty>("viewSize");
  OrientableSizeProxy oriSize(size, mask);
  getSpacingParameters(dataSet, nodeSpacing, spacing);

  if (pluginProgress)
    pluginProgress->showPreview(false);
  Graph *tree = TreeTest::computeTree(graph, 0, false, pluginProgress);
  if (pluginProgress && pluginProgress->state() != TLP_CONTINUE)
    return false;

  node root;
  if (!tlp::getSource(tree, root))
    // graph is empty
    return true;

  computeLevelHeights(tree, root, 0, &oriSize);
  // check if the specified layer spacing is greater
  // than the max of the minimum layer spacing of the tree
  for (unsigned int i = 0; i < levelHeights.size() - 1;  ++i) {
    float minLayerSpacing = (levelHeights[i] + levelHeights[i + 1]) / 2;
    if (minLayerSpacing + nodeSpacing > spacing)
      spacing = minLayerSpacing + nodeSpacing;
  }
  dfsPlacement(tree, root, 0, 0, 0, &oriLayout, &oriSize);

  TreeTest::cleanComputedTree(graph, tree);
  return true;
}
