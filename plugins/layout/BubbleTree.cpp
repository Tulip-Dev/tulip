/**
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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
#include <tulip/Circle.h>
#include "BubbleTree.h"
#include "DatasetTools.h"

PLUGIN(BubbleTree)

using namespace std;
using namespace tlp;

struct greaterRadius {
  const std::vector<double> &radius;
  greaterRadius(const std::vector<double> &r) : radius(r) {}
  bool operator()(unsigned i1, unsigned i2) const {
    return radius[i1] > radius[i2];
  }
};

double BubbleTree::computeRelativePosition(
    tlp::node n, TLP_HASH_MAP<tlp::node, tlp::Vector<double, 5> > *relativePosition) {

  Size tmpSizeFather = nodeSize->getNodeValue(n);
  tmpSizeFather[2] = 0.; // remove z-coordiantes because the drawing is 2D
  double sizeFather = tmpSizeFather.norm() / 2.;

  if (sizeFather < 1E-5)
    sizeFather = 1.;

  double sizeVirtualNode = 1.;

  if (tree->indeg(n) == 0)
    sizeVirtualNode = 0.;

  /*
   * Initialize node position
   */
  (*relativePosition)[n][0] = 0.;
  tlp::Vector<double, 5> &rPos = (*relativePosition)[n];
  rPos[1] = 0.;

  /*
   * Special case if the node is a leaf.
   */
  if (tree->outdeg(n) == 0) {
    rPos[2] = 0.;
    rPos[3] = 0.;
    Size tmpSizeNode = nodeSize->getNodeValue(n);
    tmpSizeNode[2] = 0.;
    return (rPos[4] = tmpSizeNode.norm() / 2.);
  }

  /*
   * Recursive call to obtain the set of radius of the children of n
   * A node is dynamically inserted in the neighborhood of n in order to
   * reserve space for the connection of the father of n
   */
  unsigned int Nc = tree->outdeg(n) + 1;
  vector<double> angularSector(Nc);
  std::vector<double> realCircleRadius(Nc);
  realCircleRadius[0] = sizeVirtualNode;
  double sumRadius = sizeVirtualNode;

  Iterator<node> *itN = tree->getOutNodes(n);

  for (unsigned int i = 1; itN->hasNext(); ++i) {
    node itn = itN->next();
    realCircleRadius[i] = computeRelativePosition(itn, relativePosition);
    sumRadius += realCircleRadius[i];
  }

  delete itN;

  double resolution = 0.;

  if (nAlgo) {
    std::vector<double> subCircleRadius(Nc);
    subCircleRadius[0] = realCircleRadius[0];
    double maxRadius = sizeVirtualNode;
    unsigned int maxRadiusIndex = 0;

    for (unsigned int i = 0; i < Nc; ++i) {
      subCircleRadius[i] = realCircleRadius[i];

      if (maxRadius < subCircleRadius[i]) {
        maxRadius = subCircleRadius[i];
        maxRadiusIndex = i;
      }
    }

    if (maxRadius > (sumRadius / 2.)) {
      double ratio;

      if (sumRadius - maxRadius > 1E-5)
        ratio = maxRadius / (sumRadius - maxRadius);
      else
        ratio = 1.;

      for (unsigned int i = 0; i < Nc; ++i) {
        if (i != maxRadiusIndex)
          subCircleRadius[i] *= ratio;
      }

      sumRadius = 2. * maxRadius;
    }

    for (unsigned int i = 0; i < Nc; ++i) {
      angularSector[i] = 2. * M_PI * subCircleRadius[i] / sumRadius;
    }
  } else {
    resolution = 2. * M_PI;
    std::vector<unsigned> index(Nc);

    for (unsigned i = 0; i < Nc; ++i)
      index[i] = i;

    sort(index.begin(), index.end(), greaterRadius(realCircleRadius));
    std::vector<unsigned>::const_iterator i = index.begin();

    for (; i != index.end(); ++i) {
      double radius = realCircleRadius[*i];
      double angleMax = 2. * asin(radius / (radius + sizeFather));
      double angle = radius * resolution / sumRadius;

      if (angle > angleMax) {
        angularSector[*i] = angleMax;
        sumRadius -= radius;
        resolution -= angleMax;
      } else
        break;
    }

    if (i != index.end()) {
      for (; i != index.end(); ++i) {
        double radius = realCircleRadius[*i];
        double angle = radius * resolution / sumRadius;
        angularSector[*i] = angle;
      }

      resolution = 0.;
    } else
      resolution /= Nc;
  }

  double angle = 0.;
  vector<Circled> circles(Nc);

  for (unsigned int i = 0; i < Nc; ++i) {
    double packRadius;

    if (fabs(sin(angularSector[i])) > 1E-05)
      packRadius = realCircleRadius[i] / sin(angularSector[i] / 2.);
    else
      packRadius = 0.;

    packRadius = std::max(packRadius, sizeFather + realCircleRadius[i]);

    if (i > 0)
      angle += (angularSector[i - 1] + angularSector[i]) / 2. + resolution;

    circles[i][0] = packRadius * cos(angle);
    circles[i][1] = packRadius * sin(angle);
    circles[i].radius = realCircleRadius[i];
  }

  Circled circleH = tlp::enclosingCircle(circles);
  rPos[2] = -circleH[0];
  rPos[3] = -circleH[1];
  rPos[4] = sqrt(circleH.radius * circleH.radius - circleH[1] * circleH[1]) - fabs(circleH[0]);
  /*
   * Set relative position of all children
   * according to the center of the enclosing circle
   */
  itN = tree->getOutNodes(n);

  for (unsigned int i = 1; i < Nc; ++i) {
    node itn = itN->next();
    (*relativePosition)[itn][0] = circles[i][0] - circleH[0];
    (*relativePosition)[itn][1] = circles[i][1] - circleH[1];
  }

  delete itN;
  return circleH.radius;
}

