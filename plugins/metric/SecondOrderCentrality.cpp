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

#include <tulip/DoubleProperty.h>
#include <tulip/IntegerProperty.h>
#include <tulip/StaticProperty.h>
#include <tulip/BooleanProperty.h>
#include <tulip/TlpTools.h>

#include <algorithm>
#include <numeric>

using namespace std;
using namespace tlp;

/** \addtogroup metric */
/*@{*/
/** \file
 * \brief  An implementation of the Second Order centrality algorithm
 *
 * This plugin is an implementation of the Second Order centrality measure
 * first published as:
 *
 * Kermarrec, A.-M., et al. (2011). "Second order centrality: Distributed assessment of nodes
 * criticity in complex networks." Computer Communications 34(5): 619-628. doi:
 * https://dx.doi.org/10.1016/j.comcom.2010.06.007.
 *
 * This algorithm computes the standard deviation of the return time on each node of
 * a random walker. Central nodes are those with the lower values.
 *
 * \author Bruno Pinaud, LaBRI
 *
 * \todo Make it parallel: 30 walkers per node instead of one
 *
 **/
class SecondOrderCentrality : public tlp::DoubleAlgorithm {

public:
  PLUGININFORMATION(
      "Second Order Centrality", "Bruno Pinaud", "01/10/2020",
      "<p>An implementation of the Second Order centrality measure first published as:<br>"
      "Kermarrec, A.-M., et al. (2011). \"Second order centrality: Distributed assessment of nodes "
      "criticity in complex networks.\" Computer Communications 34(5): 619-628, <br/>doi: <a "
      "href=\"https://dx.doi.org/10.1016/j.comcom.2010.06.007\">https://dx.doi.org/10.1016/"
      "j.comcom.2010.06.007</a>.</p>"
      "<p>This algorithm computes the standard deviation of the return time on each node of"
      " a random walker. Central nodes are those with the lower values.</p>",
      "1.0", "Clustering")
  SecondOrderCentrality(const tlp::PluginContext *);
  bool run() override;
  bool check(string &err) override;
  bool randomWalk(NodeStaticProperty<vector<int>> &tickVector, const unsigned &maxNbSteps);
  node getRandomNeighbor(const node n);
};

//========================================================================================
static const char *paramHelp[] = {
    // selection
    "Boolean Property for choosing the starting node instead of choosing a node randomly if "
    "nothing is selected.",
    // debug
    "Activate debug mode to get the vector of each time the walker pass through a node in a "
    "property called tickVector."

};
//========================================================================================

SecondOrderCentrality::SecondOrderCentrality(const tlp::PluginContext *context)
    : DoubleAlgorithm(context) {
  addInParameter<BooleanProperty>("Selection", paramHelp[0], "viewSelection", false);
  addInParameter<bool>("Debug mode", paramHelp[1], "false");
}

node SecondOrderCentrality::getRandomNeighbor(const node n) {
  // get a random integer based on the number of neighbors
  unsigned randidx(randomUnsignedInteger(graph->deg(n) - 1));
  // get this neighbor
  node neigh;
  for (auto v : graph->getInOutNodes(n)) {
    if (randidx > 0) {
      randidx--;
      continue;
    }
    neigh = v;
    break;
  }
  return neigh;
}

bool SecondOrderCentrality::randomWalk(NodeStaticProperty<vector<int>> &tickVector,
                                       const unsigned &maxNbSteps) {
  unsigned tick = 0, nbSteps = 0;
  node activeNode, previousNode;

  // Start: finds a node that has viewSelection to True.
  // if None, selects a node at random
  BooleanProperty *viewSelection = graph->getProperty<BooleanProperty>("viewSelection");
  if (dataSet != nullptr) {
    dataSet->get("Selection", viewSelection);
  }
  if (viewSelection->hasNonDefaultValuatedNodes()) {
    auto *selNodesIt = viewSelection->getNonDefaultValuatedNodes(graph);
    while (selNodesIt->hasNext()) {
      node n = selNodesIt->next();
      // we must find a node with neighbors
      if (graph->deg(n))
        activeNode = n;
    }
    delete selNodesIt;
  }
  if (!activeNode.isValid())
    activeNode = graph->ends(graph->getRandomEdge()).first;

  // start random walk
  while (tick < maxNbSteps) {
    // select neighbors of the active node at random following the Metropolis-Gibbs algorithm to
    // ensure an equiprobabilistic choice
    node neigh(getRandomNeighbor(activeNode));
    double ratio = graph->deg(activeNode) / static_cast<double>(graph->deg(neigh));
    if (randomDouble() < ratio)
      activeNode = neigh;

    // update tick vector
    if (activeNode != previousNode) {
      tickVector[activeNode].push_back(tick);
      tick++;
    }
    nbSteps++;
    unsigned progress((nbSteps + 1) % 1000);
    if (pluginProgress && progress == 0 && pluginProgress->state() == TLP_CONTINUE) {
      pluginProgress->progress(progress, maxNbSteps);
    }
    if (pluginProgress && pluginProgress->state() != TLP_CONTINUE) {
      pluginProgress->setError("Cancelled by user");
      return false;
    }

    previousNode = activeNode;
  }
  return true;
}

//========================================================================================
bool SecondOrderCentrality::check(string &err) {
  if (graph->numberOfEdges() == 0) {
    err = "No edges. Cannot compute metric on this graph.";
    return false;
  }
  return true;
}

//========================================================================================
bool SecondOrderCentrality::run() {

  // initialize a random sequence according the given seed
  tlp::initRandomSequence();

  if (pluginProgress)
    pluginProgress->showPreview(false);

  NodeStaticProperty<vector<int>> tickVector(graph);

  if (pluginProgress) {
    pluginProgress->setComment("Walking on the graph...");
  }
  if (!randomWalk(tickVector, 25 * graph->numberOfEdges()))
    return false;
  if (pluginProgress && pluginProgress->state() == TLP_CONTINUE) {
    pluginProgress->setComment("Computing second order centrality");
  }
  NodeStaticProperty<double> res(graph);
  TLP_PARALLEL_MAP_NODES_AND_INDICES(graph, [&](const node n, unsigned i) {
    // compute mean and std dev of the return time (difference between two ticks)
    const vector<int> &tickvectorn = tickVector[n];
    // compute value only if the node has been visited at least 3 times
    if (tickvectorn.size() >= 3) {
      vector<int> val;
      // compute return time (difference between two consecutive ticks)
      std::transform(tickvectorn.begin() + 1, tickvectorn.end(), tickvectorn.begin(),
                     std::back_inserter(val), [](const int a, const int b) { return a - b; });
      // mean of the return time
      auto mean = accumulate(val.begin(), val.end(), 0) / val.size();

      // stddev of the return time
      double accum = 0.0;
      std::for_each(val.begin(), val.end(),
                    [&](const double d) { accum += (d - mean) * (d - mean); });

      res[i] = sqrt(accum / (val.size() - 1));

    } else {
      res[i] = DBL_MAX;
    }
  });
  res.copyToProperty(result);
  bool debug(false);
  if (dataSet != nullptr)
    dataSet->get("Debug mode", debug);
  if (debug) {
    IntegerVectorProperty *tickprop = graph->getProperty<IntegerVectorProperty>("tickVector");
    tickVector.copyToProperty(tickprop);
  }

  return true;
}
//========================================================================================
PLUGIN(SecondOrderCentrality)
