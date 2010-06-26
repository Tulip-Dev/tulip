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
#include <tulip/SuperGraph.h>
#include <tulip/MapIterator.h>
#include <tulip/TlpTools.h>

using namespace std;

//==========================================================================
namespace {
  bool haveChoords(SuperGraph *graph, const node n, const set<node> &outerface){
    assert(graph->isElement(n));
    Iterator<node> *it = graph->getInOutNodes(n);
    unsigned int count =0;
    while (count < 3 && it->hasNext()) {
      if (outerface.find(it->next())!=outerface.end())
	++count;
    } delete it;
    return count > 2;
  }
}
//==========================================================================
namespace tlp {
  void canonicalOrdering(SuperGraph *graph, vector<node> &order) {
    //input embed graph planar maximum 
    unsigned int nbNodes = graph->numberOfNodes();
    order.resize(nbNodes);
    //choose V0,V1, Vn-1
    edge e = graph->getOneEdge();
    order[0] = graph->source(e);
    order[1] = graph->target(e);
    edge e2 = tlp::nextFaceEdge(graph, e, order[1]);
    order[nbNodes-1] = graph->opposite(e2, order[1]);
    SuperGraph *subgraph = tlp::newCloneSubGraph(graph);
    cerr << subgraph->numberOfNodes() << "," << subgraph->numberOfEdges() << endl;
    //  cerr << subgraph << endl;
    set<node> candidates;
    candidates.insert(order[nbNodes -1]);
    candidates.insert(order[0]);
    candidates.insert(order[1]);
    unsigned int k = nbNodes - 1;
    while (k>2) {
      set<node>::iterator it = candidates.begin();
      for (;it!=candidates.end();++it) {
	if (*it!=order[0] && *it!= order[1] && !haveChoords(subgraph, *it, candidates)) {
	  order[k] = *it;
	  --k;
	  Iterator<node> *itN = subgraph->getInOutNodes(*it);
	  while (itN->hasNext()) {
	    node n = itN->next();
	    if ( n!=order[0] && n!=order[1] )
	      candidates.insert(n);
	  } delete itN;
	  subgraph->delNode(*it);
	  candidates.erase(it);
	  break;
	}
      }
    }
    subgraph->delNode(order[0]);
    subgraph->delNode(order[1]);
    assert(subgraph->numberOfNodes()==1);
    order[2] = subgraph->getOneNode();
    graph->delSubGraph(subgraph);
  }
}
//==========================================================================
