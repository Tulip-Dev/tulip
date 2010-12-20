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
#include <tulip/TulipPlugin.h>

using namespace std;
using namespace tlp;



namespace
{

	const char * paramHelp[] =
	{
		// depth
		HTML_HELP_OPEN() \
		HTML_HELP_DEF( "type", "int" ) \
		HTML_HELP_DEF( "default", "5" ) \
		HTML_HELP_BODY() \
		"This parameter defines the tree's depth to build." \
		HTML_HELP_CLOSE(),

		// degree
		HTML_HELP_OPEN() \
		HTML_HELP_DEF( "type", "int" ) \
		HTML_HELP_DEF( "default", "2" ) \
		HTML_HELP_BODY() \
		"This parameter defines the tree's degree to build." \
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
    addParameter<int>("depth",paramHelp[0],"5");
    addParameter<int>("degree",paramHelp[1],"2");
  }
  ~CompleteTree(){
  }
  void buildNode(node n,unsigned int degree,int depth) {
    if (depth<1) return;
    for (unsigned int i=0;i<degree;++i) {
      node n1=graph->addNode();
      graph->addEdge(n,n1);
      buildNode(n1,degree,depth - 1);
    }
  }
  bool import(const string &) {
    int degree  = 2;
    int depth   = 5;
    if (dataSet!=0) {
      dataSet->get("depth", depth);
      dataSet->get("degree", degree);
    }
    node n=graph->addNode();
    buildNode(n,degree,depth);
    return true;
  }
};
/*@}*/
IMPORTPLUGINOFGROUP(CompleteTree,"Complete Tree","Auber","08/09/2002","","1.0","Graphs")
