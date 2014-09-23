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
#include <math.h>
#include <stack>
#include <tulip/TulipPluginHeaders.h>
#include "DatasetTools.h"

using namespace std;
using namespace tlp;

/** \addtogroup layout */

/// An implementation of a radial drawing of trees.
/**
 * This algorithm is inspired from
 * MoireGraphs: Radial Focus+Context Visualization and Interaction for Graphs with Visual Nodes
 * from T. J. Jankun-Kelly, Kwan-Liu Ma
 * published in IEEE Symposium on Information Visualization (2003)
 **/
class TreeRadial:public LayoutAlgorithm {
public:
  PLUGININFORMATION("Tree Radial","Patrick Mary","14/05/2007","Ok","1.0","Tree")
  Graph *tree;
  vector<float> nRadii;
  vector<float> lRadii;
  vector<vector <node> > bfs;

  /* the original code using dfs recursive calls
     is easier to understand but may result in stack overflow
  void dfsComputeNodeRadii(node n, unsigned depth, SizeProperty *sizes) {
    node on;
    float radius = sizes->getNodeValue(n).getW()/2;
    if (bfs.size() == depth) {
      bfs.push_back(vector<node>());
      nRadii.push_back(radius);
    } else if (radius > nRadii[depth]) {
  nRadii[depth] = radius;
    }
    bfs[depth].push_back(n);
    forEach(on, tree->getOutNodes(n)) {
      dfsComputeNodeRadii(on, depth + 1, sizes);
    }
    }*/

  // simple structure to implement
  // the dfs loop for node radii computation below
  struct dfsNodeRadiiStruct {
    node current;
    float radius;
    unsigned int depth;
    Iterator<node>* neighbours;

    dfsNodeRadiiStruct(node n = node(), float r = 0, unsigned int d = 0,
                       Iterator<node>* it = NULL):
      current(n), radius(r), depth(d), neighbours(it) {}
  };

  void dfsComputeNodeRadii(node n, SizeProperty *sizes) {
    MutableContainer<bool> visited;
    visited.setAll(false);
    stack<dfsNodeRadiiStruct> dfsLevels;
    dfsNodeRadiiStruct dfsParams(n, sizes->getNodeValue(n).getW()/2, 0,
                                 tree->getOutNodes(n));
    dfsLevels.push(dfsParams);

    while(!dfsLevels.empty()) {
      dfsParams = dfsLevels.top();
      n = dfsParams.current;
      float radius = dfsParams.radius;
      unsigned int depth = dfsParams.depth;
      Iterator<node>* it = dfsParams.neighbours;

      if (!visited.get(n.id)) {
        if (bfs.size() == depth) {
          bfs.push_back(vector<node>());
          nRadii.push_back(radius);
        }
        else if (radius > nRadii[depth]) {
          nRadii[depth] = radius;
        }

        bfs[depth].push_back(n);
        visited.set(n.id, true);
      }

      if (it->hasNext()) {
        // go deeper in the dfs loop
        n = it->next();
        dfsParams.current = n;
        dfsParams.radius = sizes->getNodeValue(n).getW()/2;
        dfsParams.depth = depth + 1;
        dfsParams.neighbours = tree->getOutNodes(n);
        dfsLevels.push(dfsParams);
      }
      else {
        // go back in the dfs levels
        dfsLevels.pop();
        delete it;
      }
    }
  }

  void bfsComputeLayerRadii(float lSpacing, float nSpacing, SizeProperty *) {
    if (bfs.size() < 2) return;

    float lRadius = 0, lSpacingMax = 0;
    lRadii.push_back(0);
    unsigned int nbLayers = bfs.size() - 1;

    for (unsigned int i = 0; i < nbLayers; ++i) {
      float lRadiusPrev = lRadius;
      lRadius += nRadii[i] + nRadii[i + 1] + lSpacing;
      // check if there is enough space for nodes of layer i + 1
      float mRadius = (bfs[i + 1].size() * (nRadii[i + 1] + nSpacing))/static_cast<float>(2 * M_PI);

      if (mRadius > lRadius)
        lRadius = mRadius;

      lRadii.push_back(lRadius);

      if ((lRadius - lRadiusPrev) > lSpacingMax)
        lSpacingMax = lRadius - lRadiusPrev;
    }

    ++nbLayers;
    lRadius = lSpacingMax;

    for (unsigned int i = 1; i < nbLayers; ++i, lRadius += lSpacingMax) {
      lRadii[i] = lRadius;
    }
  }

