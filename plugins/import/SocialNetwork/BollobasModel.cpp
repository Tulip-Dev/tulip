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
    "This parameter defines the amount of nodes used to build the scale-free graph.",

    // d
    "Minimum degree."};

/**
 *
 * This plugin is an implementation of the model
 * described in
 * B. Bollobas, O.M Riordan, J. Spencer and G. Tusnady.
 * The Degree Sequence of a Scale-Free Random Graph Process.
 * RSA: Random Structures & Algorithms, 18, 279 (2001)
 *
 * first implemented by :
 * Vladimir Batagelj and Ulrik Brandes. Efficient Generation of Large Random Networks, 2005.
 *
 */
struct BollobasModel : public ImportModule {
  PLUGININFORMATION("Bollobas et al. Model", "Arnaud Sallaberry", "21/02/2011",
                    "Randomly generates a scale-free graph using the model described in<br/>B. "
                    "Bollobas, O.M Riordan, J. Spencer and G. Tusnady.<br/><b>The Degree Sequence "
                    "of a Scale-Free Random Graph Process.</b><br/>RSA: Random Structures & "
                    "Algorithms, 18, 279 (2001)",
                    "1.1", "Social network")

  BollobasModel(PluginContext *context) : ImportModule(context) {
    addInParameter<unsigned int>("nodes", paramHelp[0], "2000");
    addInParameter<unsigned int>("minimum degree", paramHelp[1], "4");
  }

  bool importGraph() {
    unsigned int n = 2000;
    unsigned int d = 4;

    if (dataSet != NULL) {
      dataSet->get("nodes", n);
      dataSet->get("minimum degree", d);
    }

    // check arguments
    if (d > n) {
      pluginProgress->setError("The minimum degree cannot be greater than the number of nodes.");
      return false;
    }

    pluginProgress->showPreview(false);
    tlp::initRandomSequence();

    vector<unsigned int> M(2 * n * d);
    graph->addNodes(n);

    for (unsigned int v = 0; v < n; ++v) {
      for (unsigned int i = 0; i < d; ++i) {
        M[2 * (v * d + i)] = v;
        int r = tlp::randomInteger(2 * (v * d + i) + 1);
        M[2 * (v * d + i) + 1] = M[r];
      }

      if (v % 100 == 0) {
        if (pluginProgress->progress(v, n * (d + 1)) != TLP_CONTINUE)
          return pluginProgress->state() != TLP_CANCEL;
      }
    }

    graph->reserveEdges(n * d);

    const vector<node> &nodes = graph->nodes();

    for (unsigned int i = 0; i < (n * d); ++i) {
      graph->addEdge(nodes[M[2 * i]], nodes[M[2 * i + 1]]);

      if (i % 100 == 0) {
        if (pluginProgress->progress(i, n * (d + 1)) != TLP_CONTINUE)
          return pluginProgress->state() != TLP_CANCEL;
      }
    }

    return true;
  }
};

PLUGIN(BollobasModel)
