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


LAYOUTPLUGIN(Dendrogram, "Dendrogram",
             "Julien Testut, Antony Durand, Pascal Ollier, "
             "Yashvin Nababsing, Sebastien Leclerc, "
             "Thibault Ruchon, Eric Dauchier",
             "03/12/04", "ok", "0", "1");

const float Dendrogram::INTER_NODE_DISTANCE_X = 1.f;
const float Dendrogram::INTER_NODE_DISTANCE_Y = 2.f;

//====================================================================
Dendrogram::Dendrogram(const PropertyContext& context)
    :LayoutAlgorithm(context){
    addOrientationParameters(this);
}

//====================================================================
Dendrogram::~Dendrogram() {
    delete oriLayout;
    delete oriSize;
}

//====================================================================
bool Dendrogram::run() {
    orientationType mask 	  = getMask(dataSet);
    oriLayout                 = new OrientableLayout(layoutResult, mask);
    SizeProperty* viewSize = graph->getLocalProperty<SizeProperty>("viewSize");
                           
    oriSize = new OrientableSizeProxy(viewSize, mask);
    root = searchRoot(graph);

    setAllNodesCoordX(root,0.f);
    shiftAllNodes(root, 0.f);
    setAllNodesCoordY();
    setOrthogonalEdge(oriLayout, oriSize, graph, 
                     INTER_NODE_DISTANCE_Y);
   
    delete oriLayout;
    oriLayout   = NULL;
    oriSize     = NULL;
	
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
float Dendrogram::setAllNodesCoordX(node n, float rightMargin) {
    float leftMargin       = rightMargin;

    Iterator<node>* itNode = graph->getOutNodes(n);
    while (itNode->hasNext()) {
        node currentNode   = itNode->next();
        leftMargin         = setAllNodesCoordX(currentNode, leftMargin);
    }
    delete itNode;
 
    const float nodeWidth  =  oriSize->getNodeValue(n).getW()
                            + INTER_NODE_DISTANCE_X;

    if (isLeaf(graph, n))
        leftMargin = rightMargin + nodeWidth;               
    const float freeRange  = leftMargin - rightMargin;

    float posX;
	if (isLeaf(graph, n))
        posX = freeRange / 2.f + rightMargin; 
	else
    	posX = computeFatherXPosition(n);
    
    const float rightOverflow = max(rightMargin-(posX-nodeWidth/2.f), 0.f);
    const float leftOverflow  = max((posX+nodeWidth/2.f)-leftMargin, 0.f);
    leftshift[n]              = rightOverflow;
    
    setNodePosition(n, posX, 0.f, 0.f);
    return  leftMargin + leftOverflow + rightOverflow;
}

//====================================================================
void Dendrogram::setAllNodesCoordY() {
    float maxYLeaf         = -FLT_MAX;
    float maxHeightLeaf    = -FLT_MAX;
    setCoordY(root, &maxYLeaf, &maxHeightLeaf);
    
    Iterator<node>* itNode = graph->getNodes();
    while (itNode->hasNext()) {
    	node currentNode   = itNode->next();
        if (isLeaf(graph,currentNode)) {
            OrientableCoord coord = oriLayout->getNodeValue(currentNode);
            float newY            = maxYLeaf + maxHeightLeaf
                             - oriSize->getNodeValue(currentNode).getH() / 2.f;
            float coordX          = coord.getX();
            float coordZ          = coord.getZ();
            setNodePosition(currentNode, coordX, newY, coordZ);
        }
    }
    delete itNode;
}

//====================================================================
float Dendrogram::computeFatherXPosition(node father) {
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
void Dendrogram::shiftAllNodes(node n, float shift) {
    OrientableCoord coord   =  oriLayout->getNodeValue(n);
    shift                  +=  leftshift[n];
    float coordX            =  coord.getX();
    
    coord.setX(coordX + shift);
    oriLayout->setNodeValue(n, coord);
    
    Iterator<node>* itNode  =   graph->getOutNodes(n);
    while (itNode->hasNext()) 
        shiftAllNodes(itNode->next(), shift);
    delete itNode;
}

//====================================================================
inline void Dendrogram::setNodePosition(node n, float x, float y, 
                                             float z) {
    OrientableCoord coord = oriLayout->createCoord(x, y, z);
    oriLayout->setNodeValue(n, coord);
}

//====================================================================
void Dendrogram::setCoordY(node n, float* maxYLeaf, float* maxHeightLeaf) {
    float nodeY;
    if (graph->indeg(n) != 0) {
        node fatherNode             = graph->getInNode(n, 1);
        OrientableCoord coord       = oriLayout->getNodeValue(n); 
        OrientableCoord coordFather = oriLayout->getNodeValue(fatherNode);
        nodeY                       = coordFather.getY()  
                                + oriSize->getNodeValue(fatherNode).getH() / 2.f 
                                + INTER_NODE_DISTANCE_Y 
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
        setCoordY(itNode->next(), maxYLeaf, maxHeightLeaf); 
    delete itNode;
}
