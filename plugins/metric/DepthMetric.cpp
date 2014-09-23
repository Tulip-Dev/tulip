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
#include <stack>
#include "DepthMetric.h"

PLUGIN(DepthMetric)

using namespace std;
using namespace tlp;

static const char * paramHelp[] = {
  HTML_HELP_OPEN() \
  HTML_HELP_DEF( "type", "NumericProperty" ) \
  HTML_HELP_DEF( "default", "None" ) \
  HTML_HELP_BODY() \
  "This parameter defines the metric used for edges weight." \
  HTML_HELP_CLOSE(),
};

DepthMetric::DepthMetric(const tlp::PluginContext* context):DoubleAlgorithm(context), edgeWeight(NULL) {
  addInParameter<NumericProperty*> ("edge weight", paramHelp[0], "", false);
}

// structure below is used to implement dfs loop
struct dfsDepthStruct {
  node current;
  Iterator<edge>* outEdges;
  double maxDepth;
  double edgeValue;

  dfsDepthStruct(): outEdges(NULL), maxDepth(0.0), edgeValue(0.0) {}
  dfsDepthStruct(node n, Iterator<edge>* edges):
    current(n), outEdges(edges), maxDepth(0.0), edgeValue(0.0) {}
};
//=================================================
double DepthMetric::getNodeValue(tlp::node current) {
  if (graph->outdeg(current) == 0) return 0.0;

  double value = result->getNodeValue(current);

  if (value > 0.0)
    return value;

  // dfs loop
  stack<dfsDepthStruct> dfsLevels;
  Iterator<edge>* outEdges = graph->getOutEdges(current);
  dfsDepthStruct dfsParams(current, outEdges);
  double maxDepth = 0.0;
  dfsLevels.push(dfsParams);

  while(!dfsLevels.empty()) {
    while(outEdges->hasNext()) {
      edge e = outEdges->next();
      double edgeValue = edgeWeight ? edgeWeight->getEdgeDoubleValue(e) : 1.0;
      node neighbour = graph->target(e);
      value = result->getNodeValue(neighbour);

      // compute max
      if (value > 0.0)
        // since depth value of neighbour is already computed
        // update maxDepth if needed
        maxDepth = std::max(maxDepth, value + edgeValue);
      else {
        // we need to compute depth value for neighbour
        outEdges = graph->getOutEdges(neighbour);

        if (outEdges->hasNext()) {
          // store maxDepth for current
          dfsLevels.top().maxDepth = maxDepth;
          // store edgeValue for current
          // it will be needed as soon as the value
          // for neighbour will be computed
          dfsLevels.top().edgeValue = edgeValue;
          // push neighbour dfsParams on stack
          current = dfsParams.current = neighbour;
          dfsParams.outEdges = outEdges;
          maxDepth = dfsParams.maxDepth = dfsParams.edgeValue = 0.0;
          dfsLevels.push(dfsParams);
          // and go deeper
          break;
        }
        else { // no out edges for neighbour
          // so its depth value will remain to 0.0
          delete outEdges;
          // go on with the current
          outEdges = dfsParams.outEdges;
          // update current maxDepth if needed
          maxDepth = std::max(maxDepth, edgeValue);
        }
      }
    }

    if (outEdges->hasNext())
      // new dfsParams has been pushed on stack
      continue;

    // save current maxDepth
    result->setNodeValue(current, maxDepth);
    // unstack current dfsParams
    delete dfsLevels.top().outEdges;
    dfsLevels.pop();

    if (dfsLevels.empty())
      break;

    // get dfsParams on top of dfsLevels
    dfsParams = dfsLevels.top();
    current = dfsParams.current;
    outEdges = dfsParams.outEdges;
    // update current maxDepth
    dfsParams.maxDepth = std::max(dfsParams.maxDepth,
                                  maxDepth + dfsParams.edgeValue);
    maxDepth = dfsParams.maxDepth;
  }

  return maxDepth;
}
//====================================================================
bool DepthMetric::run() {
  if ( dataSet!=NULL) {
    dataSet->get("edge weight", edgeWeight);
  }

  result->setAllEdgeValue(0);
  result->setAllNodeValue(0);
  node _n;
  forEach(_n, graph->getNodes()) {
    result->setNodeValue(_n, getNodeValue(_n));
  }
  return true;
}
//=================================================
bool DepthMetric::check(std::string &erreurMsg) {
  if (AcyclicTest::isAcyclic(graph))
    return true;
  else {
    erreurMsg="The graph must be acyclic.";
    return false;
  }
}
//=================================================
