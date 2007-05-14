#include <tulip/Graph.h>
#include <tulip/GraphTools.h>
#include <tulip/DrawingTools.h>

#include "ConnectedComponentPacking.h"
#include "rectanglePackingFonctions.h"
#include "DatasetTools.h"

using namespace std;
using namespace tlp;

LAYOUTPLUGINOFGROUP(ConnectedComponentPacking, "Connected Component Packing", "David Auber", "26/05/05", "beta", "1.0", "Misc");

const float spacing = 8;
#define COMPLEXITY "auto;n5;n4logn;n4;n3logn;n3;n2logn;n2;nlogn;n;"
namespace {
const char* paramHelp[] = {
//LayoutAlgorithm
HTML_HELP_OPEN() \
    HTML_HELP_DEF("Type", "LayoutProperty") \
    HTML_HELP_DEF("Values", "Any layout property") \
    HTML_HELP_DEF("Default", "viewLayout") \
    HTML_HELP_BODY() \
        "Choose the input coordinates of nodes and edges" \
HTML_HELP_CLOSE(),
//Rotation
HTML_HELP_OPEN() \
    HTML_HELP_DEF("Type", "DoubleProperty") \
    HTML_HELP_DEF("Values", "Any double property used for rotation of nodes on z-axis") \
    HTML_HELP_DEF("Default", "viewRotation") \
    HTML_HELP_BODY() \
        "Choose the input rotation of nodes on z-axis" \
HTML_HELP_CLOSE(),
//Complexity
HTML_HELP_OPEN() \
    HTML_HELP_DEF("Type", "StringCollection") \
    HTML_HELP_DEF("Default", "auto") \
    HTML_HELP_BODY() \
        "Choose the complexity of the algorithm.<br> n is the number of connected components in the graph" \
HTML_HELP_CLOSE()};
}                     

//====================================================================
ConnectedComponentPacking::ConnectedComponentPacking(const PropertyContext& context)
    : LayoutAlgorithm(context) {
  addParameter<LayoutProperty> ("coordinates",paramHelp[0],"viewLayout");
  addNodeSizePropertyParameter(this);
  addParameter<DoubleProperty> ("rotation",paramHelp[1],"viewRotation");
  addParameter<StringCollection> ("complexity", paramHelp[2], COMPLEXITY );
  // Connected component metric dependency
  addDependency<DoubleAlgorithm>("Connected Component", "1.0");
  // Equal Value algorithm dependency
  addDependency<Algorithm>("Equal Value", "1.0");
}
//====================================================================
bool ConnectedComponentPacking::run() {
  
  LayoutProperty *layout = 0;
  SizeProperty *size = 0;
  DoubleProperty *rotation = 0;
  string complexity("auto");

  workingGraph = tlp::newCloneSubGraph(graph, "workingGraph");

  if ( dataSet!=0 ) {
    dataSet->get("coordinates", layout);
    getNodeSizePropertyParameter(dataSet, size);
    dataSet->get("rotation", rotation);
    StringCollection complexityCol;
    if (dataSet->get("complexity", complexityCol))
      complexity = complexityCol.getCurrentString(); 
  }

  if (layout==0)
    layout = workingGraph->getProperty<LayoutProperty>("viewLayout");
  if (size==0)
    size = workingGraph->getProperty<SizeProperty>("viewSize");  
  if (rotation==0)
    rotation = workingGraph->getProperty<DoubleProperty>("viewRotation");  
  if (complexity=="none")
    complexity = "auto";


  DoubleProperty connectedComponent(workingGraph);
  string err;
  workingGraph->computeProperty(string("Connected Component"), &connectedComponent, err);
  DataSet tmp;
  tmp.set("Property", &connectedComponent);
  tlp::applyAlgorithm(workingGraph, err, &tmp, "Equal Value");

  vector<Rectangle<float> > rectangles;
  Iterator<Graph *> *it = workingGraph->getSubGraphs();
  while(it->hasNext()) {
    Graph *sg = it->next();
    pair<Coord, Coord> tmp = tlp::computeBoundingBox(sg, layout, size, rotation);
    Rectangle<float> tmpRec;
    tmpRec[1][0] = tmp.first[0] + spacing;
    tmpRec[1][1] = tmp.first[1] + spacing;
    tmpRec[0][0] = tmp.second[0] + spacing;
    tmpRec[0][1] = tmp.second[1] + spacing;
    rectangles.push_back(tmpRec);
  } delete it;

  if (complexity == "auto") {
    if (rectangles.size()<25) { 
      complexity="n5";
    } else
    if (rectangles.size()<50) { 
      complexity="n4logn";
    } else
    if (rectangles.size()<100) { 
      complexity="n4";
    } else
    if (rectangles.size()<150) { 
      complexity="n3logn";
    } else
    if (rectangles.size()<250) { 
      complexity="n3";
    } else
    if (rectangles.size()<500) { 
      complexity="n2logn";
    } else
    if (rectangles.size()<1000) { 
      complexity="n2";
    } else
    if (rectangles.size()<5000) { 
      complexity="nlogn";
    } else
    complexity="n";
  }

  vector<Rectangle<float> > rectanglesBackup(rectangles);
  RectanglePackingLimitRectangles(rectangles, complexity.c_str(), pluginProgress);
  Iterator<node> *itN = graph->getNodes();
  while(itN->hasNext()){
    node n = itN->next();
    layoutResult->setNodeValue(n, layout->getNodeValue(n));
  } delete itN;
  Iterator<edge> *itE = graph->getEdges();
  while(itE->hasNext()){
    edge e = itE->next();
    layoutResult->setEdgeValue(e, layout->getEdgeValue(e));
  } delete itE;
  unsigned int i = 0;
  it = workingGraph->getSubGraphs();
  while(it->hasNext()) {
    Graph *sg = it->next();
    Coord move(rectangles[i][0][0]-rectanglesBackup[i][0][0], rectangles[i][0][1]-rectanglesBackup[i][0][1], 0);
    layoutResult->translate(move, sg);
    ++i;
  } delete it;
  graph->delAllSubGraphs(workingGraph);

  return true;
}
