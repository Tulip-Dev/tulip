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

    // m
    "Number of activated nodes.",

    // proba
    "Probability to connect a node to a random other node<br/>instead of an activated node."};

/**
 *
 * This plugin is an implementation of the model
 * described in
 * Konstantin Klemm and Victor M. Eguiluz.
 * Growing Scale-Free Networks with Small World Behavior.
 * Physical Review E, 65, 057102,(2002).
 *
 */
struct KlemmEguiluzModel : public ImportModule {
  PLUGININFORMATION("Klemm Eguiluz Model", "Sallaberry & Pennarun", "21/02/2011 & 08/04/2014",
                    "Randomly generates a small world graph using the model described "
                    "in<br/>Konstantin Klemm and Victor M. Eguiluz.<br/><b>Growing Scale-Free "
                    "Networks with Small World Behavior.</b><br/>Physical Review E, 65, "
                    "057102,(2002).",
                    "1.0", "Social network")

  KlemmEguiluzModel(PluginContext *context) : ImportModule(context) {
    addInParameter<unsigned int>("nodes", paramHelp[0], "200");
    addInParameter<unsigned int>("m", paramHelp[1], "10");
    addInParameter<double>("mu", paramHelp[2], "0.5");
  }

  bool importGraph() {

    unsigned int n = 200;
    unsigned int m = 10;
    double mu = 0.5;

    if (dataSet != NULL) {
      dataSet->get("nodes", n);
      dataSet->get("m", m);
      dataSet->get("mu", mu);
    }

    // check arguments
    if (m > n) {
      pluginProgress->setError("The m parameter cannot be greater than the number of nodes.");
      return false;
    }

    if (mu > 1 || mu < 0) {
      pluginProgress->setError("The mu parameter must belong to [0, 1].");
      return false;
    }

    pluginProgress->showPreview(false);
    tlp::initRandomSequence();

    double a, pr, pr_sum, proba;

    vector<bool> activated(n, false);

    graph->addNodes(n);
    const vector<node> &nodes = graph->nodes();

    // fully connect and activate the m first nodes
    for (unsigned int i = 0; i < m; ++i) {
      activated[i] = true;

      for (unsigned int j = i + 1; j < m; ++j)
        graph->addEdge(nodes[i], nodes[j]);
    }

    for (unsigned i = m; i < n; ++i) {
      if (i % 100 == 0) {
        if (pluginProgress->progress(i, n) != TLP_CONTINUE)
          return pluginProgress->state() != TLP_CANCEL;
      }

      a = 0;

      for (unsigned int j = 0; j < i; ++j)
        a += 1 / double(graph->deg(nodes[j]));

      // the new node is connected to m nodes
      for (unsigned int j = 0; j < i; ++j) {
        if (activated[j]) {
          proba = tlp::randomDouble();

          if (proba < mu) { // rewire the edge to a random node chosen with preferential attachment
            pr = tlp::randomDouble();
            pr_sum = 0;
            unsigned int sn = 0;

            while (pr_sum < pr && sn <= i) {
              pr_sum += (1 / double(graph->deg(nodes[sn]))) * a;
              ++sn;
            }

            graph->addEdge(nodes[i], nodes[--sn]);
          } else { // keep the edge
            graph->addEdge(nodes[i], nodes[j]);
          }
        }
      }

      // the new node becomes active
      activated[i] = true;

      // deactivate one of the previously m activated nodes
      a = 0;

      for (unsigned int j = 0; j < i; ++j)
        if (activated[j])
          a += 1 / double(graph->deg(nodes[j]));

      pr = tlp::randomDouble();
      pr_sum = 0;
      unsigned int sn = 0;

      while (pr_sum < pr && sn < i) {
        if (activated[sn])
          pr_sum += a * (1 / double(graph->deg(nodes[sn])));

        ++sn;
      }

      activated[--sn] = false;
    }

    return true;
  }
};

PLUGIN(KlemmEguiluzModel)
