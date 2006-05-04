#ifndef IMPROVEDWALKER_H
#define IMPROVEDWALKER_H

#include <map>
#include <vector>
#include <tulip/TulipPlugin.h>
#include "TreeTools.h"

class OrientableLayout;
class OrientableCoord;
class OrientableSizeProxy;
class ImprovedWalkerIterator;

/** \addtogroup layout */
/*@{*/
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

class ImprovedWalker : public LayoutAlgorithm {
  friend class ImprovedWalkerUnitTests;

 public:
  ImprovedWalker(const PropertyContext&);
  ~ImprovedWalker();

  bool run();
  bool check(std::string&);
  void reset();

 private:
  typedef std::vector<float>      levelToFloatType;
  typedef std::map<node, float>   nodeToFloatType;
  typedef std::map<node, int>     nodeToIntegerPropertyType;
  typedef std::map<node, node>    nodeToNodeType;

  static const float      INTER_NODE_DISTANCE_X;
  static const float      INTER_NODE_DISTANCE_Y;
  static const node       BADNODE;

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

  int                     initializeAllNodes();
  int                     initializeNode(node root, unsigned int depth);
  int                     countSibling(node from, node to);
  ImprovedWalkerIterator* iterateSibling(node from, node to);
  Iterator<node>*         getChildren(node n);
  ImprovedWalkerIterator* getReversedChildren(node n);

  void                    firstWalk(node v);
  void                    secondWalk(node v, float modifierX, int depth);
  void                    combineSubtree(node v, node* defaultAncestor);
  void                    moveSubtree(node fromNode, node toNode, 
				      float rightShift);
  void                    executeShifts(node v);
    
  inline node getFather(node n);

  inline node leftmostChild(node n);
  inline node rightmostChild(node n);

  inline node leftSibling(node n);    
  inline node rightSibling(node n);
  inline node leftMostSibling(node n);

  inline node nextRightContour(node v);
  inline node nextLeftContour(node v);
  inline node findCommonAncestor(node left, node right, node defaultAncestor);
};

//====================================================================    
inline node ImprovedWalker::getFather(node n) {
   if (graph->indeg(n)<1)
        return BADNODE;
    return graph->getInNode(n,1);
}

//====================================================================    
inline node ImprovedWalker::leftmostChild(node n) {    
    if (graph->outdeg(n)<1)
        return BADNODE;
    return graph->getOutNode(n,1);
}

//====================================================================
inline node ImprovedWalker::rightmostChild(node n) {
    int pos;
    if ((pos=graph->outdeg(n))<1)
        return BADNODE;        
    return graph->getOutNode(n,pos);
}

//====================================================================    
inline node ImprovedWalker::leftSibling(node n) {    
    if (order[n]<=1)
        return BADNODE; 
    else
        return graph->getOutNode( getFather(n) ,order[n]-1);
}

//====================================================================
inline node ImprovedWalker::rightSibling(node n) {
    node father=getFather(n);
    if (order[n]>=int(graph->outdeg(father)))
        return BADNODE;     
    return graph->getOutNode(father ,order[n]+1);
}

//====================================================================
inline node ImprovedWalker::leftMostSibling(node n) {
    node father=getFather(n);
    return leftmostChild(father);
}

//====================================================================
inline node ImprovedWalker::nextRightContour(node n) {
    if (isLeaf(graph, n))
        return thread[n];
    else
        return rightmostChild(n);
}

//====================================================================
inline node ImprovedWalker::nextLeftContour(node n) {
    if (isLeaf(graph, n))
        return thread[n];
    else
        return leftmostChild(n);
}

//====================================================================
inline node ImprovedWalker::findCommonAncestor(node left, node right, 
                                               node defaultAncestor) {
    if (getFather(ancestor[left]) == getFather(right) /*&& left!=right*/)
        return ancestor[left];
    else
        return defaultAncestor;
}
/*@}*/
#endif
