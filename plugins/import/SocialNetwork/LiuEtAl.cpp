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
  "Number of nodes."
};


/**
 *
 * This plugin is an implementation of the model
 * described in
 * J.-G. Liu, Y.-Z. Dang, and Z. tuo Wang.
 * Multistage random growing small-world networks with power-law degree distribution.
 * Chinese Phys. Lett., 23(3):746, Oct. 31 2005.
 *
 */
struct LiuEtAl:public ImportModule {
  PLUGININFORMATION("Liu et al. model","Arnaud Sallaberry","20/06/2011","Randomly generates a small world graph using the model described in<br/>J.-G. Liu, Y.-Z. Dang, and Z. tuo Wang.<br/><b>Multistage random growing small-world networks with power-law degree distribution.</b><br/>Chinese Phys. Lett., 23(3):746, Oct. 31 2005.","1.0", "Social network")
	
  LiuEtAl(PluginContext* context):ImportModule(context) 
  {
    addInParameter<unsigned int>("nodes",paramHelp[0],"300");
  }
  
  ~LiuEtAl(){}
  
  bool importGraph() {
    unsigned int n = 300;
    unsigned int m = 5;
	    
    if (dataSet!=NULL) {
      dataSet->get("nodes", n);
    }

    pluginProgress->showPreview(false);
    tlp::initRandomSequence();

    unsigned int i, j;
		
    /*
     * Initial ring construction
     */
    unsigned int m0 = 3;
    vector<node> sg(n);
    graph->addNodes(n, sg);
    graph->reserveEdges(m0 + (2 * (n - m0) * m/2));
    for (i=1; i<m0 ; ++i) {
      graph->addEdge(sg[i-1],sg[i]);
    }
    graph->addEdge(sg[m0-1],sg[0]);
		
    /*
     * Main loop
     */
    for (i=m0; i<n; ++i) {
      if (i % 100 == 0) {
	if (pluginProgress->progress(i, n) != TLP_CONTINUE)
	  return pluginProgress->state()!=TLP_CANCEL;
      }
      double k_sum = 0;
      for(j=0; j<i ; ++j) {
	k_sum += (double)graph->deg(sg[j]);
      }

      /*
       * Preferential attachement
       */
      for(j=0; j<m/2 ; ++j) {
	double pr = tlp::randomDouble();
	double pr_sum = 0;
	unsigned int rn = 0;
	while (pr_sum<pr && rn<(i-1)) {
	  pr_sum += (double)graph->deg(sg[rn])/(k_sum+j);
	  ++rn;
	}
	--rn;
				
	/*
	 * Triad formation
	 * Preferential attachement with a neighbour of the fisrt node selected
	 */
	double k2_sum = 0;
    node n;
    forEach(n, graph->getInOutNodes(sg[rn])) {
        k2_sum += (double)graph->deg(n);
    }
	pr = tlp::randomDouble();
	pr_sum = 0;
    node v;
    Iterator<node>* it = graph->getInOutNodes(sg[rn]);
	while(it->hasNext() && pr_sum<pr) {
      v = it->next();
      pr_sum += (double)graph->deg(v)/(k2_sum);
	}
	delete it;
				
	graph->addEdge(sg[i],sg[rn]);
	graph->addEdge(sg[i],v);
      }
    }
		
    return true;
  }
};

PLUGIN(LiuEtAl)
