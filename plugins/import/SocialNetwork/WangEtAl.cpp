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
    // n
    "Number of nodes."};

/**
 *
 * This plugin is an implementation of the model
 * described in
 * L.Wang, F. Du, H. P. Dai, and Y. X. Sun.
 * Random pseudofractal scale-free networks with small-world effect.
 * The European Physical Journal B - Condensed Matter and Complex Systems, 53, 361-366, (2006).
 *
 */
struct WangEtAl : public ImportModule {
  PLUGININFORMATION("Wang et al. Model", "Arnaud Sallaberry", "21/02/2011",
                    "Randomly generates a small world graph using the model described "
                    "in<br/>L.Wang, F. Du, H. P. Dai, and Y. X. Sun.<br/><b>Random pseudofractal "
                    "scale-free networks with small-world effect.</b><br/>The European Physical "
                    "Journal B - Condensed Matter and Complex Systems, 53, 361-366, (2006).",
                    "1.0", "Social network")

  WangEtAl(PluginContext *context) : ImportModule(context) {
    addInParameter<unsigned int>("nodes", paramHelp[0], "300");
  }

  bool importGraph() {

    unsigned int n = 300;

    if (dataSet != NULL) {
      dataSet->get("nodes", n);
    }

    pluginProgress->showPreview(false);
    tlp::initRandomSequence();

    graph->addNodes(n);
    const vector<node> &nodes = graph->nodes();

    graph->reserveEdges(2 * n - 3);

    vector<edge> e(2 * n - 3);
    e[0] = graph->addEdge(nodes[0], nodes[1]);
    unsigned int nbe = 1;

    for (unsigned i = 2; i < n; ++i) {
      if (i % 100 == 0) {
        if (pluginProgress->progress(i, n) != TLP_CONTINUE)
          return pluginProgress->state() != TLP_CANCEL;
      }

      int id = tlp::randomInteger(nbe - 1);
      const pair<node, node> ends = graph->ends(e[id]);
      e[nbe] = graph->addEdge(ends.first, nodes[i]);
      e[nbe + 1] = graph->addEdge(ends.second, nodes[i]);
      nbe += 2;
    }

    return true;
  }
};

PLUGIN(WangEtAl)
