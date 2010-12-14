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
#include "PathLengthMetric.h"

DOUBLEPLUGINOFGROUP(PathLengthMetric,"Path Length","David Auber","15/02/2001","Alpha","1.0","Tree");

using namespace std;
using namespace tlp;

//=======================================
PathLengthMetric::PathLengthMetric(const tlp::PropertyContext &context):DoubleAlgorithm(context) {
  // Leaf metric needed
  addDependency<DoubleAlgorithm>("Leaf", "1.0");
}
//=======================================
double PathLengthMetric::getNodeValue(const tlp::node n) {
  if (graph->outdeg(n)==0) return 0.0;
  if (doubleResult->getNodeValue(n) > 0.1)
    return doubleResult->getNodeValue(n);
  double result=0;
  node child;
  forEach(child, graph->getOutNodes(n)) {
    result += getNodeValue(child);
  }
  result += leafMetric->getNodeValue(n);
  doubleResult->setNodeValue(n, result);
  return result;
}
//=======================================
bool PathLengthMetric::run() {
  doubleResult->setAllNodeValue(0);
  doubleResult->setAllEdgeValue(0);
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
