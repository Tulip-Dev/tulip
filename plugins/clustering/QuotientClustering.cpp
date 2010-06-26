/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */
#include <set>
#include <stdio.h>
#include <math.h>
#include <sstream>
#include <tulip/Graph.h>
#include <tulip/DrawingTools.h>
#include "QuotientClustering.h"


using namespace std;
using namespace tlp;

ALGORITHMPLUGIN(QuotientClustering,"Quotient Clustering","David Auber","13/06/2001","OK","1.3");

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
    HTML_HELP_CLOSE(),
    // layout quotient graph
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "bool" ) \
    HTML_HELP_DEF( "values", "[true, false]" ) \
    HTML_HELP_DEF( "default", "false" ) \
    HTML_HELP_BODY() \
    "This parameter indicates whether the layout of the quotient graph(s) has to be computed or not." \
    HTML_HELP_CLOSE(),
  };
}
#define AGGREGATION_FUNCTIONS "none;average;sum;max;min"
//================================================================================
QuotientClustering::QuotientClustering(AlgorithmContext context):Algorithm(context) {
  addDependency<LayoutAlgorithm>("Circular", "1.0");
  addDependency<LayoutAlgorithm>("GEM (Frick)", "1.0");
  addDependency<SizeAlgorithm>("Auto Sizing", "1.0");
  addParameter<bool>("oriented", paramHelp[0], "true");
  addParameter<StringCollection>("node function", paramHelp[2], AGGREGATION_FUNCTIONS);
  addParameter<StringCollection>("edge function", paramHelp[3], AGGREGATION_FUNCTIONS);
  addParameter<StringProperty>("meta-node label", paramHelp[4], 0, false);
  addParameter<bool>("use name of subgraph", paramHelp[5], "false");
  addParameter<bool>("recursive", paramHelp[1], "false");
  addParameter<bool>("layout quotient graph(s)", paramHelp[7], "false");
  addParameter<bool>("edge cardinality", paramHelp[6], "false");
}
//================================================================================
QuotientClustering::~QuotientClustering(){}
//===============================================================================
// define a specific calculator for viewLabel
class QuotientLabelCalculator :public AbstractStringProperty::MetaValueCalculator {
  StringProperty* sgLabel;
  bool useSubGraphName;
public:
  QuotientLabelCalculator(StringProperty* label, bool useSgName)
  : AbstractStringProperty::MetaValueCalculator(),
    sgLabel(label), useSubGraphName(useSgName) {}
  
  void computeMetaValue(AbstractStringProperty* label, node mN, Graph* sg,
			Graph*) {
    if (sgLabel)
      label->setNodeValue(mN, label->getNodeValue(sg->getOneNode()));
    else if (useSubGraphName)
      label->setNodeValue(mN, sg->getAttribute<string>("name"));
  }
};      

// define a specific calculator for edgeCardinality
class EdgeCardinalityCalculator :public AbstractIntegerProperty::MetaValueCalculator {
public:
  void computeMetaValue(AbstractIntegerProperty* card, edge mE,
			Iterator<edge>* itE, Graph* g) {
    unsigned int nbEdges = 0;
    while(itE->hasNext()) {
      itE->next();
      ++nbEdges;
    }
    card->setEdgeValue(mE, nbEdges);
  }
};      
      
