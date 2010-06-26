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
#include <tulip/OuterPlanarTest.h>
#include <tulip/Graph.h>
#include <tulip/PlanarityTestImpl.h>
#include <tulip/ForEach.h>

using namespace std;
using namespace tlp;
//=================================================================
#ifdef _WIN32 
#ifdef DLL_EXPORT
OuterPlanarTest * OuterPlanarTest::instance=0;
#endif
#else
OuterPlanarTest * OuterPlanarTest::instance=0;
#endif
//=================================================================
bool OuterPlanarTest::isOuterPlanar(Graph *graph){
  if(instance==0)
    instance = new OuterPlanarTest();
  Observable::holdObservers();
  bool result = instance->compute(graph);
  Observable::unholdObservers();
  return result;
}
//=================================================================
bool OuterPlanarTest::compute(Graph *graph) { 

  if (resultsBuffer.find((unsigned long)graph)!=resultsBuffer.end()) 
    return resultsBuffer[(unsigned long)graph];
  else if(graph->numberOfNodes()==0){
    resultsBuffer[(unsigned long)graph] = true;
    return true;
  }
  PlanarityTestImpl planarTest(graph);
  if (!planarTest.isPlanar(true))
    return (resultsBuffer[(unsigned long)graph] = false);
  else {
    node n = graph->addNode();
    node current;
    forEach(current, graph->getNodes()) {
      if(current != n)
	graph->addEdge(n,current);
    }
    resultsBuffer[(unsigned long)graph] = planarTest.isPlanar(true);
    graph->delNode(n);
    graph->addGraphObserver(this);
    return resultsBuffer[(unsigned long)graph];
  }
}
//=================================================================
void OuterPlanarTest::addEdge(Graph *graph,const edge) {
  if (resultsBuffer.find((unsigned long)graph)!=resultsBuffer.end())
    if (!resultsBuffer[(unsigned long)graph]) return;
  graph->removeGraphObserver(this);
  resultsBuffer.erase((unsigned long)graph);
}
//=================================================================
void OuterPlanarTest::delEdge(Graph *graph,const edge) {
  if (resultsBuffer.find((unsigned long)graph)!=resultsBuffer.end())
    if (resultsBuffer[(unsigned long)graph]) return;
  graph->removeGraphObserver(this);
  resultsBuffer.erase((unsigned long)graph);
}
//=================================================================
void OuterPlanarTest::reverseEdge(Graph *graph,const edge) {
}
//=================================================================
void OuterPlanarTest::addNode(Graph *graph,const node) {
}
//=================================================================
void OuterPlanarTest::delNode(Graph *graph,const node) {
  if (resultsBuffer.find((unsigned long)graph)!=resultsBuffer.end())
    if (resultsBuffer[(unsigned long)graph]) return;
  graph->removeGraphObserver(this);
  resultsBuffer.erase((unsigned long)graph);
}
//=================================================================
void OuterPlanarTest::destroy(Graph *graph) {
  graph->removeGraphObserver(this);
  resultsBuffer.erase((unsigned long)graph);
}
//=================================================================
