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
#include <iostream>
#include <cmath>
#include <algorithm>
#include <tulip/GraphTools.h>
#include "DatasetTools.h"
#include "Orientation.h"
#include "ImprovedWalker.h"

using namespace std;
using namespace tlp;

PLUGIN(ImprovedWalker)

//====================================================================
const node  ImprovedWalker::BADNODE;

//====================================================================
class ImprovedWalkerIterator : public Iterator<node> {
private:
  Graph* graph;
  node        n;
  int         currentChild;
  int         endChild;
  bool        isReversed;

public:
  ImprovedWalkerIterator(Graph* graphParam, node nParam,
                         int currentChildParam, int endChildParam) :
    graph(graphParam), n(nParam), currentChild(currentChildParam),
    endChild(endChildParam) {
    isReversed = currentChild > endChild;
  }

  bool hasNext() {
    return currentChild != endChild;
  }

  node next() {
    node tmp = graph->getOutNode(n, currentChild);

    if (isReversed)
      currentChild--;
    else
      currentChild++;

    return tmp;
  }
};
//====================================================================
ImprovedWalker::ImprovedWalker(const tlp::PluginContext* context) :
  LayoutAlgorithm(context) {
  addNodeSizePropertyParameter(this);
  addOrientationParameters(this);
  addOrthogonalParameters(this);
  addSpacingParameters(this);
}
//====================================================================
ImprovedWalker::~ImprovedWalker() {
}
//====================================================================
bool ImprovedWalker::run() {
  if (pluginProgress)
    pluginProgress->showPreview(false);

  // push a temporary graph state (not redoable)
  // preserving layout updates
  std::vector<PropertyInterface*> propsToPreserve;

  if (result->getName() != "")
    propsToPreserve.push_back(result);

  graph->push(false, &propsToPreserve);

  result->setAllEdgeValue(vector<Coord>());

  tree = TreeTest::computeTree(graph, pluginProgress);

  if (pluginProgress && pluginProgress->state() != TLP_CONTINUE) {
    graph->pop();
    return false;
  }



  node root = tree->getSource();
  orientationType mask = getMask(dataSet);
  oriLayout = new OrientableLayout(result, mask);
  SizeProperty* size;

  if (!getNodeSizePropertyParameter(dataSet, size))
    size = graph->getProperty<SizeProperty>("viewSize");

  getSpacingParameters(dataSet, nodeSpacing, spacing);

  oriSize                   = new OrientableSizeProxy(size, mask);
  depthMax                  = initializeAllNodes(root);
  order[root]               = 1;

  firstWalk(root);

  // check if the specified layer spacing is greater
  // than the max of the minimum layer spacing of the tree
  for (unsigned int i = 0; i < maxYbyLevel.size() - 1;  ++i) {
    float minLayerSpacing = (maxYbyLevel[i] + maxYbyLevel[i + 1]) / 2.f;

    if (minLayerSpacing + nodeSpacing > spacing)
      spacing = minLayerSpacing + nodeSpacing;
  }

  secondWalk(root,0,0);

  if (hasOrthogonalEdge(dataSet))
    oriLayout->setOrthogonalEdge(tree, spacing);

  // forget last temporary graph state
  graph->pop();

  delete oriLayout;
  delete oriSize;
  return true;
}
//====================================================================
int ImprovedWalker::initializeAllNodes(tlp::node root) {
  return initializeNode(root, 0);
}
//====================================================================
int ImprovedWalker::initializeNode(tlp::node n, unsigned int depth) {
  if (maxYbyLevel.size() == depth) {
    maxYbyLevel.push_back(0);
  }

  float nodeHeight       = oriSize->getNodeValue(n).getH();
  maxYbyLevel[depth]     = max(maxYbyLevel[depth], nodeHeight);

  prelimX[n]             = 0;
  modChildX[n]           = 0;

  shiftNode[n]           = 0;
  shiftDelta[n]          = 0;

  ancestor[n]            = n;
  thread[n]              = BADNODE;

  int maxDepth           = 0;
  int count              = 0;
  Iterator<node>* itNode = tree->getOutNodes(n);

  while (itNode->hasNext()) {
    node currentNode   = itNode->next();
    order[currentNode] = ++count;
    int treeDepth      = initializeNode(currentNode, depth + 1);
    maxDepth           = max(treeDepth, maxDepth);
  }

  delete itNode;

  return maxDepth + 1;
}
//====================================================================
int ImprovedWalker::countSibling(tlp::node from, tlp::node to) {
  return abs(order[from] - order[to]);
}
//====================================================================
ImprovedWalkerIterator* ImprovedWalker::iterateSibling(tlp::node from, tlp::node to) {
  int modifier = (order[from] > order[to] ? 1 : -1 );
  node father  = tree->getInNode(from,1);

  return new ImprovedWalkerIterator(tree,father, order[from],
                                    order[to]+modifier);
}
//====================================================================
Iterator<node>* ImprovedWalker::getChildren(tlp::node n) {
  return tree->getOutNodes(n);
}
//====================================================================
ImprovedWalkerIterator* ImprovedWalker::getReversedChildren(tlp::node n) {
  int nbChildren = tree->outdeg(n);
  return new ImprovedWalkerIterator(tree, n, nbChildren, 0);
}

