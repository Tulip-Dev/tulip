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
#include "Random.h"

using namespace tlp;

PLUGIN(RandomMetric)

//===========================================
RandomMetric::RandomMetric(const tlp::PluginContext* context):DoubleAlgorithm(context) {
}
//===========================================
bool RandomMetric::run() {
  // initialize a random sequence according the given seed
  tlp::initRandomSequence();

  node n;
  forEach(n, graph->getNodes())
  result->setNodeValue(n, (double)rand() / double(RAND_MAX));
  edge e;
  forEach(e, graph->getEdges())
  result->setEdgeValue(e, (double)rand() / double(RAND_MAX));
  return true;
}
//===========================================
