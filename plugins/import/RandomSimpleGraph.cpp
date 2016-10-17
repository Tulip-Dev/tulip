/**
 *
 * This file is part of Tulip (www.tulip-software.org)
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
  unsigned  source,target;
};

namespace std {
template<>
struct less<edgeS> {
  bool operator()(const edgeS &c,const edgeS &d) const {
    int cs,ct,ds,dt;

    if (c.source<=c.target) {
      cs=c.source;
      ct=c.target;
    }
    else {
      ct=c.source;
      cs=c.target;
    }

    if (d.source<=d.target) {
      ds=d.source;
      dt=d.target;
    }
    else {
      dt=d.source;
      ds=d.target;
    }

    return (cs < ds) || ((cs == ds) && (ct < dt));
  }
};
}

//see http://stackoverflow.com/questions/10984974/why-do-people-say-there-is-modulo-bias-when-using-a-random-number-generator
static int random_number(const int n) {
    int x = rand();

    // Keep searching for an x in a range divisible by n
    while (x >= RAND_MAX - (RAND_MAX % n)) {
      x = rand();
    }

   return  x % n;
}

static const char *paramHelp[] = {
  // nodes
  "Number of nodes in the final graph.",

  // edges
  "Number of edges in the final graph."
};

/** \addtogroup import */

/// Random Simple Graph - Import of a random simple graph
/** This plugin enables to create a random simple graph
 *
 *  User can specify the number of nodes and the number of edges of the graph.
 */
class RandomSimpleGraph:public ImportModule {
public:
  PLUGININFORMATION("Random Simple Graph","Auber","16/06/2002","Import a new randomly generated simple graph with only one edge between two nodes.","1.0","Graph")
  RandomSimpleGraph(tlp::PluginContext* context):ImportModule(context) {
    addInParameter<unsigned int>("nodes",paramHelp[0],"500");
    addInParameter<unsigned int>("edges",paramHelp[1],"1000");
  }

  bool importGraph() {
    // initialize a random sequence according the given seed
    tlp::initRandomSequence();

    unsigned int nbNodes  = 500;
    unsigned int nbEdges  = 1000;

    if (dataSet!=NULL) {
      dataSet->get("nodes", nbNodes);
      dataSet->get("edges", nbEdges);
    }

    if (nbNodes == 0) {
      if (pluginProgress)
        pluginProgress->setError("The number of nodes must be greater than 0");

      return false;
    }

    int ite = nbNodes*nbEdges;
    int nbIteration = ite;
    set<edgeS> myGraph;

    if (pluginProgress)
      pluginProgress->showPreview(false);

    while (ite>0) {
      if (ite%nbNodes==1) if (pluginProgress->progress(nbIteration-ite,nbIteration)!=TLP_CONTINUE)
          return pluginProgress->state()!=TLP_CANCEL;

      edgeS tmp;
      tmp.source=random_number(nbNodes);
      tmp.target=random_number(nbNodes);

      while (tmp.source==tmp.target) {
        tmp.source=random_number(nbNodes);
        tmp.target=random_number(nbNodes);
      }

      if (myGraph.find(tmp)==myGraph.end()) {
        myGraph.insert(tmp);

        if (myGraph.size() == nbEdges)
          break;
      }

      ite--;
    }

    vector<node> tmpVect(nbNodes);
    graph->addNodes(nbNodes, tmpVect);

    graph->reserveEdges(myGraph.size());

    for (set<edgeS>::iterator it=myGraph.begin(); it!=myGraph.end(); ++it)   {
      graph->addEdge(tmpVect[it->source],tmpVect[it->target]);
    }

    return true;
  }
};

PLUGIN(RandomSimpleGraph)
