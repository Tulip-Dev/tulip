/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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
#include <tulip/ForEach.h>
#include <tulip/TulipPluginHeaders.h>

#ifdef _OPENMP
#include <omp.h>
#endif

class BubblePack : public tlp::LayoutAlgorithm {
public:
  PLUGININFORMATION("Bubble Pack", "D.Auber", "01/10/2012", "Stable", "1.0", "Tree")
  BubblePack(const tlp::PluginContext *context);
  ~BubblePack();
  bool run();

private:
  double computeRelativePosition(tlp::node n,
                                 tlp::NodeStaticProperty<tlp::Vec4f> &relativePosition);
  void calcLayout(tlp::node n, tlp::Vec2f pos,
                  tlp::NodeStaticProperty<tlp::Vec4f> &relativePosition);

  tlp::Graph *tree;
  tlp::SizeProperty *nodeSize;
  bool nAlgo;
};

PLUGIN(BubblePack)

using namespace std;
using namespace tlp;

struct greaterRadius {
  const std::vector<double> &radius;
  greaterRadius(const std::vector<double> &r) : radius(r) {}
  bool operator()(unsigned i1, unsigned i2) const {
    return radius[i1] > radius[i2];
  }
};

struct lessRadius {
  const std::vector<double> &radius;
  lessRadius(const std::vector<double> &r) : radius(r) {}
  bool operator()(unsigned i1, unsigned i2) const {
    return radius[i1] < radius[i2];
  }
};

double BubblePack::computeRelativePosition(tlp::node n,
                                           NodeStaticProperty<Vec4f> &relativePosition) {

  Size centralNodeSize = nodeSize->getNodeValue(n);
  centralNodeSize[2] = 0.; // remove z-coordinates because the drawing is 2D
  double sizeFather = std::max(centralNodeSize[0], centralNodeSize[1]) / 2.;

  if (sizeFather < 1E-5)
    sizeFather = 0.1;

  unsigned int outdeg = tree->outdeg(n);

  /**
   * Special case if the node is a leaf.
   */
  if (outdeg == 0) {
    return sizeFather + 1.; // minimum spacing
  }

  /**
   * Recursive call to obtain the set of radius of the children of n
   */
  std::vector<double> realCircleRadius(outdeg);

  node ni;
  int i = 0;
  forEach(ni, tree->getOutNodes(n)) {
    realCircleRadius[i] = computeRelativePosition(ni, relativePosition);
    ++i;
  }

  /**
   * Pack Circles
   */
  vector<Circled> circles(outdeg);
  double angle = 2. * M_PI; // start position
  double bestAngle = angle;

  {
    std::vector<unsigned> index(outdeg);

    for (unsigned int i = 0; i < outdeg; ++i)
      index[i] = i;

    sort(index.begin(), index.end(), lessRadius(realCircleRadius));

    vector<Circled> placed;

    if (index.size() > 3) {
      double alpha = 0;
      double curRad = sizeFather;
      bool sens = true;

      for (unsigned int i = 0; i < index.size(); ++i) {
        double radius = realCircleRadius[index[i]];
        double crad = radius + curRad + 0.01;
        double calpha;

        if (sens)
          calpha = alpha + radius / crad;
        else
          calpha = alpha - radius / crad;

        Circled tmp(crad * cos(calpha), crad * sin(calpha), radius);
        bool reject = false;

        for (unsigned int k = 0; k < placed.size(); ++k) {
          if (placed[k].dist(tmp) < placed[k].radius + tmp.radius) {
            reject = true;
            break;
          }
        }

        if (reject) {
          curRad += radius + 0.01;
          sens = !sens;
          --i;
          continue;
        }

        double newalpha;

        if (sens)
          newalpha = alpha + 2.2 * radius / crad;
        else
          newalpha = alpha - 2.2 * radius / crad;

        Vec2f v0(crad * cos(calpha), crad * sin(calpha));
        Vec2f v1(crad * cos(newalpha), crad * sin(newalpha));

        while (v0.dist(v1) < radius) {
          if (sens)
            newalpha += 0.01;
          else
            newalpha -= 0.01;

          v1 = Vec2f(crad * cos(newalpha), crad * sin(newalpha));
        }

        alpha = newalpha;
        circles[index[i]] = tmp;
        placed.push_back(circles[index[i]]);
      }
    } else { //        if (false) //polyo packing
      for (unsigned int i = 0; i < index.size(); ++i) {
        // cerr << i << "." << flush;
        double radius = realCircleRadius[index[i]];
        double bestRadius = FLT_MAX;
        int discret = ceil(2. * (sizeFather + radius) * M_PI) + 3;
        angle += M_PI / 3.;
#ifdef _OPENMP
#pragma omp parallel for
#endif

        for (int j = 0; j < discret; ++j) {
          float _angle = float(j) * 2. * M_PI / float(discret) + angle;
          double spiralRadius = sizeFather + radius + 1E-3;
          Circled tmp(spiralRadius * cos(_angle), spiralRadius * sin(_angle), radius);
          bool restart = true;

          // int restcnt = 0;
          while (restart) {
            // restcnt += 1;
            restart = false;

            for (unsigned int k = 0; k < placed.size(); ++k) {
              if (placed[k].dist(tmp) < placed[k].radius + tmp.radius) {
                spiralRadius = std::max(spiralRadius, double(placed[k].norm()) + placed[k].radius +
                                                          radius + 1E-3);
                // spiralRadius += 0.01;
                tmp = Circled(spiralRadius * cos(_angle), spiralRadius * sin(_angle), radius);
                // restart = true;
              }
            }
          }

// if (restcnt > 10) cout << "Aie" << endl;
#ifdef _OPENMP
#pragma omp critical(GOODCIRCLE)
#endif
          {
            if (spiralRadius < bestRadius) {
              bestRadius = spiralRadius;
              bestAngle = _angle;
            }
          }
        }

        circles[index[i]][0] = bestRadius * cos(bestAngle);
        circles[index[i]][1] = bestRadius * sin(bestAngle);
        circles[index[i]].radius = radius;
        placed.push_back(circles[index[i]]);
      }
    }
  }
  circles.push_back(Circled(0., 0., sizeFather));
  // cerr << "Nb circles : " << circles.size() << endl << flush;
  Circled circleH;

  if (circles.size() > 2000) { // Stack overflow when number of circles exceed 2k
    circleH = tlp::lazyEnclosingCircle(circles);
  } else {
    circleH = tlp::enclosingCircle(circles);
  }

  // cerr << "packed " << endl << flush;
  /*
   * Set relative position of all children
   * according to the center of the enclosing circle
   */
  Iterator<node> *itN = tree->getOutNodes(n);

  for (unsigned int i = 0; i < outdeg; ++i) {
    Vec4f &relPos = relativePosition[graph->nodePos(itN->next())];
    Circled &circle = circles[i];
    relPos[0] = circle[0] - circleH[0];
    relPos[1] = circle[1] - circleH[1];
  }

  delete itN;

  Vec4f &relPos = relativePosition[graph->nodePos(n)];
  relPos[2] = -circleH[0];
  relPos[3] = -circleH[1];

  return circleH.radius + 1.;
}

