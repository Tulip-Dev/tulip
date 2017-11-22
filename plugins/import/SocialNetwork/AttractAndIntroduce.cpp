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

#include <tulip/ImportModule.h>
#include <tulip/TlpTools.h>
#include <tulip/PluginProgress.h>
#include <tulip/Graph.h>
#include <tulip/StaticProperty.h>

using namespace std;
using namespace tlp;

static const char *paramHelp[] = {
    // nodes
    "This parameter defines the amount of nodes used to build the graph.",

    // edges
    "This parameter defines the amount of edges used to build the graph.",

    // alpha
    "This parameter defines the alpha parameter between [0,1]. This one is a percentage and "
    "describes the distribution of attractiveness; the model suggests about 1 - alpha of the "
    "individuals have very low attractiveness whereas the remaining alpha are approximately evenly "
    "distributed between low, medium, and high attractiveness",

    // beta
    "This parameter defines the beta parameter between [0,1]. This parameter indicates the "
    "probability a person will have the desire to introduce someone."};

//=================================================================================

/**
 * This plugin is an implementation of the "Attract and Introduce Model"
 * described in
 * J. H. Fowlera, C. T. Dawesa, N. A. Christakisb.
 * Model of genetic variation in human social networks.
 * PNAS 106 (6): 1720-1724, 2009.
 *
 */
class AttractAndIntroduce : public ImportModule {
public:
  PLUGININFORMATION("Attract And Introduce Model", "Arnaud Sallabery & Patrick Mary", "25/03/2014",
                    "Randomly generates a graph using the Attract and Introduce Model described "
                    "in<br/>J. H. Fowlera, C. T. Dawesa, N. A. Christakisb.<br/><b>Model of "
                    "genetic variation in human social networks.</b><br/>PNAS 106 (6): 1720-1724, "
                    "2009.",
                    "1.0", "Social network")

  AttractAndIntroduce(tlp::PluginContext *context) : ImportModule(context) {
    addInParameter<unsigned int>("nodes", paramHelp[0], "750");
    addInParameter<unsigned int>("edges", paramHelp[1], "3150");
    addInParameter<double>("alpha", paramHelp[2], "0.9");
    addInParameter<double>("beta", paramHelp[3], "0.3");
  }

  bool importGraph() {
    unsigned int n = 750;
    unsigned int e = 3150;

    double alpha = 0.9;
    double beta = 0.3;

    if (dataSet != nullptr) {
      dataSet->get("nodes", n);
      dataSet->get("edges", e);
      dataSet->get("alpha", alpha);
      dataSet->get("beta", beta);
    }

    // check arguments
    if (alpha < 0 || alpha > 1) {
      pluginProgress->setError("alpha is not a percentage,\nit is not between [0, 1]");
      return false;
    } else if (beta < 0 || beta > 1) {
      pluginProgress->setError("beta is not a probability,\nit is is not between [0, 1]");
      return false;
    }

    pluginProgress->showPreview(false);
    tlp::initRandomSequence();

    unsigned int iterations = e + n;

    graph->addNodes(n);
    graph->reserveEdges(e);

    NodeStaticProperty<double> pAttractProperty(graph);
    NodeStaticProperty<double> pIntroduceProperty(graph);

    unsigned int i, j;
    double pIAttract, pIIntroduce;

    for (i = 0; i < n; ++i) {
      if ((1 - alpha) > randomDouble(1.0))
        pIAttract = 0;
      else
        pIAttract = randomDouble(1.0);

      if (beta > randomDouble(1.0))
        pIIntroduce = 1;
      else
        pIIntroduce = 0;

      pAttractProperty[i] = pIAttract;
      pIntroduceProperty[i] = pIIntroduce;

      if (i % 1000 == 0) {
        if (pluginProgress->progress(i, iterations) != TLP_CONTINUE)
          return pluginProgress->state() != TLP_CANCEL;
      }
    }

    unsigned int tmpE = 0;
    const vector<node> &nodes = graph->nodes();

    while (tmpE < e) {
      i = randomInteger(n - 1);

      do {
        j = randomInteger(n - 1);
      } while (i == j);

      node nj = nodes[j];

      if (pAttractProperty[j] > randomDouble(1.0)) {
        node ni = nodes[i];

        if (pIntroduceProperty[i] > randomDouble(1.0)) {
          node fd;
          forEach(fd, graph->getInOutNodes(ni)) {
            if (fd == nj || graph->hasEdge(fd, nj, false))
              continue;

            if (pAttractProperty[j] > randomDouble(1.0)) {
              graph->addEdge(fd, nj);
              ++tmpE;
              continue;
            }

            if (pAttractProperty.getNodeValue(fd) > randomDouble(1.0)) {
              graph->addEdge(nj, fd);
              ++tmpE;
            }
          }
        }

        if (!graph->hasEdge(ni, nj, false)) {
          graph->addEdge(ni, nj);
          ++tmpE;
        }

        if (tmpE % 1000 == 0) {
          if (pluginProgress->progress(tmpE, iterations) != TLP_CONTINUE)
            return pluginProgress->state() != TLP_CANCEL;
        }
      }
    }

    return true;
  }
};

PLUGIN(AttractAndIntroduce)
