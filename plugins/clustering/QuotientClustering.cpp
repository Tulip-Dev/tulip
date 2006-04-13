#include <set>
#include <stdio.h>
#include <math.h>
#include <sstream>
#include <tulip/SuperGraph.h>
#include <tulip/MetaGraph.h>
#include <tulip/TlpTools.h>
#include <tulip/DrawingTools.h>

#include "QuotientClustering.h"


CLUSTERINGPLUGIN(QuotientClustering,"QuotientClustering","David Auber","13/06/2001","Alpha","0","1");

using namespace std;

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
QuotientClustering::QuotientClustering(ClusterContext context):Clustering(context) {}
//================================================================================
QuotientClustering::~QuotientClustering(){}
//===============================================================================
bool QuotientClustering::run() {
  SuperGraph *quotientGraph = tlp::newSubGraph(superGraph->getRoot());
  MetaGraph *meta = quotientGraph->getProperty<MetaGraph>("viewMetaGraph");
  //Create one metanode for each subgraph(cluster) of current graph.
  SuperGraph *graph= superGraph;
  map<SuperGraph*,node> mapping;
  Iterator<SuperGraph *> *itS=graph->getSubGraphs();
  while (itS->hasNext()) {
    SuperGraph *its=itS->next();
    if (its!=quotientGraph) {
      node n = quotientGraph->addNode();
      meta->setNodeValue(n,its);
      mapping[its]=n;
    }
  } delete itS;
  //
  set<Edge> myQuotientGraph;
  Iterator<edge>*itE=superGraph->getEdges();
  while (itE->hasNext()) {
    edge ite=itE->next();
    node source=superGraph->source(ite);
    node target=superGraph->target(ite);
    list<SuperGraph *> clusterSource;
    list<SuperGraph *> clusterTarget;
    clusterSource.clear();
    clusterTarget.clear();
    itS=graph->getSubGraphs();
    while (itS->hasNext()) {
      SuperGraph *its=itS->next();
      if (its!=quotientGraph) {
	SuperGraph *tmp=its;
	if (tmp->isElement(source)) clusterSource.push_back(its);
	if (tmp->isElement(target)) clusterTarget.push_back(its);
      }
    } delete itS;
    
    for (std::list<SuperGraph *>::iterator it1=clusterSource.begin(); it1!=clusterSource.end(); ++it1)
      for (std::list<SuperGraph *>::iterator it2=clusterTarget.begin(); it2!=clusterTarget.end(); ++it2) {
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
  Sizes *size  = quotientGraph->getProperty<Sizes>("viewSize");
  Iterator<node> *itN = quotientGraph->getNodes();
  while (itN->hasNext()) {
    node n = itN->next();
    SuperGraph * graph = meta->getNodeValue(n);
    Layout * graphlayout = graph->getProperty<Layout>("viewLayout");
    Sizes * graphsize = graph->getProperty<Sizes>("viewSize");
    Metric * graphrot = graph->getProperty<Metric>("viewRotation");
    pair<Coord, Coord> bboxe = tlp::computeBoundingBox(graph, graphlayout, graphsize, graphrot);
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




