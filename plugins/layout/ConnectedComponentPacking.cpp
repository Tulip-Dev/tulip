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

#include <tulip/DrawingTools.h>
#include <tulip/StringCollection.h>

#include "ConnectedComponentPacking.h"
#include "rectanglePackingFonctions.h"
#include "DatasetTools.h"

using namespace std;
using namespace tlp;

PLUGIN(ConnectedComponentPacking)

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
  "Input coordinates of nodes and edges" \
  HTML_HELP_CLOSE(),
//Rotation
  HTML_HELP_OPEN() \
  HTML_HELP_DEF("Type", "DoubleProperty") \
  HTML_HELP_DEF("Values", "Any double property used for rotation of nodes on z-axis") \
  HTML_HELP_DEF("Default", "viewRotation") \
  HTML_HELP_BODY() \
  "Input rotation of nodes on z-axis" \
  HTML_HELP_CLOSE(),
//Complexity
  HTML_HELP_OPEN() \
  HTML_HELP_DEF("Type", "StringCollection") \
  HTML_HELP_DEF("Default", "auto") \
  HTML_HELP_BODY() \
  "Complexity of the algorithm.<br> n is the number of connected components in the graph." \
  HTML_HELP_CLOSE()
};
}

//====================================================================
ConnectedComponentPacking::ConnectedComponentPacking(const tlp::PluginContext* context)
  : LayoutAlgorithm(context) {
  addInParameter<LayoutProperty> ("coordinates",paramHelp[0],"viewLayout");
  addNodeSizePropertyParameter(this);
  addInParameter<DoubleProperty> ("rotation",paramHelp[1],"viewRotation");
  addInParameter<StringCollection> ("complexity", paramHelp[2], COMPLEXITY );
}
//====================================================================
bool ConnectedComponentPacking::run() {

  LayoutProperty *layout = NULL;
  SizeProperty *size = NULL;
  DoubleProperty *rotation = NULL;
  string complexity("auto");

  workingGraph = graph->addCloneSubGraph("workingGraph");

  if ( dataSet!=NULL ) {
    dataSet->get("coordinates", layout);
    getNodeSizePropertyParameter(dataSet, size);
    dataSet->get("rotation", rotation);
    StringCollection complexityCol;

    if (dataSet->get("complexity", complexityCol))
      complexity = complexityCol.getCurrentString();
  }

  if (layout==NULL)
    layout = workingGraph->getProperty<LayoutProperty>("viewLayout");

  if (size==NULL)
    size = workingGraph->getProperty<SizeProperty>("viewSize");

  if (rotation==NULL)
    rotation = workingGraph->getProperty<DoubleProperty>("viewRotation");

  if (complexity=="none")
    complexity = "auto";

  // compute the connected components's subgraphs
  std::vector<std::set<node> > components;
  ConnectedTest::computeConnectedComponents(workingGraph, components);

  for (unsigned int i = 0; i < components.size(); ++i) {
    workingGraph->inducedSubGraph(components[i]);
  }

  vector<Rectangle<float> > rectangles;
  Iterator<Graph *> *it = workingGraph->getSubGraphs();

  while(it->hasNext()) {
    Graph *sg = it->next();
    BoundingBox tmp = tlp::computeBoundingBox(sg, layout, size, rotation);
    Rectangle<float> tmpRec;
    tmpRec[1][0] = tmp[1][0] + spacing;
    tmpRec[1][1] = tmp[1][1] + spacing;
    tmpRec[0][0] = tmp[0][0] + spacing;
    tmpRec[0][1] = tmp[0][1] + spacing;
    assert(tmpRec.isValid());
    rectangles.push_back(tmpRec);
  }

  delete it;

  if (complexity == "auto") {
    if (rectangles.size()<25) {
      complexity="n5";
    }
    else if (rectangles.size()<50) {
      complexity="n4logn";
    }
    else if (rectangles.size()<100) {
      complexity="n4";
    }
    else if (rectangles.size()<150) {
      complexity="n3logn";
    }
    else if (rectangles.size()<250) {
      complexity="n3";
    }
    else if (rectangles.size()<500) {
      complexity="n2logn";
    }
    else if (rectangles.size()<1000) {
      complexity="n2";
    }
    else if (rectangles.size()<5000) {
      complexity="nlogn";
    }
    else
      complexity="n";
  }

  vector<Rectangle<float> > rectanglesBackup(rectangles);
  RectanglePackingLimitRectangles(rectangles, complexity.c_str(), pluginProgress);
  Iterator<node> *itN = graph->getNodes();

  while(itN->hasNext()) {
    node n = itN->next();
    result->setNodeValue(n, layout->getNodeValue(n));
  }

  delete itN;
  Iterator<edge> *itE = graph->getEdges();

  while(itE->hasNext()) {
    edge e = itE->next();
    result->setEdgeValue(e, layout->getEdgeValue(e));
  }

  delete itE;
  unsigned int i = 0;
  it = workingGraph->getSubGraphs();

  while(it->hasNext()) {
    Graph *sg = it->next();
    Coord move(rectangles[i][0][0]-rectanglesBackup[i][0][0], rectangles[i][0][1]-rectanglesBackup[i][0][1], 0);
    result->translate(move, sg);
    ++i;
  }

  delete it;
  graph->delAllSubGraphs(workingGraph);

  return true;
}
