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
  y += levelHeights[depth]/2;
  if (depth > 1)
    y += levelHeights[depth - 1]/ 2;
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
  spacing = 64.0;
  nodeSpacing = 18.0;

  orientationType mask = getMask(dataSet);
  OrientableLayout oriLayout(layoutResult, mask);
  SizeProperty* size = graph->getProperty<SizeProperty>("viewSize");
  if (dataSet!=0) {
    getNodeSizePropertyParameter(dataSet, size);
    getSpacingParameters(dataSet, nodeSpacing, spacing);
  }
  OrientableSizeProxy oriSize(size, mask);

  if (pluginProgress)
    pluginProgress->showPreview(false);
  Graph *tree = computeTree(graph, 0, false, pluginProgress);
  if (pluginProgress && pluginProgress->state() != TLP_CONTINUE)
    return false;

  node root;
  if (!tlp::getSource(tree, root))
    // graph is empty
    return true;

  computeLevelHeights(tree, root, 0, &oriSize);
  dfsPlacement(tree, root, 0, -levelHeights[0]/2, 0, &oriLayout, &oriSize);

  cleanComputedTree(graph, tree);
  return true;
}
