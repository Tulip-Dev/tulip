#include <iostream>
#include <cmath>
#include <algorithm>
#include <tulip/TreeTest.h>
#include "DatasetTools.h"
#include "EdgeTools.h"
#include "Orientation.h"
#include "ImprovedWalker.h"

using namespace std;
using namespace tlp;

LAYOUTPLUGINOFGROUP(ImprovedWalker, "ImprovedWalker",
             "Julien Testut, Antony Durand, Pascal Ollier, "
             "Yashvin Nababsing, Sebastien Leclerc, "
             "Thibault Ruchon, Eric Dauchier",
		    "11/11/04", "ok", "0", "1","Tree");

//====================================================================
const float ImprovedWalker::INTER_NODE_DISTANCE_X = 1;
const float ImprovedWalker::INTER_NODE_DISTANCE_Y = 1;
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
  addOrientationParameters(this);
    addOrthogonalParameters(this);
}
//====================================================================
ImprovedWalker::~ImprovedWalker() {
}
//====================================================================
bool ImprovedWalker::run() {
  node root                 = searchRoot(graph);
  orientationType mask      = getMask(dataSet);
  oriLayout                 = new OrientableLayout(layoutResult, mask);
  SizeProperty* viewSize = graph->getLocalProperty<SizeProperty>("viewSize");
  oriSize                   = new OrientableSizeProxy(viewSize, mask);
  depthMax                  = initializeAllNodes();    
  order[root]               = 1;
  
  firstWalk(root);
        
  float sumY                       = 0;
  levelToFloatType::iterator itMax = maxYbyLevel.begin();
  levelToFloatType::iterator itPos = posYbyLevel.begin();
  while(itMax != maxYbyLevel.end()) {
    *itPos = sumY + (INTER_NODE_DISTANCE_Y + *itMax)/2.f;
    sumY   += *itMax +  INTER_NODE_DISTANCE_Y;
    ++itMax;
    ++itPos;
  }     
  secondWalk(root,0,0);

  if (hasOrthogonalEdge(dataSet))
    setOrthogonalEdge(oriLayout, oriSize, graph,INTER_NODE_DISTANCE_Y);
    
  delete oriLayout;
  delete oriSize;
  return true;
}
//====================================================================
bool ImprovedWalker::check(string& errorMsg) {
  if (TreeTest::isTree(graph)) {
    errorMsg = "";
    return true;
  }
  else {
    errorMsg = "The Graph must be a Tree";
    return false;
  }
}
//====================================================================
void ImprovedWalker::reset() {
}

//====================================================================
int ImprovedWalker::initializeAllNodes() {    
  node root = searchRoot(graph);
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
  Iterator<node>* itNode = graph->getOutNodes(n);
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
  node father  = graph->getInNode(from,1);
  
  return new ImprovedWalkerIterator(graph,father, order[from],
				    order[to]+modifier);
}
//====================================================================
Iterator<node>* ImprovedWalker::getChildren(node n) {    
    return graph->getOutNodes(n);
}
//====================================================================
ImprovedWalkerIterator* ImprovedWalker::getReversedChildren(node n) {
  int nbChildren = graph->outdeg(n);
  return new ImprovedWalkerIterator(graph, n, nbChildren, 0);
}

//==================================================================== 
void ImprovedWalker::firstWalk(node v) {       
  if (isLeaf(graph, v)) {   
    prelimX[v]        = 0;
    node vleftSibling = leftSibling(v);
    if (vleftSibling  != BADNODE)
      prelimX[v]     += prelimX[vleftSibling] + INTER_NODE_DISTANCE_X
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
      prelimX[v]   =  prelimX[leftBrother] + INTER_NODE_DISTANCE_X
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
  node nodeInsideRight; 
  node nodeOutsideRight;
  node nodeInsideLeft;
  node nodeOutsideLeft;
  
  float shiftInsideRight; 
  float shiftOutsideRight; 
  float shiftInsideLeft; 
  float shiftOutsideLeft; 
    
  node leftBrother = leftSibling(v);
  if (leftBrother != BADNODE) {
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
      nodeOutsideLeft   = nextLeftContour(nodeOutsideLeft);
      nodeOutsideRight  = nextRightContour(nodeOutsideRight);

      ancestor[nodeOutsideRight] = v;
            
      float shift =   (prelimX[nodeInsideLeft]  + shiftInsideLeft)
	- (prelimX[nodeInsideRight] + shiftInsideRight)
	+ INTER_NODE_DISTANCE_X
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
