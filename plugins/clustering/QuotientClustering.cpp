#include <set>
#include <stdio.h>
#include <math.h>
#include <sstream>
#include <tulip/Graph.h>
#include <tulip/GraphProperty.h>
#include <tulip/DrawingTools.h>

#include "QuotientClustering.h"


using namespace std;
using namespace tlp;

ALGORITHMPLUGIN(QuotientClustering,"QuotientClustering","David Auber","13/06/2001","Alpha","0","1");

struct Edge {
  unsigned int source,target;
};

namespace std {
  template<>
  struct less<Edge> {
    bool operator()(const Edge &c,const Edge &d) const {
      if (c.source<d.source) return true;
      if (c.source>d.source) return false;
      if (c.target<d.target) return true;
      if (c.target>d.target) return false;
      return false;
    }
  };
};
//================================================================================
QuotientClustering::QuotientClustering(AlgorithmContext context):Algorithm(context) {}
//================================================================================
QuotientClustering::~QuotientClustering(){}
//===============================================================================
bool QuotientClustering::run() {
  Graph *quotientGraph = tlp::newSubGraph(graph->getRoot());
  GraphProperty *meta = quotientGraph->getProperty<GraphProperty>("viewMetaGraph");
  //Create one metanode for each subgraph(cluster) of current graph.
  Graph *sg= graph;
  map<Graph*,node> mapping;
  Iterator<Graph *> *itS=sg->getSubGraphs();
  while (itS->hasNext()) {
    Graph *its=itS->next();
    if (its!=quotientGraph) {
      node n = quotientGraph->addNode();
      meta->setNodeValue(n,its);
      mapping[its]=n;
    }
  } delete itS;
  //
  set<Edge> myQuotientGraph;
  Iterator<edge>*itE=graph->getEdges();
  while (itE->hasNext()) {
    edge ite=itE->next();
    node source=graph->source(ite);
    node target=graph->target(ite);
    list<Graph *> clusterSource;
    list<Graph *> clusterTarget;
    clusterSource.clear();
    clusterTarget.clear();
    itS=sg->getSubGraphs();
    while (itS->hasNext()) {
      Graph *its=itS->next();
      if (its!=quotientGraph) {
	Graph *tmp=its;
	if (tmp->isElement(source)) clusterSource.push_back(its);
	if (tmp->isElement(target)) clusterTarget.push_back(its);
      }
    } delete itS;
    
    for (std::list<Graph *>::iterator it1=clusterSource.begin(); it1!=clusterSource.end(); ++it1)
      for (std::list<Graph *>::iterator it2=clusterTarget.begin(); it2!=clusterTarget.end(); ++it2) {
	Edge tmp;
	tmp.source=mapping[*it1].id;
	tmp.target=mapping[*it2].id;
	if ( (tmp.source!=tmp.target) && (myQuotientGraph.find(tmp)==myQuotientGraph.end()) ) {
	  myQuotientGraph.insert(tmp);
	  quotientGraph->addEdge(mapping[*it1],mapping[*it2]);
	}
      }
  } delete itE;

  //compute layout according to the layouts of subgraphs
  SizeProperty *size  = quotientGraph->getProperty<SizeProperty>("viewSize");
  Iterator<node> *itN = quotientGraph->getNodes();
  while (itN->hasNext()) {
    node n = itN->next();
    Graph * graph = meta->getNodeValue(n);
    LayoutProperty* graphlayout = sg->getProperty<LayoutProperty>("viewLayout");
    SizeProperty* graphsize = sg->getProperty<SizeProperty>("viewSize");
    DoubleProperty* graphrot = sg->getProperty<DoubleProperty>("viewRotation");
    pair<Coord, Coord> bboxe = tlp::computeBoundingBox(sg, graphlayout, graphsize, graphrot);
    Coord max = bboxe.first;
    Coord min = bboxe.second;
    Coord center = (max + min) / 2.0;
    graphlayout->setNodeValue(n, center);
    Coord nodeSize = (max - min);
    size->setNodeValue(n, Size(nodeSize[0], nodeSize[1], nodeSize[2]));
  } delete itN;

  if (dataSet!=0) {
    dataSet->set("quotientGraph",quotientGraph);
  }
  return true;

}
//================================================================================
bool QuotientClustering::check(string &erreurMsg) {
  erreurMsg="";
  return true;
}
//================================================================================
void QuotientClustering::reset(){}
//================================================================================




