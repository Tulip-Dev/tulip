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
#include <tulip/Graph.h>
#include <tulip/PlanarityTest.h>
#include <tulip/PlanarityTestImpl.h>
#include <tulip/BiconnectedTest.h>

using namespace std;
using namespace tlp;
//=================================================================
PlanarityTest * PlanarityTest::instance=0;
//=================================================================
bool PlanarityTest::isPlanar(Graph *graph){
  if(instance==0)
    instance = new PlanarityTest();
  Observable::holdObservers();
  bool result = instance->compute(graph);
  Observable::unholdObservers();
  return result;
}
bool PlanarityTest::isPlanarEmbedding(Graph *graph) {
    return PlanarityTestImpl::isPlanarEmbedding(graph);
}
//=================================================================
bool PlanarityTest::planarEmbedding(Graph *graph) {
  if (!PlanarityTest::isPlanar(graph))
    return false;
  Observable::holdObservers();
  vector<edge> addedEdges;
  BiconnectedTest::makeBiconnected(graph, addedEdges);
  PlanarityTestImpl planarTest(graph);
  planarTest.isPlanar(true);
  vector<edge>::const_iterator it = addedEdges.begin();
  for (; it!=addedEdges.end(); ++it)
    graph->delEdge(*it, true);
  Observable::unholdObservers();
  return true;
}
//=================================================================
list<edge> PlanarityTest::getObstructionsEdges(Graph *graph) {
  if (PlanarityTest::isPlanar(graph))
    return list<edge>();
  vector<edge> addedEdges;
  Observable::holdObservers();
  BiconnectedTest::makeBiconnected(graph, addedEdges);
  PlanarityTestImpl planarTest(graph);
  planarTest.isPlanar(true);
  list<edge> tmpList = planarTest.getObstructions();
  {
    vector<edge>::const_iterator it = addedEdges.begin();
    for (; it!=addedEdges.end(); ++it)
      graph->delEdge(*it, true);
  }
  Observable::unholdObservers();
  set<edge> tmpAdded(addedEdges.begin(), addedEdges.end());
  list<edge> result;
  for (list<edge>::iterator it = tmpList.begin(); it != tmpList.end(); ++it) {
    if (tmpAdded.find(*it) == tmpAdded.end()) 
      result.push_back(*it);
  }
  return result;
}
//=================================================================
bool PlanarityTest::compute(Graph *graph) { 

  if (resultsBuffer.find((unsigned long)graph)!=resultsBuffer.end()) 
    return resultsBuffer[(unsigned long)graph];
  else if(graph->numberOfNodes()==0){
    resultsBuffer[(unsigned long)graph] = true;
    return true;
  }
  vector<edge> addedEdges;
  BiconnectedTest::makeBiconnected(graph, addedEdges);
  PlanarityTestImpl planarTest(graph);
  resultsBuffer[(unsigned long)graph] = planarTest.isPlanar(true);
  vector<edge>::const_iterator it = addedEdges.begin();
  for (; it!=addedEdges.end(); ++it)
    graph->delEdge(*it, true);
  graph->addGraphObserver(this);
  return resultsBuffer[(unsigned long)graph];
}
//=================================================================
void PlanarityTest::addEdge(Graph *graph,const edge) {
  if (resultsBuffer.find((unsigned long)graph)!=resultsBuffer.end())
    if (!resultsBuffer[(unsigned long)graph]) return;
  graph->removeGraphObserver(this);
  resultsBuffer.erase((unsigned long)graph);
}
//=================================================================
void PlanarityTest::delEdge(Graph *graph,const edge) {
  if (resultsBuffer.find((unsigned long)graph)!=resultsBuffer.end())
    if (resultsBuffer[(unsigned long)graph]) return;
  graph->removeGraphObserver(this);
  resultsBuffer.erase((unsigned long)graph);
}
//=================================================================
void PlanarityTest::addNode(Graph*,const node) {
}
//=================================================================
void PlanarityTest::delNode(Graph *graph,const node) {
  if (resultsBuffer.find((unsigned long)graph)!=resultsBuffer.end())
    if (resultsBuffer[(unsigned long)graph]) return;
  graph->removeGraphObserver(this);
  resultsBuffer.erase((unsigned long)graph);
}
//=================================================================
void PlanarityTest::destroy(Graph *graph) {
  graph->removeGraphObserver(this);
  resultsBuffer.erase((unsigned long)graph);
}
//=================================================================
