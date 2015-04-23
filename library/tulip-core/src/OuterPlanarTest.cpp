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
#include <tulip/OuterPlanarTest.h>
#include <tulip/Graph.h>
#include <tulip/PlanarityTestImpl.h>
#include <tulip/ForEach.h>

using namespace std;
using namespace tlp;
//=================================================================
OuterPlanarTest * OuterPlanarTest::instance=NULL;
//=================================================================
bool OuterPlanarTest::isOuterPlanar(tlp::Graph* graph) {
  if(instance==NULL)
    instance = new OuterPlanarTest();

  Observable::holdObservers();
  bool result = instance->compute(graph);
  Observable::unholdObservers();
  return result;
}
//=================================================================
bool OuterPlanarTest::compute(tlp::Graph* graph) {

  if (resultsBuffer.find(graph)!=resultsBuffer.end())
    return resultsBuffer[graph];
  else if(graph->numberOfNodes()==0) {
    resultsBuffer[graph] = true;
    return true;
  }

  PlanarityTestImpl planarTest(graph);

  if (!planarTest.isPlanar(true))
    return (resultsBuffer[graph] = false);
  else {
    node n = graph->addNode();
    node current;
    forEach(current, graph->getNodes()) {
      if(current != n)
        graph->addEdge(n,current);
    }
    resultsBuffer[graph] = planarTest.isPlanar(true);
    graph->delNode(n);
    graph->addListener(this);
    return resultsBuffer[graph];
  }
}
//=================================================================
void OuterPlanarTest::treatEvent(const Event& evt) {
  const GraphEvent* gEvt = dynamic_cast<const GraphEvent*>(&evt);

  if (gEvt) {
    Graph* graph = gEvt->getGraph();

    switch(gEvt->getType()) {
    case GraphEvent::TLP_ADD_EDGE:

      if (resultsBuffer.find(graph)!=resultsBuffer.end())
        if (!resultsBuffer[graph]) return;

      graph->removeListener(this);
      resultsBuffer.erase(graph);
      break;

    case GraphEvent::TLP_DEL_EDGE:

      if (resultsBuffer.find(graph)!=resultsBuffer.end())
        if (resultsBuffer[graph]) return;

      graph->removeListener(this);
      resultsBuffer.erase(graph);
      break;

    case GraphEvent::TLP_DEL_NODE:

      if (resultsBuffer.find(graph)!=resultsBuffer.end())
        if (resultsBuffer[graph]) return;

      graph->removeListener(this);
      resultsBuffer.erase(graph);
      break;

    case GraphEvent::TLP_REVERSE_EDGE:
      graph->removeListener(this);
      resultsBuffer.erase(graph);
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
