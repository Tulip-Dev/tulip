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
#include <cmath>

using namespace std;
using namespace tlp;

namespace {

const char * paramHelp[] = {
  // minsize
  HTML_HELP_OPEN() \
  HTML_HELP_DEF( "type", "int" ) \
  HTML_HELP_DEF( "default", "10" ) \
  HTML_HELP_BODY() \
  "Minimal number of nodes in the tree." \
  HTML_HELP_CLOSE(),
  // maxsize
  HTML_HELP_OPEN() \
  HTML_HELP_DEF( "type", "int" ) \
  HTML_HELP_DEF( "default", "100" ) \
  HTML_HELP_BODY() \
  "Maximal number of nodes in the tree." \
  HTML_HELP_CLOSE(),
  // maxdegree
  HTML_HELP_OPEN() \
  HTML_HELP_DEF( "type", "int" ) \
  HTML_HELP_DEF( "default", "5" ) \
  HTML_HELP_BODY() \
  "Maximal degree of the nodes." \
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

/// Random General Tree - Import of a random general tree
/** This plugin enables to create a random general tree
 *
 *  User can specify the minimal/maximal number of nodes and the maximal degree.
 */
class RandomTreeGeneral:public ImportModule {

  bool buildNode(node n,unsigned int sizeM,int arityMax) {
    if (graph->numberOfNodes()>=sizeM) return true;

    bool result=true;
    int randNumber=rand();
    int i = 0;

    while (randNumber < RAND_MAX/pow(2.0,1.0+i))
      ++i;

    i = i % arityMax;
    graph->reserveNodes(i);
    graph->reserveEdges(i);

    for (; i>0; --i) {
      node n1;
      n1=graph->addNode();
      graph->addEdge(n,n1);
      result= result && buildNode(n1,sizeM,arityMax);
    }

    return result;
  }

public:
  PLUGININFORMATION("Random General Tree","Auber","16/02/2001","Imports a new randomly generated tree.","1.1","Graph")
  RandomTreeGeneral(tlp::PluginContext* context):ImportModule(context) {
    addInParameter<unsigned>("Minimum size",paramHelp[0],"10");
    addInParameter<unsigned>("Maximum size",paramHelp[1],"100");
    addInParameter<unsigned>("Maximal node's degree",paramHelp[2],"5");
    addInParameter<bool>("tree layout",paramHelp[3],"false");
    addDependency("Tree Leaf", "1.0");
  }

  bool importGraph() {
    // initialize a random sequence according the given seed
    tlp::initRandomSequence();

    unsigned int sizeMin  = 10;
    unsigned int sizeMax  = 100;
    unsigned int arityMax = 5;
    bool needLayout = false;

    if (dataSet!=NULL) {
      if(dataSet->exist("Minimum size"))
        dataSet->get("Minimum size", sizeMin);
      else
        dataSet->get("minsize", sizeMin);   //keep old parameter name for backward compatibility

      if(dataSet->exist("Maximum size"))
        dataSet->get("Maximum size", sizeMax);
      else
        dataSet->get("maxsize", sizeMax); //keep old parameter name for backward compatibility

      if(dataSet->exist("Maximal node's degree"))
        dataSet->get("Maximal node's degree", arityMax);
      else
        dataSet->get("maxdegree", arityMax); //keep old parameter name for backward compatibility

      dataSet->get("tree layout", needLayout);
    }

    if (arityMax < 1) {
      if (pluginProgress)
        pluginProgress->setError("Error: maximum node's degree must be a strictly positive integer");

      return false;
    }

    if (sizeMax < 1) {
      if (pluginProgress)
        pluginProgress->setError("Error: maximum size must be a strictly positive integer");

      return false;
    }

    if (sizeMax < sizeMin) {
      if (pluginProgress)
        pluginProgress->setError("Error: maximum size must be greater than minimum size");

      return false;
    }

    bool ok=true;
    int i=0;
    unsigned int nbTest=0;

    while (ok) {
      ++nbTest;

      if (nbTest%100 == 0) {
        if (pluginProgress->progress((i/100)%100,100)!=TLP_CONTINUE) break;
      }

      ++i;
      graph->clear();
      node n=graph->addNode();
      ok=!buildNode(n,sizeMax,arityMax);

      if (graph->numberOfNodes()<sizeMin) ok=true;
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

PLUGIN(RandomTreeGeneral)
