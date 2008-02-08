#include <set>
#include <stdio.h>
#include <math.h>
#include <sstream>
#include <tulip/Graph.h>
#include <tulip/GraphProperty.h>
#include <tulip/DrawingTools.h>
#include <tulip/ExtendedClusterOperation.h>

#include "QuotientClustering.h"


using namespace std;
using namespace tlp;

ALGORITHMPLUGIN(QuotientClustering,"Quotient Clustering","David Auber","13/06/2001","Alpha","1.0");

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
//==============================================================================
namespace {
  const char * paramHelp[] = {
    // oriented
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "bool" ) \
    HTML_HELP_DEF( "values", "[true, false]" ) \
    HTML_HELP_DEF( "default", "true" ) \
    HTML_HELP_BODY() \
    "This parameter indicates whether the graph has to be considered as oriented or not." \
    HTML_HELP_CLOSE(),
    // node aggregation function
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "String Collection" ) \
    HTML_HELP_DEF( "default", "average" )	 \
    HTML_HELP_BODY() \
    "This parameter enables to choose the function used to compute a measure value for a meta-node using the values of its underlying nodes." \
    HTML_HELP_CLOSE(),
    // edge aggregation function
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "String Collection" ) \
    HTML_HELP_DEF( "default", "average" )	 \
    HTML_HELP_BODY() \
    "This parameter enables to choose the function used to compute a measure value for a meta-edge using the values of its underlying edges." \
    HTML_HELP_CLOSE(),
    // edge cardinality
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "bool" ) \
    HTML_HELP_DEF( "values", "[true, false]" ) \
    HTML_HELP_DEF( "default", "true" ) \
    HTML_HELP_BODY() \
    "This parameter indicates whether the cardinality of the underlying edges of the meta-edges has to be computed or not. If yes, the property edgeCardinality will be created for the quotient graph." \
    HTML_HELP_CLOSE()
  };
}
#define AGGREGATION_FUNCTIONS "average;sum;max;min"
#define AVG_FN 0
#define SUM_FN 1
#define MAX_FN 2
#define MIN_FN 3
//================================================================================
QuotientClustering::QuotientClustering(AlgorithmContext context):Algorithm(context) {
  addParameter<bool>("oriented", paramHelp[0], "true");
  addParameter<StringCollection>("node function", paramHelp[1], AGGREGATION_FUNCTIONS);
  addParameter<StringCollection>("edge function", paramHelp[2], AGGREGATION_FUNCTIONS);
  addParameter<bool>("edge cardinality", paramHelp[3], "false");
}
//================================================================================
QuotientClustering::~QuotientClustering(){}
//===============================================================================
static void computeMNodeMetric(Graph *mGraph, node mN, DoubleProperty *metric,
			       unsigned int nodeFn) {
  double value;
  switch(nodeFn) {
  case AVG_FN:
  case SUM_FN: {
    value = 0;
    break;
  }
  case MAX_FN: {
    value = DBL_MIN;
    break;
  }
  case MIN_FN:
    value = DBL_MAX;
    break;
  }

  unsigned int nbNodes = 0;
  node n;
  forEach(n, mGraph->getNodes()) {
    double nVal = metric->getNodeValue(n);
    switch(nodeFn) {
    case AVG_FN:
      ++nbNodes;
    case SUM_FN: {
      value += nVal;
      break;
    }
    case MAX_FN: {
      if (nVal > value)
	value = nVal;
      break;
    }
    case MIN_FN: {
      if (nVal < value)
	value = nVal;
      break;
    }
    }
  }
  if (nodeFn == AVG_FN)
    value /= nbNodes;
  metric->setNodeValue(mN, value);
}
//===============================================================================
static void computeMEdgeMetric(Graph* graph, edge mE, Graph *sGraph, Graph *tGraph,
			       DoubleProperty *metric, unsigned int edgeFn,
			       IntegerProperty *cardProp) {
  double value;
  switch(edgeFn) {
  case AVG_FN:
  case SUM_FN: {
    value = 0;
    break;
  }
  case MAX_FN: {
    value = DBL_MIN;
    break;
  }
  case MIN_FN:
    value = DBL_MAX;
    break;
  }

  edge e;
  unsigned int nbEdges = 0;
  forEach(e, graph->getEdges()) {
    if (sGraph->isElement(graph->source(e)) && tGraph->isElement(graph->target(e))) {
      double eVal = metric->getEdgeValue(e);
      ++nbEdges;
      switch(edgeFn) {
      case AVG_FN:
      case SUM_FN: {
	value += eVal;
	break;
      }
      case MAX_FN: {
	if (eVal > value)
	  value = eVal;
	break;
      }
      case MIN_FN: {
	if (eVal < value)
	  value = eVal;
	break;
      }
      }
    }
  }
  if (edgeFn == AVG_FN)
    value /= nbEdges;
  metric->setEdgeValue(mE, value);
  if (cardProp)
    cardProp->setEdgeValue(mE, nbEdges);
}
//===============================================================================
bool QuotientClustering::run() {
  bool oriented = true, edgeCardinality = true;
  StringCollection nodeFunctions(AGGREGATION_FUNCTIONS);
  nodeFunctions.setCurrent(0);
  StringCollection edgeFunctions(AGGREGATION_FUNCTIONS);
  edgeFunctions.setCurrent(0);
  if (dataSet!=0) {
    dataSet->get("oriented", oriented);
    dataSet->get("node function", nodeFunctions);
    dataSet->get("edge function", edgeFunctions);
    dataSet->get("edge cardinality", edgeCardinality);
  }

  IntegerProperty *opProp = 0, *cardProp = 0;
  Graph *quotientGraph = tlp::newSubGraph(graph->getRoot());
  stringstream sstr;
  sstr << "quotient of ";
  string graphName = graph->getAttribute<string>("name");
  if (graphName.size() == 0)
    sstr << graph->getId();
  else {
    sstr << graphName;
    if (graphName == "unnamed")
      sstr << " " << graph->getId();
  }
  quotientGraph->setAttribute(string("name"), sstr.str());
  GraphProperty *meta = quotientGraph->getProperty<GraphProperty>("viewMetaGraph");
  if (!oriented) {
    opProp = quotientGraph->getLocalProperty<IntegerProperty>("opposite edge");
    opProp->setAllEdgeValue(edge().id);
  }
  if (edgeCardinality)
    cardProp = quotientGraph->getLocalProperty<IntegerProperty>("edgeCardinality");
  //Create one metanode for each subgraph(cluster) of current graph.
  Graph *sg= graph;
  map<Graph*,node> mapping;
  Iterator<Graph *> *itS=sg->getSubGraphs();
  while (itS->hasNext()) {
    Graph *its=itS->next();
    if (its!=quotientGraph) {
      node n = quotientGraph->addNode();
      quotientGraph->getProperty<ColorProperty>("viewColor")->setNodeValue(n,Color(255,255,255,100));
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
	  edge e = quotientGraph->addEdge(mapping[*it1],mapping[*it2]);
	  edge op;
	  if (!oriented &&
	      (op = quotientGraph->existEdge(node(tmp.target),
					     node(tmp.source))).isValid()) {
	    opProp->setEdgeValue(op, e.id);
	    opProp->setEdgeValue(e, op.id);
	  }
	}
      }
  } delete itE;

  // compute metrics
  string pName;
  unsigned int nodeFn = nodeFunctions.getCurrent();
  unsigned int edgeFn = edgeFunctions.getCurrent();
  forEach(pName, graph->getProperties()) {
    PropertyInterface *property = graph->getProperty(pName);
    if (dynamic_cast<DoubleProperty *>(property) &&
	// try to avoid view... properties
	(pName.find("view") != 0 || pName == "viewMetric")) {
      DoubleProperty *metric = graph->getProperty<DoubleProperty>(pName);
      Iterator<node> *itN = quotientGraph->getNodes();
      while (itN->hasNext()) {
	node mN = itN->next();
	computeMNodeMetric(meta->getNodeValue(mN), mN, metric, nodeFn);
      } delete itN;
      itE = quotientGraph->getEdges();
      while (itE->hasNext()) {
	edge mE = itE->next();
	computeMEdgeMetric(graph, mE, meta->getNodeValue(quotientGraph->source(mE)),
			   meta->getNodeValue(quotientGraph->target(mE)), metric,
			   edgeFn, cardProp);
      } delete itE;
    }
  }
  // orientation
  if (!oriented) {
    set<edge> edgesToDel;
    DoubleProperty* viewMetric = quotientGraph->getProperty<DoubleProperty>("viewMetric");
    itE = quotientGraph->getEdges();
    while (itE->hasNext()) {
      edge mE = itE->next();
      edge op(opProp->getEdgeValue(mE));
      if (op.isValid() &&
	  edgesToDel.find(mE) == edgesToDel.end() &&
	  edgesToDel.find(op) == edgesToDel.end()) {
	// if the opposite edge viewMetric associated value is greater
	// than the mE associated value than we will keep it instead of mE
	bool opOK = viewMetric->getEdgeValue(mE) < viewMetric->getEdgeValue(op);
	forEach(pName, graph->getProperties()) {
	  PropertyInterface *property = graph->getProperty(pName);
	  if (dynamic_cast<DoubleProperty *>(property) &&
	      // try to avoid view... properties
	      (pName.find("view") != 0 || pName == "viewMetric")) {
	    DoubleProperty *metric = graph->getProperty<DoubleProperty>(pName);
	    double value = metric->getEdgeValue(mE);	    
	    switch(edgeFn) {
	    case AVG_FN:
	      value = (value + metric->getEdgeValue(op))/2;
	      break;
	    case SUM_FN:
	      value += metric->getEdgeValue(op);
	      break;
	    case MAX_FN:
	      if (value < metric->getEdgeValue(op))
		value = metric->getEdgeValue(op);
	      break;
	    case MIN_FN:
	      if (value > metric->getEdgeValue(op))
		value = metric->getEdgeValue(op);
	      break;
	    }
	    if (opOK)
	      metric->setEdgeValue(op, value);
	    else
	      metric->setEdgeValue(mE, value);
	  }
	}
	// compute cardinaly if needed
	if (cardProp) {
	  unsigned int card = cardProp->getEdgeValue(mE) + cardProp->getEdgeValue(op);
	  if (opOK)
	    cardProp->setEdgeValue(op, card);
	  else
	    cardProp->setEdgeValue(mE, card);
	}
	edgesToDel.insert(opOK ? mE: op);
      }
    } delete itE;
    set<edge>::const_iterator it;
    for ( it = edgesToDel.begin(); it!=edgesToDel.end(); ++it)
      quotientGraph->delEdge(*it);
  }
  quotientGraph->delLocalProperty("opposite edge");
	    
  //compute layout according to the layouts of subgraphs
  //SizeProperty *size  = quotientGraph->getProperty<SizeProperty>("viewSize");
  Iterator<node> *itN = quotientGraph->getNodes();
  while (itN->hasNext()) {
    updateGroupLayout(sg, quotientGraph, itN->next());
  } delete itN;

  if (dataSet!=0) {
    dataSet->set("quotientGraph", quotientGraph);
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
