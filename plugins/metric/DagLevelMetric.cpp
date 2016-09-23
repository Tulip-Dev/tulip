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
#include "DagLevelMetric.h"

PLUGIN(DagLevelMetric)

using namespace std;
using namespace tlp;

DagLevelMetric::DagLevelMetric(const tlp::PluginContext* context):DoubleAlgorithm(context) {}

DagLevelMetric::~DagLevelMetric() {}
//======================================================
bool DagLevelMetric::run() {
  MutableContainer<unsigned int> level;
  dagLevel(graph, level, pluginProgress);
  node n;
  forEach(n, graph->getNodes())
  result->setNodeValue(n, level.get(n.id));
  return true;
}
//======================================================
bool DagLevelMetric::check(std::string &erreurMsg) {
  if (AcyclicTest::isAcyclic(graph)) {
    erreurMsg="";
    return true;
  }
  else {
    erreurMsg="The graph must be a acylic.";
    return false;
  }
}
