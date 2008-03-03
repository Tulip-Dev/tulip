/**
 Author: David Auber
 Email : auber@labri.fr
 Last modification : 20/08/2001
 This program is free software; you can redistribute it and/or modify  *
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
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
