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

#include <cmath>
#include <tulip/ImportModule.h>
#include <tulip/TlpTools.h>
#include <tulip/PluginProgress.h>
#include <tulip/Graph.h>

using namespace std;
using namespace tlp;

static const char *paramHelp[] = {
    // nodes
    "This parameter defines the amount of nodes used to build the small-world graph."};

//=================================================================================

struct BottomNode {
  node n;
  unsigned int degree;
};

//=================================================================================

struct TopNode {
  unsigned int degree;
  vector<node> bottom_nodes;
};

//=================================================================================

/**
 *
 * This plugin is an implementation of the model
 * described in
 * J.-L. Guillaume and M. Latapy.
 * Bipartite graphs as models of complex networks.
 * In Workshop on Combinatorial and Algorithmic Aspects of Networking (CAAN),
 * LNCS, volume 1, 2004.
 *
 * An equivalent model is also presented in :
 * M. E. J. Newman, D. J. Watts, and S. H. Strogatz.
 * Random graph models of social networks. PNAS, 99(Suppl 1):2566-2572, 2002.
 *
 */
struct GuillaumeLatapyModel : public ImportModule {
  PLUGININFORMATION("Guillaume Latapy Model", "Arnaud Sallaberry", "20/06/2011",
                    "Randomly generates a small word graph using the model described in<br/>J.-L. "
                    "Guillaume and M. Latapy.<br/><b>Bipartite graphs as models of complex "
                    "networks.</b><br/>In Workshop on Combinatorial and Algorithmic Aspects of "
                    "Networking (CAAN), LNCS, volume 1, 2004.",
                    "1.0", "Social network")

  GuillaumeLatapyModel(PluginContext *context) : ImportModule(context) {
    addInParameter<unsigned int>("nodes", paramHelp[0], "200");
  }

  bool importGraph() override {

    unsigned int nbNodes = 200;

    if (dataSet != nullptr) {
      dataSet->get("nodes", nbNodes);
    }

    pluginProgress->showPreview(false);
    unsigned int iterations = nbNodes * 3;
    unsigned int i, j, l;

    vector<BottomNode> vec_bottom_nodes(nbNodes);
    vector<TopNode> vec_top_nodes(nbNodes);

    unsigned int nbNodesSmallWorld = uint(ceil(0.8 * nbNodes));
    unsigned int nbNodesScaleFree = nbNodes - nbNodesSmallWorld;
    unsigned int maxDegreeSmallWorldNodes = 2;
    unsigned int numberOfEdges = 0;

    tlp::initRandomSequence();

    graph->reserveNodes(nbNodes);

    for (i = 0; i < nbNodes; ++i) {
      if (i % 100 == 0) {
        if (pluginProgress->progress(i, iterations) != TLP_CONTINUE)
          return pluginProgress->state() != TLP_CANCEL;
      }

      if (i < nbNodesScaleFree)
        vec_bottom_nodes[i].degree =
            uint(ceil(((nbNodes / 2.0 - 10.0) / nbNodesScaleFree / 2) * (i + 1)));
      else
        vec_bottom_nodes[i].degree = maxDegreeSmallWorldNodes;

      numberOfEdges += vec_bottom_nodes[i].degree;
      vec_bottom_nodes[i].n = graph->addNode();
    }

    unsigned int degreeTop = numberOfEdges / nbNodes;
    unsigned int dixieme = uint(ceil((10.0 * numberOfEdges) / nbNodes)) % 10;

    for (i = 0; i < nbNodes - 1; ++i) {
      if (i % 10 >= dixieme)
        vec_top_nodes[i].degree = degreeTop;
      else
        vec_top_nodes[i].degree = degreeTop + 1;

      numberOfEdges -= vec_top_nodes[i].degree;
    }

    vec_top_nodes[nbNodes - 1].degree = numberOfEdges;

    for (i = 0; i < nbNodes; ++i) {
      if (i % 100 == 0) {
        if (pluginProgress->progress(i + nbNodes, iterations) != TLP_CONTINUE)
          return pluginProgress->state() != TLP_CANCEL;
      }

      for (j = 0; j < vec_top_nodes[i].degree; ++j) {
        int bottom_id = tlp::randomInteger(vec_bottom_nodes.size() - 1);

        if (isNotNodeInVector(vec_top_nodes[i].bottom_nodes, vec_bottom_nodes[bottom_id].n))
          vec_top_nodes[i].bottom_nodes.push_back(vec_bottom_nodes[bottom_id].n);

        vec_bottom_nodes[bottom_id].degree--;

        if (vec_bottom_nodes[bottom_id].degree == 0)
          vec_bottom_nodes.erase(vec_bottom_nodes.begin() + bottom_id);
      }
    }

    for (i = 0; i < nbNodes; ++i) {
      if (i % 100 == 0) {
        if (pluginProgress->progress(i + 2 * nbNodes, iterations) != TLP_CONTINUE)
          return pluginProgress->state() != TLP_CANCEL;
      }

      for (j = 0; j < vec_top_nodes[i].bottom_nodes.size(); ++j) {
        for (l = 0; l < j; ++l) {
          if (!graph->hasEdge(vec_top_nodes[i].bottom_nodes[j], vec_top_nodes[i].bottom_nodes[l])) {
            if (!graph->hasEdge(vec_top_nodes[i].bottom_nodes[l],
                                vec_top_nodes[i].bottom_nodes[j])) {
              graph->addEdge(vec_top_nodes[i].bottom_nodes[j], vec_top_nodes[i].bottom_nodes[l]);
            }
          }
        }
      }
    }

    return pluginProgress->state() != TLP_CANCEL;
  }

  bool isNotNodeInVector(vector<node> vec, node n) {
    return std::find(vec.begin(), vec.end(), n) == vec.end();
  }
};

PLUGIN(GuillaumeLatapyModel)
