/**
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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
class TriconnectedTestListener : public Observable {
public:
  // override of Observable::treatEvent to remove the cached result for a graph if it is modified.
  void treatEvent(const Event &) override;

  /**
   * @brief Stored results for graphs. When a graph is updated, its entry is removed from the map.
   **/
  tlp_hash_map<const Graph *, bool> resultsBuffer;
};

//=================================================================
void TriconnectedTestListener::treatEvent(const Event &evt) {
  const GraphEvent *gEvt = dynamic_cast<const GraphEvent *>(&evt);

  if (gEvt) {
    Graph *graph = gEvt->getGraph();

    auto it = resultsBuffer.find(graph);

    switch (gEvt->getType()) {
    case GraphEvent::TLP_ADD_EDGE:

      if (it != resultsBuffer.end())
        if (it->second)
          return;
      [[fallthrough]];
    case GraphEvent::TLP_AFTER_DEL_EDGE:
    case GraphEvent::TLP_AFTER_DEL_NODE:
      if (it != resultsBuffer.end()) {
        graph->removeListener(this);
        resultsBuffer.erase(it);
      }
      break;

    case GraphEvent::TLP_ADD_NODE:
      resultsBuffer[graph] = false;
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
static TriconnectedTestListener instance;
//=================================================================
bool TriconnectedTest::isTriconnected(Graph *graph) {
  auto it = instance.resultsBuffer.find(graph);
  if (it != instance.resultsBuffer.end())
    return it->second;

  if (graph->isEmpty())
    return false;

  bool result = true;
  Graph *tmp = graph->addCloneSubGraph();

  for (auto n : graph->nodes()) {
    tmp->delNode(n);

    if (!BiconnectedTest::isBiconnected(tmp)) {
      result = false;
      break;
    }

    tmp->addNode(n);

    for (auto e : graph->getInOutEdges(n)) {
      tmp->addEdge(e);
    }
  }

  graph->delSubGraph(tmp);
  graph->addListener(instance);
  return instance.resultsBuffer[graph] = result;
}
