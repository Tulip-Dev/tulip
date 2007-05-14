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
bool Dendrogram::run() {
  spacing = 64.0;
  nodeSpacing = 18.0;

  orientationType mask 	  = getMask(dataSet);
  OrientableLayout oriLayout(layoutResult, mask);
  SizeProperty* size = graph->getProperty<SizeProperty>("viewSize");
  if (dataSet!=0) {
    getNodeSizePropertyParameter(dataSet, size);
    getSpacingParameters(dataSet, nodeSpacing, spacing);
  }
  OrientableSizeProxy oriSize(size, mask);

  tree = computeTree(graph);
  tlp::getSource(tree, root);

  setAllNodesCoordX(root, 0.f, &oriLayout, &oriSize);
  shiftAllNodes(root, 0.f, &oriLayout);
  setAllNodesCoordY(&oriLayout, &oriSize);
  setOrthogonalEdge(&oriLayout, &oriSize, graph, 
		    spacing);

  cleanComputedTree(graph, tree);
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
  float maxHeightLeaf    = -FLT_MAX;
  setCoordY(root, &maxYLeaf, &maxHeightLeaf, oriLayout, oriSize);
    
  Iterator<node>* itNode = tree->getNodes();
  while (itNode->hasNext()) {
    node currentNode   = itNode->next();
    if (isLeaf(tree,currentNode)) {
      OrientableCoord coord = oriLayout->getNodeValue(currentNode);
      float newY            = maxYLeaf + maxHeightLeaf
	- oriSize->getNodeValue(currentNode).getH() / 2.f;
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
void Dendrogram::setCoordY(node n, float* maxYLeaf, float* maxHeightLeaf,
			   OrientableLayout *oriLayout, OrientableSizeProxy *oriSize) {
  float nodeY;
  if (tree->indeg(n) != 0) {
    node fatherNode             = tree->getInNode(n, 1);
    OrientableCoord coord       = oriLayout->getNodeValue(n); 
    OrientableCoord coordFather = oriLayout->getNodeValue(fatherNode);
    nodeY                       = coordFather.getY()  
      + oriSize->getNodeValue(fatherNode).getH() / 2.f 
      + spacing 
      + oriSize->getNodeValue(n).getH() / 2.f;                                                                                      
    coord.setY(nodeY);
    oriLayout->setNodeValue(n, coord);
    if (isLeaf(tree, n)) {
      float nodeHeight = oriSize->getNodeValue(n).getH();
      (*maxHeightLeaf)    = max((*maxHeightLeaf), nodeHeight / 2.f);
      (*maxYLeaf)         = max((*maxYLeaf), nodeY);                       
    }
                                
  }    
    
  Iterator<node> *itNode = tree->getOutNodes(n);
  while (itNode->hasNext()) 
    setCoordY(itNode->next(), maxYLeaf, maxHeightLeaf, oriLayout, oriSize); 
  delete itNode;
}
