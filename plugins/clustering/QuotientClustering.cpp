#include <set>
#include <stdio.h>
#include <math.h>
#include <sstream>
#include <tulip/Graph.h>
#include <tulip/DrawingTools.h>
#include "QuotientClustering.h"


using namespace std;
using namespace tlp;

ALGORITHMPLUGIN(QuotientClustering,"Quotient Clustering","David Auber","13/06/2001","Alpha","1.2");

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
    const double& nVal = metric->getNodeValue(n);
    switch(nodeFn) {
    case AVG_FN:
      ++nbNodes;
    case SUM_FN:
      value += nVal;
      break;
    case MAX_FN:
      if (nVal > value)
	value = nVal;
      break;
    case MIN_FN:
      if (nVal < value)
	value = nVal;
      break;
    }
  }
  if (nodeFn == AVG_FN)
    value /= nbNodes;
  metric->setNodeValue(mN, value);
}
//===============================================================================
static void computeMEdgeMetric(Graph* graph, edge mE, Iterator<edge>* itE,
			       DoubleProperty *metric, unsigned int edgeFn,
			       IntegerProperty *cardProp) {
  double value;
  if (edgeFn != NO_FN) {
    switch(edgeFn) {
    case AVG_FN:
    case SUM_FN:
      value = 0;
      break;
    case MAX_FN:
      value = DBL_MIN;
      break;
    case MIN_FN:
      value = DBL_MAX;
      break;
    }
  }
   
  unsigned int nbEdges = 0;
  while(itE->hasNext()) {
    edge e = itE->next();
    ++nbEdges;
    if (edgeFn != NO_FN) {
      const double& eVal = metric->getEdgeValue(e);
      switch(edgeFn) {
      case AVG_FN:
      case SUM_FN:
	value += eVal;
	break;
      case MAX_FN:
	if (eVal > value)
	  value = eVal;
	break;
      case MIN_FN:
	if (eVal < value)
	  value = eVal;
	break;
      }
    }
  } delete itE;

  if (edgeFn == AVG_FN)
    value /= nbEdges;
  if (edgeFn != NO_FN)
    metric->setEdgeValue(mE, value);
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
  if (!itS->hasNext()) {
    delete itS;
    return true;
  }
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
  StringProperty *label = NULL;
  if (useSubGraphName || metaLabel)
    label = quotientGraph->getProperty<StringProperty>("viewLabel");
  if (!oriented) {
    opProp = new IntegerProperty(quotientGraph);
    opProp->setAllEdgeValue(edge().id);
  }
  if (edgeCardinality)
    cardProp = quotientGraph->getLocalProperty<IntegerProperty>("edgeCardinality");

  // compute mete nodes
  itS = graph->getSubGraphs();
  vector<node> mNodes;
  graph->createMetaNodes(itS, quotientGraph, mNodes);
  delete itS;
  // update label property if needed
  if (label) {
    vector<node>::iterator itn = mNodes.begin();
    while(itn != mNodes.end()) {
      string mLabel;
      Graph* sg = quotientGraph->getNodeMetaInfo(*itn);
      if (useSubGraphName)
	mLabel = sg->getAttribute<string>("name");
      else
	mLabel = label->getNodeValue(sg->getOneNode());
      label->setNodeValue(*itn, mLabel);
      ++itn;
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
	  computeMNodeMetric(quotientGraph->getNodeMetaInfo(mN), mN,
			     metric, nodeFn);
	} delete itN;
      }
      if (edgeFn != NO_FN || edgeCardinality) {
	edge mE;
	forEach(mE, quotientGraph->getEdges()) {
	  computeMEdgeMetric(graph, mE, quotientGraph->getEdgeMetaInfo(mE),
			     metric, edgeFn, cardProp);
	}
      }
    }
  }
  // orientation
  if (!oriented) {
    // for each edge 
    // store opposite edge in opProp
    edge mE;
    forEach(mE, quotientGraph->getEdges()) {
      edge op = quotientGraph->existEdge(quotientGraph->target(mE),
					 quotientGraph->source(mE));
      if (op.isValid()) {
	opProp->setEdgeValue(op, mE.id);
	opProp->setEdgeValue(mE, op.id);
      }
    }
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

  if (opProp)
    delete opProp;
	    
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
