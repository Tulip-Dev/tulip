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

#define MAX_DENSITY_FOR_LINEAR 0.85

using namespace std;
using namespace tlp;

struct edgeS {
  edgeS() {}
  edgeS(unsigned s, unsigned t) : source(s), target(t) {}
  unsigned source, target;
};

namespace std {
struct edgeS_comp {
  edgeS_comp(bool d) : directed(d) {}
  bool operator()(const edgeS &c, const edgeS &d) const {
    if (!directed) {
      int cs, ct, ds, dt;

      if (c.source <= c.target) {
        cs = c.source;
        ct = c.target;
      } else {
        ct = c.source;
        cs = c.target;
      }

      if (d.source <= d.target) {
        ds = d.source;
        dt = d.target;
      } else {
        dt = d.source;
        ds = d.target;
      }
      return (cs < ds) || ((cs == ds) && (ct < dt));
    } else {
      return (c.source < d.source) || ((c.source == d.source) && (c.target < d.target));
    }
  }
  bool directed;
};
} // namespace std

static const char *paramHelp[] = {
    // nodes
    "Number of nodes in the graph.",

    // edges
    "Number of edges in the graph.",

    // directed
    "If true, the graph may contain edges a->b and b->a.",

    // multiple edges
    "If true, the graph may contain multiple edges between distinct nodes.",

};

/** \addtogroup import */

/// Random Graph - Import of a random graph
/** This plugin enables to create a random graph
 *
 *  User can specify the number of nodes and the number of edges of the graph.
 *
 * <b>HISTORY</b>
 * - 16/06/2002 Version 1.0: Initial Release (David Auber)
 * - 08/04/2019 Version 2.0: Add directed parameter + fix for high-density (François Queyroi)
 */
class RandomGraph : public ImportModule {
public:
  PLUGININFORMATION("Random General Graph", "Auber", "16/06/2002",
                    "Imports a new randomly generated graph.", "2.1", "Graph")
  RandomGraph(tlp::PluginContext *context) : ImportModule(context) {
    addInParameter<unsigned int>("nodes", paramHelp[0], "500");
    addInParameter<unsigned int>("edges", paramHelp[1], "1000");
    addInParameter<bool>("directed", paramHelp[2], "false", false);
    addInParameter<bool>("multiple edges", paramHelp[3], "false", false);
  }
  ~RandomGraph() override {}

  bool importGraph() override {
    // initialize a random sequence according the given seed
    tlp::initRandomSequence();

    unsigned int nbNodes = 500;
    unsigned int nbEdges = 1000;
    bool directed = false, multipleEdges = false;

    if (dataSet != nullptr) {
      dataSet->get("nodes", nbNodes);
      dataSet->get("edges", nbEdges);
      dataSet->get("directed", directed);
      dataSet->get("multiple edges", multipleEdges);
    }

    if (nbNodes == 0) {
      pluginProgress->setError("Error: the number of nodes cannot be null");

      return false;
    }

    double density_g = (2. * nbEdges) / double(nbNodes * (nbNodes - 1));
    if (!multipleEdges && directed)
      density_g /= 2.;

    if (!multipleEdges && (density_g > 1)) {
      string msg("Error: a simple ");
      msg += std::string(directed ? "directed" : "undirected") + " graph with ";
      msg += std::to_string(nbNodes);
      msg += " nodes cannot have more than ";
      msg += std::to_string(nbNodes * (nbNodes - 1) / (directed ? 1 : 2)) + " edges.";
      pluginProgress->setError(msg);
      return false;
    }

    unsigned int nb_distinct_pairs_needed = nbEdges;
    if (!multipleEdges && (density_g > MAX_DENSITY_FOR_LINEAR)) {
      if (directed)
        nb_distinct_pairs_needed = nbNodes * (nbNodes - 1) - nbEdges;
      else
        nb_distinct_pairs_needed = nbNodes * (nbNodes - 1) / 2 - nbEdges;
    }

    edgeS_comp comp_e(directed);
    set<edgeS, edgeS_comp> myGraph(comp_e);

    if (pluginProgress)
      pluginProgress->showPreview(false);

    while (myGraph.size() < nb_distinct_pairs_needed) {
      if (myGraph.size() % nbNodes == 1 &&
          (pluginProgress->progress(myGraph.size(), nb_distinct_pairs_needed) != TLP_CONTINUE))
        return pluginProgress->state() != TLP_CANCEL;

      edgeS tmp;
      tmp.source = randomUnsignedInteger(nbNodes - 1);
      tmp.target = randomUnsignedInteger(nbNodes - 1);

      while (tmp.source == tmp.target) {
        tmp.source = randomUnsignedInteger(nbNodes - 1);
        tmp.target = randomUnsignedInteger(nbNodes - 1);
      }

      if ((multipleEdges || (myGraph.find(tmp) == myGraph.end())) && (myGraph.size() < nbEdges))
        myGraph.insert(tmp);
    }

    vector<node> nodes;
    graph->addNodes(nbNodes, nodes);

    graph->reserveEdges(nbEdges);

    if (!multipleEdges && (density_g > MAX_DENSITY_FOR_LINEAR)) {
      for (unsigned i = 0; i < nodes.size(); ++i) {
        unsigned min_j = 0;
        if (!directed)
          min_j = i + 1;
        for (unsigned j = min_j; j < nodes.size(); ++j) {
          if (i != j && myGraph.find(edgeS(i, j)) == myGraph.end()) {
            graph->addEdge(nodes[i], nodes[j]);
          }
        }
      }
    } else {
      for (auto it = myGraph.begin(); it != myGraph.end(); ++it) {
        graph->addEdge(nodes[it->source], nodes[it->target]);
      }
    }

    return true;
  }
};

PLUGIN(RandomGraph)

/** \addtogroup import */

/// Random Simple Graph - Import of a random graph
/** This plugin enables to create a random simple (undirected) graph
 *
 *  User can specify the number of nodes and the number of edges of the graph.
 *
 * <b>HISTORY</b>
 * - 16/06/2002 Version 1.0: Initial Release (David Auber)
 * - 20/04/2019 Version 2.0: Call to more general plugin "Random General Graph"
 */
class RandomSimpleGraph : public RandomGraph {
public:
  PLUGININFORMATION("Random Simple Graph", "Auber", "16/06/2002",
                    "Imports a new randomly generated simple graph.", "1.1", "Graph")
  RandomSimpleGraph(tlp::PluginContext *context) : RandomGraph(context) {
    removeParameter("multiple edges");
  }
};

PLUGIN(RandomSimpleGraph)
