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
#include <cmath>
#include <algorithm>
#include <tulip/GraphTools.h>
#include "TreeTools.h"
#include "Orientation.h"
#include "DatasetTools.h"
#include "EdgeTools.h"
#include "Dendrogram.h"
using namespace std;
using namespace tlp;


LAYOUTPLUGINOFGROUP(Dendrogram, "Dendrogram",
             "Julien Testut, Antony Durand, Pascal Ollier, "
             "Yashvin Nababsing, Sebastien Leclerc, "
             "Thibault Ruchon, Eric Dauchier",
	     "03/12/04", "ok", "1.0","Tree");

//====================================================================
Dendrogram::Dendrogram(const PropertyContext& context)
    :LayoutAlgorithm(context) {
  addNodeSizePropertyParameter(this);
  addOrientationParameters(this);
  addSpacingParameters(this);
}

//====================================================================
Dendrogram::~Dendrogram() {
}
//====================================================================
void Dendrogram::computeLevelHeights(Graph *tree, node n, unsigned int depth,
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
//====================================================================
bool Dendrogram::run() {
  orientationType mask = getMask(dataSet);
  OrientableLayout oriLayout(layoutResult, mask);
  SizeProperty* size;
  if (!getNodeSizePropertyParameter(dataSet, size))
    size = graph->getProperty<SizeProperty>("viewSize");
  OrientableSizeProxy oriSize(size, mask);
  getSpacingParameters(dataSet, nodeSpacing, spacing);

  if (pluginProgress)
    pluginProgress->showPreview(false);
  tree = TreeTest::computeTree(graph, 0, false, pluginProgress);
  if (pluginProgress && pluginProgress->state() != TLP_CONTINUE)
    return false;

  tlp::getSource(tree, root);
  computeLevelHeights(tree, root, 0, &oriSize);
  // check if the specified layer spacing is greater
  // than the max of the minimum layer spacing of the tree
  for (unsigned int i = 0; i < levelHeights.size() - 1;  ++i) {
    float minLayerSpacing = (levelHeights[i] + levelHeights[i + 1]) / 2;
    if (minLayerSpacing + nodeSpacing > spacing)
      spacing = minLayerSpacing + nodeSpacing;
  }

  setAllNodesCoordX(root, 0.f, &oriLayout, &oriSize);
  shiftAllNodes(root, 0.f, &oriLayout);
  setAllNodesCoordY(&oriLayout, &oriSize);
  setOrthogonalEdge(&oriLayout, graph, spacing);

  TreeTest::cleanComputedTree(graph, tree);
  return true;
}

//====================================================================
float Dendrogram::setAllNodesCoordX(node n, float rightMargin,
				    OrientableLayout *oriLayout, OrientableSizeProxy *oriSize) {
  float leftMargin       = rightMargin;

  Iterator<node>* itNode = tree->getOutNodes(n);
  while (itNode->hasNext()) {
    node currentNode   = itNode->next();
    leftMargin         = setAllNodesCoordX(currentNode, leftMargin, oriLayout, oriSize);
  }
  delete itNode;
 
  const float nodeWidth  =  oriSize->getNodeValue(n).getW()
    + nodeSpacing;

  if (isLeaf(tree, n))
    leftMargin = rightMargin + nodeWidth;               
  const float freeRange  = leftMargin - rightMargin;

  float posX;
  if (isLeaf(tree, n))
    posX = freeRange / 2.f + rightMargin; 
  else
    posX = computeFatherXPosition(n, oriLayout);
    
  const float rightOverflow = max(rightMargin-(posX-nodeWidth/2.f), 0.f);
  const float leftOverflow  = max((posX+nodeWidth/2.f)-leftMargin, 0.f);
  leftshift[n]              = rightOverflow;
    
  setNodePosition(n, posX, 0.f, 0.f, oriLayout);
  return  leftMargin + leftOverflow + rightOverflow;
}

//====================================================================
void Dendrogram::setAllNodesCoordY(OrientableLayout *oriLayout,
				   OrientableSizeProxy *oriSize) {
  float maxYLeaf         = -FLT_MAX;
  setCoordY(root, &maxYLeaf, oriLayout, oriSize);
    
  Iterator<node>* itNode = tree->getNodes();
  while (itNode->hasNext()) {
    node currentNode   = itNode->next();
    if (isLeaf(tree,currentNode)) {
      OrientableCoord coord = oriLayout->getNodeValue(currentNode);
      float newY            = maxYLeaf;
      float coordX          = coord.getX();
      float coordZ          = coord.getZ();
      setNodePosition(currentNode, coordX, newY, coordZ, oriLayout);
    }
  }
  delete itNode;
}

//====================================================================
float Dendrogram::computeFatherXPosition(node father, OrientableLayout *oriLayout) {
  float minX             =  FLT_MAX;
  float maxX             = -FLT_MAX;
    
  Iterator<node> *itNode =  tree->getOutNodes(father);    
  while (itNode->hasNext()) {
    node currentNode   = itNode->next();
    const float x      =  oriLayout->getNodeValue(currentNode).getX()
      + leftshift[currentNode];
    minX               = min(minX, x);
    maxX               = max(maxX, x);
  }
  delete itNode;
  return (maxX + minX) / 2.f;
}   

//====================================================================
void Dendrogram::shiftAllNodes(node n, float shift, OrientableLayout *oriLayout) {
  OrientableCoord coord   =  oriLayout->getNodeValue(n);
  shift                  +=  leftshift[n];
  float coordX            =  coord.getX();
    
  coord.setX(coordX + shift);
  oriLayout->setNodeValue(n, coord);
    
  Iterator<node>* itNode  =   tree->getOutNodes(n);
  while (itNode->hasNext()) 
    shiftAllNodes(itNode->next(), shift, oriLayout);
  delete itNode;
}

//====================================================================
inline void Dendrogram::setNodePosition(node n, float x, float y, 
					float z, OrientableLayout *oriLayout) {
  OrientableCoord coord = oriLayout->createCoord(x, y, z);
  oriLayout->setNodeValue(n, coord);
}

//====================================================================
void Dendrogram::setCoordY(node n, float* maxYLeaf,
			   OrientableLayout *oriLayout, OrientableSizeProxy *oriSize) {
  float nodeY;
  if (tree->indeg(n) != 0) {
    node fatherNode             = tree->getInNode(n, 1);
    OrientableCoord coord       = oriLayout->getNodeValue(n); 
    OrientableCoord coordFather = oriLayout->getNodeValue(fatherNode);
    nodeY                       = coordFather.getY() + spacing;
                               
    coord.setY(nodeY);
    oriLayout->setNodeValue(n, coord);
    if (isLeaf(tree, n)) {
      (*maxYLeaf)         = max((*maxYLeaf), nodeY);                       
    }
                                
  }    
    
  Iterator<node> *itNode = tree->getOutNodes(n);
  while (itNode->hasNext()) 
    setCoordY(itNode->next(), maxYLeaf, oriLayout, oriSize); 
  delete itNode;
}
