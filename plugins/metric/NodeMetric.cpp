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
#include "NodeMetric.h"

DOUBLEPLUGINOFGROUP(NodeMetric,"Node","David Auber","20/12/1999","Alpha","1.0","Tree");

using namespace std;
using namespace tlp;

//====================================================================
NodeMetric::NodeMetric(const tlp::PropertyContext &context):DoubleAlgorithm(context) 
{}
//====================================================================
double NodeMetric::getNodeValue(const tlp::node n) {
  if (doubleResult->getNodeValue(n)!=0)
    return doubleResult->getNodeValue(n);
  double result = 1;
  node _n;
  forEach(_n, graph->getOutNodes(n))
    result += getNodeValue(_n);
  doubleResult->setNodeValue(n, result);
  return result;
}
//====================================================================
bool NodeMetric::run() {
  doubleResult->setAllEdgeValue(0);
  doubleResult->setAllNodeValue(0);
  node _n;
  forEach(_n, graph->getNodes())
    doubleResult->setNodeValue(_n,getNodeValue(_n));
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
