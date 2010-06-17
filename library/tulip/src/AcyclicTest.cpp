/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
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
#include "tulip/AcyclicTest.h"
#include "tulip/BooleanProperty.h"
#include "tulip/StableIterator.h"

using namespace std;
using namespace tlp;

#ifdef _WIN32 
#ifdef DLL_EXPORT
AcyclicTest * AcyclicTest::instance=0;
#endif
#else
AcyclicTest * AcyclicTest::instance=0;
#endif
//**********************************************************************
AcyclicTest::AcyclicTest(){
}
//**********************************************************************
bool AcyclicTest::isAcyclic(const Graph *graph) {
  if (instance==0)
    instance = new AcyclicTest();

  if (instance->resultsBuffer.find((unsigned long)graph) == instance->resultsBuffer.end()) {
    instance->resultsBuffer[(unsigned long)graph] = acyclicTest(graph);
    graph->addGraphObserver(instance);
  }
  
  return instance->resultsBuffer[(unsigned long)graph];
}
//**********************************************************************
void AcyclicTest::makeAcyclic(Graph* graph,vector<edge> &reversed, vector<tlp::SelfLoops> &selfLoops) {
  if (AcyclicTest::isAcyclic(graph)) return;

  //replace self loops by three edges and two nodes.
  StableIterator<edge> itE(graph->getEdges());
  while (itE.hasNext()) {
    edge e = itE.next();
    const pair<node, node>& eEnds = graph->ends(e);
    if (eEnds.first == eEnds.second) {
      node n1 = graph->addNode();
      node n2 = graph->addNode();
      selfLoops.push_back(tlp::SelfLoops(n1 , n2 , 
					 graph->addEdge(eEnds.first, n1) , 
					 graph->addEdge(n1,n2) , 
					 graph->addEdge(eEnds.first, n2) , 
					 e ));
      graph->delEdge(e);
    }
  }

  //find obstruction edges
  reversed.clear();
  acyclicTest(graph, &reversed);
  //  cerr << "reversed : " << reversed.size() << endl;
  if (reversed.size() > graph->numberOfEdges() / 2) {
    cerr << "[Warning]: " << __FUNCTION__ << ", is not efficient" << endl;
  }

  vector<edge>::const_iterator it = reversed.begin();
  for (; it != reversed.end(); ++it)
    graph->reverse(*it);

  assert(AcyclicTest::isAcyclic(graph));
 }

//=================================================================
bool AcyclicTest::dfsAcyclicTest(const Graph *graph, const node n, 
				 MutableContainer<bool> &visited, 
				 MutableContainer<bool> &finished,
				 vector<edge> *obstructionEdges) {
  visited.set(n.id,true);
  bool result = true;
  Iterator<edge> *it = graph->getOutEdges(n);
  while (it->hasNext()) {
    edge tmp = it->next();
    node nextNode = graph->target(tmp);
    if (visited.get(nextNode.id)) {
      if (!finished.get(nextNode.id)) {
	result = false;
	if (obstructionEdges != 0)
	  obstructionEdges->push_back(tmp);
	else {
	  break;
	}
      }
    }
    else {
      bool tmp = dfsAcyclicTest(graph, nextNode, visited, finished, obstructionEdges);
      result = tmp && result;
      if ((!result) && (obstructionEdges==0)) break;
    }
  } delete it;
  finished.set(n.id,true);
  return result;
}
//**********************************************************************
bool AcyclicTest::acyclicTest(const Graph *graph, vector<edge> *obstructionEdges) {
  MutableContainer<bool> visited;
  MutableContainer<bool> finished;
  visited.setAll(false);
  finished.setAll(false);
  bool result = true;
  Iterator<node> *it = graph->getNodes();
  while (it->hasNext()) {
    node curNode=it->next();
    if (!visited.get(curNode.id)) {
      if (!dfsAcyclicTest(graph, curNode, visited, finished, obstructionEdges)) {
	result = false;
	if (obstructionEdges == 0) {
	  break;
	}
      }
    }
  } delete it;
  return result;
}
//**********************************************************************
void AcyclicTest::destroy(Graph *graph) {
  graph->removeGraphObserver(this);
  resultsBuffer.erase((unsigned long)graph);
}
//**********************************************************************
void AcyclicTest::reverseEdge(Graph *graph,const edge ) {
  graph->removeGraphObserver(this);
  resultsBuffer.erase((unsigned long)graph);
}
//**********************************************************************
void AcyclicTest::addEdge(Graph *graph,const edge) {
  if (resultsBuffer[(unsigned long)graph]==false) return;
  graph->removeGraphObserver(this);
  resultsBuffer.erase((unsigned long)graph);
}
//**********************************************************************
void AcyclicTest::delEdge(Graph *graph,const edge) {
  if (resultsBuffer[(unsigned long)graph]==true) return;
  graph->removeGraphObserver(this);
  resultsBuffer.erase((unsigned long)graph);
}
//**********************************************************************
