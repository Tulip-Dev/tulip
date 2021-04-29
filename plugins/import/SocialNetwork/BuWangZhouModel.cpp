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
#include <tulip/TlpTools.h>
#include <tulip/PluginProgress.h>
#include <tulip/Graph.h>

using namespace std;
using namespace tlp;

static const char *paramHelp[] = {
    // nodes
    "Number of nodes.",
    // nodes
    "Number of node types.",
    // m
    "Number of edges added for each new node."};

/**
 *
 * This plugin is an implementation of the model
 * described in
 * Shouliang Bu, Bing-Hong Wang, Tao Zhou.
 * Gaining scale-free and high clustering complex networks.
 * Physica A, 374, 864-868, 2007.
 *
 * The model indicates that a probability must be assign to every type of node
 * (such the sum is equal to 1), but this implementation assign the same value
 * as the article states that this give small world, scale-free networks.
 *
 */
class BuWangZhouModel : public ImportModule {
public:
  PLUGININFORMATION("Bu Wang Zhou Model", "sallaberry", "20/06/2011",
                    "Randomly generates a scale-free graph unsing the model described "
                    "in<br/>Shouliang Bu, Bing-Hong Wang, Tao Zhou.<br/><b>Gaining scale-free and "
                    "high clustering complex networks.</b><br/>Physica A, 374, 864--868, 2007.",
                    "1.0", "Social network")
  BuWangZhouModel(PluginContext *context) : ImportModule(context) {
    addInParameter<unsigned int>("nodes", paramHelp[0], "200");
    addInParameter<unsigned int>("types of nodes", paramHelp[1], "3");
    addInParameter<unsigned int>("m", paramHelp[2], "2");
  }

  bool importGraph() override {
    unsigned int nb_nodes = 200;
    unsigned int types_of_nodes = 3;
    unsigned int m = 2;

    if (dataSet != nullptr) {
      dataSet->get("nodes", nb_nodes);
      dataSet->get("types of nodes", types_of_nodes);
      dataSet->get("m", m);
    }

    // check arguments
    if (types_of_nodes > nb_nodes) {
      pluginProgress->setError(
          "The number of node types cannot be greater than the number of nodes");
      return false;
    }

    pluginProgress->showPreview(false);
    tlp::initRandomSequence();

    vector<vector<node>> nodes(types_of_nodes);
    graph->reserveNodes(nb_nodes);

    // In the paper, there are 3 types starting from a triangle without telling if the whole graph
    // or a cycle has to be taken into account.
    for (unsigned int i = 0; i < types_of_nodes; ++i) {
      nodes[i].push_back(graph->addNode());

      for (unsigned j = 0; j < i; ++j) {
        graph->addEdge(nodes[j][0], nodes[i][0]);
      }
    }

    graph->addEdge(nodes[types_of_nodes - 1][0], nodes[0][0]);

    unsigned int random_type, random_node;
    double pr, k_sum, pr_sum;

    for (unsigned int i = 0; i < nb_nodes - types_of_nodes; ++i) {
      nodes[i % types_of_nodes].push_back(graph->addNode());

      for (unsigned int j = 0; j < m; ++j) {

        // Random type
        do {
          random_type = tlp::randomUnsignedInteger(types_of_nodes - 1);
        } while (random_type == i % types_of_nodes);

        // Random node
        k_sum = 0;
        pr_sum = 0;

        for (random_node = 0; random_node < nodes[random_type].size(); random_node++)
          k_sum += graph->deg(nodes[random_type][random_node]);

        pr = tlp::randomDouble();
        random_node = 0;

        while (pr_sum < pr && nodes[random_type].size() > (random_node + 1)) {
          pr_sum += graph->deg(nodes[random_type][random_node]) / k_sum;
          random_node++;
        }

        graph->addEdge(nodes[i % types_of_nodes].back(), nodes[random_type][random_node]);
      }

      if (i % 100 == 0) {
        if (pluginProgress->progress(i, nb_nodes - types_of_nodes) != TLP_CONTINUE)
          return pluginProgress->state() != TLP_CANCEL;
      }
    }

    return true;
  }
};

PLUGIN(BuWangZhouModel)
