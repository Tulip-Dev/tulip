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

#include "PathBooleanProperty.h"
#include <tulip/BooleanProperty.h>
#include <tulip/MethodFactory.h>

BOOLEANPLUGIN(PathSelection,"Path Selection","David Auber","23/04/2003","Alpha","0","1");

using namespace std;
using namespace tlp;

PathSelection::PathSelection(const PropertyContext &context):BooleanAlgorithm(context)  {
}

PathSelection::~PathSelection() {
}

void PathSelection::reconnect(node n, IntegerProperty *values) {
  int value=values->getNodeValue(n);
  values->setNodeValue(n,-1);
  BooleanResult->setNodeValue(n,true);
  Iterator<node>*itN=graph->getInOutNodes(n);
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
  Selection *viewSel=graph->getProperty<BooleanProperty>("viewSelection");
  Iterator<node> *itN=graph->getNodes();
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
  IntegerProperty *values=graph->getLocalProperty<IntegerProperty>("depthValue");
  values->setAllNodeValue(-1);
  values->setNodeValue(source,0);
  while(!fifo.empty()) {
    node curNode=fifo.front();
    fifo.pop();
    Iterator<node> *itN=graph->getInOutNodes(curNode);
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
    Iterator<edge> *itE=graph->getEdges();
    for (;itE->hasNext();) {
      edge ite=itE->next();
      if (BooleanResult->getNodeValue(graph->source(ite)) && BooleanResult->getNodeValue(graph->target(ite)))
	BooleanResult->setEdgeValue(ite,true);
    } delete itE; 
  }
  return true;
}
