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
#include <assert.h>
#include "LoopSelection.h"

BOOLEANPLUGIN(LoopSelection,"Loop Selection","David Auber","20/01/2003","Alpha","1.0");

using namespace std;
using namespace tlp;

LoopSelection::LoopSelection(const tlp::PropertyContext &context):BooleanAlgorithm(context) {}
//============================================
bool LoopSelection::run() {
  _result->setAllNodeValue(false);
  edge e;
  forEach(e, graph->getEdges())
    _result->setEdgeValue(e, 
			       graph->source(e)==graph->target(e));
  return true;
}
//============================================

