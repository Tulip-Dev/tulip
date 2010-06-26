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
#include "Random.h"

using namespace tlp;

DOUBLEPLUGINOFGROUP(RandomMetric,"Random","David Auber","04/10/2001","Alpha","1.0","Misc")

//===========================================
RandomMetric::RandomMetric(const PropertyContext &context):DoubleAlgorithm(context) 
{}
//===========================================
bool RandomMetric::run() {
  node n;
  forEach(n, graph->getNodes())
    doubleResult->setNodeValue(n, (double)rand() / double(RAND_MAX));
  edge e;
  forEach(e, graph->getEdges())
    doubleResult->setEdgeValue(e, (double)rand() / double(RAND_MAX));
  return true;
}
//===========================================
