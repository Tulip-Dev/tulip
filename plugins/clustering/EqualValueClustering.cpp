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
namespace stdext {
  template<>
  struct hash<double> {
    size_t operator()(const double s) const { return (size_t) s; }
  };
};
//===============================================================================
bool EqualValueClustering::run() {
  string tmp1,tmp2;
  PropertyInterface *property=0;
  if (dataSet!=0) 
    dataSet->get("Property", property);  
  if (property == 0)
    property = graph->getProperty("viewMetric");

  StableIterator<node> itN(graph->getNodes());
  StableIterator<edge> itE(graph->getEdges());
  int step = 0, maxSteps = graph->numberOfNodes();
  if (maxSteps < 100)
    maxSteps = 100;
  if (pluginProgress)
    pluginProgress->setComment("Partitioning nodes...");

  // try to work with double value if it's a DoubleProperty
  if (typeid(*property) == typeid(DoubleProperty)) {
    stdext::hash_map<double, Graph *> partitions;
    DoubleProperty *metric = (DoubleProperty *) property;
    while (itN.hasNext()) {
      Graph *sg;
      node n = itN.next();
      double tmp = metric->getNodeValue(n);
      if (partitions.find(tmp) == partitions.end()) {
	sg = graph->addSubGraph();
	stringstream sstr;
	sstr << "value = " << tmp;
	sg->setAttribute("name", sstr.str());
	partitions[tmp]=sg;
      } else
	sg = partitions[tmp];
      sg->addNode(n);
      if ((++step % (maxSteps/100)) == 0) {
	pluginProgress->progress(step, maxSteps);
	if (pluginProgress->state() !=TLP_CONTINUE)
	  return pluginProgress->state()!= TLP_CANCEL;
      }
    }

    step = 0;
    maxSteps = graph->numberOfEdges();
    if (maxSteps < 100)
      maxSteps = 100;
    pluginProgress->setComment("Partitioning edges...");
    while(itE.hasNext()) {
      edge ite = itE.next();
      double tmp = metric->getNodeValue(graph->source(ite));
      if (tmp == metric->getNodeValue(graph->target(ite))) {
	partitions[tmp]->addEdge(ite);
      }
      if ((++step % (maxSteps/100)) == 0) {
	pluginProgress->progress(step, maxSteps);
	if (pluginProgress->state() !=TLP_CONTINUE)
	  return pluginProgress->state()!= TLP_CANCEL;
      }
    }    
  } else {
    stdext::hash_map<string, Graph *> partitions;
    while (itN.hasNext()) {
      Graph *sg;
      node n = itN.next();
      string tmp=property->getNodeStringValue(n);
      if (partitions.find(tmp)==partitions.end()) {
	sg = graph->addSubGraph();
	sg->setAttribute("name", string("value = ") + tmp);
	partitions[tmp]=sg;
      } else
	sg = partitions[tmp];
      sg->addNode(n);
      if ((++step % (maxSteps/100)) == 0) {
	pluginProgress->progress(step, maxSteps);
	if (pluginProgress->state() !=TLP_CONTINUE)
	  return pluginProgress->state()!= TLP_CANCEL;
      }
    }

    step = 0;
    maxSteps = graph->numberOfEdges();
    if (maxSteps < 100)
      maxSteps = 100;
    pluginProgress->setComment("Partitioning edges...");
    while(itE.hasNext()) {
      edge ite = itE.next();
      string tmp = property->getNodeStringValue(graph->source(ite));
      if (tmp == property->getNodeStringValue(graph->target(ite))) {
	partitions[tmp]->addEdge(ite);
      }
      if ((++step % (maxSteps/100)) == 0) {
	pluginProgress->progress(step, maxSteps);
	if (pluginProgress->state() !=TLP_CONTINUE)
	  return pluginProgress->state()!= TLP_CANCEL;
      }
    }
  }
  return true;
}
//================================================================================
