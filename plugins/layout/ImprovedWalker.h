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
#ifndef IMPROVEDWALKER_H
#define IMPROVEDWALKER_H

#include <map>
#include <vector>
#include <tulip/TulipPluginHeaders.h>
#include "TreeTools.h"

class OrientableLayout;
class OrientableCoord;
class OrientableSizeProxy;
class ImprovedWalkerIterator;

/** \addtogroup layout */

/** This plugin is an implementation of a linear Walker's algorithm:
 *
 *  Christoph Buchheim and Michael Junger and Sebastian Leipert,
 *  Improving Walker's Algorithm to Run in Linear Time
 *  citeseer.ist.psu.edu/buchheim02improving.html
 *
 *  \note This algorith works on tree.
 *  Let n be the number of nodes, the algorithm complexity is in O(n).
 *
 *  \author Julien Testut, Antony Durand, Pascal Ollier, Yashvin Nababsing, \n
 *  Sebastien Leclerc, Thibault Ruchon, Eric Dauchier \n
 *  University Bordeaux I France
 **/

class ImprovedWalker : public tlp::LayoutAlgorithm {
  friend class ImprovedWalkerUnitTests;

public:
  PLUGININFORMATION( "Improved Walker",
                     "Julien Testut, Antony Durand, Pascal Ollier, "
                     "Yashvin Nababsing, Sebastien Leclerc, "
                     "Thibault Ruchon, Eric Dauchier",
                     "11/11/04", "ok", "1.0","Tree")
  ImprovedWalker(const tlp::PluginContext* context);
  ~ImprovedWalker();

  bool run();

private:
  typedef std::vector<float>      levelToFloatType;
  typedef std::map<tlp::node, float>   nodeToFloatType;
  typedef std::map<tlp::node, int>     nodeToIntegerPropertyType;
  typedef std::map<tlp::node, tlp::node>    nodeToNodeType;

  tlp::Graph *tree;

  float spacing;
  float nodeSpacing;
  static const tlp::node       BADNODE;

  OrientableLayout*       oriLayout;
  OrientableSizeProxy*    oriSize;

  int                     depthMax;
  nodeToIntegerPropertyType           order;
  levelToFloatType        maxYbyLevel;
  levelToFloatType        posYbyLevel;
  nodeToFloatType         prelimX;
  nodeToFloatType         modChildX;
  nodeToNodeType          thread;
  nodeToFloatType         shiftNode;
  nodeToFloatType         shiftDelta;
  nodeToNodeType          ancestor;

  int                     initializeAllNodes(tlp::node root);
  int                     initializeNode(tlp::node root, unsigned int depth);
  int                     countSibling(tlp::node from, tlp::node to);
  ImprovedWalkerIterator* iterateSibling(tlp::node from, tlp::node to);
  tlp::Iterator<tlp::node>*         getChildren(tlp::node n);
  ImprovedWalkerIterator* getReversedChildren(tlp::node n);

  void                    firstWalk(tlp::node v);
  void                    secondWalk(tlp::node v, float modifierX, int depth);
  void                    combineSubtree(tlp::node v, tlp::node* defaultAncestor);
  void                    moveSubtree(tlp::node fromNode, tlp::node toNode,
                                      float rightShift);
  void                    executeShifts(tlp::node v);

  inline tlp::node getSuperGraph(tlp::node n);

  inline tlp::node leftmostChild(tlp::node n);
  inline tlp::node rightmostChild(tlp::node n);

  inline tlp::node leftSibling(tlp::node n);
  inline tlp::node rightSibling(tlp::node n);
  inline tlp::node leftMostSibling(tlp::node n);

  inline tlp::node nextRightContour(tlp::node v);
  inline tlp::node nextLeftContour(tlp::node v);
  inline tlp::node findCommonAncestor(tlp::node left, tlp::node right, tlp::node defaultAncestor);
};

//====================================================================
inline tlp::node ImprovedWalker::getSuperGraph(tlp::node n) {
  if (tree->indeg(n)<1)
    return BADNODE;

  return tree->getInNode(n,1);
}

//====================================================================
inline tlp::node ImprovedWalker::leftmostChild(tlp::node n) {
  if (tree->outdeg(n)<1)
    return BADNODE;

  return tree->getOutNode(n,1);
}

//====================================================================
inline tlp::node ImprovedWalker::rightmostChild(tlp::node n) {
  int pos;

  if ((pos=tree->outdeg(n))<1)
    return BADNODE;

  return tree->getOutNode(n,pos);
}

//====================================================================
inline tlp::node ImprovedWalker::leftSibling(tlp::node n) {
  if (order[n]<=1)
    return BADNODE;
  else
    return tree->getOutNode( getSuperGraph(n) ,order[n]-1);
}

//====================================================================
inline tlp::node ImprovedWalker::rightSibling(tlp::node n) {
  tlp::node father=getSuperGraph(n);

  if (order[n]>=int(tree->outdeg(father)))
    return BADNODE;

  return tree->getOutNode(father ,order[n]+1);
}

//====================================================================
inline tlp::node ImprovedWalker::leftMostSibling(tlp::node n) {
  tlp::node father=getSuperGraph(n);
  return leftmostChild(father);
}

//====================================================================
inline tlp::node ImprovedWalker::nextRightContour(tlp::node n) {
  if (isLeaf(tree, n))
    return thread[n];
  else
    return rightmostChild(n);
}

//====================================================================
inline tlp::node ImprovedWalker::nextLeftContour(tlp::node n) {
  if (isLeaf(tree, n))
    return thread[n];
  else
    return leftmostChild(n);
}

//====================================================================
inline tlp::node ImprovedWalker::findCommonAncestor(tlp::node left, tlp::node right,
    tlp::node defaultAncestor) {
  if (getSuperGraph(ancestor[left]) == getSuperGraph(right) /*&& left!=right*/)
    return ancestor[left];
  else
    return defaultAncestor;
}

#endif
