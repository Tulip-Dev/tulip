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
#include <ctime>
#include <tulip/TulipPluginHeaders.h>

using namespace std;
using namespace tlp;

struct edgeS {
  unsigned source, target;
};

namespace std {
template <>
struct less<edgeS> {
  bool operator()(const edgeS &c, const edgeS &d) const {
    return (c.source < d.source) || ((c.source == d.source) && (c.target < d.target));
  }
};
}

static const char *paramHelp[] = {
    // nodes
    "Number of nodes in the final graph.",

    // edges
    "Number of edges in the final graph."};

/** \addtogroup import */

/// Random Graph - Import of a random graph
/** This plugin enables to create a random graph
 *
 *  User can specify the number of nodes and the number of edges of the graph.
 */
class RandomGraph : public ImportModule {
public:
  PLUGININFORMATION("Random General Graph", "Auber", "16/06/2002",
                    "Imports a new randomly generated graph.", "1.0", "Graph")
  RandomGraph(tlp::PluginContext *context) : ImportModule(context) {
    addInParameter<unsigned int>("nodes", paramHelp[0], "500");
    addInParameter<unsigned int>("edges", paramHelp[1], "1000");
  }
  ~RandomGraph() {}

  bool importGraph() {
    // initialize a random sequence according the given seed
    tlp::initRandomSequence();

    unsigned int nbNodes = 5;
    unsigned int nbEdges = 9;

    if (dataSet != NULL) {
      dataSet->get("nodes", nbNodes);
      dataSet->get("edges", nbEdges);
    }

    if (nbNodes == 0) {
      if (pluginProgress)
        pluginProgress->setError(string("Error: the number of nodes cannot be null"));

      return false;
    }

    unsigned int nbIteration = nbNodes * nbEdges;
    unsigned int ite = 0;
    set<edgeS> myGraph;

    if (pluginProgress)
      pluginProgress->showPreview(false);

    while (ite < nbIteration) {
      if (ite % nbNodes == 1 && (pluginProgress->progress(ite, nbIteration) != TLP_CONTINUE))
        return pluginProgress->state() != TLP_CANCEL;

      edgeS tmp;
      tmp.source = randomUnsignedInteger(nbNodes - 1);
      tmp.target = randomUnsignedInteger(nbNodes - 1);

      if ((myGraph.erase(tmp) == 0) && (myGraph.size() < nbEdges))
        myGraph.insert(tmp);

      ++ite;
    }

    graph->addNodes(nbNodes);

    graph->reserveEdges(myGraph.size());

    const vector<node> &nodes = graph->nodes();

    for (set<edgeS>::const_iterator it = myGraph.begin(); it != myGraph.end(); ++it) {
      graph->addEdge(nodes[it->source], nodes[it->target]);
    }

    return true;
  }
};

PLUGIN(RandomGraph)
