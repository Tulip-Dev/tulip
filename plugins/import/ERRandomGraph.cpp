/**
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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

    // self loops
    "Generate self loops (an edge with source and target on the same node) with the same probability",

    // directed
    "Generate a directed graph (edges u->v and v->u have the same probability)"};
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
                    "1.2", "Graph")
  ERRandomGraph(tlp::PluginContext *context) : ImportModule(context) {
    addInParameter<unsigned int>("nodes", paramHelp[0], "50");
    addInParameter<double>("p", paramHelp[1], "0.01");
    addInParameter<bool>("self loops", paramHelp[2], "false");
    addInParameter<bool>("directed", paramHelp[3], "false");
  }

  bool importGraph() override {
    // initialize a random sequence according to the given seed
    tlp::initRandomSequence();

    unsigned int nbNodes = 50;
    double p = 0.5;
    bool self_loop = false;
    bool directed = false;

    if (dataSet != nullptr) {
      dataSet->get("nodes", nbNodes);
      dataSet->getDeprecated("p", "probability", p);
      dataSet->get("self loops", self_loop);
      dataSet->get("directed", directed);
    }

    if (nbNodes == 0) {
      if (pluginProgress)
        pluginProgress->setError(string("Error: \"nodes\" cannot be null."));

      return false;
    }

    if ((p < 0) || (p > 1)) {
      if (pluginProgress)
        pluginProgress->setError(string("Error: \"p\" must be between [0, 1]."));

      return false;
    }

    // add nodes
    vector<node> nodes;
    graph->addNodes(nbNodes, nodes);

    unsigned i = 0;

    while (i != nbNodes) {
      ++i;
      node u = nodes[nbNodes - i];

      if (pluginProgress && pluginProgress->progress(i, nbNodes) != TLP_CONTINUE)
        return pluginProgress->state() != TLP_CANCEL;

      unsigned int max_index_j = nbNodes - i + 1;
      if (directed)
        max_index_j = nbNodes;

      for (unsigned int j = 0; j < max_index_j; ++j) {
        node v = nodes[j];

        if ((u == v) && (!self_loop))
          continue;

        if (tlp::randomDouble() < p) {
          graph->addEdge(u, v);
        }
      }
    }

    return true;
  }
};

PLUGIN(ERRandomGraph)
