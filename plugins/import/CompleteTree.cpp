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
#include <list>
#include <tulip/TulipPlugin.h>

using namespace std;
using namespace tlp;



namespace
{

  static const char * paramHelp[] =
    {
      // depth
      HTML_HELP_OPEN()					\
      HTML_HELP_DEF( "type", "unsigned int" )		\
      HTML_HELP_DEF( "default", "5" )			\
      HTML_HELP_BODY()						    \
      "This parameter defines the tree's depth to build."	    \
      HTML_HELP_CLOSE(),

      // degree
      HTML_HELP_OPEN() \
      HTML_HELP_DEF( "type", "unsigned int" ) \
      HTML_HELP_DEF( "default", "2" ) \
      HTML_HELP_BODY() \
      "This parameter defines the tree's degree to build." \
      HTML_HELP_CLOSE(),
      // tree layout
      HTML_HELP_OPEN() \
      HTML_HELP_DEF( "type", "bool" ) \
      HTML_HELP_DEF( "default", "false" ) \
      HTML_HELP_BODY() \
      "This parameter indicates if the generated tree has to be drawn with a tree layout algorithm." \
      HTML_HELP_CLOSE(),
    };

}



/** \addtogroup import */
/*@{*/
/// Complete Tree - Import of a complete tree
/** This plugin enables to create a complete tree
 *
 *  User can specify the depth and the degree of the tree.
 */
class CompleteTree:public ImportModule {
public:
  CompleteTree(AlgorithmContext context):ImportModule(context) {
    addParameter<unsigned int>("depth",paramHelp[0],"5");
    addParameter<unsigned int>("degree",paramHelp[1],"2");
    addParameter<bool>("tree layout",paramHelp[2],"false");
    addDependency<LayoutAlgorithm>("Tree Leaf", "1.0");
  }
  ~CompleteTree(){
  }

  bool import(const string &) {
    unsigned int degree  = 2;
    unsigned int depth   = 5;
    bool needLayout = false;
    if (dataSet!=0) {
      dataSet->get("depth", depth);
      dataSet->get("degree", degree);
      dataSet->get("tree layout", needLayout);
   }
    // reserve enough memory for nodes/edges to add
    unsigned int total = 0, previous = 1;
    for(unsigned int i = 0; i < depth; ++i) {
      previous *= degree;
      total += previous;
    }
    graph->reserveEdges(total);
    graph->reserveNodes(total + 1);
    node n = graph->addNode();
    list<node> nodes;
    nodes.push_front(n);
    while(total) {
      n = nodes.front();
      nodes.pop_front();
      for (unsigned int i = 0; i < degree; ++i, --total) {
	node n1 = graph->addNode();
	graph->addEdge(n, n1);
	nodes.push_back(n1);
      }
    }
    if (needLayout) {
      // apply Tree Leaf
      DataSet dSet;
      string errMsg;
      LayoutProperty *layout = graph->getProperty<LayoutProperty>("viewLayout");
      return graph->computeProperty("Tree Leaf", layout, errMsg,
				    pluginProgress, &dSet);
    }
    return true;
  }
};
/*@}*/
IMPORTPLUGINOFGROUP(CompleteTree,"Complete Tree","Auber","08/09/2002","","1.1","Graphs")
