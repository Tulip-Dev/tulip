/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
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
#include <tulip/TriconnectedTest.h>
#include <tulip/BiconnectedTest.h>

using namespace std;
using namespace tlp;
//=================================================================
TriconnectedTest * TriconnectedTest::instance=NULL;
//=================================================================
TriconnectedTest::TriconnectedTest() {
}
//=================================================================
bool TriconnectedTest::isTriconnected(Graph* graph) {
  if (instance==NULL)
    instance=new TriconnectedTest();

  return instance->compute(graph);
}
//=================================================================
bool TriconnectedTest::compute(Graph* graph) {
  if (resultsBuffer.find(graph)!=resultsBuffer.end())
    return resultsBuffer[graph];

  if (graph->numberOfNodes()==0) return false;

  graph->addListener(this);
  bool result = true;
  Graph *tmp = graph->addCloneSubGraph();
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
    }

    delete itE;
  }

  delete itN;
  graph->delSubGraph(tmp);
  resultsBuffer[graph] = result;
  return result;
}
//=================================================================
void TriconnectedTest::treatEvent(const Event& evt) {
  const GraphEvent* gEvt = dynamic_cast<const GraphEvent*>(&evt);

  if (gEvt) {
    Graph* graph = gEvt->getGraph();

    switch(gEvt->getType()) {
    case GraphEvent::TLP_ADD_EDGE:

      if (resultsBuffer.find(graph)!=resultsBuffer.end())
        if (resultsBuffer[graph]) return;

      graph->removeListener(this);
      resultsBuffer.erase(graph);
      break;

    case GraphEvent::TLP_DEL_EDGE:
      graph->removeListener(this);
      resultsBuffer.erase(graph);
      break;

    case GraphEvent::TLP_DEL_NODE:
      graph->removeListener(this);
      resultsBuffer.erase(graph);
      break;

    case GraphEvent::TLP_ADD_NODE:
      resultsBuffer[graph]=false;
      break;

    default:
      //we don't care about other events
      break;
    }
  }
  else {
    // From my point of view the use of dynamic_cast should be correct
    // but it fails, so I use reinterpret_cast (pm)
    Graph* graph = reinterpret_cast<Graph *>(evt.sender());

    if (graph && evt.type() == Event::TLP_DELETE)
      resultsBuffer.erase(graph);
  }
}

