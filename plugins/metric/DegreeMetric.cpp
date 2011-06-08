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
#include "DegreeMetric.h"

using namespace tlp;

DOUBLEPLUGINOFGROUP(DegreeMetric,"Degree","David Auber","04/10/2001","Stable","1.0","Graph");

namespace {
  const char * paramHelp[] = {
    //Degree type
    HTML_HELP_OPEN()				 \
    HTML_HELP_DEF( "type", "String Collection" ) \
    HTML_HELP_DEF( "default", "InOut" )	 \
    HTML_HELP_BODY() \
    "This parameter indicates the type of degree to compute (in/out/inout)."	\
    HTML_HELP_CLOSE(),
    HTML_HELP_OPEN()							\
    HTML_HELP_DEF( "type", "DoubleProperty" )				\
    HTML_HELP_DEF( "value", "An existing metric corresponding to weights.")		\
    HTML_HELP_DEF( "default", "none" )					\
    HTML_HELP_BODY()							\
    "The weighted degree of a node is the sum of weights of "\
    "all its in/out/inout edges. "\
    "If no metric is specified, using a uniform metric value of 1 for all edges " \
    "returns the usual degree for nodes (number of in/out/inout neighbors)."\
    HTML_HELP_CLOSE(),
    HTML_HELP_OPEN()				 \
    HTML_HELP_DEF( "type", "bool" ) \
    HTML_HELP_DEF( "default", "false" )	 \
    HTML_HELP_BODY() \
    "If true the mesure will be normalized unweight: m(n) = deg(n) / (#V - 1) "	\
    "If true the mesure will be normalized unweight: m(n) = deg_w(n) / [(sum(e_w)/#E)(#V - 1)] " \
    HTML_HELP_CLOSE(),

  };
}
#define DEGREE_TYPE "type"
#define DEGREE_TYPES "InOut;In;Out;"
#define INOUT 0
#define IN 1
#define OUT 2
//==============================================================================
DegreeMetric::DegreeMetric(const tlp::PropertyContext &context):DoubleAlgorithm(context) {
   addParameter<StringCollection>(DEGREE_TYPE, paramHelp[0], DEGREE_TYPES);
   addParameter<DoubleProperty>("metric", paramHelp[1], "", false);
   addParameter<bool>("norm", paramHelp[2], "false", false);
}
//==================================================================
bool DegreeMetric::run() {
  StringCollection degreeTypes(DEGREE_TYPES);
  degreeTypes.setCurrent(0);
  DoubleProperty* weights = 0;
  bool norm = false;
  if (dataSet!=0) {
    dataSet->get(DEGREE_TYPE, degreeTypes);
    dataSet->get("metric", weights);
    dataSet->get("norm", norm);
  }

  //sum w_e = E_w/#E, sum d_n = 2E_w 
  double normalization = 1.;
  if (norm && graph->numberOfNodes() > 1 && graph->numberOfEdges())
    normalization = graph->numberOfNodes() - 1;
  node n;
  if (!weights) {
    switch(degreeTypes.getCurrent()) {
    case INOUT:
      forEach(n, graph->getNodes())
	result->setNodeValue(n, double(graph->deg(n))/normalization);
      break;
    case IN:
      forEach(n, graph->getNodes())
	result->setNodeValue(n, double(graph->indeg(n))/normalization);
      break;
    case OUT:
      forEach(n, graph->getNodes())
	result->setNodeValue(n, double(graph->outdeg(n))/normalization);
      break;
    }
    // null value for edges
    result->setAllEdgeValue(0);
  }
  else {
    if (norm && graph->numberOfNodes() > 1 && graph->numberOfEdges() > 0) {
      double sum = 0;
      edge e;
      forEach(e, graph->getEdges())
	sum += fabs(weights->getEdgeValue(e));
      normalization = (sum / double(graph->numberOfEdges())) * double(graph->numberOfNodes() - 1);
      if (fabs(normalization) < 1E-9) normalization = 1.0;
    }
    switch(degreeTypes.getCurrent()) {
    case INOUT:
      forEach(n, graph->getNodes()) {
	edge e;
	double nWeight = 0.0;
	forEach(e, graph->getInOutEdges(n)) {
	  nWeight += weights->getEdgeValue(e);
	}
	result->setNodeValue(n, nWeight / normalization);
      }
      break;
    case IN:
      forEach(n, graph->getNodes()) {
	edge e;
	double nWeight = 0.0;
	forEach(e, graph->getInEdges(n)) {
	  nWeight += weights->getEdgeValue(e);
	}
	result->setNodeValue(n, nWeight / normalization);
      }
      break;
    case OUT:
      forEach(n, graph->getNodes()) {
	edge e;
	double nWeight = 0.0;
	forEach(e, graph->getOutEdges(n)) {
	  nWeight += weights->getEdgeValue(e);
	}
	result->setNodeValue(n, nWeight / normalization);
      }
      break;
    }
  }
  return true;
}
//==================================================================
