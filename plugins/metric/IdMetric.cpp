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
#include "IdMetric.h"

using namespace tlp;

DOUBLEPLUGINOFGROUP(IdMetric,"Id","David Auber","06/04/2000","Alpha","1.0","Misc");

//==================================================================
IdMetric::IdMetric(const tlp::PropertyContext &context):DoubleAlgorithm(context) {
}
//==================================================================
bool IdMetric::run() {
  node n;
  forEach(n, graph->getNodes())
    doubleResult->setNodeValue(n, n.id);
  edge e;
  forEach(e, graph->getEdges())
    doubleResult->setEdgeValue(e, e.id);
  return true;
}
//=================================================================
