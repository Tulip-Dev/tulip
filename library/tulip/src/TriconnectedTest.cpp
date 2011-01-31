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
#include "tulip/Graph.h"
#include "tulip/TriconnectedTest.h"
#include "tulip/BiconnectedTest.h"
#include "tulip/MutableContainer.h"

using namespace std;
using namespace tlp;
//=================================================================
TriconnectedTest * TriconnectedTest::instance=0;
//=================================================================
TriconnectedTest::TriconnectedTest(){
}
//=================================================================
bool TriconnectedTest::isTriconnected(Graph *graph) {
  if (instance==0)
    instance=new TriconnectedTest();
  return instance->compute(graph);
}
//=================================================================
bool TriconnectedTest::compute(Graph *graph) {
  if (resultsBuffer.find((unsigned long)graph)!=resultsBuffer.end()) 
    return resultsBuffer[(unsigned long)graph];
  if (graph->numberOfNodes()==0) return false;
  graph->addGraphObserver(this);
  bool result = true;
  Graph *tmp = tlp::newCloneSubGraph(graph);
  Iterator<node> *itN = graph->getNodes();
  while(itN->hasNext()) {
    node n = itN->next();
    tmp->delNode(n);
    if (!BiconnectedTest::isBiconnected(tmp)) {
      result = false;
      break;
    }
    tmp->addNode(n);
    Iterator<edge> *itE = graph->getInOutEdges(n);
    while(itE->hasNext()) {
      tmp->addEdge(itE->next());
    } delete itE;
  } delete itN;
  graph->delSubGraph(tmp);
  resultsBuffer[(unsigned long)graph] = result;
  return result;
}
//=================================================================
void TriconnectedTest::addEdge(Graph *graph,const edge) {
  if (resultsBuffer.find((unsigned long)graph)!=resultsBuffer.end())
    if (resultsBuffer[(unsigned long)graph]) return;
  graph->removeGraphObserver(this);
  resultsBuffer.erase((unsigned long)graph);
}
//=================================================================
void TriconnectedTest::delEdge(Graph *graph,const edge) {
  graph->removeGraphObserver(this);
  resultsBuffer.erase((unsigned long)graph);
}
//=================================================================
void TriconnectedTest::reverseEdge(Graph *,const edge) {
}
//=================================================================
void TriconnectedTest::addNode(Graph *graph,const node) {
  resultsBuffer[(unsigned long)graph]=false;
}
//=================================================================
void TriconnectedTest::delNode(Graph *graph,const node) {
  graph->removeGraphObserver(this);
  resultsBuffer.erase((unsigned long)graph);
}
//=================================================================
void TriconnectedTest::destroy(Graph *graph) {
  graph->removeGraphObserver(this);
  resultsBuffer.erase((unsigned long)graph);
}
//=================================================================

