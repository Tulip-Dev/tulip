/*
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

#include <algorithm>

#include <tulip/SizeProperty.h>
#include <tulip/ForEach.h>
#include <tulip/StringCollection.h>

#include "FastOverlapRemoval.h"
#include "generate-constraints.h"
#include "remove_rectangle_overlap.h"

using namespace std;
using namespace tlp;

PLUGIN(FastOverlapRemoval)

static const char* paramHelp[] = {
  // layout
  HTML_HELP_OPEN()
  HTML_HELP_DEF( "type", "String Collection" )
  HTML_HELP_DEF("values", "X-Y <i>(Remove overlaps in both X and Y directions)</i><br/>X <i>(Remove overlaps only in X direction)</i><br/>Y <i>(Remove overlaps only in Y direction)</i>")
  HTML_HELP_DEF( "default", "X-Y" )
  HTML_HELP_BODY()
  "Overlaps removal type."
  HTML_HELP_CLOSE(),
  HTML_HELP_OPEN()          \
  HTML_HELP_DEF("Type", "LayoutProperty")    \
  HTML_HELP_DEF("Values", "Any layout property")   \
  HTML_HELP_DEF("Default", "viewLayout")     \
  HTML_HELP_BODY()           \
  "The property used for the input layout of nodes and edges"    \
  HTML_HELP_CLOSE(),
  // node size
  HTML_HELP_OPEN() \
  HTML_HELP_DEF( "type", "Size" ) \
  HTML_HELP_DEF( "values", "An existing size property" ) \
  HTML_HELP_DEF( "default", "viewSize" ) \
  HTML_HELP_BODY() \
  "The property used for node's sizes." \
  HTML_HELP_CLOSE(),
  //Rotation
  HTML_HELP_OPEN()          \
  HTML_HELP_DEF("Type", "DoubleProperty")       \
  HTML_HELP_DEF("Values", "Any double property used for rotation of nodes on z-axis") \
  HTML_HELP_DEF("Default", "viewRotation")        \
  HTML_HELP_BODY()              \
  "The property used as rotation of nodes on z-axis"      \
  HTML_HELP_CLOSE(),
  //Iterations
  HTML_HELP_OPEN()          \
  HTML_HELP_DEF("Type", "int")       \
  HTML_HELP_DEF("Values", "The number of time the algorithm will be applied with increasing node size.") \
  HTML_HELP_DEF("Default", "5")        \
  HTML_HELP_BODY()              \
  "The algorithm will be applied N times, each time increasing node size to attain original size at the final iteration. This greatly enhances the layout."      \
  HTML_HELP_CLOSE(),
  HTML_HELP_OPEN()          \
  HTML_HELP_DEF("Type", "double")       \
  HTML_HELP_DEF("Values", "floating point number") \
  HTML_HELP_DEF("Default", "0.0")        \
  HTML_HELP_BODY()              \
  "The minimal x border value that will separate the graph nodes after application of the algorithm"      \
  HTML_HELP_CLOSE(),
  HTML_HELP_OPEN()          \
  HTML_HELP_DEF("Type", "double")       \
  HTML_HELP_DEF("Values", "floating point number") \
  HTML_HELP_DEF("Default", "0.0")        \
  HTML_HELP_BODY()              \
  "The minimal y border value that will separate the graph nodes after application of the algorithm"      \
  HTML_HELP_CLOSE(),
};

#define OVERLAP_TYPE "X-Y;X;Y"

FastOverlapRemoval::FastOverlapRemoval(const tlp::PluginContext* context) :
  tlp::LayoutAlgorithm(context) {
  addInParameter<StringCollection> ("overlaps removal type", paramHelp[0], OVERLAP_TYPE);
  addInParameter<LayoutProperty>("layout", paramHelp[1], "viewLayout");
  addInParameter<SizeProperty>("bounding box", paramHelp[2], "viewSize");
  addInParameter<DoubleProperty>("rotation", paramHelp[3], "viewRotation");
  addInParameter<int>("number of passes", paramHelp[4], "5");
  addInParameter<double>("x border", paramHelp[5], "0.0");
  addInParameter<double>("y border", paramHelp[6], "0.0");
}

/**
 * The following function transfers the node set into vpsc rectangles
 * and runs fast overlap removal.  This vpsc code was a port of Dwyer
 * used in the InkScape Open Source Software.
 */
