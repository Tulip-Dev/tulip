/**
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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
#include <tulip/ImportModule.h>
#include <tulip/Graph.h>
#include <tulip/TlpTools.h>

using namespace std;
using namespace tlp;

namespace {
const char *paramHelp[] = {
    // nodes
    "Number of nodes in the final graph.",

    // probability
    "Probability of having an edge between each pair of vertices in the graph.",

    // self loop
    "Generate self loops (an edge with source and target on the same node) with the same "
    "probability"};
}

/** \addtogroup import */

/** Random Graph - Import of a random graph based on the Erdős-Rényi Model
 *
 *  User can specify the number of nodes and the probability of having an edge between two nodes.
 */
class ERRandomGraph : public ImportModule {
public:
  PLUGININFORMATION("Erdős-Rényi Random Graph", "Bruno Pinaud", "08/09/2014",
                    "Import a randomly generated graph following the Erdős-Rényi model. Given a "
                    "positive integer n and a probability value in [0,1], define the graph G(n,p) "
                    "to be the undirected graph on n vertices whose edges are chosen as follows: "
                    "For all pairs of vertices v,w there is an edge (v,w) with probability p.",
                    "1.0", "Graph")
  ERRandomGraph(tlp::PluginContext *context) : ImportModule(context) {
    addInParameter<unsigned int>("nodes", paramHelp[0], "50");
    addInParameter<double>("probability", paramHelp[1], "0.5");
    addInParameter<bool>("self loop", paramHelp[2], "false");
  }

  bool importGraph() {
    // initialize a random sequence according to the given seed
    tlp::initRandomSequence();

    unsigned int nbNodes = 50;
    double proba = 0.5;
    bool self_loop = false;

    if (dataSet != nullptr) {
      dataSet->get("nodes", nbNodes);
      dataSet->get("probability", proba);
      dataSet->get("self loop", self_loop);
    }

    if (nbNodes == 0) {
      if (pluginProgress)
        pluginProgress->setError(string("Error: the number of nodes cannot be null."));

      return false;
    }

    if ((proba < 0) || (proba > 1)) {
      if (pluginProgress)
        pluginProgress->setError(string("Error: the probability must be between ]0, 1[."));

      return false;
    }

    // add nodes
    graph->addNodes(nbNodes);
    const vector<node> &nodes = graph->nodes();

    unsigned i = 0;

    while (i != nbNodes) {
      ++i;
      node u = nodes[nbNodes - i];

      if (pluginProgress && pluginProgress->progress(i, nbNodes) != TLP_CONTINUE)
        return pluginProgress->state() != TLP_CANCEL;

      for (unsigned int j = 0; j < nbNodes - i + 1; ++j) {
        node v = nodes[j];

        if ((u == v) && (!self_loop))
          continue;

        if (tlp::randomDouble() < proba) {
          graph->addEdge(u, v);
        }
      }
    }

    return true;
  }
};

PLUGIN(ERRandomGraph)
