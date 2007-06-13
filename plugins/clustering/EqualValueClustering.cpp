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

ALGORITHMPLUGIN(EqualValueClustering,"Equal Value","David Auber","13/06/2001","Alpha","1.0");

namespace {
  const char * paramHelp[] = {
    // selectedNodes
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "PropertyInterface*" ) \
    HTML_HELP_BODY() \
    "Specify the property that will be used to partition the graph" \
    HTML_HELP_CLOSE(),
    HTML_HELP_OPEN()				 \
    HTML_HELP_DEF( "type", "String Collection" ) \
    HTML_HELP_DEF("Values", "nodes <BR> edges") \
    HTML_HELP_DEF( "default", "nodes" )	 \
    HTML_HELP_BODY() \
    "This parameter enables to choose the type of graph elements to partition"	\
    HTML_HELP_CLOSE()
  };
}
#define ELT_TYPE "Type"
#define ELT_TYPES "nodes;edges;"
#define NODE_ELT 0
#define EDGE_ELT 1
//================================================================================
EqualValueClustering::EqualValueClustering(AlgorithmContext context):Algorithm(context) {
  addParameter<PropertyInterface*>("Property", paramHelp[0], "viewMetric");
  addParameter<StringCollection>(ELT_TYPE, paramHelp[1], ELT_TYPES);
  addDependency<LayoutAlgorithm>("Circular", "1.0");
  addDependency<LayoutAlgorithm>("GEM (Frick)", "1.0");
  addDependency<SizeAlgorithm>("Auto Sizing", "1.0");
}
//================================================================================
namespace stdext {
  template<>
  struct hash<double> {
    size_t operator()(const double s) const { return (size_t) s; }
  };
};
//==============================================================================
/*
  void drawGraph(Graph *tmpg) {
  //  cerr << __PRETTY_FUNCTION__ << endl;
  string errMsg;
  string layoutName;
  if (tmpg->numberOfNodes() > 300)
    layoutName = "Circular";
  else
    layoutName = "GEM (Frick)";
  string sizesName="Auto Sizing";
  tmpg->computeProperty(layoutName,tmpg->getLocalProperty<LayoutProperty>("viewLayout"),errMsg);
  if (tmpg->numberOfNodes() < 300)
    tmpg->computeProperty(sizesName,tmpg->getLocalProperty<SizeProperty>("viewSize"),errMsg);
  }
*/
//===============================================================================
bool EqualValueClustering::run() {
  string tmp1,tmp2;
  PropertyInterface *property=0;
  StringCollection eltTypes(ELT_TYPES);
  eltTypes.setCurrent(0);
  if (dataSet!=0) {
    dataSet->get("Property", property);  
    dataSet->get(ELT_TYPE, eltTypes);
  }

  if (property == 0)
    property = graph->getProperty("viewMetric");

  bool onNodes = eltTypes.getCurrent() == NODE_ELT;

  StableIterator<node> itN(graph->getNodes());
  StableIterator<edge> itE(graph->getEdges());
  int step = 0, maxSteps = graph->numberOfNodes();
  if (maxSteps < 100)
    maxSteps = 100;
  if (pluginProgress)
    pluginProgress->setComment(onNodes ? "Partitioning nodes..." : "Partitioning edges");

  // try to work with double value if it's a DoubleProperty
  if (typeid(*property) == typeid(DoubleProperty)) {
    stdext::hash_map<double, Graph *> partitions;
    DoubleProperty *metric = (DoubleProperty *) property;
    if (onNodes) {
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
      while (itE.hasNext()) {
	Graph *sg;
	edge e = itE.next();
	double tmp = metric->getEdgeValue(e);
	if (partitions.find(tmp) == partitions.end()) {
	  sg = graph->addSubGraph();
	  stringstream sstr;
	  sstr << "value = " << tmp;
	  sg->setAttribute("name", sstr.str());
	  partitions[tmp]=sg;
	} else
	  sg = partitions[tmp];
	sg->addNode(graph->source(e));
	sg->addNode(graph->target(e));
	sg->addEdge(e);
	if ((++step % (maxSteps/100)) == 0) {
	  pluginProgress->progress(step, maxSteps);
	  if (pluginProgress->state() !=TLP_CONTINUE)
	    return pluginProgress->state()!= TLP_CANCEL;
	}
      }
    }

    // do subgraphs layout
    /*
    stdext::hash_map<double, Graph *>::const_iterator it;
    for (it = partitions.begin(); it != partitions.end(); ++it) {
      drawGraph((*it).second);
    }
    */
  } else {
    stdext::hash_map<string, Graph *> partitions;
    if (onNodes) {
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
    } else {
      while (itE.hasNext()) {
	Graph *sg;
	edge e = itE.next();
	string tmp = property->getEdgeStringValue(e);
	if (partitions.find(tmp) == partitions.end()) {
	  sg = graph->addSubGraph();
	  sg->setAttribute("name", string("value = ") + tmp);
	  partitions[tmp]=sg;
	} else
	  sg = partitions[tmp];
	sg->addNode(graph->source(e));
	sg->addNode(graph->target(e));
	sg->addEdge(e);
	if ((++step % (maxSteps/100)) == 0) {
	  pluginProgress->progress(step, maxSteps);
	  if (pluginProgress->state() !=TLP_CONTINUE)
	    return pluginProgress->state()!= TLP_CANCEL;
	}
      }
    }
    // do subgraphs layout
    /*
    stdext::hash_map<string, Graph *>::const_iterator it;
    for (it = partitions.begin(); it != partitions.end(); ++it) {
      drawGraph((*it).second);
    }
    */
  }
  return true;
}
//================================================================================
