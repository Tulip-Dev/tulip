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
#include "PathLengthMetric.h"

DOUBLEPLUGINOFGROUP(PathLengthMetric,"Path Length","David Auber","15/02/2001","Alpha","1.0","Tree");

using namespace std;
using namespace tlp;

//=======================================
PathLengthMetric::PathLengthMetric(const tlp::PropertyContext &context):DoubleAlgorithm(context) {
  // Leaf metric needed
  addDependency<DoubleAlgorithm>("Leaf", "1.0");
}

// structure below is used to implement dfs loop
struct dfsStruct {
  node current;
  Iterator<node>* outNodes;
  double res;

  dfsStruct() {}
  dfsStruct(node n, Iterator<node>* nodes):
    current(n), outNodes(nodes), res(0.0) {}
};
//=======================================
// original recursive algorithm
/*double PathLengthMetric::getNodeValue(const tlp::node n) {
  if (graph->outdeg(n)==0) return 0.0;
  if (result->getNodeValue(n) > 0.1)
    return result->getNodeValue(n);
  double res=0;
  node child;
  forEach(child, graph->getOutNodes(n)) {
    res += getNodeValue(child);
  }
  res += leafMetric->getNodeValue(n);
  result->setNodeValue(n, res);
  return res;
  }*/
//=======================================================================
double PathLengthMetric::getNodeValue(tlp::node current) {
  if (graph->outdeg(current) == 0) return 0.0;
  double value = result->getNodeValue(current);
  if (value > 0.1)
    return value;

  // dfs loop
  stack<dfsStruct> dfsLevels;
  Iterator<node>* outNodes = graph->getOutNodes(current);
  dfsStruct dfsParams(current, outNodes);
  double res = 0.0;
  dfsLevels.push(dfsParams);
  while(!dfsLevels.empty()) {
    while (outNodes->hasNext()) {
      node neighbour = outNodes->next();
      value = result->getNodeValue(neighbour);
      // compute res
      if (value > 0.1)
	res += value;
      else {
	outNodes = graph->getOutNodes(neighbour);
	if (outNodes->hasNext()) {
	  // store res for current
	  dfsLevels.top().res = res;
	  // push new dfsParams on stack
	  current = dfsParams.current = neighbour;
	  outNodes = dfsParams.outNodes = graph->getOutNodes(neighbour);
	  res = dfsParams.res = 0.0;
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
    res += leafMetric->getNodeValue(current);
    // save current res
    result->setNodeValue(current, res);
    // unstack current dfsParams
    delete outNodes;
    dfsLevels.pop();
    if (dfsLevels.empty())
      break;
    // get dfsParams on top of dfsLevels
    dfsParams = dfsLevels.top();
    current = dfsParams.current;
    outNodes = dfsParams.outNodes;
    // update new current res if any
    dfsParams.res += res;
    res = dfsParams.res;
  }
  return res;
}
//=======================================
bool PathLengthMetric::run() {
  result->setAllNodeValue(0);
  result->setAllEdgeValue(0);
  leafMetric = new DoubleProperty(graph);
  string erreurMsg;
  if (!graph->computeProperty("Leaf", leafMetric, erreurMsg)) {
    cerr << erreurMsg << endl;
    return false;
  }
  node _n;
  forEach(_n, graph->getNodes())
    getNodeValue(_n);
  delete leafMetric;
  return true;
}
//=======================================
bool PathLengthMetric::check(std::string &erreurMsg) {
  if (AcyclicTest::isAcyclic(graph))
    return true;
  else {
    erreurMsg="The Graph must be acyclic";
    return false;
  }
}
//=======================================
