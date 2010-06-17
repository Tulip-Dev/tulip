/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
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
#include "DepthMetric.h"

DOUBLEPLUGINOFGROUP(DepthMetric,"Depth","David Auber","15/02/2001","Alpha","1.0","Tree");

using namespace std;
using namespace tlp;

DepthMetric::DepthMetric(const PropertyContext &context):DoubleAlgorithm(context) {}

//=================================================
double DepthMetric::getNodeValue(const node n) {
  if (graph->outdeg(n)==0) return 0.0;
  if (doubleResult->getNodeValue(n) > 0.1)
    return doubleResult->getNodeValue(n);
  double max = 0;
  node _n2;
  forEach(_n2, graph->getOutNodes(n))
    max = std::max(max, getNodeValue(_n2));
  doubleResult->setNodeValue(n, max + 1);
  return max + 1;
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
bool DepthMetric::check(string &erreurMsg) {
  if (AcyclicTest::isAcyclic(graph))
    return true;
  else {
    erreurMsg="The Graph must be acyclic";
    return false;
  }
}
//=================================================
