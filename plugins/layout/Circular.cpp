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
#include "Circular.h"
#include "DatasetTools.h"

#include <tulip/ConnectedTest.h>
#include <tulip/GraphTools.h>

PLUGIN(Circular)

using namespace std;
using namespace tlp;

static const char *paramHelp[] = {
    // search cycle
    "If true, search first for the maximum length cycle (be careful, this problem is NP-Complete). "
    "If false, nodes are ordered using a depth first search."};

Circular::Circular(const tlp::PluginContext *context) : LayoutAlgorithm(context) {
  addNodeSizePropertyParameter(this);
  addInParameter<bool>("search cycle", paramHelp[0], "false");
}

//===============================================================================
static vector<node> extractCycle(node n, deque<node> &st) {
  vector<node> result;
  auto it = st.rbegin();

  while ((*it) != n) {
    result.push_back(*it);
    ++it;
  }

  result.push_back(*it);
  return result;
}
//===============================================================================
static void dfs(node n, const Graph *sg, deque<node> &st, vector<node> &maxCycle,
                MutableContainer<bool> &flag, unsigned int &nbCalls,
                PluginProgress *pluginProgress) {
  {
    // to enable stop of the recursion
    ++nbCalls;

    if (nbCalls % 10000 == 0) {
      pluginProgress->progress(randomInteger(100), 100);
      nbCalls = 0;
    }

    if (pluginProgress->state() != TLP_CONTINUE)
      return;
  }

  if (flag.get(n.id)) {
    vector<node> cycle(extractCycle(n, st));

    if (cycle.size() > maxCycle.size())
      maxCycle.swap(cycle);

    return;
  }

  st.push_back(n);
  flag.set(n.id, true);
  for (auto n2 : sg->getInOutNodes(n)) {
    dfs(n2, sg, st, maxCycle, flag, nbCalls, pluginProgress);
  }
  flag.set(n.id, false);
  st.pop_back();
}

//=======================================================================
static vector<node> findMaxCycle(Graph *graph, PluginProgress *pluginProgress) {
  // compute the connected components's subgraphs
  std::vector<std::vector<node>> components;
  ConnectedTest::computeConnectedComponents(graph, components);

  vector<node> max;
  unsigned int nbCalls = 0;
  for (unsigned int i = 0; i < components.size(); ++i) {
    Graph *sg = graph->inducedSubGraph(components[i]);
    MutableContainer<bool> flag;
    flag.setAll(false);
    deque<node> st;
    vector<node> res;

    dfs(sg->getOneNode(), sg, st, res, flag, nbCalls, pluginProgress);

    if (max.size() < res.size())
      max.swap(res);

    graph->delAllSubGraphs(sg);
  }

  return max;
}

// this inline function computes the radius size given a size
inline double computeRadius(const Size &s) {
  return std::max(1E-3, sqrt(s[0] * s[0] / 4.0 + s[1] * s[1] / 4.0));
} // end computeRad

bool Circular::run() {
  SizeProperty *nodeSize;
  bool searchCycle = false;

  if (!getNodeSizePropertyParameter(dataSet, nodeSize)) {
    if (graph->existProperty("viewSize"))
      nodeSize = graph->getProperty<SizeProperty>("viewSize");
    else {
      nodeSize = graph->getProperty<SizeProperty>("viewSize");
      nodeSize->setAllNodeValue(Size(1.0, 1.0, 1.0));
    }
  }

  if (dataSet != nullptr)
    dataSet->get("search cycle", searchCycle);

  // compute the sum of radii and the max radius of the graph
  double sumOfRad = 0;
  double maxRad = 0;
  node maxRadNode;
  for (auto n : graph->nodes()) {
    double rad = computeRadius(nodeSize->getNodeValue(n));
    sumOfRad += rad;

    if (maxRad < rad) {
      maxRad = rad;
      maxRadNode = n;
    } // end if
  }

  // with two nodes, lay them on a line max rad apart
  if (graph->numberOfNodes() <= 2) {
    // set the (max 2) nodes maxRad apart
    double xcoord = maxRad / 2.0;
    for (auto n : graph->nodes()) {
      result->setNodeValue(n, Coord(xcoord, 0, 0));
      xcoord *= -1;
    }
  } // end if
  else {
    // this is the current angle
    double gamma = 0;
    // if the ratio of maxRad/sumOfRad > .5, we need to adjust angles
    bool angleAdjust = false;

    if (maxRad / sumOfRad > 0.5) {
      sumOfRad -= maxRad;
      angleAdjust = true;
    } // end if

    // tlp::warning() << "*************************" << endl;

    vector<node> cycleOrdering;

    if (searchCycle)
      cycleOrdering = findMaxCycle(graph, pluginProgress);

    vector<node> dfsOrdering;
    tlp::dfs(graph, dfsOrdering);

    MutableContainer<bool> inCir;
    inCir.setAll(false);

    for (unsigned int i = 0; i < cycleOrdering.size(); ++i)
      inCir.set(cycleOrdering[i].id, true);

    for (unsigned int i = 0; i < dfsOrdering.size(); ++i)
      if (!inCir.get(dfsOrdering[i].id))
        cycleOrdering.push_back(dfsOrdering[i]);

    for (auto n : cycleOrdering) {
      // compute the radius to ensure non overlap.  If adjustment to
      // ensure no angle greater than pi done, detect it.
      double nodeRad = computeRadius(nodeSize->getNodeValue(n));
      double halfAngle = (nodeRad / sumOfRad) * ((angleAdjust) ? M_PI / 2.0 : M_PI);
      double rayon = nodeRad / sin(halfAngle);

      // if this node was the node that took up more than half the circle,
      // we complete the adjustment to make sure that it does not.
      if (angleAdjust && (maxRadNode == n)) {
        halfAngle = M_PI / 2.0;
        rayon = nodeRad;
      } // end if

      // compute the position of the node.
      gamma += halfAngle;
      result->setNodeValue(n, Coord(rayon * cos(gamma), rayon * sin(gamma), 0));
      gamma += halfAngle;
    } // end while
  }   // end else

  return true;
} // end run
