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

#include <tulip/TulipPluginHeaders.h>
#include <tulip/GraphParallelTools.h>

using namespace std;
using namespace tlp;

static const char *paramHelp[] = {
    // d
    "Enables to choose a damping factor in ]0,1[.",

    // directed
    "Indicates if the graph should be considered as directed or not."};

/*@{*/
/** \file
 *  \brief  An implementation of the PageRank metric
 *
 * This plugin is an implementation of the PageRank metric.
 * First designed by Larry Page and Sergey Brin, it is a link analysis
 * algorithm that assigns a measure to each node of an 'hyperlinked' graph.
 *
 * (see http://en.wikipedia.org/wiki/PageRank for more details)
 *
 *  <b>HISTORY</b>
 *
 *  - 2007 Version 1.0: Initial release
 *  by Mohamed Bouklit, LaBRI, University Bordeaux I, France
 *  - 2010 Version 1.1: rewrite to work properly with subgraph
 *  by David Auber, LaBRI, University Bordeaux I, France
 *  - 2011 Version 2.0: fix computation for undirected graph
 *  by François Queyroi, LaBRI, University Bordeaux I, France
 *
 *
*/
/*@}*/
struct PageRank : public DoubleAlgorithm {

  PLUGININFORMATION("Page Rank", "Mohamed Bouklit & David Auber", "16/12/10",
                    "Nodes measure used for links analysis.<br/>"
                    "First designed by Larry Page and Sergey Brin, it is a link analysis algorithm "
                    "that assigns a measure to each node of an 'hyperlinked' graph.",
                    "2.0", "Graph")

  PageRank(const PluginContext *context) : DoubleAlgorithm(context) {
    addInParameter<double>("d", paramHelp[0], "0.85");
    addInParameter<bool>("directed", paramHelp[1], "true");
  }

  bool run() override {
    double d = 0.85;
    bool directed = true;

    if (dataSet != nullptr) {
      dataSet->get("d", d);
      dataSet->get("directed", directed);
    }

    if (d <= 0 || d >= 1)
      return false;

    // Initialize the PageRank
    NodeStaticProperty<double> pr(graph);
    NodeStaticProperty<double> next_pr(graph);
    unsigned int nbNodes = graph->numberOfNodes();

    double oon = 1. / nbNodes;

    pr.setAll(oon);

    const double one_minus_d = (1 - d) / nbNodes;
    const unsigned int kMax = uint(15 * log(nbNodes));

    for (unsigned int k = 0; k < kMax + 1; ++k) {
      if (directed) {
        TLP_PARALLEL_MAP_NODES_AND_INDICES(graph, [&](const node n, unsigned int i) {
          double n_sum = 0;
          for (auto nin : graph->getInNodes(n))
            n_sum += pr.getNodeValue(nin) / graph->outdeg(nin);
          next_pr[i] = one_minus_d + d * n_sum;
        });
      } else {
        TLP_PARALLEL_MAP_NODES_AND_INDICES(graph, [&](const node n, unsigned int i) {
          double n_sum = 0;
          for (auto nin : graph->getInOutNodes(n))
            n_sum += pr.getNodeValue(nin) / graph->deg(nin);
          next_pr[i] = one_minus_d + d * n_sum;
        });
      }

      // swap pr and next_pr
      pr.swap(next_pr);
    }

    // store the pr values
    pr.copyToProperty(result);

    return true;
  }
};

PLUGIN(PageRank)
