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
#include <cassert>
#include "DagLevelMetric.h"

DOUBLEPLUGINOFGROUP(DagLevelMetric,"Dag Level","David Auber","10/03/2000","Alpha","1.0","Tree");

using namespace std;
using namespace tlp;

DagLevelMetric::DagLevelMetric(const PropertyContext &context):DoubleAlgorithm(context) {}

DagLevelMetric::~DagLevelMetric() {}
//====================================================== 
bool DagLevelMetric::run() {
  MutableContainer<unsigned int> level;
  dagLevel(graph, level, pluginProgress);
  node n;
  forEach(n, graph->getNodes())
    doubleResult->setNodeValue(n, level.get(n.id));
  return true;
}
//====================================================== 
bool DagLevelMetric::check(string &erreurMsg) {
  if (AcyclicTest::isAcyclic(graph)) {
    erreurMsg="";
    return true;
  }
  else {
    erreurMsg="The Graph must be a acylic";
    return false;
  }
}