//===============================================================================
bool QuotientClustering::run() {
  bool oriented = true, edgeCardinality = true;
  bool recursive = false, quotientLayout = true, useSubGraphName = false;
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
    dataSet->get("layout quotient graph(s)", quotientLayout);
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
  if (!oriented) {
    opProp = new IntegerProperty(quotientGraph);
    opProp->setAllEdgeValue(edge().id);
  }

  EdgeCardinalityCalculator cardCalc;
  if (edgeCardinality) {
    cardProp = quotientGraph->getLocalProperty<IntegerProperty>("edgeCardinality");
    cardProp->setMetaValueCalculator(&cardCalc);
  }

  // set specific meta value calculators
  // for most properties
  DoubleProperty::PredefinedMetaValueCalculator nodeFn =
    (DoubleProperty::PredefinedMetaValueCalculator ) nodeFunctions.getCurrent();
  DoubleProperty::PredefinedMetaValueCalculator edgeFn =
    (DoubleProperty::PredefinedMetaValueCalculator) edgeFunctions.getCurrent();
  QuotientLabelCalculator viewLabelCalc(metaLabel, useSubGraphName);
  TLP_HASH_MAP<unsigned long, PropertyInterface::MetaValueCalculator *> prevCalcs;
  string pName;
  forEach(pName, quotientGraph->getProperties()) {
    PropertyInterface *prop = quotientGraph->getProperty(pName);
    if (dynamic_cast<DoubleProperty *>(prop)) {
      prevCalcs[(unsigned long) prop] = prop->getMetaValueCalculator();
      ((DoubleProperty *)prop)->setMetaValueCalculator(nodeFn, edgeFn);
    }
    if (pName == "viewLabel") {
      prevCalcs[(unsigned long) prop] = prop->getMetaValueCalculator();
      ((StringProperty*) prop)->setMetaValueCalculator(&viewLabelCalc);
    }
  }
  // compute meta nodes, edges and associated meta values
  itS = graph->getSubGraphs();
  vector<node> mNodes;
  graph->createMetaNodes(itS, quotientGraph, mNodes);
  delete itS;

  // restore previous calculators
  TLP_HASH_MAP<unsigned long, PropertyInterface::MetaValueCalculator *>::iterator itC =
    prevCalcs.begin();
  while(itC != prevCalcs.end()) {
    ((PropertyInterface*) (*itC).first)->setMetaValueCalculator((*itC).second);
    itC++;
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
	if (edgeFn != DoubleProperty::NO_CALC) {
	  forEach(pName, graph->getProperties()) {
	    PropertyInterface *property = graph->getProperty(pName);
	    if (dynamic_cast<DoubleProperty *>(property) &&
		// try to avoid view... properties
		(pName.find("view") != 0 || pName == "viewMetric")) {
	      DoubleProperty *metric = graph->getProperty<DoubleProperty>(pName);
	      double value = metric->getEdgeValue(mE);	    
	      switch(edgeFn) {
	      case DoubleProperty::AVG_CALC:
		value = (value + metric->getEdgeValue(op))/2;
		break;
	      case DoubleProperty::SUM_CALC:
		value += metric->getEdgeValue(op);
		break;
	      case DoubleProperty::MAX_CALC:
		if (value < metric->getEdgeValue(op))
		  value = metric->getEdgeValue(op);
		break;
	      case DoubleProperty::MIN_CALC:
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

  // layouting if needed
  if (quotientLayout) {
    string errMsg;
    string layoutName;
    if (mNodes.size() > 300  ||
	quotientGraph->numberOfEdges() == 0)
      layoutName = "Circular";
    else
      layoutName = "GEM (Frick)";
    string sizesName="Auto Sizing";
    quotientGraph->computeProperty(layoutName, quotientGraph->getLocalProperty<LayoutProperty>("viewLayout"), errMsg);
    if (mNodes.size() < 300)
      quotientGraph->computeProperty(sizesName, quotientGraph->getLocalProperty<SizeProperty>("viewSize"),errMsg);
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
    dSet.set("layout quotient graph(s)", quotientLayout);
    GraphProperty *metaInfo =
      graph->getRoot()->getProperty<GraphProperty>("viewMetaGraph");
    vector<node>::iterator itn = mNodes.begin();
    while(itn != mNodes.end()) {
      node mn = *itn;
      Graph* sg = quotientGraph->getNodeMetaInfo(mn);
      string eMsg;
      tlp::applyAlgorithm(sg, eMsg, &dSet, "Quotient Clustering",
			  pluginProgress);
      // if a quotient graph has been computed
      // update metaInfo of current meta node
      if (dSet.getAndFree("quotientGraph", sg))
	metaInfo->setNodeValue(mn, sg);
      itn++;
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
