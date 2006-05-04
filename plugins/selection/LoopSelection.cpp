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
#include "LoopSelection.h"
#include <tulip/ForEach.h>
#include <tulip/BooleanProperty.h>
#include <tulip/MethodFactory.h>

BOOLEANPLUGIN(LoopSelection,"Loop Selection","David Auber","20/01/2003","Alpha","0","1");

using namespace std;

LoopSelection::LoopSelection(const PropertyContext &context):BooleanAlgorithm(context) {}
//============================================
bool LoopSelection::run() {
  booleanResult->setAllNodeValue(false);
  edge e;
  forEach(e, graph->getEdges())
    booleanResult->setEdgeValue(e, 
			       graph->source(e)==graph->target(e));
  return true;
}
//============================================