bool FastOverlapRemoval::run () {
  tlp::StringCollection stringCollection(OVERLAP_TYPE);
  stringCollection.setCurrent(0);
  LayoutProperty *viewLayout = NULL;
  SizeProperty *viewSize = NULL;
  DoubleProperty *viewRot = NULL;
  double xBorder = 0.;
  double yBorder = 0.;
  int nbPasses = 5;

  if (dataSet != NULL) {
    dataSet->get("overlaps removal type", stringCollection);
    dataSet->get("layout", viewLayout);

    if (!dataSet->get("bounding box", viewSize))
      // old name of the parameter
      dataSet->get("boundingBox", viewSize);

    dataSet->get("rotation", viewRot);
    dataSet->get("number of passes", nbPasses);
    dataSet->get("x border", xBorder);
    dataSet->get("y border", yBorder);
  }

  if (viewLayout == NULL)
    viewLayout = graph->getProperty<LayoutProperty> ("viewLayout");

  if (viewSize == NULL)
    viewSize = graph->getProperty<SizeProperty> ("viewSize");

  if (viewRot == NULL)
    viewRot = graph->getProperty<DoubleProperty> ("viewRotation");

  // initialize result for edges
  result->setAllEdgeValue(viewLayout->getEdgeDefaultValue());
  edge e;
  forEach(e, viewLayout->getNonDefaultValuatedEdges())
  result->setEdgeValue(e, viewLayout->getEdgeValue(e));

  SizeProperty size(graph);

  for(float passIndex = 1.; passIndex <= nbPasses; ++passIndex) {
    node n;
    // size initialization
    forEach(n, graph->getNodes())
    size.setNodeValue(n, viewSize->getNodeValue(n) *
                      passIndex/(float)nbPasses);

    //actually apply fast overlap removal
    vector<vpsc::Rectangle *>nodeRectangles (graph->numberOfNodes());
    node curNode;
    unsigned int nodeCounter = 0;
    vector<node> nodeOrder(graph->numberOfNodes());
    forEach (curNode, graph->getNodes()) {
      const Coord &pos = viewLayout->getNodeValue (curNode);
      const Size &sz = size.getNodeValue (curNode);
      double curRot = viewRot->getNodeValue (curNode);
      Size rotSize = Size (sz.getW()*fabs (cos (curRot*M_PI/180.0)) +
                           sz.getH()*fabs (sin (curRot*M_PI/180.0)),
                           sz.getW()*fabs (sin (curRot*M_PI/180.0)) +
                           sz.getH()*fabs (cos (curRot*M_PI/180.0)), 1.0f);
      double maxX = pos.getX() + rotSize.getW()/2.0;
      double maxY = pos.getY() + rotSize.getH()/2.0;
      double minX = pos.getX() - rotSize.getW()/2.0;
      double minY = pos.getY() - rotSize.getH()/2.0;

      nodeRectangles[nodeCounter] =
        new vpsc::Rectangle (minX, maxX, minY, maxY, xBorder, yBorder);
      nodeOrder[nodeCounter] = curNode;
      ++nodeCounter;
    }//end forEach

    if (stringCollection.getCurrentString() == "X-Y") {
      removeRectangleOverlap(graph->numberOfNodes(),
#if defined(__APPLE__)
                             &(nodeRectangles[0]),
#else
                             nodeRectangles.data(),
#endif
                             xBorder, yBorder);
    }
    else if (stringCollection.getCurrentString() == "X") {
      removeRectangleOverlapX(graph->numberOfNodes(),
#if defined(__APPLE__)
                              &(nodeRectangles[0]),
#else
                              nodeRectangles.data(),
#endif
                              xBorder, yBorder);
    }
    else {
      removeRectangleOverlapY(graph->numberOfNodes(),
#if defined(__APPLE__)
                              &(nodeRectangles[0]),
#else
                              nodeRectangles.data(),
#endif
                              yBorder);
    }

    for (unsigned int i = 0; i < graph->numberOfNodes(); ++i) {
      Coord newPos (nodeRectangles[i]->getCentreX(),
                    nodeRectangles[i]->getCentreY(), 0.0);
      LayoutAlgorithm::result->setNodeValue (nodeOrder[i], newPos);
    }//end for

    for (unsigned int i = 0; i < graph->numberOfNodes(); ++i)
      delete nodeRectangles[i];

  }

  return true;
}//end run

