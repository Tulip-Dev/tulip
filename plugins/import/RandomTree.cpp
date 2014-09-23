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

namespace {

const char * paramHelp[] = {
  // minsize
  HTML_HELP_OPEN() \
  HTML_HELP_DEF( "type", "unsigned int" ) \
  HTML_HELP_DEF( "default", "100" ) \
  HTML_HELP_BODY() \
  "Minimal number of nodes in the tree." \
  HTML_HELP_CLOSE(),

  // maxsize
  HTML_HELP_OPEN() \
  HTML_HELP_DEF( "type", "unsigned int" ) \
  HTML_HELP_DEF( "default", "1000" ) \
  HTML_HELP_BODY() \
  "Maximal number of nodes in the tree." \
  HTML_HELP_CLOSE(),

  // tree layout
  HTML_HELP_OPEN() \
  HTML_HELP_DEF( "type", "bool" ) \
  HTML_HELP_DEF( "default", "false" ) \
  HTML_HELP_BODY() \
  "If true, the generated tree is drawn with a tree layout algorithm." \
  HTML_HELP_CLOSE(),
};
}

/** \addtogroup import */

/// Random Tree - Import of a random uniform binary tree
/** This plugin enables to create a random tree
 *
 *  User can specify the minimal/maximal numbers of nodes used to build of the tree.
 */
class RandomTree:public ImportModule {

  bool buildNode(node n, unsigned int sizeM) {
    if (graph->numberOfNodes()>=sizeM+2) return false;

    bool result=true;
    int randNumber=rand();

    if (randNumber>RAND_MAX/2) {
      node n1,n2;
      n1=graph->addNode();
      n2=graph->addNode();
      graph->addEdge(n,n1);
      graph->addEdge(n,n2);
      result= result && buildNode(n1,sizeM);
      result= result && buildNode(n2,sizeM);
    }

    return result;
  }

public:
  PLUGININFORMATION("Uniform Random Binary Tree","Auber","16/02/2001","Imports a new randomly generated uniform binary tree.","1.1","Graph")
  RandomTree(tlp::PluginContext* context):ImportModule(context) {
    addInParameter<unsigned int>("Minimum size",paramHelp[0],"100");
    addInParameter<unsigned int>("Maximum size",paramHelp[1],"1000");
    addInParameter<bool>("tree layout",paramHelp[2],"false");
    addDependency("Tree Leaf", "1.0");
  }

  bool importGraph() {
    // initialize a random sequence according the given seed
    tlp::initRandomSequence();

    unsigned int minSize  = 100;
    unsigned int maxSize  = 1000;
    bool needLayout = false;

    if (dataSet!=NULL) {
      if(dataSet->exist("Minimum size"))
        dataSet->get("Minimum size", minSize);
      else
        dataSet->get("minsize", minSize);   //keep old name for backward compatibility

      if(dataSet->exist("Maximum size"))
        dataSet->get("Maximum size", maxSize);
      else
        dataSet->get("maxsize", maxSize); //keep old name for backward compatibility

      dataSet->get("tree layout", needLayout);
    }

    if (maxSize < 1) {
      if (pluginProgress)
        pluginProgress->setError("Error: maximum size must be a strictly positive integer");

      return false;
    }

    if (maxSize < minSize) {
      if (pluginProgress)
        pluginProgress->setError("Error: maximum size must be greater than minimum size");

      return false;
    }

    bool ok=true;
    int i=0;

    while (ok) {
      if (pluginProgress->progress(i%100,100)!=TLP_CONTINUE) break;

      ++i;
      graph->clear();
      node n=graph->addNode();
      ok = !buildNode(n,maxSize);

      if (graph->numberOfNodes()<minSize-2) ok=true;
    }

    if (pluginProgress->progress(100,100)==TLP_CANCEL)
      return false;

    if (needLayout) {
      // apply Tree Leaf
      DataSet dSet;
      string errMsg;
      LayoutProperty *layout = graph->getProperty<LayoutProperty>("viewLayout");
      return graph->applyPropertyAlgorithm("Tree Leaf", layout, errMsg,
                                           pluginProgress, &dSet);
    }

    return true;
  }
};

PLUGIN(RandomTree)
