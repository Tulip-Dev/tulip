#include <tulip/MetaGraph.h>
#include <tulip/TlpTools.h>
#include <tulip/StableIterator.h>
#include <iostream>
using namespace std;

#include <tulip/TulipPlugin.h>
/** \addtogroup metric */
/*@{*/
/// ConnectedAndTreeComponent.h - Conected componnent and tree component decomposition.
/** This plugin implementation implements a decomposition of the graph into connected component 
 *  and tree component (ie. subgraph that are tree and subgraph that are connected)  
 *
 *  <b>HISTORY</b>
 *
 *  - 03/01/2005 Verson 0.0.1: Initial release
 *
 *
 *  \note This algorithm assigns to each node a value defined as following : If two nodes are in the same
 *  connected component they have the same value else they have a 
 *  different value.
 *
 *
 *  \author David Auber University Bordeaux I France: Email:auber@tulip-software.org
 *  \author Daniel Archambault University Of British Columbia
 *
 *  <b>LICENCE</b>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by  
 *  the Free Software Foundation; either version 2 of the License, or     
 *  (at your option) any later version.
 *
 */
class ConnectedAndTreeComponent:public MetricAlgorithm { 
public:
  ConnectedAndTreeComponent(const PropertyContext &context):MetricAlgorithm(context){};

  void dfsErase(node n, SuperGraph * graph, set<node> &deleted, MutableContainer<bool> &visited) {
    if (visited.get(n.id)) return;
    visited.set(n.id,true);
    StableIterator<node> it(graph->getInOutNodes(n));
    while(it.hasNext()){
      dfsErase(it.next(), graph, deleted, visited);
    }
    if (graph->deg(n) < 2) {
      deleted.insert(n);
      graph->delNode(n);
    }
  }

  void startChainErase(node n, SuperGraph * graph, set<node> &deleted) {
    while(graph->deg(n) == 1) {
      deleted.insert(n);
      Iterator<node> *it = graph->getInOutNodes(n);
      node tmp = it->next();
      delete it;
      deleted.insert(n);
      graph->delNode(n);
      n = tmp;
    }
  }

  bool run() {
    SuperGraph *graph=tlp::newCloneSubGraph(superGraph);
    string errMsg;    
    MutableContainer<bool> visited;
    visited.setAll(false);
    set<node> deleted;
    StableIterator<node> it(graph->getNodes());
    while(it.hasNext()) { 
      node n = it.next();
      if (!visited.get(n.id)) {
	dfsErase(n, graph, deleted, visited);
      }
    }
    
    StableIterator<node> it2(graph->getNodes());
    while(it2.hasNext()) { 
      node n = it2.next();
      if (graph->isElement(n))
	startChainErase(n, graph, deleted);
    }
    
    Metric connectedcomponent(graph);
    graph->computeProperty("Connected Component", &connectedcomponent, errMsg);
    
    SuperGraph * graph2 = tlp::inducedSubGraph(superGraph, deleted);
    Metric connectedcomponent2(graph2);
    graph2->computeProperty("Connected Component", &connectedcomponent2, errMsg);
    {
      double theMax = 0;
      Iterator<node> *it = graph->getNodes();
      while(it->hasNext()){
	node n = it->next();
	double nv = connectedcomponent.getNodeValue(n);
	if (theMax < nv) theMax = nv;
	metricObj->setNodeValue(n, nv);
      } delete it;
      theMax += 1.0;
      it = graph2->getNodes();
      while(it->hasNext()){
	node n = it->next();
	metricObj->setNodeValue(n, connectedcomponent2.getNodeValue(n) + theMax);
      }delete it;
    }
    superGraph->delAllSubGraphs(graph);
    superGraph->delAllSubGraphs(graph2);
    
    return true;
  }
};
/*@}*/
METRICPLUGIN(ConnectedAndTreeComponent,"Connected_Tree Component","David Auber","03/01/2005","Alpha","0","1");
