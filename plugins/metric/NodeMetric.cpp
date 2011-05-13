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
#include "NodeMetric.h"

DOUBLEPLUGINOFGROUP(NodeMetric,"Node","David Auber","20/12/1999","Alpha","1.0","Tree");

using namespace std;
using namespace tlp;

//====================================================================
NodeMetric::NodeMetric(const tlp::PropertyContext &context):DoubleAlgorithm(context) 
{}

// structure below is used to implement dfs loop
struct dfsStruct {
  node current;
  Iterator<node>* outNodes;
  double result;

  dfsStruct() {}
  dfsStruct(node n, Iterator<node>* nodes):
    current(n), outNodes(nodes), result(1.0) {}
};
//=======================================================================
// original recursive algorithm
/*double NodeMetric::getNodeValue(const tlp::node n) {
  if (doubleResult->getNodeValue(n)!=0)
    return doubleResult->getNodeValue(n);
  double result = 1;
  node _n;
  forEach(_n, graph->getOutNodes(n))
    result += getNodeValue(_n);
  doubleResult->setNodeValue(n, result);
  return result;
  }*/
//=======================================================================
double NodeMetric::getNodeValue(tlp::node current) {
  double value = doubleResult->getNodeValue(current);
  if (value != 0.0)
    return value;

  // dfs loop
  stack<dfsStruct> dfsLevels;
  Iterator<node>* outNodes = graph->getOutNodes(current);
  dfsStruct dfsParams(current, outNodes);
  double result = 1.0;
  dfsLevels.push(dfsParams);
  while(!dfsLevels.empty()) {
    while (outNodes->hasNext()) {
      node neighbour = outNodes->next();
      value = doubleResult->getNodeValue(neighbour);
      // compute result
      if (value != 0.0)
	result += value;
      else {
	// store result for current
	dfsLevels.top().result = result;
	// push new dfsParams on stack
	current = dfsParams.current = neighbour;
	outNodes = dfsParams.outNodes = graph->getOutNodes(neighbour);
	result = dfsParams.result = 1.0;
	dfsLevels.push(dfsParams);
	// and go deeper
	break;
      }
    }
    if (outNodes->hasNext())
      // new dfsParams has been pushed on stack
      continue;
    // save current result
    doubleResult->setNodeValue(current, result);
    // unstack current dfsParams
    delete outNodes;
    dfsLevels.pop();
    if (dfsLevels.empty())
      break;
    // get dfsParams on top of dfsLevels
    dfsParams = dfsLevels.top();
    current = dfsParams.current;
    outNodes = dfsParams.outNodes;
    // update new current result if any
    dfsParams.result += result;
    result = dfsParams.result;
  }
  return result;
}
//====================================================================
bool NodeMetric::run() {
  doubleResult->setAllEdgeValue(0);
  doubleResult->setAllNodeValue(0);
  node n;
  forEach(n, graph->getNodes()) {
    double value = getNodeValue(n);
    if (value < 0.0) {
      if (pluginProgress) {
	pluginProgress->setError("The Graph must be acyclic");
	return false;
      }
    }
    doubleResult->setNodeValue(n, value);
  }
  return true;
}
//====================================================================
bool NodeMetric::check(std::string &erreurMsg) {
  if (AcyclicTest::isAcyclic(graph)) {
    erreurMsg="";
    return true;
  }
  else  {
    erreurMsg="The Graph must be acyclic";
    return false;
  }
}
//====================================================================
