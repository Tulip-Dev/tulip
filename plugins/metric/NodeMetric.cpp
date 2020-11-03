/**
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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
#include "NodeMetric.h"

#include <tulip/AcyclicTest.h>

PLUGIN(NodeMetric)

using namespace std;
using namespace tlp;

//====================================================================
NodeMetric::NodeMetric(const tlp::PluginContext *context) : DoubleAlgorithm(context) {}

// structure below is used to implement dfs loop
struct dfsStruct {
  node current;
  Iterator<node> *outNodes;
  double res;

  dfsStruct(node n, Iterator<node> *nodes) : current(n), outNodes(nodes), res(1.0) {}
};
//=======================================================================
// original recursive algorithm
/*double NodeMetric::getNodeValue(const tlp::node n) {
  if (result->getNodeValue(n)!=0)
      return result->getNodeValue(n);
    double res = 1;
    for(const node &m : graph->getOutNodes(n))
      res += getNodeValue(m);
    result->setNodeValue(n, res);
    return res;
  }*/
//=======================================================================
double NodeMetric::getNodeValue(tlp::node current) {
  double value = result->getNodeValue(current);

  if (value != 0.0)
    return value;

  // dfs loop
  stack<dfsStruct> dfsLevels;
  Iterator<node> *outNodes = graph->getOutNodes(current);
  dfsStruct dfsParams(current, outNodes);
  double res = 1.0;
  dfsLevels.push(dfsParams);

  while (!dfsLevels.empty()) {
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
        res = dfsParams.res = 1.0;
        dfsLevels.push(dfsParams);
        // and go deeper
        break;
      }
    }

    if (outNodes->hasNext())
      // new dfsParams has been pushed on stack
      continue;

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
//====================================================================
bool NodeMetric::run() {
  result->setAllEdgeValue(0);
  result->setAllNodeValue(0);
  for (auto n : graph->nodes())
    result->setNodeValue(n, getNodeValue(n));
  return true;
}
//====================================================================
bool NodeMetric::check(std::string &erreurMsg) {
  if (!AcyclicTest::isAcyclic(graph)) {
    erreurMsg = "The graph must be acyclic.";
    return false;
  }
  return true;
}
//===================================================================
