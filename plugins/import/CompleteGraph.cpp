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
#include <tulip/TulipPluginHeaders.h>

using namespace std;
using namespace tlp;

/**
 * TODO Add documentation
 **/

namespace {

const char * paramHelp[] = {
  // nodes
  HTML_HELP_OPEN() \
  HTML_HELP_DEF( "type", "int" ) \
  HTML_HELP_DEF( "default", "5" ) \
  HTML_HELP_BODY() \
  "Number of nodes in the final graph." \
  HTML_HELP_CLOSE(),
  // bool
  HTML_HELP_OPEN() \
  HTML_HELP_DEF( "type", "bool" ) \
  HTML_HELP_DEF( "default", "true" ) \
  HTML_HELP_BODY() \
  "If true, the generated graph is undirected. If false, two edges are created between each pair of nodes." \
  HTML_HELP_CLOSE(),

};
}


class CompleteGraph:public ImportModule {
public:
  PLUGININFORMATIONS("Complete General Graph","Auber","16/12/2002","","1.1","Graphs")
  CompleteGraph(tlp::PluginContext* context):ImportModule(context) {
    addInParameter<unsigned int>("nodes",paramHelp[0],"5");
    addInParameter<bool>("undirected",paramHelp[1],"true");
  }
  ~CompleteGraph() {
  }

  bool importGraph() {
    unsigned int nbNodes  = 5;
    bool undirected = false;

    if (dataSet!=NULL) {
      dataSet->get("nodes", nbNodes);
      dataSet->get("undirected", undirected);
    }

    if (nbNodes == 0) {
      if (pluginProgress)
        pluginProgress->setError(string("Error: number of nodes cannot be null"));

      return false;
    }

    if (pluginProgress)
      pluginProgress->showPreview(false);

    vector<node> nodes(nbNodes);

    for (size_t j=0; j<nbNodes; ++j)
      nodes[j] = graph->addNode();

    for (size_t i=0; i < nbNodes-1; ++i)
      for (size_t j = i+1; j < nbNodes; ++j) {
        graph->addEdge(nodes[i], nodes[j]);

        if (!undirected)
          graph->addEdge(nodes[j], nodes[i]);
      }

    return true;
  }
};

PLUGIN(CompleteGraph)
