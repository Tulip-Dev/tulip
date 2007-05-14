#include <iostream>
#include <cmath>
#include <algorithm>
#include <tulip/GraphTools.h>
#include "DatasetTools.h"
#include "EdgeTools.h"
#include "Orientation.h"
#include "ImprovedWalker.h"

using namespace std;
using namespace tlp;

LAYOUTPLUGINOFGROUP(ImprovedWalker, "Improved Walker",
		    "Julien Testut, Antony Durand, Pascal Ollier, "
		    "Yashvin Nababsing, Sebastien Leclerc, "
		    "Thibault Ruchon, Eric Dauchier",
		    "11/11/04", "ok", "1.0","Tree");

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
ImprovedWalker::ImprovedWalker(const PropertyContext& context) :
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
  spacing = 64.0;
  nodeSpacing = 18.0;

  tree = computeTree(graph);

  node root;
  tlp::getSource(tree, root);
  orientationType mask = getMask(dataSet);
  oriLayout = new OrientableLayout(layoutResult, mask);
  SizeProperty* size = graph->getProperty<SizeProperty>("viewSize");
  if (dataSet!=0) {
    getNodeSizePropertyParameter(dataSet, size);
    getSpacingParameters(dataSet, nodeSpacing, spacing);
  }
  oriSize                   = new OrientableSizeProxy(size, mask);
  depthMax                  = initializeAllNodes(root);    
  order[root]               = 1;
  
  firstWalk(root);
        
  float sumY                       = 0;
  levelToFloatType::iterator itMax = maxYbyLevel.begin();
  levelToFloatType::iterator itPos = posYbyLevel.begin();
  while(itMax != maxYbyLevel.end()) {
    *itPos = sumY + (spacing + *itMax)/2.f;
    sumY   += *itMax + spacing;
    ++itMax;
    ++itPos;
  }     
  secondWalk(root,0,0);

  if (hasOrthogonalEdge(dataSet))
    setOrthogonalEdge(oriLayout, oriSize, tree, spacing);
    
  cleanComputedTree(graph, tree);

  delete oriLayout;
  delete oriSize;
  return true;
}
//====================================================================
int ImprovedWalker::initializeAllNodes(node root) {    
  return initializeNode(root, 0);
}
//====================================================================
int ImprovedWalker::initializeNode(node n, unsigned int depth) {    
  if (maxYbyLevel.size() <= depth) {
    posYbyLevel.resize(maxYbyLevel.size() + 10);
    maxYbyLevel.resize(maxYbyLevel.size() + 10);   
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
int ImprovedWalker::countSibling(node from, node to) {
  return abs(order[from] - order[to]);
}
//====================================================================
ImprovedWalkerIterator* ImprovedWalker::iterateSibling(node from, node to) {
  int modifier = (order[from] > order[to] ? 1 : -1 );
  node father  = tree->getInNode(from,1);
  
  return new ImprovedWalkerIterator(tree,father, order[from],
				    order[to]+modifier);
}
//====================================================================
Iterator<node>* ImprovedWalker::getChildren(node n) {    
    return tree->getOutNodes(n);
}
//====================================================================
ImprovedWalkerIterator* ImprovedWalker::getReversedChildren(node n) {
  int nbChildren = tree->outdeg(n);
  return new ImprovedWalkerIterator(tree, n, nbChildren, 0);
}

//==================================================================== 
void ImprovedWalker::firstWalk(node v) {       
  if (isLeaf(tree, v)) {   
    prelimX[v]        = 0;
    node vleftSibling = leftSibling(v);
    if (vleftSibling  != BADNODE)
      prelimX[v]     += prelimX[vleftSibling] + nodeSpacing
	+ oriSize->getNodeValue(v).getW()/2.
	+ oriSize->getNodeValue(vleftSibling).getW()/2.;
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
void ImprovedWalker::secondWalk(node v, float modifierX, int depth) {   
    OrientableCoord coord  = oriLayout->createCoord(prelimX[v]+modifierX,
                                                   posYbyLevel[depth], 0);        
    oriLayout->setNodeValue(v,coord);
    Iterator<node>* itNode = getChildren(v);
    while (itNode->hasNext())
        secondWalk(itNode->next(),modifierX+modChildX[v],depth+1);
    delete itNode;
}   
//====================================================================
void ImprovedWalker::combineSubtree(node v, node* defaultAncestor) {
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
void ImprovedWalker::moveSubtree(node fromNode, node toNode, float rightShift) {
    int nbElementSubtree  = countSibling(toNode,fromNode);
    float shiftByElem     = rightShift/nbElementSubtree;

    shiftDelta[toNode]    -= shiftByElem;
    shiftNode[toNode]     += rightShift;    
    shiftDelta[fromNode]  += shiftByElem;
    
    prelimX[toNode]       += rightShift;
    modChildX[toNode]     += rightShift;   
}
//====================================================================
void ImprovedWalker::executeShifts(node v) {   
                
   float shift = 0;
   float delta = 0;   
   Iterator<node>* itNode = getReversedChildren(v);
   while(itNode->hasNext()) {
     node currentNode       = itNode-> next();
     
     prelimX[currentNode]   += shift;
     modChildX[currentNode] += shift;
     
     delta                  += shiftDelta[currentNode];
     shift                  += shiftNode[currentNode] + delta;          
   } delete itNode;
}
//====================================================================
