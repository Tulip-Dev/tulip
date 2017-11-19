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
#ifdef _OPENMP
#include <omp.h>
#endif
#include <algorithm>
#include <tulip/TulipPluginHeaders.h>

using namespace std;
using namespace tlp;

class CompNodes {

public:
  Graph *graph;
  CompNodes(Graph *g) : graph(g) {}

  bool operator()(const node u, const node v) {
    unsigned int du = graph->deg(u), dv = graph->deg(v);

    if (du == dv)
      return u.id > v.id;

    return du > dv;
  }
};

/*@{*/
/** \file
 *  \brief  An implementation of the Welsh and Powell algorithm
 *
 * This plugin is an implementation of the Welsh and Powell
 * graph coloring algorithm. It computes a set of integer measures
 * for the nodes of a graph in such a way that the measures assigned to 2
 * adjacent nodes are always different.
 *
 * (see http://en.wikipedia.org/wiki/Graph_coloring for more details)
 *
 *  <b>HISTORY</b>
 *
 *  - 2005 Version 1.0: Initial release
 *  by David Auber, LaBRI, University Bordeaux I, France
 *
 *
*/
class WelshPowell : public DoubleAlgorithm {
  // to maximize the locality of reference we will use a vector
  // holding the all the nodes needed info in the structure below
  struct nodeInfo {
    node n;
    int val;
  };

  // a comparator used to sort the vector
  struct nodesInfoCmp {
    bool operator()(const nodeInfo &u, const nodeInfo &v) {
      int du = u.val, dv = v.val;

      if (du == dv)
        return u.n.id > v.n.id;

      return du > dv;
    }
  };

public:
  PLUGININFORMATION(
      "Welsh & Powell", "David Auber", "03/01/2005",
      "Nodes coloring measure,<br/>values assigned to adjacent nodes are always different.", "1.0",
      "Graph")

  WelshPowell(const tlp::PluginContext *context) : DoubleAlgorithm(context) {}

  bool run() override {
    const std::vector<node> nodes = graph->nodes();
    unsigned int nbNodes = nodes.size();
    NodeStaticProperty<nodeInfo> nodesInfo(graph);
    node n;
    OMP_ITER_TYPE i = 0;
#ifdef _OPENMP
#pragma omp parallel for
#endif

    for (i = 0; i < nbNodes; ++i) {
      node n = nodes[i];
      nodeInfo &nInfo = nodesInfo[i];
      nInfo.n = n, nInfo.val = graph->deg(n);
    }

    // sort the nodes in descending order of their degrees
    sort(nodesInfo.begin(), nodesInfo.end(), nodesInfoCmp());
    // build a map
    NodeStaticProperty<unsigned int> toNodesInfo(graph);
#ifdef _OPENMP
#pragma omp parallel for
#endif

    for (i = 0; i < nbNodes; ++i) {
      nodeInfo &nInfo = nodesInfo[i];
      // initialize the value
      nInfo.val = -1;
      toNodesInfo.setNodeValue(nInfo.n, i);
    }

    int currentColor = 0;
    unsigned int numberOfColoredNodes = 0;
    unsigned int minIndex = 0;
    unsigned int maxIndex = nbNodes;

    while (numberOfColoredNodes != nbNodes) {
#ifndef NDEBUG
      cout << "nbColored :" << numberOfColoredNodes << endl;
#endif
      unsigned int nextMaxIndex = minIndex;

      for (i = minIndex; i < maxIndex; ++i) {
#ifndef NDEBUG
        cout << "i:" << i << endl;
#endif
        nodeInfo &nInfo = nodesInfo[i];

        if (nInfo.val == -1) {
          bool sameColor = false;
          node u;
          forEach(u, graph->getInOutNodes(nInfo.n)) {
            if (nodesInfo[toNodesInfo.getNodeValue(u)].val == currentColor) {
              sameColor = true;
              break;
            }
          }

          if (!sameColor) {
#ifndef NDEBUG
            cout << "new node found color : " << currentColor << endl;
#endif
            nInfo.val = currentColor;
            ++numberOfColoredNodes;

            if (i == minIndex)
              ++minIndex;
          } else
            nextMaxIndex = i + 1;
        } else if (i == minIndex)
          ++minIndex;
      }

      maxIndex = nextMaxIndex;
      ++currentColor;
    }

    // finally set the values
    for (i = 0; i < nbNodes; ++i) {
      nodeInfo &nInfo = nodesInfo[i];
      result->setNodeValue(nInfo.n, nInfo.val);
    }

    return true;
  }
};
/*@}*/
PLUGIN(WelshPowell)
