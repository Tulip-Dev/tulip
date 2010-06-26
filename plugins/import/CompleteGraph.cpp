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
		// nodes
		HTML_HELP_OPEN() \
		HTML_HELP_DEF( "type", "int" ) \
		HTML_HELP_DEF( "default", "5" ) \
		HTML_HELP_BODY() \
		"This parameter defines the amount of nodes composing the completed graph." \
		HTML_HELP_CLOSE(),
	};

}



struct CompleteGraph:public ImportModule {

  CompleteGraph(AlgorithmContext context):ImportModule(context) {
    addParameter<int>("nodes",paramHelp[0],"5");
  }
  ~CompleteGraph(){
  }
  
  bool import(const string &) {
    int nbNodes  = 5;
    if (dataSet!=0) {
      dataSet->get("nodes", nbNodes);
    }
    
    if (pluginProgress)
      pluginProgress->showPreview(false);

    for (int j=0; j<nbNodes; ++j) 
      graph->addNode();
    Iterator<node> *it1,*it2;
    it1=graph->getNodes();
    int i=0;
    while (it1->hasNext()) {
      if (pluginProgress->progress(i,nbNodes) != TLP_CONTINUE) 
	return false;
      i++;
      node n1=it1->next();
      it2=graph->getNodes();
      while (it2->hasNext()) {
	node n2=it2->next();
	if (n1!=n2) graph->addEdge(n1,n2);
      } delete it2;
    } delete it1;
    return true;
  }
};

IMPORTPLUGINOFGROUP(CompleteGraph,"Complete General Graph","Auber","16/12/2002","","1.0","Graphs")
