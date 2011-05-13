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
#include "LeafMetric.h"

DOUBLEPLUGINOFGROUP(LeafMetric,"Leaf","David Auber","20/12/1999","Alpha","1.0","Tree");

using namespace std;
using namespace tlp;

//=======================================================================
LeafMetric::LeafMetric(const tlp::PropertyContext &context):DoubleAlgorithm(context) {}

// structure below is used to implement dfs loop
struct dfsLeafStruct {
  node current;
  Iterator<node>* outNodes;
  double result;

  dfsLeafStruct() {}
  dfsLeafStruct(node n, Iterator<node>* nodes):
    current(n), outNodes(nodes), result(0.0) {}
};
//=======================================================================
// original recursive algorithm
/*double LeafMetric::getNodeValue(const tlp::node n) {
  if (doubleResult->getNodeValue(n) != 0) 
    return doubleResult->getNodeValue(n);
  double result=0;
  node _n;
  forEach(_n, graph->getOutNodes(n)) 
    result += getNodeValue(_n);
  if (result==0) result=1.0;
  doubleResult->setNodeValue(n, result);
  return result;
  }*/
//=======================================================================
double LeafMetric::getNodeValue(tlp::node current) {
  double value = doubleResult->getNodeValue(current);
  if (value != 0.0)
    return value;

  // dfs loop
  stack<dfsLeafStruct> dfsLevels;
  Iterator<node>* outNodes = graph->getOutNodes(current);
  dfsLeafStruct dfsParams(current, outNodes);
  double result = 0.0;
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
	result = dfsParams.result = 0.0;
	dfsLevels.push(dfsParams);
	// and go deeper
	break;
      }
    }
    if (outNodes->hasNext())
      // new dfsParams has been pushed on stack
      continue;
    // save current result
    if (result == 0.0)
      result = 1.0;
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
//=======================================================================
bool LeafMetric::run() {
  doubleResult->setAllNodeValue(0);
  doubleResult->setAllEdgeValue(0);
  node n;
  forEach(n, graph->getNodes())
    doubleResult->setNodeValue(n, getNodeValue(n));
  return true;
}
//=======================================================================
bool LeafMetric::check(std::string &erreurMsg) {
  if (AcyclicTest::isAcyclic(graph))
    return true;
  else {
    erreurMsg="The Graph must be a Acyclic";
    return false;
  }
}
//=======================================================================



