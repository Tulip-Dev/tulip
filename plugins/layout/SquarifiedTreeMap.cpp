#include <cmath>
#include <float.h>
#include <algorithm>
#include <tulip/TreeTest.h>
#include "tulip/RectangleArea.h"
#include "tulip/Border.h"
#include "SquarifiedTreeMap.h"
#include "TreeTools.h"

using namespace std;
using namespace tlp;

LAYOUTPLUGINOFGROUP(SquarifiedTreeMap,"Squarified Tree Map",
             "Julien Testut, Antony Durand, Pascal Ollier, "
             "Yashvin Nababsing, Sebastien Leclerc, "
             "Thibault Ruchon, Eric Dauchier",
		    "03/11/2004", "ok", "0", "1", "Tree");

//====================================================================

const float SEPARATION_Z  = 150; 
const float DEFAULT_RATIO = 1.;
const int DEFAULT_WIDTH   = 1024;
const int DEFAULT_HEIGHT  = 1024;

const int TEXTUREDGLYPHID = 101;

//====================================================================
SquarifiedTreeMap::SquarifiedTreeMap(const PropertyContext& context)
                                                    :LayoutAlgorithm(context){
  aspectRatio = DEFAULT_RATIO;
  addParameter<float>("Aspect Ratio", NULL, "1");
  addParameter<bool>("Texture?", NULL, "false");
}

//====================================================================
SquarifiedTreeMap::~SquarifiedTreeMap() {
}

//====================================================================
bool SquarifiedTreeMap::check(string& errorMsg) {
  metric = graph->getProperty<DoubleProperty>("viewMetric"); 
  if (dataSet != 0)
    dataSet->get("property", metric);    
  if (!metric) {
    errorMsg = "Metric is not valid";
    return false;
  }

  if (TreeTest::isTree(graph)) {
    if (verifyMetricIsPositive()) {
      errorMsg = "Graph's nodes must have positive metric";
      return false;
    }
    else {
      errorMsg = "";
      return true;
    }
  }
  else {
    errorMsg = "The Graph must be a Tree";
    return false;
  }
}

//====================================================================
bool SquarifiedTreeMap::run() {
  float aspectRatio  = DEFAULT_RATIO;
  bool glyphTextured = false;
    
  if (dataSet != 0) {
    dataSet->get("Aspect Ratio", aspectRatio);
    dataSet->get("Texture?", glyphTextured);
  }
    
  size  = graph->getLocalProperty<SizeProperty>("viewSize");    
  glyph = graph->getLocalProperty<IntegerProperty>("viewShape"); 
    
  if (glyphTextured)
    glyph->setAllNodeValue(TEXTUREDGLYPHID);

  RectangleArea initialSpace(0, 0, DEFAULT_WIDTH * aspectRatio,
			     DEFAULT_HEIGHT);

  node root = searchRoot(graph);

  initializeMapSum(root);
  Coord initialSpaceCenterCoord = initialSpace.getCenterCoord();   
  layoutResult->setNodeValue(root, initialSpaceCenterCoord);
  Size initialSpaceSize = initialSpace.getSize();
  size->setNodeValue(root, initialSpaceSize);
  squarify(root, initialSpace, 1);

  return true;
}

//====================================================================
void SquarifiedTreeMap::reset() {
}

//====================================================================
inline float computeAspectRatio(float area, float childrenArea,
                                float widthArea) {
  float ratio = (area * widthArea * widthArea) / 
    (childrenArea * childrenArea);
    
  return max(ratio, 1.f / ratio);
}   

//====================================================================
inline float computeAspectRatio(float metric, float rowMetric,
                                const RectangleArea& rectArea) {
  float height = metric / rowMetric * rectArea.getVirtualHeight();
  float ratio  = rectArea.getVirtualWidth() / height;
  return max(ratio, 1.f / ratio);
}   

//====================================================================
inline void setRectangleOrientation(RectangleArea& rectArea) {
  directionType direction = (rectArea.getWidth() > rectArea.getHeight() ?
			     HORIZONTAL_DIRECTION : VERTICAL_DIRECTION);
  rectArea.setDirection(direction);
}

//====================================================================
static bool isGreater(const pairNodeF& p1, const pairNodeF& p2) {
  return p1.second > p2.second;
}

