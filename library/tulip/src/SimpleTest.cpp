/*
 * Authors: David Auber, J��y Compostella, Jean Darracq, Benjamin Muller,
 *          Fabrice Rochambeau, Fabiani Simplice, Jyl Cristoff Zobeide
 * 
 * Last modification : $id $
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation; either version 2 of the License, or     
 * (at your option) any later version.
 */

#include <vector>
#include "tulip/Graph.h"
#include "tulip/SimpleTest.h"
#include "tulip/MutableContainer.h"
#include "tulip/BooleanProperty.h"
#include "tulip/StableIterator.h"

using namespace std;
using namespace tlp;
//=================================================================
#ifdef _WIN32 
#ifdef DLL_EXPORT
SimpleTest * SimpleTest::instance=0;
#endif
#else
SimpleTest * SimpleTest::instance=0;
#endif
//=================================================================
SimpleTest::SimpleTest () {
}
//=================================================================
bool SimpleTest::isSimple(Graph *graph) {
  if(instance == 0 )
    instance = new SimpleTest();

  if (instance->resultsBuffer.find((unsigned long)graph) == instance->resultsBuffer.end()) {
    instance->resultsBuffer[(unsigned long)graph] = simpleTest(graph);
    graph->addGraphObserver(instance);
  }
  
  return instance->resultsBuffer[(unsigned long)graph];
}
//**********************************************************************
void SimpleTest::makeSimple(Graph* graph,vector<edge> &removed) {
  if (SimpleTest::isSimple(graph)) return;
  SimpleTest::simpleTest(graph, &removed, &removed);
  vector<edge>::const_iterator it;
  for(it = removed.begin(); it!=removed.end(); ++it) {
    graph->delEdge(*it);
  }
  assert(SimpleTest::isSimple(graph));
}
//=================================================================
bool SimpleTest::simpleTest(Graph *graph, vector<edge> *multipleEdges, vector<edge> *loops) {
 bool result = true;
 bool computeAll = (loops != 0) || (multipleEdges != 0);
 Iterator<node> *itNode = graph->getNodes();
 MutableContainer<bool> inserted;
 inserted.setAll(false);
 while (itNode->hasNext ()) {
   node current = itNode->next ();
   //Search for multiple edges and loops
   Iterator<edge> *itEdge = graph->getInOutEdges (current);
   MutableContainer<bool> targeted;
   targeted.setAll(false);
   while (itEdge->hasNext ()) {
     edge e = itEdge->next();
     node target = graph->opposite(e, current);
     if (target == current) { //loop 
       if (!computeAll) {
	 result = false;
	 break;
       }
       if (loops!=0) {
	 if (!inserted.get(e.id)) {
	   loops->push_back(e);
	   inserted.set(e.id, true);
	 }
       }
     }
     if (targeted.get(target.id) == true) {
       if (!computeAll) {
	 result = false;
	 break;
       }
       if (multipleEdges != 0)  {
	 if (!inserted.get(e.id)) {
	   multipleEdges->push_back(e);
	   inserted.set(e.id, true);
	 }
       }
     }
     else
       targeted.set(target.id, true);
   } delete itEdge;
   if (!computeAll && !result) break;
 } delete itNode;
 return result;
}
//=================================================================
void SimpleTest::deleteResult(Graph *graph) {
  resultsBuffer.erase((unsigned long)graph);
  graph->removeGraphObserver(this);
}
//=================================================================
void SimpleTest::addEdge(Graph *graph, const edge) {
  if (resultsBuffer[(unsigned long)graph] == true)
    deleteResult(graph);
}
//=================================================================
void SimpleTest::delEdge(Graph *graph, const edge) {
  if (resultsBuffer[(unsigned long)graph] == false)
    deleteResult(graph);
}
//=================================================================
void SimpleTest::destroy(Graph *graph) {
  deleteResult(graph);
}
//=================================================================
