#include <tulip/MetaGraphProxy.h>
#include <tulip/TlpTools.h>
#include <tulip/StableIterator.h>
#include <iostream>
using namespace std;

#include <tulip/TulipPlugin.h>
///
class ConnectedAndTreeComponent:public Metric { 
public:
  ConnectedAndTreeComponent(const PropertyContext &context):Metric(context){};

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
    
    MetricProxy connectedcomponent(graph);
    graph->computeProperty("Connected Component", &connectedcomponent, errMsg);
    
    SuperGraph * graph2 = tlp::inducedSubGraph(superGraph, deleted);
    MetricProxy connectedcomponent2(graph2);
    graph2->computeProperty("Connected Component", &connectedcomponent2, errMsg);
    {
      double max = 0;
      Iterator<node> *it = graph->getNodes();
      while(it->hasNext()){
	node n = it->next();
	max >?= connectedcomponent.getNodeValue(n);
	metricProxy->setNodeValue(n, connectedcomponent.getNodeValue(n));
      }delete it;
      max += 1.0;
      it = graph2->getNodes();
      while(it->hasNext()){
	node n = it->next();
	metricProxy->setNodeValue(n, connectedcomponent2.getNodeValue(n) + max);
      }delete it;
    }
    superGraph->delAllSubGraphs(graph);
    superGraph->delAllSubGraphs(graph2);
    
    return true;
  }
};

METRICPLUGIN(ConnectedAndTreeComponent,"Connected_Tree Component","David Auber","03/01/2005","Alpha","0","1");
