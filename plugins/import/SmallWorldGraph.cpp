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
#include <time.h>
#include <math.h>
#include <climits>
#include <tulip/TulipPluginHeaders.h>

using namespace std;
using namespace tlp;
static const int WIDTH = 1024;
static const int HEIGHT = 1024;

namespace {
const char * paramHelp[] = {
  // nodes
  HTML_HELP_OPEN() \
  HTML_HELP_DEF( "type", "unsigned int" ) \
  HTML_HELP_DEF( "default", "100" ) \
  HTML_HELP_BODY() \
  "Number of nodes in the final graph." \
  HTML_HELP_CLOSE(),

  // degree
  HTML_HELP_OPEN() \
  HTML_HELP_DEF( "type", "unsigned int" ) \
  HTML_HELP_DEF( "default", "10" ) \
  HTML_HELP_BODY() \
  "Average degree of the nodes in the final graph." \
  HTML_HELP_CLOSE(),

  // degree
  HTML_HELP_OPEN() \
  HTML_HELP_DEF( "type", "bool" ) \
  HTML_HELP_DEF( "default", "false" ) \
  HTML_HELP_BODY() \
  "If true, long distance edges are added in the grid approximation." \
  HTML_HELP_CLOSE(),
};
}

/*
 * TODO :
 * Use a quadtree in order to change the complexity from n*n to n*log(n)
 */

/** \addtogroup import */

/// Grid Approximation - Import of a grid approximation
/** This plugin enables to create a grid approximation
 *
 *  User can specify the number of nodes and their average degree.
 */
class SmallWorldGraph:public ImportModule {
public:
  PLUGININFORMATION("Grid Approximation","Auber","25/06/2002","Imports a new grid approximation graph.","1.0","Graph")
  SmallWorldGraph(tlp::PluginContext* context):ImportModule(context) {
    addInParameter<unsigned int>("nodes",paramHelp[0],"200");
    addInParameter<unsigned int>("degree",paramHelp[1],"10");
    addInParameter<bool>("long edge",paramHelp[2],"false");
  }
  ~SmallWorldGraph() {}

  bool importGraph() {
    unsigned int nbNodes  = 200;
    unsigned int avgDegree = 10;
    bool enableLongEdge = false;

    if (dataSet!=NULL) {
      dataSet->get("nodes", nbNodes);
      dataSet->get("degree", avgDegree);
      dataSet->get("long edge", enableLongEdge);
    }

    if (nbNodes == 0) {
      if (pluginProgress)
        pluginProgress->setError(string("Error: the number of nodes cannot be null"));

      return false;
    }

    if (avgDegree == 0) {
      if (pluginProgress)
        pluginProgress->setError(string("Error: the average degree cannot be null"));

      return false;
    }

    double maxDistance = sqrt(double(avgDegree)*double(WIDTH)*double(HEIGHT)
                              / (double (nbNodes) * M_PI));
    // initialize a random sequence according the given seed
    tlp::initRandomSequence();

    LayoutProperty *newLayout=graph->getLocalProperty<LayoutProperty>("viewLayout");
    vector<node> sg(nbNodes);

    pluginProgress->showPreview(false);

    graph->addNodes(nbNodes, sg);

    for (unsigned int i=0; i<nbNodes; ++i) {
      newLayout->setNodeValue(sg[i],Coord(static_cast<float>(rand()%WIDTH), static_cast<float>(rand()%HEIGHT), 0));
    }

    double minSize = DBL_MAX;

    for (unsigned int i=0; i<nbNodes-1; ++i) {
      bool longEdge =false;

      for (unsigned int j=i+1; j<nbNodes; ++j) {
        if (i!=j) {
          double distance = newLayout->getNodeValue(sg[i]).dist(newLayout->getNodeValue(sg[j]));
          minSize = std::min(distance, minSize);

          //newSize->setAllNodeValue(Size(minSize/2.0, minSize/2.0, 1));
          if ( distance  < (double)maxDistance)
            graph->addEdge(sg[i],sg[j]);
          else if (!longEdge && enableLongEdge) {
            double distrand = (double)rand()/(double)RAND_MAX;

            if (distrand < 1.0/(2.0+double(nbNodes-i-1))) {
              longEdge = true;
              graph->addEdge(sg[i],sg[j]);
            }
          }
        }
      }

      if (pluginProgress->progress(i, nbNodes - 1)!=TLP_CONTINUE) break;
    }

    return  pluginProgress->state()!=TLP_CANCEL;
  }
};

PLUGIN(SmallWorldGraph)
