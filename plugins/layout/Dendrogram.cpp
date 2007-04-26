#include <cmath>
#include <algorithm>
#include <tulip/TreeTest.h>
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
  addOrientationParameters(this);
  addSpacingParameters(this);
}

//====================================================================
Dendrogram::~Dendrogram() {
}

//====================================================================
bool Dendrogram::run() {
  orientationType mask 	  = getMask(dataSet);
  spacing = 64.0;
  nodeSpacing = 18.0;
  OrientableLayout oriLayout(layoutResult, mask);
  SizeProperty* viewSize = graph->getLocalProperty<SizeProperty>("viewSize");
  OrientableSizeProxy oriSize(viewSize, mask);
  
  if (dataSet!=0) {
    dataSet->get("layer spacing", spacing);
    dataSet->get("node spacing", nodeSpacing);
  }

  root = searchRoot(graph);

  setAllNodesCoordX(root,0.f, &oriLayout, &oriSize);
  shiftAllNodes(root, 0.f, &oriLayout);
  setAllNodesCoordY(&oriLayout, &oriSize);
  setOrthogonalEdge(&oriLayout, &oriSize, graph, 
		    spacing);
   	
  return true;
}

//====================================================================
bool Dendrogram::check(string& errorMsg) {
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
void Dendrogram::reset() {
}

//====================================================================
float Dendrogram::setAllNodesCoordX(node n, float rightMargin,
				    OrientableLayout *oriLayout, OrientableSizeProxy *oriSize) {
  float leftMargin       = rightMargin;

  Iterator<node>* itNode = graph->getOutNodes(n);
  while (itNode->hasNext()) {
    node currentNode   = itNode->next();
    leftMargin         = setAllNodesCoordX(currentNode, leftMargin, oriLayout, oriSize);
  }
  delete itNode;
 
  const float nodeWidth  =  oriSize->getNodeValue(n).getW()
    + nodeSpacing;

  if (isLeaf(graph, n))
    leftMargin = rightMargin + nodeWidth;               
  const float freeRange  = leftMargin - rightMargin;

  float posX;
  if (isLeaf(graph, n))
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
    
  Iterator<node>* itNode = graph->getNodes();
  while (itNode->hasNext()) {
    node currentNode   = itNode->next();
    if (isLeaf(graph,currentNode)) {
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
    
  Iterator<node> *itNode =  graph->getOutNodes(father);    
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
    
  Iterator<node>* itNode  =   graph->getOutNodes(n);
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
  if (graph->indeg(n) != 0) {
    node fatherNode             = graph->getInNode(n, 1);
    OrientableCoord coord       = oriLayout->getNodeValue(n); 
    OrientableCoord coordFather = oriLayout->getNodeValue(fatherNode);
    nodeY                       = coordFather.getY()  
      + oriSize->getNodeValue(fatherNode).getH() / 2.f 
      + spacing 
      + oriSize->getNodeValue(n).getH() / 2.f;                                                                                      
    coord.setY(nodeY);
    oriLayout->setNodeValue(n, coord);
    if (isLeaf(graph, n)) {
      float nodeHeight = oriSize->getNodeValue(n).getH();
      (*maxHeightLeaf)    = max((*maxHeightLeaf), nodeHeight / 2.f);
      (*maxYLeaf)         = max((*maxYLeaf), nodeY);                       
    }
                                
  }    
    
  Iterator<node> *itNode = graph->getOutNodes(n);
  while (itNode->hasNext()) 
    setCoordY(itNode->next(), maxYLeaf, maxHeightLeaf, oriLayout, oriSize); 
  delete itNode;
}