void BubbleTree::calcLayout2(tlp::node n, tlp::Vector<double, 5> &nrPos,
                             TLP_HASH_MAP<tlp::node, tlp::Vector<double, 5> > *relativePosition,
                             const tlp::Vector<double, 3> &enclosingCircleCenter,
                             const tlp::Vector<double, 3> &originNodePosition) {
  /*
   * Make rotation around the center of the enclosing circle in order to align :
   * the virtual node, the enclosing circle' center and the grand father of the node.
   */
  Vector<double, 3> bend, zeta, zetaOriginal;
  bend.fill(0.);
  bend[0] = nrPos[4];

  zeta[0] = nrPos[2];
  zeta[1] = nrPos[3];
  zeta[2] = 0.;
  zetaOriginal = zeta;

  Vector<double, 3> vect, vect3;
  vect = originNodePosition - enclosingCircleCenter;
  vect /= vect.norm();
  vect3 = zeta + bend;
  vect3 /= vect3.norm();

  double cosAlpha, sinAlpha;
  cosAlpha = (vect3.dotProduct(vect));
  sinAlpha = (vect ^ vect3)[2];

  Vector<double, 3> rot1, rot2;
  rot1[0] = cosAlpha;
  rot1[1] = -sinAlpha;
  rot1[2] = 0.;
  rot2[0] = sinAlpha;
  rot2[1] = cosAlpha;
  rot2[2] = 0.;
  zeta = rot1 * zeta[0] + rot2 * zeta[1];

  result->setNodeValue(n, Coord(float(enclosingCircleCenter[0] + zeta[0]),
                                float(enclosingCircleCenter[1] + zeta[1]), 0.));

  /*
   * Place bend on edge to prevent overlapping
   */
  if (tree->outdeg(n) > 0) {
    bend += zetaOriginal;
    bend = rot1 * bend[0] + rot2 * bend[1];
    bend += enclosingCircleCenter;
    Vector<double, 3> a = enclosingCircleCenter + zeta - bend;
    Vector<double, 3> b = originNodePosition - bend;
    a /= a.norm();
    b /= b.norm();

    if ((1. - fabs(a.dotProduct(b))) > 1E-5) {
      Iterator<edge> *itE = tree->getInEdges(n);
      edge ite = itE->next();
      delete itE;
      vector<Coord> tmp(1);
      tmp[0] = Coord(float(bend[0]), float(bend[1]), 0.);
      result->setEdgeValue(ite, tmp);
    }
  }

  /*
   * Make the recursive call, to place the children of n.
   */
  Iterator<node> *it = tree->getOutNodes(n);

  while (it->hasNext()) {
    node itn = it->next();
    tlp::Vector<double, 5> &rPos = (*relativePosition)[itn];
    Vector<double, 3> newpos;
    newpos[0] = rPos[0];
    newpos[1] = rPos[1];
    newpos[2] = 0.;
    newpos = rot1 * newpos[0] + rot2 * newpos[1];
    newpos += enclosingCircleCenter;
    calcLayout2(itn, rPos, relativePosition, newpos, enclosingCircleCenter + zeta);
  }

  delete it;
}

