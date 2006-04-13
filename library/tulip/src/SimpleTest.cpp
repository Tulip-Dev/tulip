/*
 * Authors: David Auber, Jérémy Compostella, Jean Darracq, Benjamin Muller,
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
#include "tulip/SuperGraph.h"
#include "tulip/SimpleTest.h"
#include "tulip/MutableContainer.h"
#include "tulip/Selection.h"
#include "tulip/StableIterator.h"

using namespace std;
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
bool SimpleTest::isSimple(SuperGraph *graph) {
  if(instance == 0 )
    instance = new SimpleTest();

  if (instance->resultsBuffer.find((unsigned int)graph) == instance->resultsBuffer.end()) {
    instance->resultsBuffer[(unsigned int)graph] = simpleTest(graph);
    graph->addObserver(instance);
  }
  
  return instance->resultsBuffer[(unsigned int)graph];
}
//**********************************************************************
void SimpleTest::makeSimple(SuperGraph* graph,vector<edge> &removed) {
  if (SimpleTest::isSimple(graph)) return;
  SimpleTest::simpleTest(graph, &removed, &removed);
  vector<edge>::const_iterator it;
  for(it = removed.begin(); it!=removed.end(); ++it) {
    graph->delEdge(*it);
  }
  assert(SimpleTest::isSimple(graph));
}
//=================================================================
bool SimpleTest::simpleTest(SuperGraph *graph, vector<edge> *multipleEdges, vector<edge> *loops) {
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
void SimpleTest::deleteResult(SuperGraph *graph) {
  resultsBuffer.erase((unsigned int)graph);
  graph->removeObserver(this);
}
//=================================================================
void SimpleTest::addEdge(SuperGraph *graph, const edge) {
  if (resultsBuffer[(unsigned int)graph] == true)
    deleteResult(graph);
}
//=================================================================
void SimpleTest::delEdge(SuperGraph *graph, const edge) {
  if (resultsBuffer[(unsigned int)graph] == false)
    deleteResult(graph);
}
//=================================================================
void SimpleTest::destroy(SuperGraph *graph) {
  deleteResult(graph);
}
//=================================================================
