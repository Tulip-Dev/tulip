#include <stdio.h>
#include <math.h>
#include <sstream>

#include <tulip/Graph.h>
#include <tulip/hash_string.h>
#include <tulip/BooleanProperty.h>
#include <tulip/StableIterator.h>

#include "EqualValueClustering.h"

//================================================================================
using namespace std;
using namespace tlp;

ALGORITHMPLUGIN(EqualValueClustering,"Equal Value","David Auber","13/06/2001","Alpha","0","1");

namespace {
  const char * paramHelp[] = {
    // selectedNodes
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "PropertyInterface*" ) \
    HTML_HELP_BODY() \
    "Specify the property that will be used to partition the graph" \
    HTML_HELP_CLOSE(),
  };
}
//================================================================================
EqualValueClustering::EqualValueClustering(AlgorithmContext context):Algorithm(context) {
  addParameter<PropertyInterface*>("Property", paramHelp[0], "viewMetric");
}
//================================================================================
bool EqualValueClustering::run() {
  string tmp1,tmp2;
  PropertyInterface *metric=0;
  if (dataSet!=0) 
    dataSet->get("Property", metric);  
  if (metric == 0)
    metric = graph->getProperty("viewMetric");
  
  stdext::hash_map<string, Graph *> partitions;
  StableIterator<node> itN(graph->getNodes());
  int i = 0, step = 0, maxSteps = graph->numberOfNodes();
  pluginProgress->setComment("Partitioning nodes...");
  while (itN.hasNext()) {
    Graph *sg;
    node n = itN.next();
    string tmp=metric->getNodeStringValue(n);
    if (partitions.find(tmp)==partitions.end()) {
      char str[100];
      sprintf(str, "c_%06i", i++);
      sg = graph->addSubGraph();
      sg->setAttribute("name",string(str));
      partitions[tmp]=sg;
    } else
      sg = partitions[tmp];
    sg->addNode(n);
    step++;
    pluginProgress->progress(step, maxSteps);
    if (pluginProgress->state() !=TLP_CONTINUE)
      return pluginProgress->state()!= TLP_CANCEL;
  }

  StableIterator<edge> itE(graph->getEdges());
  step = 0;
  maxSteps = graph->numberOfEdges();
  pluginProgress->setComment("Partitioning edges...");
  while(itE.hasNext()) {
    edge ite = itE.next();
    string tmp = metric->getNodeStringValue(graph->source(ite));
    if (tmp == metric->getNodeStringValue(graph->target(ite))) {
      partitions[tmp]->addEdge(ite);
    }
    step++;
    pluginProgress->progress(step, maxSteps);
    if (pluginProgress->state() !=TLP_CONTINUE)
      return pluginProgress->state()!= TLP_CANCEL;    
  }

  return true;
}
//================================================================================