void BubbleTree::calcLayout(tlp::node n,
                            TLP_HASH_MAP<tlp::node, tlp::Vector<double, 5> > *relativePosition) {
  /*
   * Make the recursive call, to place the children of n.
   */
  result->setNodeValue(n, Coord(0., 0., 0.));
  Iterator<node> *it = tree->getOutNodes(n);

  if (it->hasNext()) {
    tlp::Vector<double, 5> &nPos = (*relativePosition)[n];
    double nPos2 = nPos[2];
    double nPos3 = nPos[3];

    while (it->hasNext()) {
      node itn = it->next();
      Vector<double, 3> origin, tmp;
      tlp::Vector<double, 5> &rPos = (*relativePosition)[itn];
      origin[0] = rPos[0] - nPos2;
      origin[1] = rPos[1] - nPos3;
      origin[2] = 0.;
      tmp.fill(0.);
      calcLayout2(itn, rPos, relativePosition, origin, tmp);
    }
  }

  delete it;
}

static const char *paramHelp[] = {
    // Complexity
    "This parameter enables to choose the complexity of the algorithm."
    "If true, the complexity is O(n.log(n)), if false it is O(n)."};

BubbleTree::BubbleTree(const tlp::PluginContext *context) : LayoutAlgorithm(context) {
  addNodeSizePropertyParameter(this);
  addInParameter<bool>("complexity", paramHelp[0], "true");
  addDependency("Connected Component Packing", "1.0");
}

BubbleTree::~BubbleTree() {}

bool BubbleTree::run() {

  if (!ConnectedTest::isConnected(graph)) {
    // for each component draw
    std::vector<std::vector<node> > components;
    string err;
    // push a temporary graph state (not redoable)
    graph->push(false);
    ConnectedTest::computeConnectedComponents(graph, components);

    for (unsigned int i = 0; i < components.size(); ++i) {
      Graph *tmp = graph->inducedSubGraph(components[i]);
      tmp->applyPropertyAlgorithm("Bubble Tree", result, err, pluginProgress, dataSet);
    }

    // call connected component packing
    LayoutProperty tmpLayout(graph);
    DataSet tmpdataSet;
    tmpdataSet.set("coordinates", result);
    graph->applyPropertyAlgorithm("Connected Component Packing", &tmpLayout, err, pluginProgress,
                                  &tmpdataSet);
    // forget last temporary graph state
    graph->pop();
    *result = tmpLayout;
    return true;
  }

  if (!getNodeSizePropertyParameter(dataSet, nodeSize)) {
    if (graph->existProperty("viewSize")) {
      nodeSize = graph->getProperty<SizeProperty>("viewSize");
    } else {
      nodeSize = graph->getProperty<SizeProperty>("viewSize");
      nodeSize->setAllNodeValue(Size(1., 1., 1.));
    }
  }

  if (dataSet == 0 || !dataSet->get("complexity", nAlgo))
    nAlgo = true;

  result->setAllEdgeValue(vector<Coord>(0));

  if (pluginProgress)
    pluginProgress->showPreview(false);

  // push a temporary graph state (not redoable)
  // preserving layout updates
  std::vector<PropertyInterface *> propsToPreserve;

  if (result->getName() != "")
    propsToPreserve.push_back(result);

  graph->push(false, &propsToPreserve);

  tree = TreeTest::computeTree(graph, pluginProgress);

  if (pluginProgress && pluginProgress->state() != TLP_CONTINUE) {
    TreeTest::cleanComputedTree(graph, tree);
    graph->pop();
    return false;
  }

  node startNode = tree->getSource();
  assert(startNode.isValid());
  TLP_HASH_MAP<node, Vector<double, 5> > relativePosition;
  computeRelativePosition(startNode, &relativePosition);
  calcLayout(startNode, &relativePosition);

  TreeTest::cleanComputedTree(graph, tree);

  // forget last temporary graph state
  graph->pop();

  return true;
}
