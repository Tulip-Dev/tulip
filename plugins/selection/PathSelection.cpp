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
#include <queue>

#include "PathSelection.h"
#include <tulip/Selection.h>
#include <tulip/MethodFactory.h>

SELECTIONPLUGIN(PathSelection,"Path Selection","David Auber","23/04/2003","Alpha","0","1");

using namespace std;

PathSelection::PathSelection(const PropertyContext &context):SelectionAlgorithm(context)  {
}

PathSelection::~PathSelection() {
}

void PathSelection::reconnect(node n, Int *values) {
  int value=values->getNodeValue(n);
  values->setNodeValue(n,-1);
  selectionResult->setNodeValue(n,true);
  Iterator<node>*itN=superGraph->getInOutNodes(n);
  for (;itN->hasNext();) {
    node itn=itN->next();
    if ( (values->getNodeValue(itn)) == (value-1) && (value!=0))
      reconnect(itn,values);
  } delete itN;
}

bool PathSelection::run() {
  node target;//=targetNode();
  node source;//=sourceNode();
  bool sourceOk=false;
  Selection *viewSel=superGraph->getProperty<Selection>("viewSelection");
  Iterator<node> *itN=superGraph->getNodes();
  for (;itN->hasNext();) {
    node itn=itN->next();
    if (viewSel->getNodeValue(itn)) {
      if (!sourceOk) {
	sourceOk=true;
	source=itn;
      } else {
	target=itn;
	break;
      }
    }
  } delete itN;

  bool finished=false;
  queue<node> fifo;
  fifo.push(source);
  Int *values=superGraph->getLocalProperty<Int>("depthValue");
  values->setAllNodeValue(-1);
  values->setNodeValue(source,0);
  while(!fifo.empty()) {
    node curNode=fifo.front();
    fifo.pop();
    Iterator<node> *itN=superGraph->getInOutNodes(curNode);
    for (;itN->hasNext();) {
      node itn=itN->next();
      if (values->getNodeValue(itn)==-1 && !finished) {
	values->setNodeValue(itn,values->getNodeValue(curNode)+1);
	if (itn==target) finished=true;
	fifo.push(itn);
      }
    } delete itN;
  }

  //If it exist a path between source and target.
  if (finished) { 
    //Select nodes in the path
    reconnect(target,values);
    //Select edges in the path
    Iterator<edge> *itE=superGraph->getEdges();
    for (;itE->hasNext();) {
      edge ite=itE->next();
      if (selectionResult->getNodeValue(superGraph->source(ite)) && selectionResult->getNodeValue(superGraph->target(ite)))
	selectionResult->setEdgeValue(ite,true);
    } delete itE; 
  }
  return true;
}
