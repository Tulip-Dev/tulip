/*
 * Authors: Jérémy Compostella, Jean Darracq, Benjamin Muller,
 *          Fabrice Rochambeau, Fabiani Simplice, Jyl Cristoff Zobeide
 * 
 * Email : jcompost@etu.u-bordeaux1.fr, jdarracq@etu.u-bordeaux1.fr,
 *         bmuller@etu.u-bordeaux1.fr, frochamb@etu.u-bordeaux1.fr,
 *         fsimplic@etu.u-bordeaux1.fr, jczobeid@etu.u-bordeaux1.fr.
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
#include "tulip/SelectionProxy.h"
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
  return instance->compute(graph);
}
//**********************************************************************
void SimpleTest::makeSimple(SuperGraph* graph,vector<edge> &removed) {
  if (SimpleTest::isSimple(graph)) return;
  string erreurMsg;
  SelectionProxy multipleEdge(graph);
  graph->computeProperty("Multiple edge", &multipleEdge, erreurMsg);
  StableIterator<edge> itE(graph->getEdges());
  //We replace self loops by three edges an two nodes.
  while (itE.hasNext()) {
    edge ite=itE.next();
    if ((multipleEdge.getEdgeValue(ite))) {
      removed.push_back(ite);
      graph->delEdge(ite);
    } else
      if (graph->source(ite) == graph->target(ite)) {
	removed.push_back(ite);
	graph->delEdge(ite);
      }
  }
  assert(SimpleTest::isSimple(graph));
}
//=================================================================
bool SimpleTest::compute(SuperGraph *graph) {
  if (resultsBuffer.find((unsigned int)graph) != resultsBuffer.end())
    return resultsBuffer[(unsigned int)graph];
  Iterator<node> *itNode = graph->getNodes();
  while (itNode->hasNext ()) {
    node current = itNode->next ();
    //Search for multiple edges and loops
    Iterator<edge> *itEdge = graph->getInOutEdges (current);
    MutableContainer<bool> targeted;
    targeted.setAll(false);
    while (itEdge->hasNext ()) {
      node target = graph->opposite(itEdge->next(), current);
      if (target == current)  //loop
	return setResult(graph, false);
      if(targeted.get(target.id) == true)
	return setResult(graph, false);
      else
	targeted.set(target.id, true);
    } delete itEdge;
  } delete itNode;
  return setResult(graph, true);
}
//=================================================================
bool SimpleTest::setResult(SuperGraph *graph, bool result) {
  resultsBuffer[(unsigned int)graph] = result;
  graph->addObserver(this);
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
