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

ALGORITHMPLUGIN(QuotientClustering,"Quotient Clustering","David Auber","13/06/2001","Alpha","1.2");

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
    // recursive
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "bool" ) \
    HTML_HELP_DEF( "values", "[true, false]" ) \
    HTML_HELP_DEF( "default", "false" ) \
    HTML_HELP_BODY() \
    "This parameter indicates whether the algorithm has to be applied along the entire hierarchy of subgraphs." \
    HTML_HELP_CLOSE(),
    // node aggregation function
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "String Collection" ) \
    HTML_HELP_DEF( "default", "none" )	 \
    HTML_HELP_BODY() \
    "This parameter indicates the function used to compute a measure value for a meta-node using the values of its underlying nodes. If 'none' is choosen no value will be computed" \
    HTML_HELP_CLOSE(),
    // edge aggregation function
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "String Collection" ) \
    HTML_HELP_DEF( "default", "none" )	 \
    HTML_HELP_BODY() \
    "This parameter indicates the function used to compute a measure value for a meta-edge using the values of its underlying edges. If 'none' is choosen no value will be computed." \
    HTML_HELP_CLOSE(),
    // meta-node label
    HTML_HELP_OPEN()							\
    HTML_HELP_DEF( "type", "StringProperty" )				\
    HTML_HELP_DEF( "value", "An existing string property" )		\
    HTML_HELP_BODY()							\
    "This parameter defines the property used to compute the label of the meta-nodes. An arbitrary underlying node is choosen and its associated value for the given property becomes the meta-node label."\
    HTML_HELP_CLOSE(),
    // use name of subgraphs
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "bool" ) \
    HTML_HELP_DEF( "values", "[true, false]" ) \
    HTML_HELP_DEF( "default", "false" ) \
    HTML_HELP_BODY() \
    "This parameter indicates whether the meta-node label has to be the same as the name of the subgraph it represents." \
    HTML_HELP_CLOSE(),
    // edge cardinality
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "bool" ) \
    HTML_HELP_DEF( "values", "[true, false]" ) \
    HTML_HELP_DEF( "default", "false" ) \
    HTML_HELP_BODY() \
    "This parameter indicates whether the cardinality of the underlying edges of the meta-edges has to be computed or not. If yes, the property edgeCardinality will be created for the quotient graph." \
    HTML_HELP_CLOSE()
  };
}
#define AGGREGATION_FUNCTIONS "none;average;sum;max;min"
#define NO_FN 0
#define AVG_FN 1
#define SUM_FN 2
#define MAX_FN 3
#define MIN_FN 4
//================================================================================
QuotientClustering::QuotientClustering(AlgorithmContext context):Algorithm(context) {
  addParameter<bool>("oriented", paramHelp[0], "true");
  addParameter<StringCollection>("node function", paramHelp[2], AGGREGATION_FUNCTIONS);
  addParameter<StringCollection>("edge function", paramHelp[3], AGGREGATION_FUNCTIONS);
  addParameter<StringProperty>("meta-node label", paramHelp[4], 0, false);
  addParameter<bool>("use name of subgraph", paramHelp[5], "false");
  addParameter<bool>("recursive", paramHelp[1], "false");
  addParameter<bool>("edge cardinality", paramHelp[6], "false");
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
      ++nbEdges;
      if (edgeFn != NO_FN) {
	double eVal = metric->getEdgeValue(e);
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
  }
  if (edgeFn != NO_FN) {
    if (edgeFn == AVG_FN)
      value /= nbEdges;
    metric->setEdgeValue(mE, value);
  }
  if (cardProp)
    cardProp->setEdgeValue(mE, nbEdges);
}
//===============================================================================
bool QuotientClustering::run() {
  bool oriented = true, edgeCardinality = true;
  bool recursive = false, useSubGraphName = false;
  StringProperty *metaLabel = NULL;
  StringCollection nodeFunctions(AGGREGATION_FUNCTIONS);
  nodeFunctions.setCurrent(0);
  StringCollection edgeFunctions(AGGREGATION_FUNCTIONS);
  edgeFunctions.setCurrent(0);
  if (dataSet!=0) {
    dataSet->get("oriented", oriented);
    dataSet->get("node function", nodeFunctions);
    dataSet->get("edge function", edgeFunctions);
    dataSet->get("edge cardinality", edgeCardinality);
    dataSet->get("recursive", recursive);
    dataSet->get("meta-node label", metaLabel);
    dataSet->get("use name of subgraph", useSubGraphName);
  }

  Iterator<Graph *> *itS= graph->getSubGraphs();
  // do nothing if there is no subgraph
  if (!itS->hasNext())
    return true;
  delete itS;

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

  StringProperty *label = NULL;
  if (useSubGraphName || metaLabel)
    label = quotientGraph->getProperty<StringProperty>("viewLabel");
  if (!oriented) {
    opProp = quotientGraph->getLocalProperty<IntegerProperty>("opposite edge");
    opProp->setAllEdgeValue(edge().id);
  }
  if (edgeCardinality)
    cardProp = quotientGraph->getLocalProperty<IntegerProperty>("edgeCardinality");

  // populate quotient graph
  {
    map<node, set<node> > mapping;
    itS=graph->getSubGraphs();
    while (itS->hasNext()) {
      Graph *its=itS->next();
      if (its!=quotientGraph) {
	// Create one metanode for each subgraph(cluster) of current graph.
	node metaN = quotientGraph->addNode();
	// set meta node properties
	// according to parameters
	quotientGraph->getProperty<ColorProperty>("viewColor")->setNodeValue(metaN, Color(255,255,255,100));
	meta->setNodeValue(metaN, its);
	if (label) {
	  string mLabel;
	  if (useSubGraphName)
	    mLabel = its->getAttribute<string>("name");
	  else
	    mLabel = label->getNodeValue(its->getOneNode());
	  label->setNodeValue(metaN, mLabel);
	}
	node n;
	forEach(n, its->getNodes()) {
	  // map each subgraph's node to a set of meta nodes
	  // in order to deal consistently with overlapping clusters
	  if (mapping.find(n) == mapping.end())
	    mapping[n] = set<node>();
	  mapping[n].insert(metaN);
	}
      }
    } delete itS;

    {
      set<Edge> myQuotientGraph;
      edge e;
      // for each existing edge in the current graph
      // add a meta edge for each couple of meta source
      // and meta target if it does not already exists   
      stableForEach(e, graph->getEdges()) {
	set<node> metaSources = mapping[graph->source(e)];
	set<node> metaTargets = mapping[graph->target(e)];
	for(set<node>::const_iterator itms = metaSources.begin();
	    itms != metaSources.end(); ++itms) {
	  node mSource = *itms;
	  for(set<node>::const_iterator itmt = metaTargets.begin();
	      itmt != metaTargets.end(); ++itmt) {
	    node mTarget = *itmt;
	    if (mSource != mTarget) {
	      Edge tmp;
	      tmp.source = mSource.id, tmp.target = mTarget.id;
	      if (myQuotientGraph.find(tmp) == myQuotientGraph.end()) {
		myQuotientGraph.insert(tmp);
		edge mE = quotientGraph->addEdge(mSource, mTarget);
		edge op;
		if (!oriented &&
		    (op = quotientGraph->existEdge(mTarget, mSource)).isValid()) {
		  opProp->setEdgeValue(op, mE.id);
		  opProp->setEdgeValue(mE, op.id);
		}
	      }
	    }
	  }
	}
      }
    }
  }

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
      if (nodeFn != NO_FN) {
	Iterator<node> *itN = quotientGraph->getNodes();
	while (itN->hasNext()) {
	  node mN = itN->next();
	  computeMNodeMetric(meta->getNodeValue(mN), mN, metric, nodeFn);
	} delete itN;
      }
      if (edgeFn != NO_FN || edgeCardinality) {
	Iterator<edge>* itE = quotientGraph->getEdges();
	while (itE->hasNext()) {
	  edge mE = itE->next();
	  computeMEdgeMetric(graph, mE,
			     meta->getNodeValue(quotientGraph->source(mE)),
			     meta->getNodeValue(quotientGraph->target(mE)),
			     metric, edgeFn, cardProp);
	} delete itE;
      }
    }
  }
  // orientation
  if (!oriented) {
    set<edge> edgesToDel;
    DoubleProperty* viewMetric =
      quotientGraph->getProperty<DoubleProperty>("viewMetric");
    Iterator<edge>* itE = quotientGraph->getEdges();
    while (itE->hasNext()) {
      edge mE = itE->next();
      edge op(opProp->getEdgeValue(mE));
      if (op.isValid() &&
	  edgesToDel.find(mE) == edgesToDel.end() &&
	  edgesToDel.find(op) == edgesToDel.end()) {
	// if the opposite edge viewMetric associated value is greater
	// than the mE associated value than we will keep it instead of mE
	bool opOK =
	  viewMetric->getEdgeValue(mE) < viewMetric->getEdgeValue(op);
	if (edgeFn != NO_FN) {
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
	}
	// compute cardinaly if needed
	if (cardProp) {
	  unsigned int card =
	    cardProp->getEdgeValue(mE) + cardProp->getEdgeValue(op);
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
    updateGroupLayout(graph, quotientGraph, itN->next());
  } delete itN;

  if (dataSet!=0) {
    dataSet->set("quotientGraph", quotientGraph);
  }

  // recursive call if needed
  if (recursive) {
    DataSet dSet;
    dSet.set("oriented", oriented);
    dSet.set("node function", nodeFunctions);
    dSet.set("edge function", edgeFunctions);
    dSet.set("edge cardinality", edgeCardinality);
    dSet.set("recursive", recursive);
    dSet.set("meta-node label", metaLabel);
    dSet.set("use name of subgraph", useSubGraphName);
    StableIterator<Graph *> sitS(graph->getSubGraphs());
    while (sitS.hasNext()) {
      Graph* sg = sitS.next();
      if (sg!=quotientGraph) {
	string eMsg;
	tlp::applyAlgorithm(sg, eMsg, &dSet, "Quotient Clustering",
			    pluginProgress);
      }
    }
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