  /* the original code using dfs recursive calls
     is easier to understand but may result in stack overflow
  double dfsComputeAngularSpread(node n, unsigned int depth,
         SizeProperty *sizes, DoubleProperty *angles) {
    node on;
    double cAngle = 0;
    forEach(on, tree->getOutNodes(n)) {
      // compute the sum of the childs's angular spreads
      cAngle += dfsComputeAngularSpread(on, depth + 1, sizes, angles);
    }
    if (depth > 0) {
      // compute the node angular spread
      double nAngle = 2 * atan(sizes->getNodeValue(n).getW()/(2. * lRadii[depth]));
      // check if it is not greater than the sum
      if (nAngle > cAngle)
  cAngle = nAngle;
    }
    // affect the greater of the two computed angular spreads
    angles->setNodeValue(n, cAngle);
    return cAngle;
    }*/

  // simple structure to implement
  // the dfs loop for node radii computation below
  struct dfsAngularSpreadStruct {
    node current;
    double cAngle;
    unsigned int depth;
    Iterator<node>* neighbours;

    dfsAngularSpreadStruct(node n = node(), unsigned int d = 0,
                           Iterator<node>* it = NULL):
      current(n), cAngle(0), depth(d), neighbours(it) {}
  };

  void dfsComputeAngularSpread(node n, SizeProperty *sizes,
                               MutableContainer<double>& angles) {
    // we dont use recursive call to avoid a possible stack overflow
    stack<dfsAngularSpreadStruct> dfsLevels;
    dfsAngularSpreadStruct dfsParams(n, 0, tree->getOutNodes(n));
    dfsLevels.push(dfsParams);

    while(!dfsLevels.empty()) {
      dfsParams = dfsLevels.top();
      n = dfsParams.current;
      unsigned int depth = dfsParams.depth;
      Iterator<node>* it = dfsParams.neighbours;

      if (it->hasNext()) {
        // go deeper in the dfs levels
        n = it->next();
        dfsParams.current = n;
        dfsParams.cAngle = 0;
        dfsParams.depth = depth + 1;
        dfsParams.neighbours = tree->getOutNodes(n);
        dfsLevels.push(dfsParams);
      }
      else {
        delete it;
        dfsLevels.pop();
        double cAngle = dfsParams.cAngle;

        if (depth > 0) {
          // compute the node angular spread
          double nAngle = 2 * atan(sizes->getNodeValue(n).getW()/(2. * lRadii[depth]));

          // check if it is not greater than the sum
          if (nAngle > cAngle)
            cAngle = nAngle;
        }

        // gives the greater of the two computed angular spreads
        angles.set(n.id, cAngle);

        if (depth > 0) {
          dfsAngularSpreadStruct& upParams = dfsLevels.top();
          upParams.cAngle += cAngle;
        }
      }
    }
  }

  /* the original code using dfs recursive calls
     is easier to understand but may result in stack overflow
  void doLayout(node n, unsigned int depth, double startAngle, double endAngle,
    DoubleProperty *angles, bool checkAngle = false) {
    double sAngle = endAngle - startAngle;
    // this will avoid crossing between the egdes from n to its children
    // and the edge from its ancestor to n
    if (checkAngle && sAngle > M_PI) {
      endAngle = startAngle + M_PI;
      sAngle = M_PI;
    }
    if (depth > 0) {
      // layout the node in the middle of the sector
      double nAngle = (startAngle + endAngle)/2.0;
      result->setNodeValue(n, Coord(lRadii[depth] * cos(nAngle),
            lRadii[depth] * sin(nAngle),
            0));
    } else
      result->setNodeValue(n, Coord(0, 0, 0));
    node on;
    const double& nSpread = angles->getNodeValue(n);
    checkAngle = false;
    forEach(on, tree->getOutNodes(n)) {
      endAngle = startAngle + (sAngle * (angles->getNodeValue(on)/nSpread));
      doLayout(on, depth + 1, startAngle, endAngle, angles, checkAngle);
      checkAngle = true;
      startAngle = endAngle;
    }
    }*/

  // simple structure to implement
  // the dfs loop for node radii computation below
  struct dfsDoLayoutStruct {
    node current;
    double startAngle;
    double endAngle;
    double sAngle;
    double nSpread;
    bool checkAngle;
    unsigned int depth;
    Iterator<node>* neighbours;

