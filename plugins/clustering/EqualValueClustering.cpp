#include <stdio.h>
#include <math.h>
#include <sstream>

#include <tulip/SuperGraph.h>
#include <tulip/Selection.h>
#include <tulip/StableIterator.h>

#include "EqualValueClustering.h"

CLUSTERINGPLUGIN(EqualValueClustering,"Equal Value","David Auber","13/06/2001","Alpha","0","1");

//================================================================================
using namespace std;
namespace {
  const char * paramHelp[] = {
    // selectedNodes
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "Metric" ) \
    HTML_HELP_BODY() \
    "Define the metric that will be used in order partition to the graph" \
    HTML_HELP_CLOSE(),
  };
}
//================================================================================
EqualValueClustering::EqualValueClustering(ClusterContext context):Clustering(context) {
  addParameter<Metric>("Metric", paramHelp[0], "viewMetric");
}
//================================================================================
namespace stdext {
  template<>
  struct hash<double> {
    size_t operator()(const double s) const { return (size_t)s; }
  };
};
//===============================================================================
bool EqualValueClustering::run() {
  string tmp1,tmp2;
  Metric *metric=0;
  if (dataSet!=0) 
    dataSet->get("Metric", metric);  
  if (metric == 0)
    metric = superGraph->getProperty<Metric>("viewMetric");
  
  stdext::hash_map<double,int> partitions;
  int curPart=0;
  Iterator<node> *itN=superGraph->getNodes();
  while (itN->hasNext()) {
    double tmp=metric->getNodeValue(itN->next());
    if (partitions.find(tmp)==partitions.end()) {
      partitions[tmp]=curPart;
      curPart++;
    }
  } delete itN;

  stdext::hash_map <int, SuperGraph *> newClusters;
  char str[100];
  for (int i=0; i<curPart; ++i) {
    sprintf(str, "c_%06i", i);
    //    cerr << "create :" << str << endl;
    newClusters[i] = superGraph->addSubGraph();
    newClusters[i]->setAttribute("name",string(str));
  }
  
  StableIterator<node> itNS(superGraph->getNodes());
  while (itNS.hasNext()) {
    node itn = itNS.next();
    double tmp = metric->getNodeValue(itn);
    newClusters[partitions[tmp]]->addNode(itn);
  }

  StableIterator<edge> itE(superGraph->getEdges());
  while(itE.hasNext()) {
    edge ite = itE.next();
    double tmp = metric->getNodeValue(superGraph->source(ite));
    if (tmp == metric->getNodeValue(superGraph->target(ite))) {
      newClusters[partitions[tmp]]->addEdge(ite);
    }
  }

  return true;
}
//================================================================================
