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

const char *paramHelp[] = {
    // n
    "Number of nodes.",

    // m
    "Number of nodes added at each time step.",

    // m
    "Delta coefficient must belong to [0, 1]"};

/**
 *
 * This plugin is an implementation of the model
 * described in
 * Peihua Fu and Kun Liao.
 * An evolving scale-free network with large clustering coefficient.
 * In ICARCV, pp. 1-4. IEEE, (2006).
 *
 */
struct FuLiao : public ImportModule {
  PLUGININFORMATION("Fu and Liao Model", "Arnaud Sallaberry", "21/02/2011",
                    "Randomly generates a scale-free graph using<br/>Peihua Fu and Kun "
                    "Liao.<br/><b>An evolving scale-free network with large clustering "
                    "coefficient.</b><br/>In ICARCV, pp. 1-4. IEEE, (2006).",
                    "1.0", "Social network")

  FuLiao(PluginContext *context) : ImportModule(context) {
    addInParameter<unsigned int>("nodes", paramHelp[0], "300");
    addInParameter<unsigned int>("m", paramHelp[1], "5");
    addInParameter<double>("delta", paramHelp[2], "0.5");
  }

  bool importGraph() override {
    unsigned int n = 300;
    unsigned int m = 5;
    double d = 0.5;

    if (dataSet != nullptr) {
      dataSet->get("nodes", n);
      dataSet->get("m", m);
      dataSet->get("delta", d);
    }

    // check arguments
    if (d < 0 || d > 1) {
      pluginProgress->setError("delta is not a probability,\nit is not between [0, 1].");
      return false;
    }

    tlp::initRandomSequence();
    unsigned int i, j;

    /*
     * Initial ring construction
     */
    unsigned int m0 = 3;
    graph->addNodes(n);
    const vector<node> &nodes = graph->nodes();

    for (i = 1; i < m0; ++i) {
      graph->addEdge(nodes[i - 1], nodes[i]);
    }

    graph->addEdge(nodes[m0 - 1], nodes[0]);

    /*
     * Main loop
     */
    for (i = m0; i < n; ++i) {
      double k_sum = 0;

      for (j = 0; j < i; ++j) {
        k_sum += graph->deg(nodes[j]);
      }

      // add first edge
      double pr_sum = 0;
      unsigned int rn = 0;
      double pr = tlp::randomDouble();

      while (pr_sum < pr && rn < (i - 1)) {
        if (!graph->hasEdge(nodes[i], nodes[rn])) {
          pr_sum += double(graph->deg(nodes[rn])) / k_sum;
        }

        ++rn;
      }

      graph->addEdge(nodes[i], nodes[rn]);

      // add other edges
      for (j = 1; j < m; ++j) {
        rn = 0;
        double h_sum = 0;

        while (rn < (i - 1)) {
          if (!graph->hasEdge(nodes[i], nodes[rn])) {
            node v;
            forEach(v, graph->getInOutNodes(nodes[rn])) {
              if (graph->hasEdge(nodes[i], v)) {
                h_sum++;
              }
            }
          }

          ++rn;
        }

        pr_sum = 0;
        rn = 0;
        pr = tlp::randomDouble();

        while (pr_sum < pr && rn < (i - 1)) {
          if (!graph->hasEdge(nodes[i], nodes[rn])) {
            double h = 0;
            node v;
            forEach(v, graph->getInOutNodes(nodes[rn])) {
              if (graph->hasEdge(nodes[i], v)) {
                h++;
              }
            }
            pr_sum = pr_sum + (1.0 - d) * graph->deg(nodes[rn]) / (k_sum + j) + d * (h / h_sum);
          }

          ++rn;
        }

        --rn;
        graph->addEdge(nodes[i], nodes[rn]);
      }
    }

    return true;
  }
};

PLUGIN(FuLiao)