//====================================================================
void ImprovedWalker::firstWalk(tlp::node v) {
  if (isLeaf(tree, v)) {
    prelimX[v]        = 0;
    node vleftSibling = leftSibling(v);

    if (vleftSibling  != BADNODE)
      prelimX[v]     += prelimX[vleftSibling] + nodeSpacing
                        + oriSize->getNodeValue(v).getW()/2.f
                        + oriSize->getNodeValue(vleftSibling).getW()/2.f;
  }
  else {
    node defaultAncestor    = leftmostChild(v);
    Iterator<node>* itNode  = getChildren(v);

    while (itNode->hasNext()) {
      node currentNode    = itNode->next();
      firstWalk(currentNode);
      combineSubtree(currentNode,&defaultAncestor);
    }

    delete itNode;
    executeShifts(v);

    float midPoint   = (  prelimX[leftmostChild(v)]
                          + prelimX[rightmostChild(v)])/2.f;

    node leftBrother = leftSibling(v);

    if (leftBrother != BADNODE) {
      prelimX[v]   =  prelimX[leftBrother] + nodeSpacing
                      + oriSize->getNodeValue(v).getW()/2.f
                      + oriSize->getNodeValue(leftBrother).getW()/2.f;
      modChildX[v] = prelimX[v] - midPoint;
    }
    else
      prelimX[v] = midPoint;
  }
}
//====================================================================
void ImprovedWalker::secondWalk(tlp::node v, float modifierX, int depth) {
  OrientableCoord coord  = oriLayout->createCoord(prelimX[v]+modifierX,
                           depth * spacing, 0);
  oriLayout->setNodeValue(v,coord);
  Iterator<node>* itNode = getChildren(v);

  while (itNode->hasNext())
    secondWalk(itNode->next(),modifierX+modChildX[v],depth+1);

  delete itNode;
}
//====================================================================
void ImprovedWalker::combineSubtree(tlp::node v, tlp::node* defaultAncestor) {
  node leftBrother = leftSibling(v);

  if (leftBrother != BADNODE) {
    node nodeInsideRight;
    node nodeOutsideRight;
    node nodeInsideLeft;
    node nodeOutsideLeft;

    float shiftInsideRight;
    float shiftOutsideRight;
    float shiftInsideLeft;
    float shiftOutsideLeft;

    nodeInsideRight   = v;
    nodeOutsideRight  = v;
    nodeInsideLeft    = leftBrother;
    nodeOutsideLeft   = leftMostSibling(nodeInsideRight);

    shiftInsideRight  = modChildX[nodeInsideRight];
    shiftOutsideRight = modChildX[nodeOutsideRight];
    shiftInsideLeft   = modChildX[nodeInsideLeft];
    shiftOutsideLeft  = modChildX[nodeOutsideLeft];

    while (nextRightContour(nodeInsideLeft)  != BADNODE &&
           nextLeftContour (nodeInsideRight) != BADNODE) {

      nodeInsideLeft    = nextRightContour(nodeInsideLeft);
      nodeInsideRight   = nextLeftContour(nodeInsideRight);

      if (nodeOutsideLeft.isValid())
        nodeOutsideLeft   = nextLeftContour(nodeOutsideLeft);

      if (nodeOutsideRight.isValid())
        nodeOutsideRight  = nextRightContour(nodeOutsideRight);

      ancestor[nodeOutsideRight] = v;

      float shift =   (prelimX[nodeInsideLeft]  + shiftInsideLeft)
                      - (prelimX[nodeInsideRight] + shiftInsideRight)
                      + nodeSpacing
                      + oriSize->getNodeValue(nodeInsideLeft).getW()/2.f
                      + oriSize->getNodeValue(nodeInsideRight).getW()/2.f;

      if (shift > 0) {
        node ancest = findCommonAncestor(nodeInsideLeft, v,
                                         *defaultAncestor);
        moveSubtree(ancest, v, shift);
        shiftInsideRight   += shift;
        shiftOutsideRight  += shift;
      }

      shiftInsideRight  += modChildX[nodeInsideRight];
      shiftOutsideRight += modChildX[nodeOutsideRight];
      shiftInsideLeft   += modChildX[nodeInsideLeft];
      shiftOutsideLeft  += modChildX[nodeOutsideLeft];
    }

    if (nextRightContour(nodeInsideLeft)   != BADNODE &&
        nextRightContour(nodeOutsideRight) == BADNODE) {
      thread[nodeOutsideRight]    = nextRightContour(nodeInsideLeft);
      modChildX[nodeOutsideRight] += shiftInsideLeft - shiftOutsideRight;
    }

    if (nextLeftContour (nodeInsideRight) != BADNODE &&
        nextLeftContour (nodeOutsideLeft) == BADNODE) {
      thread[nodeOutsideLeft]      =  nextLeftContour(nodeInsideRight);
      modChildX[nodeOutsideLeft]   += shiftInsideRight - shiftOutsideLeft;

      *defaultAncestor              =  v;
    }
  }
}
//====================================================================
void ImprovedWalker::moveSubtree(tlp::node fromNode, tlp::node toNode, float rightShift) {
  int nbElementSubtree  = countSibling(toNode,fromNode);
  float shiftByElem     = rightShift/nbElementSubtree;

  shiftDelta[toNode]    -= shiftByElem;
  shiftNode[toNode]     += rightShift;
  shiftDelta[fromNode]  += shiftByElem;

  prelimX[toNode]       += rightShift;
  modChildX[toNode]     += rightShift;
}
//====================================================================
void ImprovedWalker::executeShifts(tlp::node v) {

  float shift = 0;
  float delta = 0;
  Iterator<node>* itNode = getReversedChildren(v);

  while(itNode->hasNext()) {
    node currentNode       = itNode-> next();

    prelimX[currentNode]   += shift;
    modChildX[currentNode] += shift;

    delta                  += shiftDelta[currentNode];
    shift                  += shiftNode[currentNode] + delta;
  }

  delete itNode;
}
//====================================================================
