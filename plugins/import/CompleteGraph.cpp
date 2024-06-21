/**
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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
#include <tulip/TulipPluginHeaders.h>

using namespace std;
using namespace tlp;

static const char *paramHelp[] = {
    // nodes
    "Number of nodes in the final graph.",

    // directed
    "If false, the generated graph is undirected. If true, two edges are created between each pair "
    "of nodes."};

class CompleteGraph : public ImportModule {
public:
  PLUGININFORMATION("Complete General Graph", "Auber", "16/12/2002",
                    "Imports a new complete graph.", "1.2", "Graph")
  CompleteGraph(tlp::PluginContext *context) : ImportModule(context) {
    addInParameter<unsigned int>("nodes", paramHelp[0], "5");
    addInParameter<bool>("directed", paramHelp[1], "false");
  }

  bool importGraph() override {
    unsigned int nbNodes = 5;
    bool directed = false;

    if (dataSet != nullptr) {
      dataSet->get("nodes", nbNodes);
      dataSet->get("directed", directed);
    }

    if (nbNodes == 0) {
      if (pluginProgress)
        pluginProgress->setError("Error: \"nodes\" parameter must be greater than 0");

      return false;
    }

    if (pluginProgress)
      pluginProgress->showPreview(false);

    vector<node> nodes;
    graph->addNodes(nbNodes, nodes);

    if (!directed)
      graph->reserveEdges(nbNodes - 1);
    else
      graph->reserveEdges(2 * (nbNodes - 1));

    for (size_t i = 0; i < nbNodes - 1; ++i)
      for (size_t j = i + 1; j < nbNodes; ++j) {
        graph->addEdge(nodes[i], nodes[j]);

        if (directed)
          graph->addEdge(nodes[j], nodes[i]);
      }

    return true;
  }
};

PLUGIN(CompleteGraph)
