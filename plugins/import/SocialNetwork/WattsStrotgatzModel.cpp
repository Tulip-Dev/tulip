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
#include <tulip/PluginProgress.h>
#include <tulip/Graph.h>
#include <tulip/TlpTools.h>
#include <string>

using namespace std;
using namespace tlp;

static const char *paramHelp[] = {
    // nodes
    "This parameter defines the amount of nodes used to build the scale-free graph.",

    // k
    "Number of edges added to each node in the inital ring lattice.",

    // p
    "Probability in [0,1] to rewire an edge.",

    // original model
    "Use the original model: k describes the degree of each vertex (k > 1 and even).",
};

/**
 *
 * This plugin is an implementation of the model
 * described in
 * D. J. Watts and S. H. Strogatz.
 * Collective dynamics of "small-world" networks.
 * Nature 393, 440 (1998).
 *
 */
struct WattsStrotgatzModel : public ImportModule {
  PLUGININFORMATION("Watts Strotgatz Model", "Arnaud Sallaberry", "21/02/2011",
                    "Randomly generates a small world graph using the model described in<br/>D. J. "
                    "Watts and S. H. Strogatz.<br/><b>Collective dynamics of small-world "
                    "networks.</b><br/>Nature 393, 440 (1998).",
                    "1.0", "Social network")

  WattsStrotgatzModel(PluginContext *context) : ImportModule(context) {
    addInParameter<unsigned int>("nodes", paramHelp[0], "200");
    addInParameter<unsigned int>("k", paramHelp[1], "3");
    addInParameter<double>("p", paramHelp[2], "0.02");
    addInParameter<bool>("original model", paramHelp[3], "false");
  }

  bool importGraph() {
    unsigned int nbNodes = 200;
    unsigned int k = 3;
    double p = 0.02;
    bool original_model = false;

    if (dataSet != NULL) {
      dataSet->get("nodes", nbNodes);
      dataSet->get("k", k);
      dataSet->get("p", p);
      dataSet->get("original model", original_model);
    }

    // check arguments
    if (p < 0 || p > 1) {
      pluginProgress->setError("p is not a probability,\nit does not belong to [0, 1]");
      return false;
    }

    if (original_model) {
      if (k % 2 == 1) {
        stringstream sstr("k must be pair when used in the original model; rounding k down to ");
        sstr << k - 1 << '.';
        pluginProgress->setComment(sstr.str());
        k--;
      }

      if (k > 0) {
        k = (k - 2) / 2;
      }
    }

    pluginProgress->showPreview(false);
    tlp::initRandomSequence();

    graph->addNodes(nbNodes);
    const vector<node> &nodes = graph->nodes();

    if (original_model) {
      graph->reserveEdges(nbNodes * k / 2);
    } else {
      graph->reserveEdges(nbNodes * (k + 1));
    }

    for (unsigned int i = 1; i < nbNodes; ++i) {
      graph->addEdge(nodes[i - 1], nodes[i]);
    }

    graph->addEdge(nodes[nbNodes - 1], nodes[0]);

    for (unsigned int i = 0; i < nbNodes; ++i) {
      for (unsigned int j = 0; j < k; ++j) {
        int d = i - j - 2;

        if (d < 0) {
          graph->addEdge(nodes[nbNodes + d], nodes[i]);
        } else {
          graph->addEdge(nodes[d], nodes[i]);
        }
      }
    }

    edge e;
    node n1, n2;

    if (original_model) {
      forEach(e, graph->getEdges()) {
        if (tlp::randomDouble() < p) {
          n1 = graph->source(e);

          do {
            n2 = nodes[tlp::randomInteger(nbNodes - 1)];
          } while (graph->hasEdge(n1, n2, false));

          // only reroute target; ensure to keep the graph connected
          graph->setTarget(e, n2);
        }
      }
    } else {
      forEach(e, graph->getEdges()) {
        if (tlp::randomDouble() < p) {
          do {
            n1 = nodes[tlp::randomInteger(nbNodes - 1)];
            n2 = nodes[tlp::randomInteger(nbNodes - 1)];
          } while (graph->hasEdge(n1, n2, false));

          graph->setEnds(e, n1, n2);
        }
      }
    }

    return true;
  }
};

PLUGIN(WattsStrotgatzModel)