void BubblePack::calcLayout(tlp::node n, Vec2f pos, NodeStaticProperty<Vec4f> &relativePosition) {
  /*
   * Make the recursive call, to place the children of n.
   */
  Vec4f &relPos = relativePosition[graph->nodePos(n)];
  Vec2f shift(relPos[2], relPos[3]);
  result->setNodeValue(n, Coord(pos + shift, 0));
  node ni;
  forEach(ni, tree->getOutNodes(n)) {
    Vec4f &relPos = relativePosition[graph->nodePos(ni)];
    Vec2f relat(relPos[0], relPos[1]);
    calcLayout(ni, pos + relat, relativePosition);
  }
}

static const char *paramHelp[] = {
    // Complexity
    HTML_HELP_OPEN() HTML_HELP_DEF("type", "bool")
        HTML_HELP_DEF("values", "[true, false] o(nlog(n)) / o(n)") HTML_HELP_DEF("default", "true")
            HTML_HELP_BODY() "This parameter enables to choose the complexity of the "
                             "algorithm." HTML_HELP_CLOSE(),
    HTML_HELP_OPEN() HTML_HELP_DEF("type", "Size")
        HTML_HELP_DEF("values", "An existing size property") HTML_HELP_DEF("default", "viewSize")
            HTML_HELP_BODY() "This parameter defines the property used for node's "
                             "sizes." HTML_HELP_CLOSE()};

BubblePack::BubblePack(const tlp::PluginContext *context) : LayoutAlgorithm(context) {
  addInParameter<SizeProperty>("node size", paramHelp[1], "viewSize");
  addInParameter<bool>("complexity", paramHelp[0], "true");
  addDependency("Connected Component Packing", "1.0");
}

BubblePack::~BubblePack() {}

bool BubblePack::run() {
  if (!ConnectedTest::isConnected(graph)) {
    // for each component draw
    std::vector<std::vector<node> > components;
    string err;
    // push a temporary graph state (not redoable)
    graph->push(false);
    ConnectedTest::computeConnectedComponents(graph, components);

    for (unsigned int i = 0; i < components.size(); ++i) {
      Graph *tmp = graph->inducedSubGraph(components[i]);
      tmp->applyPropertyAlgorithm("Bubble Pack", result, err, pluginProgress, dataSet);
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

  if (!((dataSet != NULL) && dataSet->get("node size", nodeSize))) {
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
    graph->pop();
    return false;
  }

  node startNode = tree->getSource();
  assert(startNode.isValid());
  NodeStaticProperty<Vec4f> relativePosition(graph);
  // cerr << "C" << endl << flush;
  computeRelativePosition(startNode, relativePosition);
  // cerr << "D" << endl << flush;
  calcLayout(startNode, Vec2f(0, 0), relativePosition);
  // cerr << "E" << endl << flush;

  // forget last temporary graph state
  graph->pop();

  return true;
}
