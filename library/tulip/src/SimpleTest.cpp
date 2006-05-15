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
bool SimpleTest::isSimple(Graph *sg) {
  if(instance == 0 )
    instance = new SimpleTest();

  if (instance->resultsBuffer.find((unsigned long)sg) == instance->resultsBuffer.end()) {
    instance->resultsBuffer[(unsigned long)sg] = simpleTest(sg);
    sg->addObserver(instance);
  }
  
  return instance->resultsBuffer[(unsigned long)sg];
}
//**********************************************************************
void SimpleTest::makeSimple(Graph* sg,vector<edge> &removed) {
  if (SimpleTest::isSimple(sg)) return;
  SimpleTest::simpleTest(sg, &removed, &removed);
  vector<edge>::const_iterator it;
  for(it = removed.begin(); it!=removed.end(); ++it) {
    sg->delEdge(*it);
  }
  assert(SimpleTest::isSimple(sg));
}
//=================================================================
bool SimpleTest::simpleTest(Graph *sg, vector<edge> *multipleEdges, vector<edge> *loops) {
 bool result = true;
 bool computeAll = (loops != 0) || (multipleEdges != 0);
 Iterator<node> *itNode = sg->getNodes();
 MutableContainer<bool> inserted;
 inserted.setAll(false);
 while (itNode->hasNext ()) {
   node current = itNode->next ();
   //Search for multiple edges and loops
   Iterator<edge> *itEdge = sg->getInOutEdges (current);
   MutableContainer<bool> targeted;
   targeted.setAll(false);
   while (itEdge->hasNext ()) {
     edge e = itEdge->next();
     node target = sg->opposite(e, current);
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
void SimpleTest::deleteResult(Graph *sg) {
  resultsBuffer.erase((unsigned long)sg);
  sg->removeObserver(this);
}
//=================================================================
void SimpleTest::addEdge(Graph *sg, const edge) {
  if (resultsBuffer[(unsigned long)sg] == true)
    deleteResult(sg);
}
//=================================================================
void SimpleTest::delEdge(Graph *sg, const edge) {
  if (resultsBuffer[(unsigned long)sg] == false)
    deleteResult(sg);
}
//=================================================================
void SimpleTest::destroy(Graph *sg) {
  deleteResult(sg);
}
//=================================================================
