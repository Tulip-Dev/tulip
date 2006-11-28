#include <tulip/GraphProperty.h>
#include <tulip/ExtendedClusterOperation.h>
#include <tulip/StableIterator.h>
#include <iostream>
using namespace std;
using namespace tlp;

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
class ConnectedAndTreeComponent:public DoubleAlgorithm { 
public:
  ConnectedAndTreeComponent(const PropertyContext &context):DoubleAlgorithm(context){
    // Connected component metric dependency
    addDependency<DoubleAlgorithm>("Connected Component");
  };

  void dfsErase(node n, Graph * sg, set<node> &deleted, MutableContainer<bool> &visited) {
    if (visited.get(n.id)) return;
    visited.set(n.id,true);
    StableIterator<node> it(sg->getInOutNodes(n));
    while(it.hasNext()){
      dfsErase(it.next(), sg, deleted, visited);
    }
    if (sg->deg(n) < 2) {
      deleted.insert(n);
      sg->delNode(n);
    }
  }

  void startChainErase(node n, Graph * sg, set<node> &deleted) {
    while(sg->deg(n) == 1) {
      deleted.insert(n);
      Iterator<node> *it = sg->getInOutNodes(n);
      node tmp = it->next();
      delete it;
      deleted.insert(n);
      sg->delNode(n);
      n = tmp;
    }
  }

  bool run() {
    Graph *sg=tlp::newCloneSubGraph(graph);
    string errMsg;    
    MutableContainer<bool> visited;
    visited.setAll(false);
    set<node> deleted;
    StableIterator<node> it(sg->getNodes());
    while(it.hasNext()) { 
      node n = it.next();
      if (!visited.get(n.id)) {
	dfsErase(n, sg, deleted, visited);
      }
    }
    
    StableIterator<node> it2(sg->getNodes());
    while(it2.hasNext()) { 
      node n = it2.next();
      if (sg->isElement(n))
	startChainErase(n, sg, deleted);
    }
    
    DoubleProperty connectedcomponent(sg);
    sg->computeProperty("Connected Component", &connectedcomponent, errMsg);
    
    Graph * sg2 = tlp::inducedSubGraph(graph, deleted);
    DoubleProperty connectedcomponent2(sg2);
    sg2->computeProperty("Connected Component", &connectedcomponent2, errMsg);
    {
      double theMax = 0;
      Iterator<node> *it = sg->getNodes();
      while(it->hasNext()){
	node n = it->next();
	double nv = connectedcomponent.getNodeValue(n);
	if (theMax < nv) theMax = nv;
	doubleResult->setNodeValue(n, nv);
      } delete it;
      theMax += 1.0;
      it = sg2->getNodes();
      while(it->hasNext()){
	node n = it->next();
	doubleResult->setNodeValue(n, connectedcomponent2.getNodeValue(n) + theMax);
      }delete it;
    }
    graph->delAllSubGraphs(sg);
    graph->delAllSubGraphs(sg2);
    
    return true;
  }
};
/*@}*/
DOUBLEPLUGINOFGROUP(ConnectedAndTreeComponent,"Connected_Tree Component","David Auber","03/01/2005","Alpha","0","1","Component");