//====================================================================
void SquarifiedTreeMap::layRow(pairIterator itFirstChildNode,
                               pairIterator itEndChildNode, int depth,
                               RectangleArea rectArea, float listSumMetric) {
  const float initialHeight        = rectArea.getVirtualHeight();
  pairIterator itCurrentNode       = itFirstChildNode;
    
  while (itCurrentNode != itEndChildNode) {
    const float childHeight      = itCurrentNode->second
      / listSumMetric* initialHeight;   

    RectangleArea childArea      = rectArea;     
    childArea.getVirtualHeight() = childHeight;
    Coord centerOfChildArea      = childArea.getCenterCoord();
        
    centerOfChildArea.setZ(depth * SEPARATION_Z);                                     
    layoutResult->setNodeValue(itCurrentNode->first, centerOfChildArea);
    Size childAreaSize = childArea.getSize();
    size->setNodeValue(itCurrentNode->first, childAreaSize);

    if (graph->outdeg(itCurrentNode->first) > 0)
      squarify(itCurrentNode->first, childArea, depth);

    rectArea.getVirtualY() += childHeight;
    itCurrentNode ++;
  }
}

//====================================================================
void computeNewSpace(RectangleArea* rectArea, float borderSize) {
  rectArea->getVirtualHeight() -= borderSize * 2;
  rectArea->getVirtualWidth()  -= borderSize * 2;
  rectArea->getVirtualX()      += borderSize;
  rectArea->getVirtualY()      += borderSize;
}

//====================================================================
void SquarifiedTreeMap::squarify(node n, RectangleArea rectArea, int depth) {
  const int glyphid = glyph->getNodeValue(n);
  if (isBorderedGlyph(glyphid)) {
    float borderSizeEvaluation = evaluateBorderSize(depth, rectArea);
    computeNewSpace(&rectArea, borderSizeEvaluation);
  }
    
  vector<pairNodeF> childOfN;
    
  Iterator<node>* itNode = graph->getOutNodes(n);
  while (itNode->hasNext()) {
    node currentNode   = itNode->next();
    childOfN.push_back(pairNodeF(currentNode,
				 sumChildrenMetric[currentNode]));
  }
  delete itNode;
    
  pairIterator itCurrentNode = childOfN.begin();
  pairIterator childOfNEnd   = childOfN.end();    
    
  sort(itCurrentNode, childOfNEnd, isGreater);

  float childMetric = sumChildrenMetric[n];
 
  itCurrentNode = childOfN.begin();	
  const pairIterator itEndChildrenList = childOfN.end();  
    
  while (itCurrentNode != itEndChildrenList) {
    setRectangleOrientation(rectArea); 
    pairIterator itFirstNode  = itCurrentNode;
    float bestAspectRatio     = FLT_MAX;
    float bestArrowMetric     = 0;
    float bestArrowWidth      = 0;
    RectangleArea bestArrow   = rectArea;

    while (itCurrentNode != itEndChildrenList) {
      const float rowMetric = bestArrowMetric + itCurrentNode->second;
      const float rowWidth  = rowMetric /
	childMetric * rectArea.getVirtualWidth();
      RectangleArea row     = rectArea;
      row.getVirtualWidth() = rowWidth;
      const float ratio     = findWorstRatio(itFirstNode->second,
					     itCurrentNode->second,
					     rowMetric, row);

      if (ratio >= bestAspectRatio)
	break;

      bestAspectRatio = ratio;
      bestArrowMetric = rowMetric;
      bestArrowWidth  = rowWidth;
      bestArrow       = row;
      ++itCurrentNode;
    }

    layRow(itFirstNode, itCurrentNode, depth + 1, bestArrow,
	   bestArrowMetric);        
    rectArea.getVirtualX()      += bestArrowWidth;
    rectArea.getVirtualWidth()  -= bestArrowWidth;
    childMetric                 -= bestArrowMetric;
  }
}

//====================================================================
float SquarifiedTreeMap::findWorstRatio(float metric1, float metric2, 
                                        float listMetric,
                                        const RectangleArea& row) {
    
  return max(computeAspectRatio(metric1, listMetric, row),
	     computeAspectRatio(metric2, listMetric, row));
}

//====================================================================
float SquarifiedTreeMap::initializeMapSum(node n) {
    
  if (isLeaf(graph, n)) {
    if ((sumChildrenMetric[n] = metric->getNodeValue(n)) == 0)
      sumChildrenMetric[n] = 1;
    return sumChildrenMetric[n];
  }

  float sum              = 0;
  Iterator<node>* itNode = graph->getOutNodes(n);
  while (itNode->hasNext()) 
    sum += initializeMapSum(itNode->next());
  delete itNode;
  sumChildrenMetric[n]   = sum;
  return sum;
}

//====================================================================
bool SquarifiedTreeMap::verifyMetricIsPositive() {
  bool result            = true;
  Iterator<node>* itNode = graph->getNodes();
  while (itNode->hasNext() && result) 
    if (metric->getNodeValue(itNode->next()) < 0)
      result = false;
  delete itNode;
  return !result;
}

