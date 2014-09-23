/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
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
#include <tulip/TulipPluginHeaders.h>
#include <tulip/StringCollection.h>

using namespace std;
using namespace tlp;

/** \addtogroup metric */
/*@{*/
/**
 * \file
 * \brief A metric based on the K-core decomposition of a graph.
 *
 * K-cores were first introduced in:
 *
 * S. B. Seidman, "Network structure and minimum degree",
 * Social Networks 5:269-287, 1983
 *
 * This is a method of simplifying graph topology to aid in analysis
 * and visualization of social networks
 *
 * (see http://en.wikipedia.org/wiki/K-core for more details)
 *
 * The K-Cores metric can also be computed according to weighted degrees. See :
 *
 * C. Giatsidis, D. Thilikos, M. Vazirgiannis, \n
 * "Evaluating cooperation in communities with the k-core structure",\n
 * "Proceedings of the 2011 International Conference on Advances in Social Networks Analysis and Mining (ASONAM)",\n
 * "2011"
 *
 * \note Use the default parameters to compute simple K-Cores (undirected and unweighted)
 *
 *  <b>HISTORY</b>
 *
 *  - 2006 Version 1.0 by David Auber, LaBRI,
 *  University Bordeaux I, France
 *  - 2011 Version 2.0: Add In/Out and Weighted computation features
 *  by François Queyroi, LaBRI, University Bordeaux I, France
 *
 *  <b>LICENCE</b>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 */
class KCores:public tlp::DoubleAlgorithm {
public:

  PLUGININFORMATION("K-Cores", "David Auber","28/05/2006","Nodes measure<br/>often used to evaluate the structure of social networks.","2.0", "Graph")

  KCores(const tlp::PluginContext *context);
  ~KCores();
  bool run();
private:
  bool peel(tlp::Graph* subgraph, tlp::NumericProperty* metric,
            tlp::DoubleProperty&);
  bool peelIn(tlp::Graph* subgraph, tlp::NumericProperty* metric,
              tlp::DoubleProperty&);
  bool peelOut(tlp::Graph* subgraph, tlp::NumericProperty* metric,
               tlp::DoubleProperty&);
};

//========================================================================================
namespace {
const char * paramHelp[] = {
  //direction
  HTML_HELP_OPEN()         \
  HTML_HELP_DEF( "type", "String Collection" ) \
  HTML_HELP_DEF( "default", "InOut" )  \
  HTML_HELP_BODY() \
  "This parameter indicates the direction used to compute K-Cores values."  \
  HTML_HELP_CLOSE(),
  // metric
  HTML_HELP_OPEN()              \
  HTML_HELP_DEF( "type", "NumericProperty" )       \
  HTML_HELP_DEF( "value", "An existing edge metric" )   \
  HTML_HELP_BODY()              \
  "An existing edge metric property"\
  HTML_HELP_CLOSE()
};
}
#define DEGREE_TYPE "type"
#define DEGREE_TYPES "InOut;In;Out;"
#define INOUT 0
#define IN 1
#define OUT 2
//========================================================================================
KCores::KCores(const PluginContext *context):DoubleAlgorithm(context) {
  addInParameter<StringCollection>(DEGREE_TYPE, paramHelp[0], DEGREE_TYPES);
  addInParameter<NumericProperty*>("metric",paramHelp[1],"",false);
  addDependency("Degree","1.0");
}
//========================================================================================
KCores::~KCores() {}
//========================================================================================
bool KCores::peel(Graph* subgraph, NumericProperty* metric,
                  DoubleProperty& wdeg) {
  double k= wdeg.getNodeMin();
  bool modify = true;
  bool onePeel = false;

  while (modify) {
    modify = false;
    node n;
    stableForEach(n,subgraph->getNodes()) {
      if (wdeg.getNodeValue(n) <= k) { //Remove n and decrease its In/Out-neighbors' degree
        result->setNodeValue(n, k);
        edge ee;
        forEach(ee,subgraph->getInOutEdges(n)) {
          node m = subgraph->opposite(ee,n);
          wdeg.setNodeValue(m, wdeg.getNodeValue(m)-(metric ? metric->getEdgeDoubleValue(ee) : 1.0));
        }
        subgraph->delNode(n);
        modify = true;
        onePeel = true;
      }
    }
  }

  return onePeel;
}
//========================================================================================
bool KCores::peelIn(Graph* subgraph, NumericProperty* metric,
                    DoubleProperty& wdeg) {
  double k= wdeg.getNodeMin();
  bool modify = true;
  bool onePeel = false;

  while (modify) {
    modify = false;
    node n;
    stableForEach(n,subgraph->getNodes()) {
      if (wdeg.getNodeValue(n) <= k) {//Remove n and decrease its Out-neighbors' degree
        result->setNodeValue(n, k);
        edge ee;
        forEach(ee,subgraph->getOutEdges(n)) {
          node m = subgraph->opposite(ee,n);
          wdeg.setNodeValue(m, wdeg.getNodeValue(m)- (metric ? metric->getEdgeDoubleValue(ee) : 1.0));
        }
        subgraph->delNode(n);
        modify = true;
        onePeel = true;
      }
    }
  }

  return onePeel;
}
//========================================================================================
bool KCores::peelOut(Graph* subgraph, NumericProperty* metric,
                     DoubleProperty& wdeg) {
  double k= wdeg.getNodeMin();
  bool modify = true;
  bool onePeel = false;

  while (modify) {
    modify = false;
    node n;
    stableForEach(n,subgraph->getNodes()) {
      if (wdeg.getNodeValue(n) <= k) { //Remove n and decrease its In-neighbors' degree
        result->setNodeValue(n, k);
        edge ee;
        forEach(ee,subgraph->getInEdges(n)) {
          node m = subgraph->opposite(ee,n);
          wdeg.setNodeValue(m, wdeg.getNodeValue(m)- (metric ? metric->getEdgeDoubleValue(ee) : 1.0));
        }
        subgraph->delNode(n);
        modify = true;
        onePeel = true;
      }
    }
  }

  return onePeel;
}
//========================================================================================
bool KCores::run() {
  NumericProperty* metric = NULL;
  StringCollection degreeTypes(DEGREE_TYPES);
  degreeTypes.setCurrent(0);

  if (dataSet!=NULL) {
    dataSet->get(DEGREE_TYPE, degreeTypes);
    dataSet->get("metric", metric);
  }

  Graph* subgraph = graph->addCloneSubGraph();
  DoubleProperty wdeg(subgraph);
  string errMsg="";
  subgraph->applyPropertyAlgorithm("Degree",&wdeg,errMsg,pluginProgress,dataSet);

  switch(degreeTypes.getCurrent()) {
  case INOUT:

    while (subgraph->numberOfNodes()>0)
      peel(subgraph, metric, wdeg);

    break;

  case IN:

    while (subgraph->numberOfNodes()>0)
      peelIn(subgraph, metric, wdeg);

    break;

  case OUT:

    while (subgraph->numberOfNodes()>0)
      peelOut(subgraph, metric, wdeg);

    break;
  }

  graph->delSubGraph(subgraph);
  return true;
}
//========================================================================================
PLUGIN(KCores)
//========================================================================================
