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
#include <stack>
#include "DepthMetric.h"

DOUBLEPLUGINOFGROUP(DepthMetric,"Depth","David Auber","15/02/2001","Alpha","1.0","Tree");

using namespace std;
using namespace tlp;

DepthMetric::DepthMetric(const tlp::PropertyContext &context):DoubleAlgorithm(context) {}

// structure below is used to implement dfs loop
struct dfsDepthStruct {
  node current;
  Iterator<node>* outNodes;
  double maxDepth;

  dfsDepthStruct() {}
  dfsDepthStruct(node n, Iterator<node>* nodes):
    current(n), outNodes(nodes), maxDepth(0.0) {}
};
//=================================================
// original recursive algorithm
/*double DepthMetric::getNodeValue(const tlp::node n) {
  if (graph->outdeg(n)==0) return 0.0;
  if (doubleResult->getNodeValue(n) > 0.1)
    return doubleResult->getNodeValue(n);
  double max = 0;
  node _n2;
  forEach(_n2, graph->getOutNodes(n))
    max = std::max(max, getNodeValue(_n2));
  doubleResult->setNodeValue(n, max + 1);
  return max + 1;
  } */
//=================================================
double DepthMetric::getNodeValue(tlp::node current) {
  if (graph->outdeg(current) == 0) return 0.0;
  double value = doubleResult->getNodeValue(current);
  if (value > 0.1)
    return value;

  // dfs loop
  stack<dfsDepthStruct> dfsLevels;
  Iterator<node>* outNodes = graph->getOutNodes(current);
  dfsDepthStruct dfsParams(current, outNodes);
  double maxDepth = 0;
  dfsLevels.push(dfsParams);
  while(!dfsLevels.empty()) {
    while (outNodes->hasNext()) {
      node neighbour = outNodes->next();
      value = doubleResult->getNodeValue(neighbour);
      // compute max
      if (value > 0.1)
	maxDepth = std::max(maxDepth, value);
      else {
	outNodes = graph->getOutNodes(neighbour);
	if (outNodes->hasNext()) {
	  // store maxDepth for current
	  dfsLevels.top().maxDepth = maxDepth;
	  // push new dfsParams on stack
	  current = dfsParams.current = neighbour;
	  dfsParams.outNodes = outNodes;
	  maxDepth = dfsParams.maxDepth = 0.0;
	  dfsLevels.push(dfsParams);
	  // and go deeper
	  break;
	} else
	  outNodes = dfsParams.outNodes;
      }
    }
    if (outNodes->hasNext())
      // new dfsParams has been pushed on stack
      continue;
    // save current maxDepth
    ++maxDepth;
    doubleResult->setNodeValue(current, maxDepth);
    // unstack current dfsParams
    delete outNodes;
    dfsLevels.pop();
    if (dfsLevels.empty())
      break;
    // get dfsParams on top of dfsLevels
    dfsParams = dfsLevels.top();
    current = dfsParams.current;
    outNodes = dfsParams.outNodes;
    // update new current maxDepth if any
    dfsParams.maxDepth = std::max(dfsParams.maxDepth, maxDepth);
    maxDepth = dfsParams.maxDepth;
  }
  return maxDepth;
}
//====================================================================
bool DepthMetric::run() {
  doubleResult->setAllEdgeValue(0);
  doubleResult->setAllNodeValue(0);
  node _n;
  forEach(_n, graph->getNodes())
    doubleResult->setNodeValue(_n, getNodeValue(_n));
  return true;
}
//=================================================
bool DepthMetric::check(std::string &erreurMsg) {
  if (AcyclicTest::isAcyclic(graph))
    return true;
  else {
    erreurMsg="The Graph must be acyclic";
    return false;
  }
}
//=================================================
