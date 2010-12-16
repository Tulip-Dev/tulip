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
#include <iostream>
#include <stack>
#include <queue>
#include <tulip/tuliphash.h>
#include <tulip/TulipPlugin.h>



using namespace std;
using namespace tlp;

static const char * paramHelp[] = {
  HTML_HELP_OPEN() \
  HTML_HELP_DEF( "type", "bool" ) \
  HTML_HELP_BODY() \
  "indicate if the graph should be considered as directed or not" \
  HTML_HELP_CLOSE()
};

/** \addtogroup metric */
/*@{*/
/** This plugin is an implementation of betweeness centrality parameter.
 *  algorithm published by:
 *
 *  U. Brandes,
 *
 */
class BetweennessCentrality:public DoubleAlgorithm { 
public:
  BetweennessCentrality(const PropertyContext &context):DoubleAlgorithm(context){
  addParameter<bool>("directed", paramHelp[0], "false");  
  };
  bool run() {
    doubleResult->setAllNodeValue(0.0);
    bool directed = false;
    if ( dataSet!=0 ) {
      dataSet->get("directed",directed);
    }

    Iterator<node> *it = graph->getNodes();
    unsigned int count = 0;
    while(it->hasNext()) {
      if (pluginProgress->progress(count++,graph->numberOfNodes())!=TLP_CONTINUE) break;
      node s = it->next();
      stack<node> S;
      TLP_HASH_MAP<node, list<node> > P;
      MutableContainer<int> sigma;
      sigma.setAll(0);
      sigma.set(s.id,1);
      MutableContainer<int> d;
      d.setAll(-1);
      d.set(s.id, 0);
      queue<node> Q;
      Q.push(s);
      while(!Q.empty()) {
	node v = Q.front();
	Q.pop();
	S.push(v);
	Iterator<node> *it2;
	if (directed)
	  it2 = graph->getOutNodes(v);
	else 
	  it2 = graph->getInOutNodes(v);
	while (it2->hasNext()) {
	  node w = it2->next();
	  if (d.get(w.id)<0) {
	    Q.push(w);
	    d.set(w.id, d.get(v.id)+1);
	  }
	  if (d.get(w.id) == d.get(v.id)+1) {
	    sigma.set(w.id, sigma.get(w.id) + sigma.get(v.id));
	    P[w].push_back(v);
	  }
	} delete it2;
      }
      MutableContainer<double> delta;
      delta.setAll(0.0);
      while(!S.empty()) {
	node w = S.top();
	S.pop();
	list<node>::const_iterator itn = P[w].begin();
	for (;itn!=P[w].end();++itn){
	  node v = *itn;
	  delta.set(v.id, delta.get(v.id) + double(sigma.get(v.id)) / double(sigma.get(w.id)) * (1.0 + delta.get(w.id)));
	}
	if (w != s) doubleResult->setNodeValue(w, doubleResult->getNodeValue(w) + delta.get(w.id)); 
      }
    } delete it;
    return pluginProgress->state()!=TLP_CANCEL;
  }
};
/*@}*/
DOUBLEPLUGINOFGROUP(BetweennessCentrality,"Betweenness Centrality","David Auber","03/01/2005","Alpha","1.0","Graph");