    dfsDoLayoutStruct(node n = node(), double bAngle = 0, double eAngle = 0,
                      double spread = 0, bool flag = false, unsigned int d = 0,
                      Iterator<node>* it = NULL):
      current(n), startAngle(bAngle), endAngle(eAngle),
      sAngle(eAngle - bAngle), nSpread(spread),
      checkAngle(flag), depth(d), neighbours(it) {}
  };

  void doLayout(node n, MutableContainer<double>& angles) {
    MutableContainer<bool> visited;
    visited.setAll(false);
    stack<dfsDoLayoutStruct> dfsLevels;
    dfsDoLayoutStruct dfsParams(n, 0, 2 * M_PI, angles.get(n.id), false,
                                0, tree->getOutNodes(n));
    dfsLevels.push(dfsParams);

    while(!dfsLevels.empty()) {
      dfsParams = dfsLevels.top();
      n = dfsParams.current;
      double startAngle = dfsParams.startAngle;
      double endAngle = dfsParams.endAngle;
      double sAngle = dfsParams.sAngle;
      unsigned int depth = dfsParams.depth;
      bool checkAngle = visited.get(n.id);

      if (!visited.get(n.id)) {
        // this will avoid crossing between the egdes from n to its children
        // and the edge from its ancestor to n
        if (dfsParams.checkAngle && sAngle > M_PI) {
          endAngle = startAngle + M_PI;
          sAngle = M_PI;
          dfsLevels.top().sAngle = sAngle;
        }

        if (depth > 0) {
          // layout the node in the middle of the sector
          double nAngle = (startAngle + endAngle)/2.0;
          result->setNodeValue(n, Coord(lRadii[depth] * static_cast<float>(cos(nAngle)),
                                        lRadii[depth] * static_cast<float>(sin(nAngle)),
                                        0));
        }
        else
          result->setNodeValue(n, Coord(0, 0, 0));

        visited.set(n.id, true);
      }

      Iterator<node>* it = dfsParams.neighbours;
      double nSpread = dfsParams.nSpread;

      if (it->hasNext()) {
        // go deeper in the dfs levels
        n = it->next();
        double angle = angles.get(n.id);
        endAngle = startAngle + (sAngle * (angle/nSpread));
        dfsLevels.top().startAngle = endAngle;
        dfsParams.current = n;
        dfsParams.endAngle = endAngle;
        dfsParams.sAngle = endAngle - startAngle,
                  dfsParams.nSpread = angle;
        dfsParams.checkAngle = checkAngle;
        dfsParams.depth = depth + 1;
        dfsParams.neighbours = tree->getOutNodes(n);
        dfsLevels.push(dfsParams);
      }
      else {
        // go back in the dfs levels
        delete it;
        dfsLevels.pop();
      }
    }
  }

  TreeRadial(const PluginContext* context):LayoutAlgorithm(context), tree(NULL) {
    addNodeSizePropertyParameter(this);
    addSpacingParameters(this);
  }

  bool run() {
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

    float nSpacing, lSpacing;
    SizeProperty* sizes;

    if (!getNodeSizePropertyParameter(dataSet, sizes))
      sizes = graph->getProperty<SizeProperty>("viewSize");

    getSpacingParameters(dataSet, nSpacing, lSpacing);

    node n;
    SizeProperty *circleSizes = new SizeProperty(graph);
    forEach(n, tree->getNodes()) {
      const Size& boundingBox = sizes->getNodeValue (n);
      double diam = 2.*sqrt (boundingBox.getW() * boundingBox.getW()/4.0 +
                             boundingBox.getH() * boundingBox.getH()/4.0);
      circleSizes->setNodeValue (n, Size (static_cast<float>(diam), static_cast<float>(diam), 1.0f));
    }
    sizes = circleSizes;

    node root = tree->getSource();
    assert(root.isValid());

    dfsComputeNodeRadii(root, sizes);
    bfsComputeLayerRadii(lSpacing, nSpacing, sizes);
    MutableContainer<double> angles;
    angles.setAll(0.);
    dfsComputeAngularSpread(root, sizes, angles);
    doLayout(root, angles);

    delete sizes;
    //clear all bends in the Layout
    result->setAllEdgeValue(vector<Coord>());
    // forget last temporary graph state
    graph->pop();

    return true;
  }
};

PLUGIN(TreeRadial)
