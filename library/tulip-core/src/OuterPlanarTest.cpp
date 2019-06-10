/**
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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

using namespace std;
using namespace tlp;
//=================================================================
class OuterPlanarTestListener : public Observable {
public:
  // override of Observable::treatEvent to remove the cached result for a graph if it is modified.
  void treatEvent(const Event &) override;

  /**
   * @brief Stored results for graphs. When a graph is updated, its entry is removed from the map.
   **/
  std::unordered_map<const Graph *, bool> resultsBuffer;
};

void OuterPlanarTestListener::treatEvent(const Event &evt) {
  const GraphEvent *gEvt = dynamic_cast<const GraphEvent *>(&evt);

  if (gEvt) {
    Graph *graph = gEvt->getGraph();

    switch (gEvt->getType()) {
    case GraphEvent::TLP_ADD_EDGE:
      if (resultsBuffer.find(graph) != resultsBuffer.end())
        if (resultsBuffer[graph])
          return;

      graph->removeListener(this);
      resultsBuffer.erase(graph);
      break;

    case GraphEvent::TLP_DEL_EDGE:
    case GraphEvent::TLP_DEL_NODE:

      if (resultsBuffer.find(graph) != resultsBuffer.end())
        if (!resultsBuffer[graph])
          return;

    case GraphEvent::TLP_REVERSE_EDGE:
      graph->removeListener(this);
      resultsBuffer.erase(graph);
      break;

    default:
      // we don't care about other events
      break;
    }
  } else {

    Graph *graph = static_cast<Graph *>(evt.sender());

    if (evt.type() == Event::TLP_DELETE)
      resultsBuffer.erase(graph);
  }
}
//=================================================================
static OuterPlanarTestListener instance;
//=================================================================
bool OuterPlanarTest::isOuterPlanar(tlp::Graph *graph) {
  auto it = instance.resultsBuffer.find(graph);
  if (it != instance.resultsBuffer.end())
    return it->second;
  else if (graph->isEmpty())
    return instance.resultsBuffer[graph] = true;

  PlanarityTestImpl planarTest(graph);

  if (!planarTest.isPlanar(true))
    return (instance.resultsBuffer[graph] = false);
  else {
    Observable::holdObservers();
    node n = graph->addNode();
    for (auto current : graph->nodes()) {
      if (current != n)
        graph->addEdge(n, current);
    }
    instance.resultsBuffer[graph] = planarTest.isPlanar(true);
    graph->delNode(n);
    Observable::unholdObservers();
    graph->addListener(instance);
    return instance.resultsBuffer[graph];
  }
}
