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
#include <time.h>
#include <tulip/TulipPlugin.h>

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
	cs=c.source;ct=c.target;
      }
      else {
	ct=c.source;cs=c.target;
      }
      if (d.source<=d.target) {
	ds=d.source;dt=d.target;
      }
      else {
	dt=d.source;ds=d.target;
      }
      if (cs<ds) return true;
      if (cs>ds) return false;
      if (ct<dt) return true;
      if (ct>dt) return false;
      return false;
    }
  };
};


namespace
{

	const char * paramHelp[] =
	{
		// nodes
		HTML_HELP_OPEN() \
		HTML_HELP_DEF( "type", "unsigned int" ) \
		HTML_HELP_DEF( "default", "5" ) \
		HTML_HELP_BODY() \
		"This parameter defines the amount of node used to build the randomized graph." \
		HTML_HELP_CLOSE(),

		// edges
		HTML_HELP_OPEN() \
		HTML_HELP_DEF( "type", "unsigned int" ) \
		HTML_HELP_DEF( "default", "9" ) \
		HTML_HELP_BODY() \
		"This parameter defines the amount of edge used to build the randomized graph." \
		HTML_HELP_CLOSE(),
	};

}



/** \addtogroup import */
/*@{*/
/// Random Simple Graph - Import of a random simple graph
/** This plugin enables to create a random simple graph
 *
 *  User can specify the number of nodes and the number of edges of the graph.
 */
class RandomSimpleGraph:public ImportModule {
public:
  RandomSimpleGraph(AlgorithmContext context):ImportModule(context) {
    addParameter<unsigned int>("nodes",paramHelp[0],"5");
    addParameter<unsigned int>("edges",paramHelp[1],"9");
  }
  ~RandomSimpleGraph(){}
  
  bool import() {
    srand(clock());
    unsigned int nbNodes  = 5;
    unsigned int nbEdges  = 9;
    if (dataSet!=0) {
      dataSet->get("nodes", nbNodes);
      dataSet->get("edges", nbEdges);
    }

    if (nbNodes == 0) {
      if (pluginProgress)
	pluginProgress->setError(string("Error: the number of nodes cannot be null"));
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
      tmp.source=rand()%nbNodes;
      tmp.target=rand()%nbNodes;
      while (tmp.source==tmp.target) {
	tmp.source=rand()%nbNodes;
	tmp.target=rand()%nbNodes;
      }
      if (myGraph.find(tmp)==myGraph.end()) {
	myGraph.insert(tmp);
	if (myGraph.size() == nbEdges)
	  break;
      }
      ite--;
    }
    vector<node> tmpVect(nbNodes);
    for (unsigned int i=0; i<nbNodes; ++i) {
      tmpVect[i]=graph->addNode();
    }
    set<edgeS>::iterator it;
    for (it=myGraph.begin(); it!=myGraph.end(); ++it)   {
      graph->addEdge(tmpVect[(*it).source],tmpVect[(*it).target]);
    }
    return true;
  }
};
/*@}*/
IMPORTPLUGINOFGROUP(RandomSimpleGraph,"Random Simple Graph","Auber","16/06/2002","","1.0","Graphs")
