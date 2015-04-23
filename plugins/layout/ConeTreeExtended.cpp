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
#include <cmath>

#include <tulip/Circle.h>
#include <tulip/StringCollection.h>

#include "ConeTreeExtended.h"
#include "DatasetTools.h"

PLUGIN(ConeTreeExtended)

using namespace std;
using namespace tlp;

//===============================================================
float sqr(float x) {
  return x*x;
}
//===============================================================
float minRadius(float radius1,float alpha1,float radius2,float alpha2) {
  return sqrt(sqr(radius1+radius2)/(sqr(cos(alpha1)-cos(alpha2)) + sqr(sin(alpha1)-sin(alpha2))));
}
//===============================================================
void ConeTreeExtended::computeLayerSize(tlp::node n, unsigned int level) {
  if (levelSize.size() < level+1 )
    levelSize.push_back(0);

  levelSize[level] = std::max(levelSize[level], nodeSize->getNodeValue(n)[1]);
  node i;
  forEach(i, tree->getOutNodes(n)) {
    computeLayerSize(i, level + 1);
  }
}
//===============================================================
void ConeTreeExtended::computeYCoodinates(tlp::node root) {
  levelSize.clear();
  yCoordinates.clear();
  computeLayerSize(root, 0);
  yCoordinates.resize(levelSize.size());
  yCoordinates[0] = 0;

  for (unsigned int i = 1; i < levelSize.size(); ++i) {
    yCoordinates[i] = yCoordinates[i-1] + levelSize[i] / 2.0f + levelSize[i-1] / 2.0f + spaceBetweenLevels;
  }
}
//===============================================================
double ConeTreeExtended::treePlace3D(tlp::node n,
                                     TLP_HASH_MAP<tlp::node,double> *posRelX,
                                     TLP_HASH_MAP<tlp::node,double> *posRelY) {
  (*posRelX)[n]=0;
  (*posRelY)[n]=0;

  if (tree->outdeg(n)==0) {
    const Coord& tmp = Coord(nodeSize->getNodeValue(n));
    return sqrt(tmp[0]*tmp[0] + tmp[2]*tmp[2])/2.0;
  }

  if (tree->outdeg(n)==1) {
    Iterator<node> *itN=tree->getOutNodes(n);
    node itn=itN->next();
    delete itN;
    return treePlace3D(itn,posRelX,posRelY);
  }

  double sumRadius=0;
  double maxRadius=0;
  float newRadius;

  vector<double> subCircleRadius(tree->outdeg(n));
  Iterator<node> *itN=tree->getOutNodes(n);

  for (int i=0; itN->hasNext(); ++i)  {
    node itn = itN->next();
    subCircleRadius[i] = treePlace3D(itn,posRelX,posRelY);
    sumRadius += 2*subCircleRadius[i];
    maxRadius = std::max(maxRadius , subCircleRadius[i]);
  }

  delete itN;

  double radius=sumRadius/(2*M_PI);

  //treat angles
  vector<double> vangles(subCircleRadius.size());
  double angle=0;
  vangles[0]=0;

  for (unsigned int i=1; i<subCircleRadius.size(); ++i) {
    angle+=(subCircleRadius[i-1]+subCircleRadius[i])/radius;
    vangles[i]=angle;
  }

  // compute new radius
  newRadius=0;

  for (unsigned int i=0; i<subCircleRadius.size()-1; ++i) {
    for (unsigned int j=i+1; j<subCircleRadius.size(); ++j) {
      newRadius = std::max(newRadius , minRadius(static_cast<float>(subCircleRadius[i]),static_cast<float>(vangles[i]),static_cast<float>(subCircleRadius[j]),static_cast<float>(vangles[j])));
    }
  }

  if (newRadius==0) newRadius=static_cast<float>(radius);

  //compute Circle Hull
  vector<Circlef > circles(subCircleRadius.size());

  for (unsigned int i=0; i<subCircleRadius.size(); ++i) {
    circles[i][0]=newRadius*static_cast<float>(cos(vangles[i]));
    circles[i][1]=newRadius*static_cast<float>(sin(vangles[i]));
    circles[i].radius=static_cast<float>(subCircleRadius[i]);
  }

  tlp::Circlef circleH=tlp::enclosingCircle(circles);

  //Place relative position
  itN = tree->getOutNodes(n);

  for (unsigned int i=0; i<subCircleRadius.size(); ++i) {
    node itn = itN->next();
    (*posRelX)[itn]=newRadius*cos(vangles[i])-circleH[0];
    (*posRelY)[itn]=newRadius*sin(vangles[i])-circleH[1];
  }

  delete itN;
  return circleH.radius;
}
//===============================================================
void ConeTreeExtended::calcLayout(tlp::node n, TLP_HASH_MAP<tlp::node,double> *px,
                                  TLP_HASH_MAP<tlp::node,double> *py,
                                  double x, double y, int level) {
  result->setNodeValue(n,Coord(static_cast<float>(x+(*px)[n]), - static_cast<float>(yCoordinates[level]),static_cast<float>(y+(*py)[n])));
  node itn;
  forEach(itn, tree->getOutNodes(n)) {
    calcLayout(itn, px, py, x+(*px)[n], y+(*py)[n], level + 1);
  }
}
//===============================================================
namespace {
const char * paramHelp[] = {
  //Orientation
  HTML_HELP_OPEN()         \
  HTML_HELP_DEF( "type", "String Collection" ) \
  HTML_HELP_DEF( "default", "horizontal" )   \
  HTML_HELP_BODY() \
  "This parameter enables to choose the orientation of the drawing" \
  HTML_HELP_CLOSE(),
  HTML_HELP_OPEN()         \
  HTML_HELP_DEF( "type", "float" ) \
  HTML_HELP_DEF( "default", "1.0" )   \
  HTML_HELP_BODY() \
  "This parameter enables to add extra spacing between the different levels of the tree" \
  HTML_HELP_CLOSE()
};
}
#define ORIENTATION "vertical;horizontal;"
//===============================================================
ConeTreeExtended::ConeTreeExtended(const tlp::PluginContext* context):LayoutAlgorithm(context), spaceBetweenLevels(10) {
  addNodeSizePropertyParameter(this);
  addInParameter<StringCollection> ("orientation", paramHelp[0], ORIENTATION );
  addInParameter<float>("space between levels", paramHelp[1], "1.0");
}
//===============================================================
ConeTreeExtended::~ConeTreeExtended() {}
//===============================================================
bool ConeTreeExtended::run() {
  nodeSize = NULL;
  string orientation = "vertical";

  if (dataSet!=NULL) {
    getNodeSizePropertyParameter(dataSet, nodeSize);
    StringCollection tmp;

    if (dataSet->get("orientation", tmp)) {
      orientation = tmp.getCurrentString();
    }

    dataSet->get("space between levels", spaceBetweenLevels);
    // no negative spacing
    spaceBetweenLevels = std::max(spaceBetweenLevels, 0.f);
  }

  if (!nodeSize)
    nodeSize = graph->getProperty<SizeProperty>("viewSize");

  //=========================================================
  //rotate size if needed
  //will be undone at then end
  if (orientation == "horizontal") {
    node n;
    forEach(n, graph->getNodes()) {
      const Size& tmp = nodeSize->getNodeValue(n);
      nodeSize->setNodeValue(n, Size(tmp[1], tmp[0], tmp[2]));
    }
  }

  //===========================================================
  result->setAllEdgeValue(vector<Coord>(0));

  if (pluginProgress)
    pluginProgress->showPreview(false);

  // push a temporary graph state (not redoable)
  // preserving layout updates
  std::vector<PropertyInterface*> propsToPreserve;

  if (result->getName() != "")
    propsToPreserve.push_back(result);

  graph->push(false, &propsToPreserve);

  tree = TreeTest::computeTree(graph, pluginProgress);

  if (pluginProgress && pluginProgress->state() != TLP_CONTINUE) {
    graph->pop();
    return false;
  }

  node root = tree->getSource();
  assert(root.isValid());
  TLP_HASH_MAP<node,double> posX;
  TLP_HASH_MAP<node,double> posY;
  treePlace3D(root,&posX,&posY);
  computeYCoodinates(root);
  calcLayout(root,&posX,&posY,0,0,0);

  //rotate layout and size
  if (orientation == "horizontal") {
    node n;
    forEach(n, graph->getNodes()) {
      // if not in tulip gui, ensure cleanup
      LayoutProperty* elementLayout;

      if (!graph->getAttribute("viewLayout", elementLayout)) {
        const Size&  tmp = nodeSize->getNodeValue(n);
        nodeSize->setNodeValue(n, Size(tmp[1], tmp[0], tmp[2]));
      }

      const Coord& tmpC = result->getNodeValue(n);

      result->setNodeValue(n, Coord(-tmpC[1], tmpC[0], tmpC[2]));
    }
  }

  // forget last temporary graph state
  graph->pop();

  return true;
}
