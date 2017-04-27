/**
 *
 * This file is part of Tulip (www.tulip-software.org)
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

static const char * paramHelp[] = {
  // n
  "Number of nodes.",

  // m
  "Number of edges added at each time step.",

  // proba
  "Probability of adding a triangle after adding a random edge."
};

/**
 *
 * This plugin is an implementation of the model
 * described in
 * Petter Holme and Beom Jun Kim.
 * Growing scale-free networks with tunable clustering.
 * Physical Review E, 65, 026107, (2002).
 *
 */
struct HolmeKim:public ImportModule {
  PLUGININFORMATION("Holme and Kim Model","Sallaberry & Pennarun","21/02/2011 & 08/04/2014","Randomly generates a scale-free graph using the model described in<br/>Petter Holme and Beom Jun Kim.<br/><b>Growing scale-free networks with tunable clustering.</b><br/>Physical Review E, 65, 026107, (2002).","1.0","Social network")

  HolmeKim(PluginContext* context):ImportModule(context) {
    addInParameter<unsigned int>("nodes",paramHelp[0],"300");
    addInParameter<unsigned int>("m",paramHelp[1],"5");
    addInParameter<double>("p",paramHelp[2],"0.5");
  }

  bool importGraph() {
    unsigned int n = 300;
    unsigned int m = 5;
    double mu = 0.5;

    if (dataSet!=NULL) {
      dataSet->get("nodes", n);
      dataSet->get("m", m);
      dataSet->get("p", mu);
    }

    // check arguments
    if (m > n) {
      pluginProgress->setError("The m parameter cannot be greater than the number of nodes.");
      return false;
    }

    if (mu > 1 || mu < 0) {
      pluginProgress->setError("The p parameter must belong to [0, 1].");
      return false;
    }

    pluginProgress->showPreview(false);
    tlp::initRandomSequence();

    /*
     * Initial ring construction
     */
    unsigned int m0 = 3;
    vector<node> sg(n);
    graph->addNodes(n, sg);

    for (unsigned int i=1; i<m0 ; ++i) {
      graph->addEdge(sg[i-1],sg[i]);
    }

    graph->addEdge(sg[m0-1],sg[0]);

    /*
     * Main loop
     */
    for (unsigned int i=m0; i<n ; ++i) {
      double k_sum = 0; // degree of present nodes

      for(unsigned int j=0; j<i ; ++j)
        k_sum += (double)graph->deg(sg[j]);

      double proba = tlp::randomDouble();

      for (unsigned int j=0; j<m; ++j) {
        //Preferential attachment
        double pr = tlp::randomDouble();
        double pr_sum = 0;
        double firstNeighbour = 0;

        while (pr_sum < pr && firstNeighbour <= i) {
          pr_sum += (double)graph->deg(sg[firstNeighbour])/(k_sum);
          ++firstNeighbour;
        }

        graph->addEdge(sg[i],sg[--firstNeighbour]);

        if (proba < mu) { // Triad formation
          // collect all neighbours of firstNeighbour
          // which are not already connected to sg[i]
          vector<node> freeNeighbours;
          Iterator<node>* it = graph->getInOutNodes(sg[firstNeighbour]);

          while (it->hasNext()) {
            node neighbour = it->next();

            if (!graph->hasEdge(sg[i], neighbour))
              freeNeighbours.push_back(neighbour);
          }

          delete it;

          if (!freeNeighbours.empty()) {
            // randomly choose one of the free neighbours to connect with
            unsigned int randomNeighbour = tlp::randomUnsignedInteger(freeNeighbours.size()-1);
            graph->addEdge(sg[i], freeNeighbours[randomNeighbour]);
            continue;
          }
        }

        // Preferential attachment
        pr = tlp::randomDouble();
        pr_sum = 0;
        unsigned int rn = 0;

        while (pr_sum<pr && rn<(i-1)) {
          pr_sum += (double)graph->deg(sg[rn])/(k_sum);
          ++rn;
        }

        graph->addEdge(sg[i],sg[--rn]);
      }
    }

    return true;
  }
};

PLUGIN(HolmeKim)
