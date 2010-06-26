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
#include "MultipleEdgeSelection.h"
#include <tulip/BooleanProperty.h>
#include <tulip/SimpleTest.h>

BOOLEANPLUGIN(MultipleEdgeSelection,"Multiple Edge","David Auber","20/01/2003","Alpha","1.0");

using namespace std;
using namespace tlp;

MultipleEdgeSelection::MultipleEdgeSelection(const PropertyContext &context):BooleanAlgorithm(context) {}

bool MultipleEdgeSelection::run() {
  vector<edge> multipleEdges;
  SimpleTest::simpleTest(graph, &multipleEdges);
  booleanResult->setAllNodeValue(false);
  booleanResult->setAllEdgeValue(false);
  vector<edge>::const_iterator it;
  for (it = multipleEdges.begin(); it != multipleEdges.end(); ++it) 
    booleanResult->setEdgeValue(*it, true);
  return true;
}
