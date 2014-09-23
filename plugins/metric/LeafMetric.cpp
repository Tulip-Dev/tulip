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
#include "LeafMetric.h"

PLUGIN(LeafMetric)

using namespace std;
using namespace tlp;

//=======================================================================
LeafMetric::LeafMetric(const tlp::PluginContext* context):DoubleAlgorithm(context) {}

// structure below is used to implement dfs loop
struct dfsLeafStruct {
  node current;
  Iterator<node>* outNodes;
  double res;

  dfsLeafStruct(node n, Iterator<node>* nodes):
    current(n), outNodes(nodes), res(0.0) {}
};
//=======================================================================
// original recursive algorithm
/*double LeafMetric::getNodeValue(const tlp::node n) {
  if (result->getNodeValue(n) != 0)
    return result->getNodeValue(n);
  double res=0;
  node _n;
  forEach(_n, graph->getOutNodes(n))
    res += getNodeValue(_n);
  if (res==0) res=1.0;
  result->setNodeValue(n, res);
  return res;
  }*/
//=======================================================================
double LeafMetric::getNodeValue(tlp::node current) {
  double value = result->getNodeValue(current);

  if (value != 0.0)
    return value;

  // dfs loop
  stack<dfsLeafStruct> dfsLevels;
  Iterator<node>* outNodes = graph->getOutNodes(current);
  dfsLeafStruct dfsParams(current, outNodes);
  double res = 0.0;
  dfsLevels.push(dfsParams);

  while(!dfsLevels.empty()) {
    while (outNodes->hasNext()) {
      node neighbour = outNodes->next();
      value = result->getNodeValue(neighbour);

      // compute res
      if (value != 0.0)
        res += value;
      else {
        // store res for current
        dfsLevels.top().res = res;
        // push new dfsParams on stack
        current = dfsParams.current = neighbour;
        outNodes = dfsParams.outNodes = graph->getOutNodes(neighbour);
        res = dfsParams.res = 0.0;
        dfsLevels.push(dfsParams);
        // and go deeper
        break;
      }
    }

    if (outNodes->hasNext())
      // new dfsParams has been pushed on stack
      continue;

    // save current res
    if (res == 0.0)
      res = 1.0;

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
//=======================================================================
bool LeafMetric::run() {
  result->setAllNodeValue(0);
  result->setAllEdgeValue(0);
  node n;
  forEach(n, graph->getNodes())
  result->setNodeValue(n, getNodeValue(n));
  return true;
}
//=======================================================================
bool LeafMetric::check(std::string &erreurMsg) {
  if (AcyclicTest::isAcyclic(graph))
    return true;
  else {
    erreurMsg="The graph must be a acyclic.";
    return false;
  }
}
//=======================================================================



