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
#include <tulip/SelectionProxy.h>
#include <tulip/MethodFactory.h>

SELECTIONPLUGIN(LoopSelection,"Loop Selection","David Auber","20/01/2003","Alpha","0","1");

using namespace std;

LoopSelection::LoopSelection(const PropertyContext &context):Selection(context) {}
//============================================
bool LoopSelection::run() {
  selectionProxy->setAllNodeValue(false);
  edge e;
  forEach(e, superGraph->getEdges())
    selectionProxy->setEdgeValue(e, 
				 superGraph->source(e)==superGraph->target(e));
  return true;
}
//============================================

